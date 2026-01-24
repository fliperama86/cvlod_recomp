nonmatching func_80152A14, 0x10C

glabel func_80152A14
    /* D5B04 80152A14 27BDFF80 */  addiu      $sp, $sp, -0x80
    /* D5B08 80152A18 AFBF001C */  sw         $ra, 0x1C($sp)
    /* D5B0C 80152A1C AFB10018 */  sw         $s1, 0x18($sp)
    /* D5B10 80152A20 AFB00014 */  sw         $s0, 0x14($sp)
    /* D5B14 80152A24 948E0054 */  lhu        $t6, 0x54($a0)
    /* D5B18 80152A28 3C118019 */  lui        $s1, %hi(D_801927A8)
    /* D5B1C 80152A2C 8E3127A8 */  lw         $s1, %lo(D_801927A8)($s1)
    /* D5B20 80152A30 15C0000B */  bnez       $t6, .L80152A60
    /* D5B24 80152A34 00008025 */   or        $s0, $zero, $zero
    /* D5B28 80152A38 24860034 */  addiu      $a2, $a0, 0x34
    /* D5B2C 80152A3C 94CF0020 */  lhu        $t7, 0x20($a2)
    /* D5B30 80152A40 25F80001 */  addiu      $t8, $t7, 0x1
    /* D5B34 80152A44 A4D80020 */  sh         $t8, 0x20($a2)
    /* D5B38 80152A48 8C990070 */  lw         $t9, 0x70($a0)
    /* D5B3C 80152A4C 13200004 */  beqz       $t9, .L80152A60
    /* D5B40 80152A50 00000000 */   nop
    /* D5B44 80152A54 0C050C8D */  jal        func_80143234
    /* D5B48 80152A58 AFA40080 */   sw        $a0, 0x80($sp)
    /* D5B4C 80152A5C 8FA40080 */  lw         $a0, 0x80($sp)
  .L80152A60:
    /* D5B50 80152A60 3C0B801D */  lui        $t3, %hi(D_801C87EC)
    /* D5B54 80152A64 956B87EC */  lhu        $t3, %lo(D_801C87EC)($t3)
    /* D5B58 80152A68 24860034 */  addiu      $a2, $a0, 0x34
    /* D5B5C 80152A6C 00C05025 */  or         $t2, $a2, $zero
    /* D5B60 80152A70 05610004 */  bgez       $t3, .L80152A84
    /* D5B64 80152A74 316C0003 */   andi      $t4, $t3, 0x3
    /* D5B68 80152A78 11800002 */  beqz       $t4, .L80152A84
    /* D5B6C 80152A7C 00000000 */   nop
    /* D5B70 80152A80 258CFFFC */  addiu      $t4, $t4, -0x4
  .L80152A84:
    /* D5B74 80152A84 1180006D */  beqz       $t4, .L80152C3C
    /* D5B78 80152A88 27A80048 */   addiu     $t0, $sp, 0x48
  .L80152A8C:
    /* D5B7C 80152A8C 8D420000 */  lw         $v0, 0x0($t2)
    /* D5B80 80152A90 26250050 */  addiu      $a1, $s1, 0x50
    /* D5B84 80152A94 3C198014 */  lui        $t9, %hi(.L80147D5C)
    /* D5B88 80152A98 1040000E */  beqz       $v0, .L80152AD4
    /* D5B8C 80152A9C 3C01801A */   lui       $at, %hi(D_801998C8)
    /* D5B90 80152AA0 8C430024 */  lw         $v1, 0x24($v0)
    /* D5B94 80152AA4 27397D5C */  addiu      $t9, $t9, %lo(.L80147D5C)
    /* D5B98 80152AA8 AFAA002C */  sw         $t2, 0x2C($sp)
    /* D5B9C 80152AAC AFA80030 */  sw         $t0, 0x30($sp)
    /* D5BA0 80152AB0 AFA60034 */  sw         $a2, 0x34($sp)
    /* D5BA4 80152AB4 0320F809 */  jalr       $t9
    /* D5BA8 80152AB8 24640050 */   addiu     $a0, $v1, 0x50
    /* D5BAC 80152ABC 8FA80030 */  lw         $t0, 0x30($sp)
    /* D5BB0 80152AC0 8FA60034 */  lw         $a2, 0x34($sp)
    /* D5BB4 80152AC4 8FAA002C */  lw         $t2, 0x2C($sp)
    /* D5BB8 80152AC8 26100001 */  addiu      $s0, $s0, 0x1
    /* D5BBC 80152ACC 10000003 */  b          .L80152ADC
    /* D5BC0 80152AD0 E5000000 */   swc1      $fv0, 0x0($t0)
  .L80152AD4:
    /* D5BC4 80152AD4 C42498C8 */  lwc1       $ft0, %lo(D_801998C8)($at)
    /* D5BC8 80152AD8 E5040000 */  swc1       $ft0, 0x0($t0)
  .L80152ADC:
    /* D5BCC 80152ADC 25080004 */  addiu      $t0, $t0, 0x4
    /* D5BD0 80152AE0 27AD0068 */  addiu      $t5, $sp, 0x68
    /* D5BD4 80152AE4 010D082B */  sltu       $at, $t0, $t5
    /* D5BD8 80152AE8 1420FFE8 */  bnez       $at, .L80152A8C
    /* D5BDC 80152AEC 254A0004 */   addiu     $t2, $t2, 0x4
    /* D5BE0 80152AF0 12000052 */  beqz       $s0, .L80152C3C
    /* D5BE4 80152AF4 00004825 */   or        $t1, $zero, $zero
    /* D5BE8 80152AF8 27A70064 */  addiu      $a3, $sp, 0x64
    /* D5BEC 80152AFC C7A6004C */  lwc1       $ft1, 0x4C($sp)
  .L80152B00:
    /* D5BF0 80152B00 C7A80048 */  lwc1       $ft2, 0x48($sp)
    /* D5BF4 80152B04 25290001 */  addiu      $t1, $t1, 0x1
    /* D5BF8 80152B08 29210007 */  slti       $at, $t1, 0x7
    /* D5BFC 80152B0C 4608303C */  c.lt.s     $ft1, $ft2
    /* D5C00 80152B10 24050004 */  addiu      $a1, $zero, 0x4
    /* D5C04 80152B14 27A3004C */  addiu      $v1, $sp, 0x4C
    /* D5C08 80152B18 45000007 */  bc1f       .L80152B38
    /* D5C0C 80152B1C 00000000 */   nop
endlabel func_80152A14
