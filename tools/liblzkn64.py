"""
Compress and Decompress Konami LZKN64 (a.k.a. Nagano) files

Credit to @LiquidCat64 for writing this script and powering through the various challenges encountered.
Credit to @Fluvian for the original lzkn64 C program https://github.com/Fluvian/lzkn64
"""

from typing import Optional

TYPE_COMPRESS = 1
TYPE_DECOMPRESS = 2

MODE_NONE = 0x7F
MODE_WINDOW_COPY = 0x00
MODE_RAW_COPY = 0x80
MODE_RLE_WRITE_A = 0xC0
MODE_RLE_WRITE_B = 0xE0
MODE_RLE_WRITE_C = 0xFF

WINDOW_SIZE = 0x3DF
COPY_SIZE = 0x21
RLE_SIZE = 0x101

"""
Max file size for a compressed LZKN64 file
"""
MAX_LZKN64_FILE_SIZE = 0xFFFFFF
"""
Maximum ROM size of 512Mbit (64Mbyte)
"""
MAX_ROM_SIZE = 0x4000000
"""
Header indicating the start of the Nisitenma-Ichigo table
"""
NISITENMA_ICHIGO_HEADER = (
    b"\x4E\x69\x73\x69\x74\x65\x6E\x6D\x61\x2D\x49\x63\x68\x69\x67\x6F"
)


def find_nisitenma_ichigo_offset(rom_buffer: bytes) -> Optional[int]:
    """
    Find the byte-offset of a Nisitenma-Ichigo table
    :param rom_buffer:
    :return: the byte-offset at the end of the Nisitenma-Ichigo header if present, None otherwise
    """
    offset = rom_buffer.find(NISITENMA_ICHIGO_HEADER)
    if offset == -1:
        return None

    return offset + len(NISITENMA_ICHIGO_HEADER)



def compress_buffer(file_buffer, pad_zeroes=False):
    """
    Compresses the data in the buffer specified in the arguments.
    :param file_buffer:
    :param pad_zeroes:
    :return:
    """
    # Size of the buffer to compress
    buffer_size = len(file_buffer)

    # Position of the current read location in the buffer.
    buffer_position = 0

    # Position of the current write location in the written buffer.
    write_position = 4

    # Allocate write_buffer with size of 0xFFFFFF (24-bit).
    write_buffer = bytearray(MAX_LZKN64_FILE_SIZE)

    # Position in the input buffer of the last time one of the copy modes was used.
    buffer_last_copy_position = 0

    while buffer_position < buffer_size:
        # Calculate maximum length we are able to copy without going out of bounds.
        if COPY_SIZE <= buffer_size - buffer_position:
            sliding_window_maximum_length = COPY_SIZE
        else:
            sliding_window_maximum_length = buffer_size - buffer_position

        # Calculate how far we are able to look back without going behind the start of the uncompressed buffer.
        if buffer_position - WINDOW_SIZE > 0:
            sliding_window_maximum_offset = buffer_position - WINDOW_SIZE
        else:
            sliding_window_maximum_offset = 0

        # Calculate maximum length the forwarding looking window is able to search.
        if RLE_SIZE < buffer_size - buffer_position - 1:
            forward_window_maximum_length = RLE_SIZE
        else:
            forward_window_maximum_length = buffer_size - buffer_position

        if forward_window_maximum_length > COPY_SIZE:
            for i in range(COPY_SIZE + 1, forward_window_maximum_length + 1):
                if (i + buffer_position) & 0xFFF in [0x021, 0x421, 0x821, 0xC21]:
                    forward_window_maximum_length = i
                    break

        sliding_window_match_position = -1
        sliding_window_match_size = 0

        forward_window_match_value = 0
        forward_window_match_size = 0

        # The current mode the compression algorithm prefers. (0x7F == None)
        current_mode = MODE_NONE

        # The current submode the compression algorithm prefers.
        current_submode = MODE_NONE

        # How many bytes will have to be copied in the raw copy command.
        raw_copy_size = buffer_position - buffer_last_copy_position

        # How many bytes we still have to copy in RLE matches with more than 0x21 bytes.
        rle_bytes_left = 0

        """Go backwards in the buffer, is there a matching value?
        If yes, search forward and check for more matching values in a loop.
        If no, go further back and repeat."""
        for search_position in range(
            buffer_position - 1, sliding_window_maximum_offset - 1, -1
        ):
            matching_sequence_size = 0

            while (
                file_buffer[search_position + matching_sequence_size]
                == file_buffer[buffer_position + matching_sequence_size]
            ):
                matching_sequence_size += 1

                if matching_sequence_size >= sliding_window_maximum_length:
                    break

            # Once we find a match or a match that is bigger than the match before it, we save its position and length.
            if matching_sequence_size > sliding_window_match_size:
                sliding_window_match_position = search_position
                sliding_window_match_size = matching_sequence_size

        """Look one step forward in the buffer, is there a matching value?
        If yes, search further and check for a repeating value in a loop.
        If no, continue to the rest of the function."""
        matching_sequence_value = file_buffer[buffer_position]
        matching_sequence_size = 0

        # If our matching sequence number is not 0x00, set the forward window maximum length to the copy size minus 1.
        # This is the highest it can really be in that case.
        if (
            matching_sequence_value != 0
            and forward_window_maximum_length > COPY_SIZE - 1
        ):
            forward_window_maximum_length = COPY_SIZE - 1

        while (
            file_buffer[buffer_position + matching_sequence_size]
            == matching_sequence_value
        ):
            matching_sequence_size += 1

            # If we find a sequence of matching values, save them.
            if matching_sequence_size >= 1:
                forward_window_match_value = matching_sequence_value
                forward_window_match_size = matching_sequence_size

            if matching_sequence_size >= forward_window_maximum_length:
                break

        # Try to pick which mode works best with the current values.
        if (
            sliding_window_match_size >= 4
            and sliding_window_match_size > forward_window_match_size
        ):
            current_mode = MODE_WINDOW_COPY

        elif forward_window_match_size >= 3:
            current_mode = MODE_RLE_WRITE_A

            if forward_window_match_value != 0x00:
                current_submode = MODE_RLE_WRITE_A
                rle_bytes_left = forward_window_match_size
            elif (
                forward_window_match_value == 0x00
                and forward_window_match_size < COPY_SIZE
            ):
                current_submode = MODE_RLE_WRITE_B
            elif (
                forward_window_match_value == 0x00
                and forward_window_match_size >= COPY_SIZE
            ):
                current_submode = MODE_RLE_WRITE_C

        elif forward_window_match_size >= 2 and forward_window_match_value == 0x00:
            current_mode = MODE_RLE_WRITE_A
            current_submode = MODE_RLE_WRITE_B

        """Write a raw copy command when these following conditions are met:
        The current mode is set and there are raw bytes available to be copied.
        The raw byte length exceeds the maximum length that can be stored.
        Raw bytes need to be written due to the proximity to the end of the buffer."""
        if (
            (current_mode != MODE_NONE and raw_copy_size >= 1)
            or raw_copy_size >= 0x1F
            or (buffer_position + 1) == buffer_size
        ):
            if buffer_position + 1 == buffer_size:
                raw_copy_size = buffer_size - buffer_last_copy_position

            while raw_copy_size > 0:
                if raw_copy_size > 0x1F:
                    write_buffer[write_position] = MODE_RAW_COPY | 0x1F
                    write_position += 1

                    for written_bytes in range(0x1F):
                        write_buffer[write_position] = file_buffer[
                            buffer_last_copy_position
                        ]
                        write_position += 1
                        buffer_last_copy_position += 1

                    raw_copy_size -= 0x1F
                else:
                    write_buffer[write_position] = MODE_RAW_COPY | raw_copy_size & 0x1F
                    write_position += 1

                    for written_bytes in range(raw_copy_size):
                        write_buffer[write_position] = file_buffer[
                            buffer_last_copy_position
                        ]
                        write_position += 1
                        buffer_last_copy_position += 1

                    raw_copy_size = 0

        if current_mode == MODE_WINDOW_COPY:
            write_buffer[write_position] = (
                MODE_WINDOW_COPY
                | ((sliding_window_match_size - 2) & 0x1F) << 2
                | (((buffer_position - sliding_window_match_position) & 0x300) >> 8)
            )
            write_position += 1
            write_buffer[write_position] = (
                buffer_position - sliding_window_match_position
            ) & 0xFF
            write_position += 1

            buffer_position += sliding_window_match_size
            buffer_last_copy_position = buffer_position

        elif current_mode == MODE_RLE_WRITE_A:
            if current_submode == MODE_RLE_WRITE_A:
                write_buffer[write_position] = (
                    MODE_RLE_WRITE_A | (forward_window_match_size - 2) & 0x1F
                )
                write_position += 1
                write_buffer[write_position] = forward_window_match_value & 0xFF
                write_position += 1

            elif current_submode == MODE_RLE_WRITE_B:
                write_buffer[write_position] = (
                    MODE_RLE_WRITE_B | (forward_window_match_size - 2) & 0x1F
                )
                write_position += 1

            elif current_submode == MODE_RLE_WRITE_C:
                write_buffer[write_position] = MODE_RLE_WRITE_C
                write_position += 1
                write_buffer[write_position] = (forward_window_match_size - 2) & 0xFF
                write_position += 1

            buffer_position += forward_window_match_size
            buffer_last_copy_position = buffer_position
        else:
            buffer_position += 1

    # Write the compressed size.
    write_buffer[1] = 0x00
    write_buffer[1] = write_position >> 16 & 0xFF
    write_buffer[2] = write_position >> 8 & 0xFF
    write_buffer[3] = write_position & 0xFF

    # Return the compressed buffer.
    if pad_zeroes:
        while write_position % 16 != 0:
            write_position += 1

    if write_position % 2 != 0:
        write_position += 1

    return write_buffer[0:write_position]



def decompress_buffer(file_buffer):
    """
    Decompresses the data in the buffer specified in the arguments.
    :param file_buffer:
    :return:
    """
    # Position of the current read location in the buffer.
    buffer_position = 4

    # Position of the current write location in the written buffer.
    write_position = 0

    # Get compressed size.
    compressed_size = (
        (file_buffer[1] << 16) + (file_buffer[2] << 8) + file_buffer[3] - 1
    )

    # Allocate writeBuffer with size of 0xFFFFFF (24-bit).
    write_buffer = bytearray(MAX_LZKN64_FILE_SIZE)

    while buffer_position < compressed_size:
        mode_command = file_buffer[buffer_position]
        buffer_position += 1

        if MODE_WINDOW_COPY <= mode_command < MODE_RAW_COPY:
            copy_length = (mode_command >> 2) + 2
            copy_offset = file_buffer[buffer_position] + (mode_command << 8) & 0x3FF
            buffer_position += 1

            for current_length in range(copy_length, 0, -1):
                write_buffer[write_position] = write_buffer[
                    write_position - copy_offset
                ]
                write_position += 1
        elif MODE_RAW_COPY <= mode_command < MODE_RLE_WRITE_A:
            copy_length = mode_command & 0x1F

            for current_length in range(copy_length, 0, -1):
                write_buffer[write_position] = file_buffer[buffer_position]
                write_position += 1
                buffer_position += 1
        elif MODE_RLE_WRITE_A <= mode_command <= MODE_RLE_WRITE_C:
            write_length = 0
            write_value = 0x00

            if MODE_RLE_WRITE_A <= mode_command < MODE_RLE_WRITE_B:
                write_length = (mode_command & 0x1F) + 2
                write_value = file_buffer[buffer_position]
                buffer_position += 1
            elif MODE_RLE_WRITE_B <= mode_command < MODE_RLE_WRITE_C:
                write_length = (mode_command & 0x1F) + 2
            elif mode_command == MODE_RLE_WRITE_C:
                write_length = file_buffer[buffer_position] + 2
                buffer_position += 1

            for current_length in range(write_length, 0, -1):
                write_buffer[write_position] = write_value
                write_position += 1

    while write_position % 16 != 0:
        write_position += 1

    # Return the decompressed buffer.
    return write_buffer[0:write_position]
