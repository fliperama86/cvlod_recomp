nonmatching func_80152C88, 0x84

glabel func_80152C88
    /* D5D78 80152C88 27BDFFD0 */  addiu      $sp, $sp, -0x30
    /* D5D7C 80152C8C 3C198000 */  lui        $t9, %hi(func_80002560)
    /* D5D80 80152C90 AFBF0014 */  sw         $ra, 0x14($sp)
    /* D5D84 80152C94 AFA40030 */  sw         $a0, 0x30($sp)
    /* D5D88 80152C98 27392560 */  addiu      $t9, $t9, %lo(func_80002560)
    /* D5D8C 80152C9C 240401C8 */  addiu      $a0, $zero, 0x1C8
    /* D5D90 80152CA0 0320F809 */  jalr       $t9
    /* D5D94 80152CA4 AFA0001C */   sw        $zero, 0x1C($sp)
    /* D5D98 80152CA8 14400003 */  bnez       $v0, .L80152CB8
    /* D5D9C 80152CAC 8FA6001C */   lw        $a2, 0x1C($sp)
    /* D5DA0 80152CB0 10000012 */  b          .L80152CFC
    /* D5DA4 80152CB4 24020200 */   addiu     $v0, $zero, 0x200
  .L80152CB8:
    /* D5DA8 80152CB8 00002025 */  or         $a0, $zero, $zero
    /* D5DAC 80152CBC 24430034 */  addiu      $v1, $v0, 0x34
    /* D5DB0 80152CC0 24050020 */  addiu      $a1, $zero, 0x20
  .L80152CC4:
    /* D5DB4 80152CC4 8C6E0000 */  lw         $t6, 0x0($v1)
    /* D5DB8 80152CC8 24840004 */  addiu      $a0, $a0, 0x4
    /* D5DBC 80152CCC 15C00005 */  bnez       $t6, .L80152CE4
    /* D5DC0 80152CD0 00000000 */   nop
    /* D5DC4 80152CD4 8FAF0030 */  lw         $t7, 0x30($sp)
    /* D5DC8 80152CD8 24060001 */  addiu      $a2, $zero, 0x1
    /* D5DCC 80152CDC 10000003 */  b          .L80152CEC
    /* D5DD0 80152CE0 AC6F0000 */   sw        $t7, 0x0($v1)
  .L80152CE4:
    /* D5DD4 80152CE4 1485FFF7 */  bne        $a0, $a1, .L80152CC4
    /* D5DD8 80152CE8 24630004 */   addiu     $v1, $v1, 0x4
  .L80152CEC:
    /* D5DDC 80152CEC 14C00003 */  bnez       $a2, .L80152CFC
    /* D5DE0 80152CF0 00001025 */   or        $v0, $zero, $zero
    /* D5DE4 80152CF4 10000001 */  b          .L80152CFC
    /* D5DE8 80152CF8 24020200 */   addiu     $v0, $zero, 0x200
  .L80152CFC:
    /* D5DEC 80152CFC 8FBF0014 */  lw         $ra, 0x14($sp)
    /* D5DF0 80152D00 27BD0030 */  addiu      $sp, $sp, 0x30
    /* D5DF4 80152D04 03E00008 */  jr         $ra
    /* D5DF8 80152D08 00000000 */   nop
endlabel func_80152C88
