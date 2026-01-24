nonmatching func_8015229C, 0x84

glabel func_8015229C
    /* D538C 8015229C 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* D5390 801522A0 AFBF0014 */  sw         $ra, 0x14($sp)
    /* D5394 801522A4 948E0044 */  lhu        $t6, 0x44($a0)
    /* D5398 801522A8 8C820024 */  lw         $v0, 0x24($a0)
    /* D539C 801522AC 3C01801A */  lui        $at, %hi(D_801998B0)
    /* D53A0 801522B0 31CFCFFF */  andi       $t7, $t6, 0xCFFF
    /* D53A4 801522B4 A48F0044 */  sh         $t7, 0x44($a0)
    /* D53A8 801522B8 C42698B0 */  lwc1       $ft1, %lo(D_801998B0)($at)
    /* D53AC 801522BC C4440054 */  lwc1       $ft0, 0x54($v0)
    /* D53B0 801522C0 3C014080 */  lui        $at, (0x40800000 >> 16)
    /* D53B4 801522C4 44819000 */  mtc1       $at, $ft5
    /* D53B8 801522C8 46062200 */  add.s      $ft2, $ft0, $ft1
    /* D53BC 801522CC 00803025 */  or         $a2, $a0, $zero
    /* D53C0 801522D0 E4480054 */  swc1       $ft2, 0x54($v0)
    /* D53C4 801522D4 C4900050 */  lwc1       $ft4, 0x50($a0)
    /* D53C8 801522D8 C44A0054 */  lwc1       $ft3, 0x54($v0)
    /* D53CC 801522DC 46128100 */  add.s      $ft0, $ft4, $ft5
    /* D53D0 801522E0 460A203E */  c.le.s     $ft0, $ft3
    /* D53D4 801522E4 00000000 */  nop
    /* D53D8 801522E8 4502000A */  bc1fl      .L80152314
    /* D53DC 801522EC 8FBF0014 */   lw        $ra, 0x14($sp)
    /* D53E0 801522F0 0C0553E9 */  jal        func_80154FA4
    /* D53E4 801522F4 AFA60018 */   sw        $a2, 0x18($sp)
    /* D53E8 801522F8 8FA60018 */  lw         $a2, 0x18($sp)
    /* D53EC 801522FC 3C198000 */  lui        $t9, %hi(func_80001CE8)
    /* D53F0 80152300 27391CE8 */  addiu      $t9, $t9, %lo(func_80001CE8)
    /* D53F4 80152304 24C40008 */  addiu      $a0, $a2, 0x8
    /* D53F8 80152308 0320F809 */  jalr       $t9
    /* D53FC 8015230C 24C5000E */   addiu     $a1, $a2, 0xE
    /* D5400 80152310 8FBF0014 */  lw         $ra, 0x14($sp)
  .L80152314:
    /* D5404 80152314 27BD0018 */  addiu      $sp, $sp, 0x18
    /* D5408 80152318 03E00008 */  jr         $ra
    /* D540C 8015231C 00000000 */   nop
endlabel func_8015229C
