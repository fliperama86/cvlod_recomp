nonmatching func_80152660, 0x14

glabel func_80152660
    /* D5750 80152660 8C820038 */  lw         $v0, 0x38($a0)
    /* D5754 80152664 844E0000 */  lh         $t6, 0x0($v0)
    /* D5758 80152668 31CFFEFF */  andi       $t7, $t6, 0xFEFF
    /* D575C 8015266C 03E00008 */  jr         $ra
    /* D5760 80152670 A44F0000 */   sh        $t7, 0x0($v0)
endlabel func_80152660
