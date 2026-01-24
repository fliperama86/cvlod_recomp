nonmatching func_8015264C, 0x14

glabel func_8015264C
    /* D573C 8015264C 8C820038 */  lw         $v0, 0x38($a0)
    /* D5740 80152650 844E0000 */  lh         $t6, 0x0($v0)
    /* D5744 80152654 31CFF6FF */  andi       $t7, $t6, 0xF6FF
    /* D5748 80152658 03E00008 */  jr         $ra
    /* D574C 8015265C A44F0000 */   sh        $t7, 0x0($v0)
endlabel func_8015264C
