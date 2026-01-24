nonmatching func_801528AC, 0x168

glabel func_801528AC
    /* D599C 801528AC 27BDFFB0 */  addiu      $sp, $sp, -0x50
    /* D59A0 801528B0 AFB10018 */  sw         $s1, 0x18($sp)
    /* D59A4 801528B4 00808825 */  or         $s1, $a0, $zero
    /* D59A8 801528B8 AFBF001C */  sw         $ra, 0x1C($sp)
    /* D59AC 801528BC 10800050 */  beqz       $a0, .L80152A00
    /* D59B0 801528C0 AFB00014 */   sw        $s0, 0x14($sp)
    /* D59B4 801528C4 3C198006 */  lui        $t9, %hi(func_8005D384)
    /* D59B8 801528C8 2739D384 */  addiu      $t9, $t9, %lo(func_8005D384)
    /* D59BC 801528CC 0320F809 */  jalr       $t9
    /* D59C0 801528D0 00000000 */   nop
    /* D59C4 801528D4 1040004A */  beqz       $v0, .L80152A00
    /* D59C8 801528D8 00408025 */   or        $s0, $v0, $zero
    /* D59CC 801528DC 3C198006 */  lui        $t9, %hi(func_8005D324)
    /* D59D0 801528E0 2739D324 */  addiu      $t9, $t9, %lo(func_8005D324)
    /* D59D4 801528E4 0320F809 */  jalr       $t9
    /* D59D8 801528E8 02202025 */   or        $a0, $s1, $zero
    /* D59DC 801528EC 10400041 */  beqz       $v0, .L801529F4
    /* D59E0 801528F0 3C198006 */   lui       $t9, %hi(func_8005D0F0)
    /* D59E4 801528F4 C6040010 */  lwc1       $ft0, 0x10($s0)
    /* D59E8 801528F8 C4460054 */  lwc1       $ft1, 0x54($v0)
    /* D59EC 801528FC 4606203C */  c.lt.s     $ft0, $ft1
    /* D59F0 80152900 00000000 */  nop
    /* D59F4 80152904 4500003B */  bc1f       .L801529F4
    /* D59F8 80152908 00000000 */   nop
    /* D59FC 8015290C 96020008 */  lhu        $v0, 0x8($s0)
    /* D5A00 80152910 3C01801A */  lui        $at, %hi(D_801998B8)
    /* D5A04 80152914 D43098B8 */  ldc1       $ft4, %lo(D_801998B8)($at)
    /* D5A08 80152918 00420019 */  multu      $v0, $v0
    /* D5A0C 8015291C 3C01801A */  lui        $at, %hi(D_801998C0)
    /* D5A10 80152920 D42498C0 */  ldc1       $ft0, %lo(D_801998C0)($at)
    /* D5A14 80152924 3C013FB0 */  lui        $at, (0x3FB00000 >> 16)
    /* D5A18 80152928 3C198009 */  lui        $t9, %hi(func_80097330)
    /* D5A1C 8015292C 27397330 */  addiu      $t9, $t9, %lo(func_80097330)
    /* D5A20 80152930 00007012 */  mflo       $t6
    /* D5A24 80152934 448E4000 */  mtc1       $t6, $ft2
    /* D5A28 80152938 00000000 */  nop
    /* D5A2C 8015293C 46804020 */  cvt.s.w    $fv0, $ft2
    /* D5A30 80152940 460002A1 */  cvt.d.s    $ft3, $fv0
    /* D5A34 80152944 46305482 */  mul.d      $ft5, $ft3, $ft4
    /* D5A38 80152948 46249183 */  div.d      $ft1, $ft5, $ft0
    /* D5A3C 8015294C 44802000 */  mtc1       $zero, $ft0
    /* D5A40 80152950 44812800 */  mtc1       $at, $ft0f
    /* D5A44 80152954 46203207 */  neg.d      $ft2, $ft1
    /* D5A48 80152958 462042A0 */  cvt.s.d    $ft3, $ft2
    /* D5A4C 8015295C E7AA0040 */  swc1       $ft3, 0x40($sp)
    /* D5A50 80152960 C610000C */  lwc1       $ft4, 0xC($s0)
    /* D5A54 80152964 9604000A */  lhu        $a0, 0xA($s0)
    /* D5A58 80152968 460084A1 */  cvt.d.s    $ft5, $ft4
    /* D5A5C 8015296C 46249181 */  sub.d      $ft1, $ft5, $ft0
    /* D5A60 80152970 46203220 */  cvt.s.d    $ft2, $ft1
    /* D5A64 80152974 0320F809 */  jalr       $t9
    /* D5A68 80152978 E608000C */   swc1      $ft2, 0xC($s0)
    /* D5A6C 8015297C 44825000 */  mtc1       $v0, $ft3
    /* D5A70 80152980 3C014700 */  lui        $at, (0x47000000 >> 16)
    /* D5A74 80152984 44819000 */  mtc1       $at, $ft5
    /* D5A78 80152988 46805420 */  cvt.s.w    $ft4, $ft3
    /* D5A7C 8015298C C606000C */  lwc1       $ft1, 0xC($s0)
    /* D5A80 80152990 3C19800A */  lui        $t9, %hi(func_800A3A50)
    /* D5A84 80152994 27393A50 */  addiu      $t9, $t9, %lo(func_800A3A50)
    /* D5A88 80152998 46128103 */  div.s      $ft0, $ft4, $ft5
    /* D5A8C 8015299C 46062202 */  mul.s      $ft2, $ft0, $ft1
    /* D5A90 801529A0 E7A8003C */  swc1       $ft2, 0x3C($sp)
    /* D5A94 801529A4 0320F809 */  jalr       $t9
    /* D5A98 801529A8 9604000A */   lhu       $a0, 0xA($s0)
    /* D5A9C 801529AC 44825000 */  mtc1       $v0, $ft3
    /* D5AA0 801529B0 3C014700 */  lui        $at, (0x47000000 >> 16)
    /* D5AA4 801529B4 44819000 */  mtc1       $at, $ft5
    /* D5AA8 801529B8 46805420 */  cvt.s.w    $ft4, $ft3
    /* D5AAC 801529BC C606000C */  lwc1       $ft1, 0xC($s0)
    /* D5AB0 801529C0 3C198006 */  lui        $t9, %hi(func_8005C638)
    /* D5AB4 801529C4 2739C638 */  addiu      $t9, $t9, %lo(func_8005C638)
    /* D5AB8 801529C8 02202025 */  or         $a0, $s1, $zero
    /* D5ABC 801529CC 27A5003C */  addiu      $a1, $sp, 0x3C
    /* D5AC0 801529D0 46128103 */  div.s      $ft0, $ft4, $ft5
    /* D5AC4 801529D4 24064000 */  addiu      $a2, $zero, 0x4000
    /* D5AC8 801529D8 46062202 */  mul.s      $ft2, $ft0, $ft1
    /* D5ACC 801529DC 0320F809 */  jalr       $t9
    /* D5AD0 801529E0 E7A80044 */   swc1      $ft2, 0x44($sp)
    /* D5AD4 801529E4 960F0008 */  lhu        $t7, 0x8($s0)
    /* D5AD8 801529E8 25F80001 */  addiu      $t8, $t7, 0x1
    /* D5ADC 801529EC 10000004 */  b          .L80152A00
    /* D5AE0 801529F0 A6180008 */   sh        $t8, 0x8($s0)
  .L801529F4:
    /* D5AE4 801529F4 2739D0F0 */  addiu      $t9, $t9, %lo(func_8005D0F0)
    /* D5AE8 801529F8 0320F809 */  jalr       $t9
    /* D5AEC 801529FC 02202025 */   or        $a0, $s1, $zero
  .L80152A00:
    /* D5AF0 80152A00 8FBF001C */  lw         $ra, 0x1C($sp)
    /* D5AF4 80152A04 8FB00014 */  lw         $s0, 0x14($sp)
    /* D5AF8 80152A08 8FB10018 */  lw         $s1, 0x18($sp)
    /* D5AFC 80152A0C 03E00008 */  jr         $ra
    /* D5B00 80152A10 27BD0050 */   addiu     $sp, $sp, 0x50
endlabel func_801528AC
