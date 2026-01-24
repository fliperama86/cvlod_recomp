nonmatching func_80152104, 0x20

glabel func_80152104
    /* D51F4 80152104 2605000E */  addiu      $a1, $s0, 0xE
    /* D51F8 80152108 0320F809 */  jalr       $t9
    /* D51FC 8015210C 00003025 */   or        $a2, $zero, $zero
  .L80152110:
    /* D5200 80152110 8FBF001C */  lw         $ra, 0x1C($sp)
  .L80152114:
    /* D5204 80152114 8FB00014 */  lw         $s0, 0x14($sp)
    /* D5208 80152118 8FB10018 */  lw         $s1, 0x18($sp)
    /* D520C 8015211C 03E00008 */  jr         $ra
    /* D5210 80152120 27BD0058 */   addiu     $sp, $sp, 0x58
endlabel func_80152104
