nonmatching func_80152BD0, 0x80

glabel func_80152BD0
    /* D5CC0 80152BD0 00004825 */  or         $t1, $zero, $zero
    /* D5CC4 80152BD4 00001825 */  or         $v1, $zero, $zero
    /* D5CC8 80152BD8 27A40048 */  addiu      $a0, $sp, 0x48
  .L80152BDC:
    /* D5CCC 80152BDC 8D420000 */  lw         $v0, 0x0($t2)
    /* D5CD0 80152BE0 00835821 */  addu       $t3, $a0, $v1
    /* D5CD4 80152BE4 24630004 */  addiu      $v1, $v1, 0x4
    /* D5CD8 80152BE8 10400014 */  beqz       $v0, .L80152C3C
    /* D5CDC 80152BEC 29210003 */   slti      $at, $t1, 0x3
    /* D5CE0 80152BF0 C56A0000 */  lwc1       $ft3, 0x0($t3)
    /* D5CE4 80152BF4 00096040 */  sll        $t4, $t1, 1
    /* D5CE8 80152BF8 00403825 */  or         $a3, $v0, $zero
    /* D5CEC 80152BFC 46005421 */  cvt.d.s    $ft4, $ft3
    /* D5CF0 80152C00 00ACC821 */  addu       $t9, $a1, $t4
    /* D5CF4 80152C04 4630003C */  c.lt.d     $fv0, $ft4
    /* D5CF8 80152C08 00000000 */  nop
    /* D5CFC 80152C0C 45000003 */  bc1f       .L80152C1C
    /* D5D00 80152C10 00000000 */   nop
    /* D5D04 80152C14 10000009 */  b          .L80152C3C
    /* D5D08 80152C18 A4400058 */   sh        $zero, 0x58($v0)
  .L80152C1C:
    /* D5D0C 80152C1C 10200004 */  beqz       $at, .L80152C30
    /* D5D10 80152C20 25290001 */   addiu     $t1, $t1, 0x1
    /* D5D14 80152C24 972D0000 */  lhu        $t5, 0x0($t9)
    /* D5D18 80152C28 10000002 */  b          .L80152C34
    /* D5D1C 80152C2C A4ED0058 */   sh        $t5, 0x58($a3)
  .L80152C30:
    /* D5D20 80152C30 A4E00058 */  sh         $zero, 0x58($a3)
  .L80152C34:
    /* D5D24 80152C34 1526FFE9 */  bne        $t1, $a2, .L80152BDC
    /* D5D28 80152C38 254A0004 */   addiu     $t2, $t2, 0x4
  .L80152C3C:
    /* D5D2C 80152C3C 8FBF001C */  lw         $ra, 0x1C($sp)
    /* D5D30 80152C40 8FB00014 */  lw         $s0, 0x14($sp)
    /* D5D34 80152C44 8FB10018 */  lw         $s1, 0x18($sp)
    /* D5D38 80152C48 03E00008 */  jr         $ra
    /* D5D3C 80152C4C 27BD0080 */   addiu     $sp, $sp, 0x80
endlabel func_80152BD0
