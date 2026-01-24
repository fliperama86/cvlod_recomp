nonmatching func_80152DE0, 0x1A0

glabel func_80152DE0
    /* D5ED0 80152DE0 27BDFFC0 */  addiu      $sp, $sp, -0x40
    /* D5ED4 80152DE4 AFBF001C */  sw         $ra, 0x1C($sp)
    /* D5ED8 80152DE8 AFB00018 */  sw         $s0, 0x18($sp)
    /* D5EDC 80152DEC 8C860070 */  lw         $a2, 0x70($a0)
    /* D5EE0 80152DF0 3C198000 */  lui        $t9, %hi(func_80005A30)
    /* D5EE4 80152DF4 00808025 */  or         $s0, $a0, $zero
    /* D5EE8 80152DF8 27395A30 */  addiu      $t9, $t9, %lo(func_80005A30)
    /* D5EEC 80152DFC 3C05801A */  lui        $a1, %hi(D_8019E5F0)
    /* D5EF0 80152E00 8CA5E5F0 */  lw         $a1, %lo(D_8019E5F0)($a1)
    /* D5EF4 80152E04 24040001 */  addiu      $a0, $zero, 0x1
    /* D5EF8 80152E08 0320F809 */  jalr       $t9
    /* D5EFC 80152E0C AFA60038 */   sw        $a2, 0x38($sp)
    /* D5F00 80152E10 8FA60038 */  lw         $a2, 0x38($sp)
    /* D5F04 80152E14 AE020024 */  sw         $v0, 0x24($s0)
    /* D5F08 80152E18 14400007 */  bnez       $v0, .L80152E38
    /* D5F0C 80152E1C 00403825 */   or        $a3, $v0, $zero
    /* D5F10 80152E20 8E190010 */  lw         $t9, 0x10($s0)
    /* D5F14 80152E24 02002025 */  or         $a0, $s0, $zero
    /* D5F18 80152E28 0320F809 */  jalr       $t9
    /* D5F1C 80152E2C 00000000 */   nop
    /* D5F20 80152E30 10000133 */  b          .L80153300
    /* D5F24 80152E34 8FBF001C */   lw        $ra, 0x1C($sp)
  .L80152E38:
    /* D5F28 80152E38 944E0002 */  lhu        $t6, 0x2($v0)
    /* D5F2C 80152E3C 3C08801D */  lui        $t0, %hi(D_801C82C0)
    /* D5F30 80152E40 250882C0 */  addiu      $t0, $t0, %lo(D_801C82C0)
    /* D5F34 80152E44 35CF0900 */  ori        $t7, $t6, 0x900
    /* D5F38 80152E48 A44F0002 */  sh         $t7, 0x2($v0)
    /* D5F3C 80152E4C 8D182B14 */  lw         $t8, 0x2B14($t0)
    /* D5F40 80152E50 AC580040 */  sw         $t8, 0x40($v0)
    /* D5F44 80152E54 850928D0 */  lh         $t1, 0x28D0($t0)
    /* D5F48 80152E58 252AFFFD */  addiu      $t2, $t1, -0x3
    /* D5F4C 80152E5C 2D410028 */  sltiu      $at, $t2, 0x28
    /* D5F50 80152E60 10200088 */  beqz       $at, .L80153084
    /* D5F54 80152E64 000A5080 */   sll       $t2, $t2, 2
    /* D5F58 80152E68 3C01801A */  lui        $at, %hi(jtbl_801999CC)
    /* D5F5C 80152E6C 002A0821 */  addu       $at, $at, $t2
    /* D5F60 80152E70 8C2A99CC */  lw         $t2, %lo(jtbl_801999CC)($at)
    /* D5F64 80152E74 01400008 */  jr         $t2
    /* D5F68 80152E78 00000000 */   nop
    /* D5F6C 80152E7C 94CB0018 */  lhu        $t3, 0x18($a2)
    /* D5F70 80152E80 3C0D802E */  lui        $t5, %hi(D_802E510C)
    /* D5F74 80152E84 25AD510C */  addiu      $t5, $t5, %lo(D_802E510C)
    /* D5F78 80152E88 000B60C0 */  sll        $t4, $t3, 3
    /* D5F7C 80152E8C 018B6023 */  subu       $t4, $t4, $t3
    /* D5F80 80152E90 000C6080 */  sll        $t4, $t4, 2
    /* D5F84 80152E94 018DC821 */  addu       $t9, $t4, $t5
    /* D5F88 80152E98 26030034 */  addiu      $v1, $s0, 0x34
    /* D5F8C 80152E9C 1000007F */  b          .L8015309C
    /* D5F90 80152EA0 AC790004 */   sw        $t9, 0x4($v1)
    /* D5F94 80152EA4 94CE0018 */  lhu        $t6, 0x18($a2)
    /* D5F98 80152EA8 3C18802E */  lui        $t8, %hi(D_802E7C24)
    /* D5F9C 80152EAC 27187C24 */  addiu      $t8, $t8, %lo(D_802E7C24)
    /* D5FA0 80152EB0 000E78C0 */  sll        $t7, $t6, 3
    /* D5FA4 80152EB4 01EE7823 */  subu       $t7, $t7, $t6
    /* D5FA8 80152EB8 000F7880 */  sll        $t7, $t7, 2
    /* D5FAC 80152EBC 01F84821 */  addu       $t1, $t7, $t8
    /* D5FB0 80152EC0 26030034 */  addiu      $v1, $s0, 0x34
    /* D5FB4 80152EC4 10000075 */  b          .L8015309C
    /* D5FB8 80152EC8 AC690004 */   sw        $t1, 0x4($v1)
    /* D5FBC 80152ECC 94CA0018 */  lhu        $t2, 0x18($a2)
    /* D5FC0 80152ED0 3C0C802E */  lui        $t4, %hi(D_802E428C)
    /* D5FC4 80152ED4 258C428C */  addiu      $t4, $t4, %lo(D_802E428C)
    /* D5FC8 80152ED8 000A58C0 */  sll        $t3, $t2, 3
    /* D5FCC 80152EDC 016A5823 */  subu       $t3, $t3, $t2
    /* D5FD0 80152EE0 000B5880 */  sll        $t3, $t3, 2
    /* D5FD4 80152EE4 016C6821 */  addu       $t5, $t3, $t4
    /* D5FD8 80152EE8 26030034 */  addiu      $v1, $s0, 0x34
    /* D5FDC 80152EEC 1000006B */  b          .L8015309C
    /* D5FE0 80152EF0 AC6D0004 */   sw        $t5, 0x4($v1)
    /* D5FE4 80152EF4 94D90018 */  lhu        $t9, 0x18($a2)
    /* D5FE8 80152EF8 3C0F802E */  lui        $t7, %hi(D_802E6D1C)
    /* D5FEC 80152EFC 25EF6D1C */  addiu      $t7, $t7, %lo(D_802E6D1C)
    /* D5FF0 80152F00 001970C0 */  sll        $t6, $t9, 3
    /* D5FF4 80152F04 01D97023 */  subu       $t6, $t6, $t9
    /* D5FF8 80152F08 000E7080 */  sll        $t6, $t6, 2
    /* D5FFC 80152F0C 01CFC021 */  addu       $t8, $t6, $t7
    /* D6000 80152F10 26030034 */  addiu      $v1, $s0, 0x34
    /* D6004 80152F14 10000061 */  b          .L8015309C
    /* D6008 80152F18 AC780004 */   sw        $t8, 0x4($v1)
    /* D600C 80152F1C 94C90018 */  lhu        $t1, 0x18($a2)
    /* D6010 80152F20 3C0B802E */  lui        $t3, %hi(D_802E7ED0)
    /* D6014 80152F24 256B7ED0 */  addiu      $t3, $t3, %lo(D_802E7ED0)
    /* D6018 80152F28 000950C0 */  sll        $t2, $t1, 3
    /* D601C 80152F2C 01495023 */  subu       $t2, $t2, $t1
    /* D6020 80152F30 000A5080 */  sll        $t2, $t2, 2
    /* D6024 80152F34 014B6021 */  addu       $t4, $t2, $t3
    /* D6028 80152F38 26030034 */  addiu      $v1, $s0, 0x34
    /* D602C 80152F3C 10000057 */  b          .L8015309C
    /* D6030 80152F40 AC6C0004 */   sw        $t4, 0x4($v1)
    /* D6034 80152F44 94CD0018 */  lhu        $t5, 0x18($a2)
    /* D6038 80152F48 3C0E802E */  lui        $t6, %hi(D_802E42D4)
    /* D603C 80152F4C 25CE42D4 */  addiu      $t6, $t6, %lo(D_802E42D4)
    /* D6040 80152F50 000DC8C0 */  sll        $t9, $t5, 3
    /* D6044 80152F54 032DC823 */  subu       $t9, $t9, $t5
    /* D6048 80152F58 0019C880 */  sll        $t9, $t9, 2
    /* D604C 80152F5C 032E7821 */  addu       $t7, $t9, $t6
    /* D6050 80152F60 26030034 */  addiu      $v1, $s0, 0x34
    /* D6054 80152F64 1000004D */  b          .L8015309C
    /* D6058 80152F68 AC6F0004 */   sw        $t7, 0x4($v1)
    /* D605C 80152F6C 94D80018 */  lhu        $t8, 0x18($a2)
    /* D6060 80152F70 3C0A802F */  lui        $t2, %hi(D_802EA204)
    /* D6064 80152F74 254AA204 */  addiu      $t2, $t2, %lo(D_802EA204)
    /* D6068 80152F78 001848C0 */  sll        $t1, $t8, 3
    /* D606C 80152F7C 01384823 */  subu       $t1, $t1, $t8
endlabel func_80152DE0
