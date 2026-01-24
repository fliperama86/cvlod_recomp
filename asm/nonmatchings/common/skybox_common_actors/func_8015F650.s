nonmatching func_8015F650, 0x6C

glabel func_8015F650
    /* E2740 8015F650 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* E2744 8015F654 3C198000 */  lui        $t9, %hi(func_80002808)
    /* E2748 8015F658 AFBF001C */  sw         $ra, 0x1C($sp)
    /* E274C 8015F65C AFB00018 */  sw         $s0, 0x18($sp)
    /* E2750 8015F660 27392808 */  addiu      $t9, $t9, %lo(func_80002808)
    /* E2754 8015F664 00808025 */  or         $s0, $a0, $zero
    /* E2758 8015F668 00002825 */  or         $a1, $zero, $zero
    /* E275C 8015F66C 24060028 */  addiu      $a2, $zero, 0x28
    /* E2760 8015F670 0320F809 */  jalr       $t9
    /* E2764 8015F674 00003825 */   or        $a3, $zero, $zero
    /* E2768 8015F678 14400007 */  bnez       $v0, .L8015F698
    /* E276C 8015F67C 26040008 */   addiu     $a0, $s0, 0x8
    /* E2770 8015F680 8E190010 */  lw         $t9, 0x10($s0)
    /* E2774 8015F684 02002025 */  or         $a0, $s0, $zero
    /* E2778 8015F688 0320F809 */  jalr       $t9
    /* E277C 8015F68C 00000000 */   nop
    /* E2780 8015F690 10000006 */  b          .L8015F6AC
    /* E2784 8015F694 8FBF001C */   lw        $ra, 0x1C($sp)
  .L8015F698:
    /* E2788 8015F698 3C198000 */  lui        $t9, %hi(func_80001CE8)
    /* E278C 8015F69C 27391CE8 */  addiu      $t9, $t9, %lo(func_80001CE8)
    /* E2790 8015F6A0 0320F809 */  jalr       $t9
    /* E2794 8015F6A4 2605000E */   addiu     $a1, $s0, 0xE
    /* E2798 8015F6A8 8FBF001C */  lw         $ra, 0x1C($sp)
  .L8015F6AC:
    /* E279C 8015F6AC 8FB00018 */  lw         $s0, 0x18($sp)
    /* E27A0 8015F6B0 27BD0020 */  addiu      $sp, $sp, 0x20
    /* E27A4 8015F6B4 03E00008 */  jr         $ra
    /* E27A8 8015F6B8 00000000 */   nop
endlabel func_8015F650
