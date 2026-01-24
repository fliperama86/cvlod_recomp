nonmatching func_8015F90C, 0x3A4

glabel func_8015F90C
    /* E29FC 8015F90C 27BDFF90 */  addiu      $sp, $sp, -0x70
    /* E2A00 8015F910 AFBF0024 */  sw         $ra, 0x24($sp)
    /* E2A04 8015F914 AFB20020 */  sw         $s2, 0x20($sp)
    /* E2A08 8015F918 AFB1001C */  sw         $s1, 0x1C($sp)
    /* E2A0C 8015F91C AFB00018 */  sw         $s0, 0x18($sp)
    /* E2A10 8015F920 8C8E0034 */  lw         $t6, 0x34($a0)
    /* E2A14 8015F924 8C830070 */  lw         $v1, 0x70($a0)
    /* E2A18 8015F928 3C0F801D */  lui        $t7, %hi(D_801CAB90)
    /* E2A1C 8015F92C 85EFAB90 */  lh         $t7, %lo(D_801CAB90)($t7)
    /* E2A20 8015F930 AFAE0068 */  sw         $t6, 0x68($sp)
    /* E2A24 8015F934 94690018 */  lhu        $t1, 0x18($v1)
    /* E2A28 8015F938 3C198019 */  lui        $t9, %hi(D_80191A44)
    /* E2A2C 8015F93C 000FC080 */  sll        $t8, $t7, 2
    /* E2A30 8015F940 0338C821 */  addu       $t9, $t9, $t8
    /* E2A34 8015F944 8F391A44 */  lw         $t9, %lo(D_80191A44)($t9)
    /* E2A38 8015F948 00095080 */  sll        $t2, $t1, 2
    /* E2A3C 8015F94C 01495023 */  subu       $t2, $t2, $t1
    /* E2A40 8015F950 000A5080 */  sll        $t2, $t2, 2
    /* E2A44 8015F954 032A8021 */  addu       $s0, $t9, $t2
    /* E2A48 8015F958 960B0000 */  lhu        $t3, 0x0($s0)
    /* E2A4C 8015F95C 3C0D8019 */  lui        $t5, %hi(D_801916D4)
    /* E2A50 8015F960 25AD16D4 */  addiu      $t5, $t5, %lo(D_801916D4)
    /* E2A54 8015F964 000B6080 */  sll        $t4, $t3, 2
    /* E2A58 8015F968 018B6023 */  subu       $t4, $t4, $t3
    /* E2A5C 8015F96C 000C6100 */  sll        $t4, $t4, 4
    /* E2A60 8015F970 8C860028 */  lw         $a2, 0x28($a0)
    /* E2A64 8015F974 8C910024 */  lw         $s1, 0x24($a0)
    /* E2A68 8015F978 018D7021 */  addu       $t6, $t4, $t5
    /* E2A6C 8015F97C AFAE0050 */  sw         $t6, 0x50($sp)
    /* E2A70 8015F980 8E020008 */  lw         $v0, 0x8($s0)
    /* E2A74 8015F984 00809025 */  or         $s2, $a0, $zero
    /* E2A78 8015F988 50400017 */  beql       $v0, $zero, .L8015F9E8
    /* E2A7C 8015F98C 862F0000 */   lh        $t7, 0x0($s1)
    /* E2A80 8015F990 1040000B */  beqz       $v0, .L8015F9C0
    /* E2A84 8015F994 00001825 */   or        $v1, $zero, $zero
    /* E2A88 8015F998 3C198000 */  lui        $t9, %hi(func_800048A0)
    /* E2A8C 8015F99C 273948A0 */  addiu      $t9, $t9, %lo(func_800048A0)
    /* E2A90 8015F9A0 3C04801D */  lui        $a0, %hi(D_801CAA60)
    /* E2A94 8015F9A4 2484AA60 */  addiu      $a0, $a0, %lo(D_801CAA60)
    /* E2A98 8015F9A8 00402825 */  or         $a1, $v0, $zero
    /* E2A9C 8015F9AC 0320F809 */  jalr       $t9
    /* E2AA0 8015F9B0 AFA60058 */   sw        $a2, 0x58($sp)
    /* E2AA4 8015F9B4 8FA60058 */  lw         $a2, 0x58($sp)
    /* E2AA8 8015F9B8 10000001 */  b          .L8015F9C0
    /* E2AAC 8015F9BC 00401825 */   or        $v1, $v0, $zero
  .L8015F9C0:
    /* E2AB0 8015F9C0 10600008 */  beqz       $v1, .L8015F9E4
    /* E2AB4 8015F9C4 26440008 */   addiu     $a0, $s2, 0x8
    /* E2AB8 8015F9C8 3C198000 */  lui        $t9, %hi(func_80001E30)
    /* E2ABC 8015F9CC 27391E30 */  addiu      $t9, $t9, %lo(func_80001E30)
    /* E2AC0 8015F9D0 2645000E */  addiu      $a1, $s2, 0xE
    /* E2AC4 8015F9D4 0320F809 */  jalr       $t9
    /* E2AC8 8015F9D8 24060003 */   addiu     $a2, $zero, 0x3
    /* E2ACC 8015F9DC 100000D5 */  b          .L8015FD34
    /* E2AD0 8015F9E0 8FBF0024 */   lw        $ra, 0x24($sp)
  .L8015F9E4:
    /* E2AD4 8015F9E4 862F0000 */  lh         $t7, 0x0($s1)
  .L8015F9E8:
    /* E2AD8 8015F9E8 02402025 */  or         $a0, $s2, $zero
    /* E2ADC 8015F9EC 02202825 */  or         $a1, $s1, $zero
    /* E2AE0 8015F9F0 35F80200 */  ori        $t8, $t7, 0x200
    /* E2AE4 8015F9F4 A6380000 */  sh         $t8, 0x0($s1)
    /* E2AE8 8015F9F8 0C050C73 */  jal        func_801431CC
    /* E2AEC 8015F9FC AFA60058 */   sw        $a2, 0x58($sp)
    /* E2AF0 8015FA00 8FA30050 */  lw         $v1, 0x50($sp)
    /* E2AF4 8015FA04 3C02800A */  lui        $v0, %hi(func_800993C0)
    /* E2AF8 8015FA08 244293C0 */  addiu      $v0, $v0, %lo(func_800993C0)
    /* E2AFC 8015FA0C 8469001C */  lh         $t1, 0x1C($v1)
    /* E2B00 8015FA10 AE290040 */  sw         $t1, 0x40($s1)
    /* E2B04 8015FA14 8C64000C */  lw         $a0, 0xC($v1)
    /* E2B08 8015FA18 0040F809 */  jalr       $v0
    /* E2B0C 8015FA1C AFA20030 */   sw        $v0, 0x30($sp)
    /* E2B10 8015FA20 AE220038 */  sw         $v0, 0x38($s1)
    /* E2B14 8015FA24 8FAA0050 */  lw         $t2, 0x50($sp)
    /* E2B18 8015FA28 962C0002 */  lhu        $t4, 0x2($s1)
    /* E2B1C 8015FA2C 3C07801D */  lui        $a3, %hi(D_801C82C0)
    /* E2B20 8015FA30 8D450014 */  lw         $a1, 0x14($t2)
    /* E2B24 8015FA34 358D0900 */  ori        $t5, $t4, 0x900
    /* E2B28 8015FA38 24E782C0 */  addiu      $a3, $a3, %lo(D_801C82C0)
    /* E2B2C 8015FA3C A62D0002 */  sh         $t5, 0x2($s1)
    /* E2B30 8015FA40 AE25003C */  sw         $a1, 0x3C($s1)
    /* E2B34 8015FA44 8CEE2B0C */  lw         $t6, 0x2B0C($a3)
    /* E2B38 8015FA48 240F00FF */  addiu      $t7, $zero, 0xFF
    /* E2B3C 8015FA4C 3C038014 */  lui        $v1, %hi(func_8014798C + 0x28)
    /* E2B40 8015FA50 AE2E0018 */  sw         $t6, 0x18($s1)
    /* E2B44 8015FA54 8CF9007C */  lw         $t9, 0x7C($a3)
    /* E2B48 8015FA58 A22F001B */  sb         $t7, 0x1B($s1)
    /* E2B4C 8015FA5C 246379B4 */  addiu      $v1, $v1, %lo(func_8014798C + 0x28)
    /* E2B50 8015FA60 AE390024 */  sw         $t9, 0x24($s1)
    /* E2B54 8015FA64 AFA3002C */  sw         $v1, 0x2C($sp)
    /* E2B58 8015FA68 0060F809 */  jalr       $v1
    /* E2B5C 8015FA6C 02202025 */   or        $a0, $s1, $zero
    /* E2B60 8015FA70 8FA50058 */  lw         $a1, 0x58($sp)
    /* E2B64 8015FA74 0C050C73 */  jal        func_801431CC
    /* E2B68 8015FA78 02402025 */   or        $a0, $s2, $zero
    /* E2B6C 8015FA7C 8FA20050 */  lw         $v0, 0x50($sp)
    /* E2B70 8015FA80 8FA60058 */  lw         $a2, 0x58($sp)
    /* E2B74 8015FA84 8458001C */  lh         $t8, 0x1C($v0)
    /* E2B78 8015FA88 ACD80040 */  sw         $t8, 0x40($a2)
    /* E2B7C 8015FA8C 8FB90030 */  lw         $t9, 0x30($sp)
    /* E2B80 8015FA90 8C44000C */  lw         $a0, 0xC($v0)
    /* E2B84 8015FA94 0320F809 */  jalr       $t9
    /* E2B88 8015FA98 00000000 */   nop
    /* E2B8C 8015FA9C 8FA40058 */  lw         $a0, 0x58($sp)
    /* E2B90 8015FAA0 3C0D801D */  lui        $t5, %hi(D_801CADCC)
    /* E2B94 8015FAA4 3C0E801D */  lui        $t6, %hi(D_801C833C)
    /* E2B98 8015FAA8 AC820038 */  sw         $v0, 0x38($a0)
    /* E2B9C 8015FAAC 8FA90050 */  lw         $t1, 0x50($sp)
    /* E2BA0 8015FAB0 948B0002 */  lhu        $t3, 0x2($a0)
    /* E2BA4 8015FAB4 240F00FF */  addiu      $t7, $zero, 0xFF
    /* E2BA8 8015FAB8 8D250018 */  lw         $a1, 0x18($t1)
    /* E2BAC 8015FABC 356C0940 */  ori        $t4, $t3, 0x940
    /* E2BB0 8015FAC0 A48C0002 */  sh         $t4, 0x2($a0)
    /* E2BB4 8015FAC4 AC85003C */  sw         $a1, 0x3C($a0)
    /* E2BB8 8015FAC8 8DADADCC */  lw         $t5, %lo(D_801CADCC)($t5)
    /* E2BBC 8015FACC AC8D0018 */  sw         $t5, 0x18($a0)
    /* E2BC0 8015FAD0 8DCE833C */  lw         $t6, %lo(D_801C833C)($t6)
    /* E2BC4 8015FAD4 A08F001B */  sb         $t7, 0x1B($a0)
    /* E2BC8 8015FAD8 AC8E0024 */  sw         $t6, 0x24($a0)
    /* E2BCC 8015FADC 8FB9002C */  lw         $t9, 0x2C($sp)
    /* E2BD0 8015FAE0 0320F809 */  jalr       $t9
    /* E2BD4 8015FAE4 00000000 */   nop
    /* E2BD8 8015FAE8 3C198000 */  lui        $t9, %hi(func_80001CE8)
    /* E2BDC 8015FAEC 27391CE8 */  addiu      $t9, $t9, %lo(func_80001CE8)
    /* E2BE0 8015FAF0 26440008 */  addiu      $a0, $s2, 0x8
    /* E2BE4 8015FAF4 2645000E */  addiu      $a1, $s2, 0xE
    /* E2BE8 8015FAF8 AFA50028 */  sw         $a1, 0x28($sp)
    /* E2BEC 8015FAFC 0320F809 */  jalr       $t9
    /* E2BF0 8015FB00 AFA4002C */   sw        $a0, 0x2C($sp)
    /* E2BF4 8015FB04 8E020008 */  lw         $v0, 0x8($s0)
    /* E2BF8 8015FB08 3C04801D */  lui        $a0, %hi(D_801CAA60)
    /* E2BFC 8015FB0C 2484AA60 */  addiu      $a0, $a0, %lo(D_801CAA60)
    /* E2C00 8015FB10 10400007 */  beqz       $v0, .L8015FB30
    /* E2C04 8015FB14 00001825 */   or        $v1, $zero, $zero
    /* E2C08 8015FB18 3C198000 */  lui        $t9, %hi(func_800048A0)
    /* E2C0C 8015FB1C 273948A0 */  addiu      $t9, $t9, %lo(func_800048A0)
    /* E2C10 8015FB20 0320F809 */  jalr       $t9
    /* E2C14 8015FB24 00402825 */   or        $a1, $v0, $zero
    /* E2C18 8015FB28 10000001 */  b          .L8015FB30
    /* E2C1C 8015FB2C 00401825 */   or        $v1, $v0, $zero
  .L8015FB30:
    /* E2C20 8015FB30 10600008 */  beqz       $v1, .L8015FB54
    /* E2C24 8015FB34 8FA4002C */   lw        $a0, 0x2C($sp)
    /* E2C28 8015FB38 3C198000 */  lui        $t9, %hi(func_80001E30)
    /* E2C2C 8015FB3C 27391E30 */  addiu      $t9, $t9, %lo(func_80001E30)
    /* E2C30 8015FB40 8FA50028 */  lw         $a1, 0x28($sp)
    /* E2C34 8015FB44 0320F809 */  jalr       $t9
    /* E2C38 8015FB48 24060003 */   addiu     $a2, $zero, 0x3
    /* E2C3C 8015FB4C 10000079 */  b          .L8015FD34
    /* E2C40 8015FB50 8FBF0024 */   lw        $ra, 0x24($sp)
  .L8015FB54:
    /* E2C44 8015FB54 8E020008 */  lw         $v0, 0x8($s0)
    /* E2C48 8015FB58 3C04801D */  lui        $a0, %hi(D_801CAA60)
    /* E2C4C 8015FB5C 2484AA60 */  addiu      $a0, $a0, %lo(D_801CAA60)
    /* E2C50 8015FB60 10400007 */  beqz       $v0, .L8015FB80
    /* E2C54 8015FB64 00001825 */   or        $v1, $zero, $zero
    /* E2C58 8015FB68 3C198000 */  lui        $t9, %hi(func_800048A0)
    /* E2C5C 8015FB6C 273948A0 */  addiu      $t9, $t9, %lo(func_800048A0)
    /* E2C60 8015FB70 0320F809 */  jalr       $t9
    /* E2C64 8015FB74 00402825 */   or        $a1, $v0, $zero
    /* E2C68 8015FB78 10000001 */  b          .L8015FB80
    /* E2C6C 8015FB7C 00401825 */   or        $v1, $v0, $zero
  .L8015FB80:
    /* E2C70 8015FB80 10600003 */  beqz       $v1, .L8015FB90
  alabel D_8015FB84
    /* E2C74 8015FB84 02202025 */   or        $a0, $s1, $zero
    /* E2C78 8015FB88 10000009 */  b          .L8015FBB0
    /* E2C7C 8015FB8C 24020008 */   addiu     $v0, $zero, 0x8
  .L8015FB90:
    /* E2C80 8015FB90 8FB80050 */  lw         $t8, 0x50($sp)
    /* E2C84 8015FB94 2402000A */  addiu      $v0, $zero, 0xA
    /* E2C88 8015FB98 97090024 */  lhu        $t1, 0x24($t8)
    /* E2C8C 8015FB9C 312A0004 */  andi       $t2, $t1, 0x4
    /* E2C90 8015FBA0 11400003 */  beqz       $t2, .L8015FBB0
    /* E2C94 8015FBA4 00000000 */   nop
    /* E2C98 8015FBA8 10000001 */  b          .L8015FBB0
    /* E2C9C 8015FBAC 24020002 */   addiu     $v0, $zero, 0x2
  .L8015FBB0:
    /* E2CA0 8015FBB0 3C198004 */  lui        $t9, %hi(func_80040724)
    /* E2CA4 8015FBB4 27390724 */  addiu      $t9, $t9, %lo(func_80040724)
    /* E2CA8 8015FBB8 00023200 */  sll        $a2, $v0, 8
    /* E2CAC 8015FBBC 30C6FFFF */  andi       $a2, $a2, 0xFFFF
    /* E2CB0 8015FBC0 0320F809 */  jalr       $t9
    /* E2CB4 8015FBC4 24050001 */   addiu     $a1, $zero, 0x1
    /* E2CB8 8015FBC8 10400027 */  beqz       $v0, .L8015FC68
    /* E2CBC 8015FBCC 24090003 */   addiu     $t1, $zero, 0x3
    /* E2CC0 8015FBD0 8FAB0068 */  lw         $t3, 0x68($sp)
    /* E2CC4 8015FBD4 8FA40050 */  lw         $a0, 0x50($sp)
    /* E2CC8 8015FBD8 44800000 */  mtc1       $zero, $fv0
    /* E2CCC 8015FBDC AD620004 */  sw         $v0, 0x4($t3)
    /* E2CD0 8015FBE0 944C000A */  lhu        $t4, 0xA($v0)
    /* E2CD4 8015FBE4 24030001 */  addiu      $v1, $zero, 0x1
    /* E2CD8 8015FBE8 240B03E8 */  addiu      $t3, $zero, 0x3E8
  alabel D_8015FBEC
    /* E2CDC 8015FBEC 318EF9FF */  andi       $t6, $t4, 0xF9FF
    /* E2CE0 8015FBF0 35D80200 */  ori        $t8, $t6, 0x200
    /* E2CE4 8015FBF4 A44E000A */  sh         $t6, 0xA($v0)
    /* E2CE8 8015FBF8 330AFFFC */  andi       $t2, $t8, 0xFFFC
    /* E2CEC 8015FBFC A458000A */  sh         $t8, 0xA($v0)
    /* E2CF0 8015FC00 A44A000A */  sh         $t2, 0xA($v0)
    /* E2CF4 8015FC04 35590003 */  ori        $t9, $t2, 0x3
    /* E2CF8 8015FC08 A043000D */  sb         $v1, 0xD($v0)
    /* E2CFC 8015FC0C A459000A */  sh         $t9, 0xA($v0)
    /* E2D00 8015FC10 A4430004 */  sh         $v1, 0x4($v0)
    /* E2D04 8015FC14 A44B0008 */  sh         $t3, 0x8($v0)
    /* E2D08 8015FC18 A440004E */  sh         $zero, 0x4E($v0)
    /* E2D0C 8015FC1C A4400050 */  sh         $zero, 0x50($v0)
    /* E2D10 8015FC20 A4400052 */  sh         $zero, 0x52($v0)
    /* E2D14 8015FC24 E4400054 */  swc1       $fv0, 0x54($v0)
    /* E2D18 8015FC28 848C001E */  lh         $t4, 0x1E($a0)
    /* E2D1C 8015FC2C E440005C */  swc1       $fv0, 0x5C($v0)
    /* E2D20 8015FC30 448C2000 */  mtc1       $t4, $ft0
  alabel D_8015FC34
    /* E2D24 8015FC34 00000000 */  nop
    /* E2D28 8015FC38 468021A0 */  cvt.s.w    $ft1, $ft0
    /* E2D2C 8015FC3C E4460058 */  swc1       $ft1, 0x58($v0)
    /* E2D30 8015FC40 848D0020 */  lh         $t5, 0x20($a0)
    /* E2D34 8015FC44 448D4000 */  mtc1       $t5, $ft2
    /* E2D38 8015FC48 00000000 */  nop
    /* E2D3C 8015FC4C 468042A0 */  cvt.s.w    $ft3, $ft2
    /* E2D40 8015FC50 E44A0060 */  swc1       $ft3, 0x60($v0)
    /* E2D44 8015FC54 848E0022 */  lh         $t6, 0x22($a0)
    /* E2D48 8015FC58 448E8000 */  mtc1       $t6, $ft4
    /* E2D4C 8015FC5C 00000000 */  nop
    /* E2D50 8015FC60 468084A0 */  cvt.s.w    $ft5, $ft4
    /* E2D54 8015FC64 E4520064 */  swc1       $ft5, 0x64($v0)
  .L8015FC68:
    /* E2D58 8015FC68 8FA80068 */  lw         $t0, 0x68($sp)
    /* E2D5C 8015FC6C 8E4F0010 */  lw         $t7, 0x10($s2)
    /* E2D60 8015FC70 3C188016 */  lui        $t8, %hi(D_80163338)
    /* E2D64 8015FC74 27183338 */  addiu      $t8, $t8, %lo(D_80163338)
    /* E2D68 8015FC78 AD0F0010 */  sw         $t7, 0x10($t0)
    /* E2D6C 8015FC7C AE580010 */  sw         $t8, 0x10($s2)
    /* E2D70 8015FC80 A5090024 */  sh         $t1, 0x24($t0)
    /* E2D74 8015FC84 96060002 */  lhu        $a2, 0x2($s0)
    /* E2D78 8015FC88 3C198000 */  lui        $t9, %hi(func_80002808)
    /* E2D7C 8015FC8C 27392808 */  addiu      $t9, $t9, %lo(func_80002808)
    /* E2D80 8015FC90 00C00821 */  addu       $at, $a2, $zero
    /* E2D84 8015FC94 00063080 */  sll        $a2, $a2, 2
    /* E2D88 8015FC98 00C13023 */  subu       $a2, $a2, $at
    /* E2D8C 8015FC9C 25080008 */  addiu      $t0, $t0, 0x8
    /* E2D90 8015FCA0 AFA80030 */  sw         $t0, 0x30($sp)
    /* E2D94 8015FCA4 000630C0 */  sll        $a2, $a2, 3
    /* E2D98 8015FCA8 02402025 */  or         $a0, $s2, $zero
    /* E2D9C 8015FCAC 00002825 */  or         $a1, $zero, $zero
endlabel func_8015F90C
