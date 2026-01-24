nonmatching func_801523D0, 0x24

glabel func_801523D0
    /* D54C0 801523D0 948E0044 */  lhu        $t6, 0x44($a0)
    /* D54C4 801523D4 4628203C */  c.lt.d     $ft0, $ft2
    /* D54C8 801523D8 00803025 */  or         $a2, $a0, $zero
    /* D54CC 801523DC 31CFCFFF */  andi       $t7, $t6, 0xCFFF
    /* D54D0 801523E0 A48F0044 */  sh         $t7, 0x44($a0)
    /* D54D4 801523E4 45010006 */  bc1t       .L80152400
    /* D54D8 801523E8 24C5000E */   addiu     $a1, $a2, 0xE
    /* D54DC 801523EC 3C198000 */  lui        $t9, %hi(func_80001CE8)
    /* D54E0 801523F0 27391CE8 */  addiu      $t9, $t9, %lo(func_80001CE8)
endlabel func_801523D0
