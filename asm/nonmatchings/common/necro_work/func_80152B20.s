nonmatching func_80152B20, 0xB0

glabel func_80152B20
    /* D5C10 80152B20 E7A60048 */  swc1       $ft1, 0x48($sp)
    /* D5C14 80152B24 E7A8004C */  swc1       $ft2, 0x4C($sp)
    /* D5C18 80152B28 8CCE0004 */  lw         $t6, 0x4($a2)
    /* D5C1C 80152B2C 8CC40000 */  lw         $a0, 0x0($a2)
    /* D5C20 80152B30 ACCE0000 */  sw         $t6, 0x0($a2)
    /* D5C24 80152B34 ACC40004 */  sw         $a0, 0x4($a2)
  .L80152B38:
    /* D5C28 80152B38 C4600004 */  lwc1       $fv0, 0x4($v1)
    /* D5C2C 80152B3C C46C0000 */  lwc1       $fa0, 0x0($v1)
    /* D5C30 80152B40 00C51021 */  addu       $v0, $a2, $a1
    /* D5C34 80152B44 460C003C */  c.lt.s     $fv0, $fa0
    /* D5C38 80152B48 00000000 */  nop
    /* D5C3C 80152B4C 45020009 */  bc1fl      .L80152B74
    /* D5C40 80152B50 C46C0008 */   lwc1      $fa0, 0x8($v1)
    /* D5C44 80152B54 E4600000 */  swc1       $fv0, 0x0($v1)
    /* D5C48 80152B58 E46C0004 */  swc1       $fa0, 0x4($v1)
    /* D5C4C 80152B5C 8C440000 */  lw         $a0, 0x0($v0)
    /* D5C50 80152B60 8C4F0004 */  lw         $t7, 0x4($v0)
    /* D5C54 80152B64 AC440004 */  sw         $a0, 0x4($v0)
    /* D5C58 80152B68 AC4F0000 */  sw         $t7, 0x0($v0)
    /* D5C5C 80152B6C C4600004 */  lwc1       $fv0, 0x4($v1)
    /* D5C60 80152B70 C46C0008 */  lwc1       $fa0, 0x8($v1)
  .L80152B74:
    /* D5C64 80152B74 00C51021 */  addu       $v0, $a2, $a1
    /* D5C68 80152B78 4600603C */  c.lt.s     $fa0, $fv0
    /* D5C6C 80152B7C 00000000 */  nop
    /* D5C70 80152B80 45020008 */  bc1fl      .L80152BA4
    /* D5C74 80152B84 24630008 */   addiu     $v1, $v1, 0x8
    /* D5C78 80152B88 E46C0004 */  swc1       $fa0, 0x4($v1)
    /* D5C7C 80152B8C E4600008 */  swc1       $fv0, 0x8($v1)
    /* D5C80 80152B90 8C580008 */  lw         $t8, 0x8($v0)
    /* D5C84 80152B94 8C440004 */  lw         $a0, 0x4($v0)
    /* D5C88 80152B98 AC580004 */  sw         $t8, 0x4($v0)
    /* D5C8C 80152B9C AC440008 */  sw         $a0, 0x8($v0)
    /* D5C90 80152BA0 24630008 */  addiu      $v1, $v1, 0x8
  .L80152BA4:
    /* D5C94 80152BA4 1467FFE4 */  bne        $v1, $a3, .L80152B38
    /* D5C98 80152BA8 24A50008 */   addiu     $a1, $a1, 0x8
    /* D5C9C 80152BAC 5420FFD4 */  bnel       $at, $zero, .L80152B00
    /* D5CA0 80152BB0 C7A6004C */   lwc1      $ft1, 0x4C($sp)
    /* D5CA4 80152BB4 3C014069 */  lui        $at, (0x40690000 >> 16)
    /* D5CA8 80152BB8 00C05025 */  or         $t2, $a2, $zero
    /* D5CAC 80152BBC 3C058019 */  lui        $a1, %hi(D_80190724)
    /* D5CB0 80152BC0 44810800 */  mtc1       $at, $fv0f
    /* D5CB4 80152BC4 44800000 */  mtc1       $zero, $fv0
    /* D5CB8 80152BC8 24A50724 */  addiu      $a1, $a1, %lo(D_80190724)
    /* D5CBC 80152BCC 24060008 */  addiu      $a2, $zero, 0x8
endlabel func_80152B20
