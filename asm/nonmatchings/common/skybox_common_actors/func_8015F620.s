nonmatching func_8015F620, 0x30

glabel func_8015F620
    /* E2710 8015F620 0328C821 */  addu       $t9, $t9, $t0
    /* E2714 8015F624 8F391B44 */  lw         $t9, 0x1B44($t9)
    /* E2718 8015F628 0320F809 */  jalr       $t9
    /* E271C 8015F62C 00000000 */   nop
    /* E2720 8015F630 8FA40018 */  lw         $a0, 0x18($sp)
    /* E2724 8015F634 8489000E */  lh         $t1, 0xE($a0)
    /* E2728 8015F638 252AFFFF */  addiu      $t2, $t1, -0x1
    /* E272C 8015F63C A48A000E */  sh         $t2, 0xE($a0)
    /* E2730 8015F640 8FBF0014 */  lw         $ra, 0x14($sp)
    /* E2734 8015F644 27BD0018 */  addiu      $sp, $sp, 0x18
    /* E2738 8015F648 03E00008 */  jr         $ra
    /* E273C 8015F64C 00000000 */   nop
endlabel func_8015F620
