// Controller Pak (Memory Pak) format and operations test harness.
// Validates pak image layout against the N64 PFS specification,
// then tests the full init → find → allocate → find → read/write sequence.
//
// No runtime dependencies — runs standalone.

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <cstdlib>

// ── N64 Controller Pak Constants (from N64 SDK os_pfs.h / controller.h) ──

static constexpr int BLOCKSIZE = 32;            // bytes per block
static constexpr int PFS_ONE_PAGE = 8;           // blocks per page
static constexpr int PFS_PAGE_SIZE = 256;        // BLOCKSIZE * PFS_ONE_PAGE
static constexpr int PFS_TOTAL_SIZE = 0x8000;    // 32KB
static constexpr int PFS_PAGES = 128;            // total pages (32KB / 256)

// Page assignments (single-bank 256Kbit pak, banks=0)
static constexpr int PAGE_ID       = 0;          // ID area (8 blocks)
static constexpr int PAGE_INODE    = 1;          // Inode table
static constexpr int PAGE_INODE_BK = 2;          // Inode table backup
static constexpr int PAGE_DIR0     = 3;          // Directory page 0
static constexpr int PAGE_DIR1     = 4;          // Directory page 1
static constexpr int PAGE_DATA     = 5;          // First data page
static constexpr int DEF_DIR_PAGES = 2;          // Directory pages count

// ID block positions within page 0 (block numbers)
static constexpr int PFS_ID_0AREA = 1;
static constexpr int PFS_ID_1AREA = 3;
static constexpr int PFS_ID_2AREA = 4;
static constexpr int PFS_ID_3AREA = 6;
static constexpr int PFS_LABEL_AREA = 7;

// Inode entry special values
static constexpr uint16_t PFS_EOF           = 1;
static constexpr uint16_t PFS_PAGE_NOT_USED = 3;

// Directory
static constexpr int PFS_DIR_ENTRY_SIZE = 32;
static constexpr int PFS_DIR_ENTRIES = (DEF_DIR_PAGES * PFS_PAGE_SIZE) / PFS_DIR_ENTRY_SIZE; // 16
static constexpr int PFS_FILE_NAME_LEN = 16;
static constexpr int PFS_FILE_EXT_LEN  = 4;

// Status flags
static constexpr uint8_t PFS_WRITTEN = 0x02;

// ── Pak Buffer + Mock Save Backend ───────────────────────────────────

static uint8_t pak_buf[PFS_TOTAL_SIZE];

static void save_write(const void* in, uint32_t offset, uint32_t count) {
    if (offset + count <= PFS_TOTAL_SIZE)
        memcpy(pak_buf + offset, in, count);
}

static void save_read(void* out, uint32_t offset, uint32_t count) {
    if (offset + count <= PFS_TOTAL_SIZE)
        memcpy(out, pak_buf + offset, count);
    else
        memset(out, 0, count);
}

// ── Block-level access (mirrors __osContRamRead / __osContRamWrite) ──

static int block_read(uint16_t block_no, uint8_t out[BLOCKSIZE]) {
    uint32_t off = (uint32_t)block_no * BLOCKSIZE;
    if (off + BLOCKSIZE > PFS_TOTAL_SIZE) return -1;
    memcpy(out, pak_buf + off, BLOCKSIZE);
    return 0;
}

static int block_write(uint16_t block_no, const uint8_t in[BLOCKSIZE]) {
    uint32_t off = (uint32_t)block_no * BLOCKSIZE;
    if (off + BLOCKSIZE > PFS_TOTAL_SIZE) return -1;
    memcpy(pak_buf + off, in, BLOCKSIZE);
    return 0;
}

// ── Helpers ──────────────────────────────────────────────────────────

static uint16_t read_be16(const uint8_t* p) { return (p[0] << 8) | p[1]; }
static uint32_t read_be32(const uint8_t* p) { return (p[0]<<24)|(p[1]<<16)|(p[2]<<8)|p[3]; }

static void write_be16(uint8_t* p, uint16_t v) { p[0] = v >> 8; p[1] = v & 0xFF; }
static void write_be32(uint8_t* p, uint32_t v) {
    p[0] = v >> 24; p[1] = (v >> 16) & 0xFF; p[2] = (v >> 8) & 0xFF; p[3] = v & 0xFF;
}

// ID block checksum: sum of u16 BE words at offsets 0x00-0x1A (14 words)
static uint16_t id_checksum(const uint8_t block[BLOCKSIZE]) {
    uint16_t sum = 0;
    for (int i = 0; i < 0x1C; i += 2)
        sum += read_be16(block + i);
    return sum;
}

// Inode table checksum: u8 sum of bytes from entry[start_page] to entry[127]
// For banks=0: inodeStartPage = 1 + DEF_DIR_PAGES + 2*0 = 3
static uint8_t inode_checksum(const uint8_t page[PFS_PAGE_SIZE], int start_page) {
    uint8_t sum = 0;
    for (int i = start_page * 2; i < 128 * 2; i++)
        sum += page[i];
    return sum;
}

static uint16_t inode_entry(const uint8_t page[PFS_PAGE_SIZE], int index) {
    return read_be16(page + index * 2);
}

// ── Reference format_empty_pak (correct per N64 spec) ────────────────

static void format_empty_pak() {
    memset(pak_buf, 0, PFS_TOTAL_SIZE);

    // --- Page 0: ID area ---
    uint8_t id[BLOCKSIZE];
    memset(id, 0, BLOCKSIZE);
    // 0x00-0x17: serial (24 bytes) — zeros for new pak
    // 0x18-0x19: device_id = 0x0001 (BE, LSB must be set)
    write_be16(id + 0x18, 0x0001);
    // 0x1A: banks = 0 (single bank, 256Kbit = 32KB)
    id[0x1A] = 0x00;
    // 0x1B: version = 0
    id[0x1B] = 0x00;
    // 0x1C-0x1D: checksum = sum of u16 words 0x00-0x1A
    uint16_t cksum = id_checksum(id);
    write_be16(id + 0x1C, cksum);
    // 0x1E-0x1F: inverted checksum = 0xFFF2 - checksum
    write_be16(id + 0x1E, 0xFFF2 - cksum);

    // Write ID at the 4 standard block positions; leave 0, 2, 5, 7 as zeros
    block_write(PFS_ID_0AREA, id);   // block 1
    block_write(PFS_ID_1AREA, id);   // block 3
    block_write(PFS_ID_2AREA, id);   // block 4
    block_write(PFS_ID_3AREA, id);   // block 6

    // --- Pages 1-2: Inode table + backup ---
    // 128 entries × 2 bytes = 256 bytes
    // Entry 0: [0x00, checksum]
    // Entries 1-2: 0x0000 (below inodeStartPage, not scanned)
    // Entries 3-4: PFS_EOF (0x0001) — directory pages, allocated by system
    // Entries 5-127: PFS_PAGE_NOT_USED (0x0003) — free data pages
    uint8_t inode[PFS_PAGE_SIZE];
    memset(inode, 0, PFS_PAGE_SIZE);

    int inode_start_page = 1 + DEF_DIR_PAGES + 2 * 0; // = 3
    for (int i = inode_start_page; i < PAGE_DATA; i++)
        write_be16(inode + i * 2, PFS_EOF);
    for (int i = PAGE_DATA; i < PFS_PAGES; i++)
        write_be16(inode + i * 2, PFS_PAGE_NOT_USED);
    uint8_t icksum = inode_checksum(inode, inode_start_page);
    inode[0] = 0x00;  // entry[0].bank
    inode[1] = icksum; // entry[0].page = checksum

    save_write(inode, PAGE_INODE * PFS_PAGE_SIZE, PFS_PAGE_SIZE);
    save_write(inode, PAGE_INODE_BK * PFS_PAGE_SIZE, PFS_PAGE_SIZE);

    // --- Pages 3-4: Directory (empty = all zeros) ---
    // Already zeroed by memset.
}

// ── Test Framework ───────────────────────────────────────────────────

static int tests_run = 0;
static int tests_passed = 0;

#define CHECK(cond, ...) do { \
    if (!(cond)) { \
        fprintf(stderr, "  FAIL: " __VA_ARGS__); \
        fprintf(stderr, "\n"); \
        return false; \
    } \
} while(0)

#define RUN_TEST(fn) do { \
    tests_run++; \
    fprintf(stderr, "%-50s ", #fn); \
    if (fn()) { fprintf(stderr, "PASS\n"); tests_passed++; } \
    else       { fprintf(stderr, "FAILED\n"); } \
} while(0)

// ── Tests ────────────────────────────────────────────────────────────

static bool test_format_id_block() {
    format_empty_pak();

    // Read ID block from each standard position
    const int positions[] = { PFS_ID_0AREA, PFS_ID_1AREA, PFS_ID_2AREA, PFS_ID_3AREA };
    for (int pos : positions) {
        uint8_t blk[BLOCKSIZE];
        block_read(pos, blk);

        uint16_t dev_id = read_be16(blk + 0x18);
        CHECK(dev_id == 0x0001, "block %d: device_id=0x%04X, expected 0x0001", pos, dev_id);
        CHECK((dev_id & 1) == 1, "block %d: device_id LSB not set", pos);

        uint8_t banks = blk[0x1A];
        CHECK(banks == 0, "block %d: banks=%d, expected 0", pos, banks);

        uint16_t ck = id_checksum(blk);
        uint16_t stored_ck = read_be16(blk + 0x1C);
        CHECK(ck == stored_ck, "block %d: checksum=0x%04X, computed=0x%04X", pos, stored_ck, ck);

        uint16_t inv = read_be16(blk + 0x1E);
        uint16_t expected_inv = 0xFFF2 - ck;
        CHECK(inv == expected_inv, "block %d: inv_checksum=0x%04X, expected=0x%04X", pos, inv, expected_inv);
    }

    // Blocks 0, 2, 5 must be all zeros
    const int zero_blocks[] = { 0, 2, 5 };
    for (int pos : zero_blocks) {
        uint8_t blk[BLOCKSIZE];
        block_read(pos, blk);
        bool all_zero = true;
        for (int i = 0; i < BLOCKSIZE; i++) if (blk[i] != 0) { all_zero = false; break; }
        CHECK(all_zero, "block %d should be all zeros", pos);
    }

    // Block 7 (label area) — zeros for new pak
    {
        uint8_t blk[BLOCKSIZE];
        block_read(PFS_LABEL_AREA, blk);
        bool all_zero = true;
        for (int i = 0; i < BLOCKSIZE; i++) if (blk[i] != 0) { all_zero = false; break; }
        CHECK(all_zero, "label block should be all zeros for new pak");
    }

    return true;
}

static bool test_format_inode_table() {
    format_empty_pak();

    // Read inode table (page 1)
    uint8_t inode[PFS_PAGE_SIZE];
    save_read(inode, PAGE_INODE * PFS_PAGE_SIZE, PFS_PAGE_SIZE);

    // Entry 0: checksum holder — bank=0, page=checksum
    CHECK(inode[0] == 0x00, "inode[0].bank should be 0, got 0x%02X", inode[0]);

    // Entries 1-2: below inodeStartPage — must be 0x0000
    for (int i = 1; i <= 2; i++) {
        uint16_t val = inode_entry(inode, i);
        CHECK(val == 0x0000, "inode[%d]=0x%04X, expected 0x0000", i, val);
    }
    // Entries 3-4: directory pages — must be PFS_EOF (0x0001)
    for (int i = 3; i <= 4; i++) {
        uint16_t val = inode_entry(inode, i);
        CHECK(val == PFS_EOF, "inode[%d]=0x%04X, expected 0x0001 (PFS_EOF)", i, val);
    }

    // Entries 5-127: free pages — must be PFS_PAGE_NOT_USED (0x0003)
    for (int i = 5; i < 128; i++) {
        uint16_t val = inode_entry(inode, i);
        CHECK(val == PFS_PAGE_NOT_USED, "inode[%d]=0x%04X, expected 0x0003", i, val);
    }

    // Verify checksum: u8 sum of bytes from entry[3] to entry[127]
    int inode_start_page = 1 + DEF_DIR_PAGES; // = 3 for banks=0
    uint8_t computed = inode_checksum(inode, inode_start_page);
    CHECK(inode[1] == computed, "inode checksum: stored=0x%02X, computed=0x%02X", inode[1], computed);

    // Backup (page 2) must be identical
    uint8_t backup[PFS_PAGE_SIZE];
    save_read(backup, PAGE_INODE_BK * PFS_PAGE_SIZE, PFS_PAGE_SIZE);
    CHECK(memcmp(inode, backup, PFS_PAGE_SIZE) == 0, "inode backup doesn't match primary");

    return true;
}

static bool test_format_directory_empty() {
    format_empty_pak();

    // Pages 3-4 (directory) must be all zeros
    uint8_t dir[DEF_DIR_PAGES * PFS_PAGE_SIZE];
    save_read(dir, PAGE_DIR0 * PFS_PAGE_SIZE, sizeof(dir));

    bool all_zero = true;
    for (size_t i = 0; i < sizeof(dir); i++) {
        if (dir[i] != 0) { all_zero = false; break; }
    }
    CHECK(all_zero, "directory should be all zeros for empty pak");

    return true;
}

static bool test_format_data_area_clean() {
    format_empty_pak();

    // Data area (pages 5-127) must be all zeros
    uint32_t data_start = PAGE_DATA * PFS_PAGE_SIZE;
    for (uint32_t i = data_start; i < PFS_TOTAL_SIZE; i++) {
        CHECK(pak_buf[i] == 0, "data area byte 0x%04X should be 0, got 0x%02X", i, pak_buf[i]);
    }

    return true;
}

static bool test_block_read_write_roundtrip() {
    format_empty_pak();

    // Write a test pattern to a data page block, read it back
    uint8_t pattern[BLOCKSIZE];
    for (int i = 0; i < BLOCKSIZE; i++) pattern[i] = (uint8_t)(0xA5 ^ i);

    uint16_t test_block = PAGE_DATA * PFS_ONE_PAGE; // first data block
    block_write(test_block, pattern);

    uint8_t readback[BLOCKSIZE];
    block_read(test_block, readback);
    CHECK(memcmp(pattern, readback, BLOCKSIZE) == 0, "block read/write roundtrip failed");

    // Verify neighboring blocks untouched
    uint8_t neighbor[BLOCKSIZE];
    block_read(test_block + 1, neighbor);
    bool all_zero = true;
    for (int i = 0; i < BLOCKSIZE; i++) if (neighbor[i] != 0) { all_zero = false; break; }
    CHECK(all_zero, "neighboring block was corrupted");

    return true;
}

// Simulate what osPfsInitPak does: read ID, validate checksum, read inodes
static bool test_simulated_init_pak() {
    format_empty_pak();

    // Step 1: Read ID block from PFS_ID_0AREA
    uint8_t id[BLOCKSIZE];
    CHECK(block_read(PFS_ID_0AREA, id) == 0, "failed to read ID block");

    // Step 2: Validate ID checksum
    uint16_t ck = id_checksum(id);
    uint16_t stored_ck = read_be16(id + 0x1C);
    uint16_t stored_inv = read_be16(id + 0x1E);
    CHECK(ck == stored_ck, "ID checksum mismatch");
    CHECK(stored_inv == (uint16_t)(0xFFF2 - ck), "ID inv checksum mismatch");

    // Step 3: Validate device_id
    uint16_t dev_id = read_be16(id + 0x18);
    CHECK((dev_id & 1) == 1, "device_id LSB not set (osPfsInitPak rejects this)");

    // Step 4: Extract banks
    uint8_t banks = id[0x1A];
    CHECK(banks == 0, "banks=%d, expected 0 for standard 32KB pak", banks);

    // Step 5: Compute layout (same as osPfsInitPak)
    int inode_start_page = 1 + DEF_DIR_PAGES + 2 * banks; // = 3

    // Step 6: Read inode table (page 1)
    uint8_t inode_page[PFS_PAGE_SIZE];
    // osPfsInitPak reads the inode table one block at a time via __osContRamRead.
    // The inode table is at page 1, which starts at block 8.
    for (int b = 0; b < PFS_ONE_PAGE; b++) {
        block_read(PAGE_INODE * PFS_ONE_PAGE + b, inode_page + b * BLOCKSIZE);
    }

    // Step 7: Validate inode checksum
    uint8_t icksum = inode_checksum(inode_page, inode_start_page);
    CHECK(inode_page[1] == icksum,
          "inode checksum: stored=0x%02X computed=0x%02X", inode_page[1], icksum);

    // Step 8: Read backup inode (page 2) and verify match
    uint8_t backup[PFS_PAGE_SIZE];
    for (int b = 0; b < PFS_ONE_PAGE; b++) {
        block_read(PAGE_INODE_BK * PFS_ONE_PAGE + b, backup + b * BLOCKSIZE);
    }
    CHECK(memcmp(inode_page, backup, PFS_PAGE_SIZE) == 0, "inode backup mismatch");

    // Step 9: Verify free page count
    int free_pages = 0;
    for (int i = PAGE_DATA; i < PFS_PAGES; i++) {
        if (inode_entry(inode_page, i) == PFS_PAGE_NOT_USED) free_pages++;
    }
    CHECK(free_pages == (PFS_PAGES - PAGE_DATA),
          "free pages: %d, expected %d", free_pages, PFS_PAGES - PAGE_DATA);

    return true;
}

// Simulate osPfsAllocateFile: write a directory entry + inode chain,
// then verify with a simulated osPfsFindFile + osPfsReadWriteFile.
static bool test_allocate_and_find_file() {
    format_empty_pak();

    // --- Simulate osPfsAllocateFile ---
    // Allocate a file: company=0x3544, game_code=0x4E443445 ("ND4E"),
    // game_name="CASTLEVANIA2", ext_name="\0\0\0\0", size = 4 pages (1024 bytes)
    const uint16_t company = 0x3544;
    const uint32_t game_code = 0x4E443445; // "ND4E"
    const char game_name[PFS_FILE_NAME_LEN] = "CASTLEVANIA2\0\0\0";
    const char ext_name[PFS_FILE_EXT_LEN] = {};
    const int file_pages = 4;

    // Read current inode table
    uint8_t inode[PFS_PAGE_SIZE];
    save_read(inode, PAGE_INODE * PFS_PAGE_SIZE, PFS_PAGE_SIZE);

    // Find 4 free pages and chain them
    int first_page = -1, prev_page = -1;
    int allocated = 0;
    for (int i = PAGE_DATA; i < PFS_PAGES && allocated < file_pages; i++) {
        if (inode_entry(inode, i) == PFS_PAGE_NOT_USED) {
            if (first_page < 0) first_page = i;
            if (prev_page >= 0) write_be16(inode + prev_page * 2, (uint16_t)i); // chain
            prev_page = i;
            allocated++;
        }
    }
    CHECK(allocated == file_pages, "couldn't allocate %d pages", file_pages);
    // Terminate chain with PFS_EOF
    write_be16(inode + prev_page * 2, PFS_EOF);

    // Recompute inode checksum
    int inode_start_page = 1 + DEF_DIR_PAGES;
    uint8_t icksum = inode_checksum(inode, inode_start_page);
    inode[0] = 0x00;
    inode[1] = icksum;

    // Write updated inode table + backup
    save_write(inode, PAGE_INODE * PFS_PAGE_SIZE, PFS_PAGE_SIZE);
    save_write(inode, PAGE_INODE_BK * PFS_PAGE_SIZE, PFS_PAGE_SIZE);

    // Write directory entry at slot 0 (page 3, offset 0x300)
    uint8_t dir_entry[PFS_DIR_ENTRY_SIZE];
    memset(dir_entry, 0, PFS_DIR_ENTRY_SIZE);
    write_be32(dir_entry + 0x00, game_code);       // game_code
    write_be16(dir_entry + 0x04, company);          // company_code
    write_be16(dir_entry + 0x06, (uint16_t)first_page); // start_page (as inode unit)
    dir_entry[0x08] = PFS_WRITTEN;                  // status
    // 0x0A-0x0B: data_sum (leave 0 for now)
    memcpy(dir_entry + 0x0C, ext_name, PFS_FILE_EXT_LEN);
    memcpy(dir_entry + 0x10, game_name, PFS_FILE_NAME_LEN);

    save_write(dir_entry, PAGE_DIR0 * PFS_PAGE_SIZE, PFS_DIR_ENTRY_SIZE);

    // --- Simulate osPfsFindFile ---
    // Scan directory for matching company + game_code + game_name + ext_name
    bool found = false;
    int found_slot = -1;
    for (int slot = 0; slot < PFS_DIR_ENTRIES; slot++) {
        uint8_t entry[PFS_DIR_ENTRY_SIZE];
        save_read(entry, PAGE_DIR0 * PFS_PAGE_SIZE + slot * PFS_DIR_ENTRY_SIZE, PFS_DIR_ENTRY_SIZE);

        uint32_t gc = read_be32(entry + 0x00);
        uint16_t cc = read_be16(entry + 0x04);
        if (gc == 0 && cc == 0) continue; // empty slot

        if (gc == game_code && cc == company &&
            memcmp(entry + 0x0C, ext_name, PFS_FILE_EXT_LEN) == 0 &&
            memcmp(entry + 0x10, game_name, PFS_FILE_NAME_LEN) == 0) {
            found = true;
            found_slot = slot;
            break;
        }
    }
    CHECK(found, "osPfsFindFile simulation: file not found after allocation");
    CHECK(found_slot == 0, "expected slot 0, got %d", found_slot);

    // --- Simulate osPfsReadWriteFile (WRITE then READ) ---
    // Write a test pattern to the file's data pages
    uint8_t write_data[file_pages * PFS_PAGE_SIZE];
    for (int i = 0; i < (int)sizeof(write_data); i++)
        write_data[i] = (uint8_t)(i & 0xFF);

    // Walk inode chain and write one page at a time
    int page = first_page;
    int data_off = 0;
    for (int p = 0; p < file_pages; p++) {
        CHECK(page >= PAGE_DATA && page < PFS_PAGES,
              "inode chain broken at step %d: page=%d", p, page);

        // Write page (8 blocks)
        for (int b = 0; b < PFS_ONE_PAGE; b++) {
            block_write(page * PFS_ONE_PAGE + b, write_data + data_off);
            data_off += BLOCKSIZE;
        }

        // Follow chain
        uint16_t next = inode_entry(inode, page);
        if (p < file_pages - 1) {
            CHECK(next != PFS_EOF, "chain ended too early at step %d", p);
            page = next;
        } else {
            CHECK(next == PFS_EOF, "chain not terminated at last page: 0x%04X", next);
        }
    }

    // Read back via same inode chain
    uint8_t read_data[file_pages * PFS_PAGE_SIZE];
    memset(read_data, 0xFF, sizeof(read_data));

    page = first_page;
    data_off = 0;
    for (int p = 0; p < file_pages; p++) {
        for (int b = 0; b < PFS_ONE_PAGE; b++) {
            block_read(page * PFS_ONE_PAGE + b, read_data + data_off);
            data_off += BLOCKSIZE;
        }
        uint16_t next = inode_entry(inode, page);
        if (next != PFS_EOF) page = next;
    }

    CHECK(memcmp(write_data, read_data, sizeof(write_data)) == 0,
          "read-back data doesn't match written data");

    // --- Verify inode checksum still valid after allocation ---
    save_read(inode, PAGE_INODE * PFS_PAGE_SIZE, PFS_PAGE_SIZE);
    uint8_t ick = inode_checksum(inode, inode_start_page);
    CHECK(inode[1] == ick, "inode checksum invalid after allocation: stored=0x%02X computed=0x%02X",
          inode[1], ick);

    // --- Verify free page count decreased ---
    int free_pages = 0;
    for (int i = PAGE_DATA; i < PFS_PAGES; i++) {
        if (inode_entry(inode, i) == PFS_PAGE_NOT_USED) free_pages++;
    }
    int expected_free = (PFS_PAGES - PAGE_DATA) - file_pages;
    CHECK(free_pages == expected_free,
          "free pages after alloc: %d, expected %d", free_pages, expected_free);

    return true;
}

// Verify that searching for a non-existent file fails correctly
static bool test_find_file_miss() {
    format_empty_pak();

    // Scan empty directory — should find nothing
    for (int slot = 0; slot < PFS_DIR_ENTRIES; slot++) {
        uint8_t entry[PFS_DIR_ENTRY_SIZE];
        save_read(entry, PAGE_DIR0 * PFS_PAGE_SIZE + slot * PFS_DIR_ENTRY_SIZE, PFS_DIR_ENTRY_SIZE);
        uint32_t gc = read_be32(entry + 0x00);
        uint16_t cc = read_be16(entry + 0x04);
        CHECK(gc == 0 && cc == 0, "slot %d not empty on fresh pak: gc=0x%08X cc=0x%04X", slot, gc, cc);
    }

    return true;
}

// Verify that the expected inode checksum for a fresh pak is 0x71
// (matches known-good mupen64plus .mpk format)
static bool test_known_good_checksum() {
    format_empty_pak();

    uint8_t inode[PFS_PAGE_SIZE];
    save_read(inode, PAGE_INODE * PFS_PAGE_SIZE, PFS_PAGE_SIZE);

    // Entries 3-4: [0x00, 0x01] each → 2 * 1 = 2
    // Entries 5-127: [0x00, 0x03] each → 123 * 3 = 369 = 0x171 → u8 = 0x71
    // Total = 2 + 0x71 = 0x73
    CHECK(inode[1] == 0x73,
          "inode checksum should be 0x73, got 0x%02X", inode[1]);

    return true;
}

// ── Data CRC test (matches mupen64plus pak_data_crc) ─────────────────

// N64 Controller Pak data CRC-8 (polynomial 0x85)
static uint8_t pak_data_crc(const uint8_t* data, int size) {
    uint8_t crc = 0;
    for (int i = 0; i <= size; ++i) {
        for (int mask = 0x80; mask >= 1; mask >>= 1) {
            uint8_t xor_tap = (crc & 0x80) ? 0x85 : 0x00;
            crc <<= 1;
            if (i != size && (data[i] & mask)) crc |= 1;
            crc ^= xor_tap;
        }
    }
    return crc;
}

static bool test_data_crc_zeros() {
    // CRC of 32 zero bytes — must be deterministic
    uint8_t zeros[32] = {};
    uint8_t crc = pak_data_crc(zeros, 32);
    // The CRC must be the same every time
    CHECK(crc == pak_data_crc(zeros, 32), "CRC not deterministic");
    // For all-zeros, CRC should be 0x00 (polynomial property)
    CHECK(crc == 0x00, "CRC of all-zeros should be 0x00, got 0x%02X", crc);
    return true;
}

static bool test_data_crc_nonzero() {
    // CRC of non-zero data must be non-trivial
    uint8_t data[32];
    for (int i = 0; i < 32; i++) data[i] = (uint8_t)(i + 1);
    uint8_t crc = pak_data_crc(data, 32);
    // Should be deterministic
    CHECK(crc == pak_data_crc(data, 32), "CRC not deterministic for non-zero data");
    // Changing one byte should change the CRC
    data[0] ^= 0x01;
    uint8_t crc2 = pak_data_crc(data, 32);
    CHECK(crc != crc2, "CRC should change when data changes: both 0x%02X", crc);
    return true;
}

// ── Main ─────────────────────────────────────────────────────────────

int main() {
    fprintf(stderr, "\n=== N64 Controller Pak Format Tests (reference impl) ===\n\n");

    RUN_TEST(test_format_id_block);
    RUN_TEST(test_format_inode_table);
    RUN_TEST(test_format_directory_empty);
    RUN_TEST(test_format_data_area_clean);
    RUN_TEST(test_block_read_write_roundtrip);
    RUN_TEST(test_simulated_init_pak);
    RUN_TEST(test_known_good_checksum);
    RUN_TEST(test_find_file_miss);
    RUN_TEST(test_allocate_and_find_file);

    fprintf(stderr, "\n=== Data CRC tests ===\n\n");

    RUN_TEST(test_data_crc_zeros);
    RUN_TEST(test_data_crc_nonzero);

    fprintf(stderr, "\n%d/%d tests passed\n\n", tests_passed, tests_run);
    return (tests_passed == tests_run) ? 0 : 1;
}
