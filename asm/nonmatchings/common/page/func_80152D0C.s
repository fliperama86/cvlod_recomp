nonmatching func_80152D0C, 0x64

glabel func_80152D0C
    /* D5DFC 80152D0C 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* D5E00 80152D10 3C198000 */  lui        $t9, %hi(func_80002560)
    /* D5E04 80152D14 AFBF0014 */  sw         $ra, 0x14($sp)
    /* D5E08 80152D18 00802825 */  or         $a1, $a0, $zero
    /* D5E0C 80152D1C 27392560 */  addiu      $t9, $t9, %lo(func_80002560)
    /* D5E10 80152D20 AFA50018 */  sw         $a1, 0x18($sp)
    /* D5E14 80152D24 0320F809 */  jalr       $t9
    /* D5E18 80152D28 240401C8 */   addiu     $a0, $zero, 0x1C8
    /* D5E1C 80152D2C 1040000C */  beqz       $v0, .L80152D60
    /* D5E20 80152D30 8FA50018 */   lw        $a1, 0x18($sp)
    /* D5E24 80152D34 00002025 */  or         $a0, $zero, $zero
    /* D5E28 80152D38 24430034 */  addiu      $v1, $v0, 0x34
    /* D5E2C 80152D3C 24060020 */  addiu      $a2, $zero, 0x20
  .L80152D40:
    /* D5E30 80152D40 8C6E0000 */  lw         $t6, 0x0($v1)
    /* D5E34 80152D44 24840004 */  addiu      $a0, $a0, 0x4
    /* D5E38 80152D48 14AE0003 */  bne        $a1, $t6, .L80152D58
    /* D5E3C 80152D4C 00000000 */   nop
    /* D5E40 80152D50 10000003 */  b          .L80152D60
    /* D5E44 80152D54 AC600000 */   sw        $zero, 0x0($v1)
  .L80152D58:
    /* D5E48 80152D58 1486FFF9 */  bne        $a0, $a2, .L80152D40
    /* D5E4C 80152D5C 24630004 */   addiu     $v1, $v1, 0x4
  .L80152D60:
    /* D5E50 80152D60 8FBF0014 */  lw         $ra, 0x14($sp)
    /* D5E54 80152D64 27BD0018 */  addiu      $sp, $sp, 0x18
    /* D5E58 80152D68 03E00008 */  jr         $ra
    /* D5E5C 80152D6C 00000000 */   nop
endlabel func_80152D0C
