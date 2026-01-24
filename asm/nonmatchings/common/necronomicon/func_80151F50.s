nonmatching func_80151F50, 0xA0

glabel func_80151F50
    /* D5040 80151F50 E7AA003C */  swc1       $ft3, 0x3C($sp)
    /* D5044 80151F54 8FAE0048 */  lw         $t6, 0x48($sp)
    /* D5048 80151F58 A7A2002A */  sh         $v0, 0x2A($sp)
    /* D504C 80151F5C 3C19800A */  lui        $t9, %hi(func_800A3A50)
    /* D5050 80151F60 85C4005E */  lh         $a0, 0x5E($t6)
    /* D5054 80151F64 27393A50 */  addiu      $t9, $t9, %lo(func_800A3A50)
    /* D5058 80151F68 0320F809 */  jalr       $t9
    /* D505C 80151F6C 3084FFFF */   andi      $a0, $a0, 0xFFFF
    /* D5060 80151F70 87B8002A */  lh         $t8, 0x2A($sp)
    /* D5064 80151F74 3C014248 */  lui        $at, (0x42480000 >> 16)
    /* D5068 80151F78 44816000 */  mtc1       $at, $fa0
    /* D506C 80151F7C 8FAF0044 */  lw         $t7, 0x44($sp)
    /* D5070 80151F80 44989000 */  mtc1       $t8, $ft5
    /* D5074 80151F84 3C014700 */  lui        $at, (0x47000000 >> 16)
    /* D5078 80151F88 44817000 */  mtc1       $at, $fa1
    /* D507C 80151F8C 3C014100 */  lui        $at, (0x41000000 >> 16)
    /* D5080 80151F90 468091A0 */  cvt.s.w    $ft1, $ft5
    /* D5084 80151F94 44812000 */  mtc1       $at, $ft0
    /* D5088 80151F98 C5F00034 */  lwc1       $ft4, 0x34($t7)
    /* D508C 80151F9C 8FA80048 */  lw         $t0, 0x48($sp)
    /* D5090 80151FA0 8FA30050 */  lw         $v1, 0x50($sp)
    /* D5094 80151FA4 46048202 */  mul.s      $ft2, $ft4, $ft0
    /* D5098 80151FA8 44822000 */  mtc1       $v0, $ft0
    /* D509C 80151FAC 460E3283 */  div.s      $ft3, $ft1, $fa1
    /* D50A0 80151FB0 468024A0 */  cvt.s.w    $ft5, $ft0
    /* D50A4 80151FB4 460E9183 */  div.s      $ft1, $ft5, $fa1
    /* D50A8 80151FB8 C7B20038 */  lwc1       $ft5, 0x38($sp)
    /* D50AC 80151FBC 460A4402 */  mul.s      $ft4, $ft2, $ft3
    /* D50B0 80151FC0 C50A0058 */  lwc1       $ft3, 0x58($t0)
    /* D50B4 80151FC4 46103202 */  mul.s      $ft2, $ft1, $ft4
    /* D50B8 80151FC8 460A4100 */  add.s      $ft0, $ft2, $ft3
    /* D50BC 80151FCC E7A40040 */  swc1       $ft0, 0x40($sp)
    /* D50C0 80151FD0 C4600004 */  lwc1       $fv0, 0x4($v1)
    /* D50C4 80151FD4 460C0081 */  sub.s      $fv1, $fv0, $fa0
    /* D50C8 80151FD8 4602903C */  c.lt.s     $ft5, $fv1
    /* D50CC 80151FDC 00000000 */  nop
    /* D50D0 80151FE0 45020004 */  bc1fl      .L80151FF4
    /* D50D4 80151FE4 460C0080 */   add.s     $fv1, $fv0, $fa0
    /* D50D8 80151FE8 1000000C */  b          .L8015201C
    /* D50DC 80151FEC E6220050 */   swc1      $fv1, 0x50($s1)
endlabel func_80151F50
