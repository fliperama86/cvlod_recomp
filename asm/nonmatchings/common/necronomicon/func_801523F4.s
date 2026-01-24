nonmatching func_801523F4, 0x1C

glabel func_801523F4
    /* D54E4 801523F4 A4800046 */  sh         $zero, 0x46($a0)
    /* D54E8 801523F8 0320F809 */  jalr       $t9
    /* D54EC 801523FC 24840008 */   addiu     $a0, $a0, 0x8
  .L80152400:
    /* D54F0 80152400 8FBF0014 */  lw         $ra, 0x14($sp)
    /* D54F4 80152404 27BD0018 */  addiu      $sp, $sp, 0x18
    /* D54F8 80152408 03E00008 */  jr         $ra
    /* D54FC 8015240C 00000000 */   nop
endlabel func_801523F4
