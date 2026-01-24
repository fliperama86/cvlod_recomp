nonmatching func_80152C50, 0x38

glabel func_80152C50
    /* D5D40 80152C50 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* D5D44 80152C54 AFBF0014 */  sw         $ra, 0x14($sp)
    /* D5D48 80152C58 948E0058 */  lhu        $t6, 0x58($a0)
    /* D5D4C 80152C5C 8C820024 */  lw         $v0, 0x24($a0)
    /* D5D50 80152C60 3C198001 */  lui        $t9, %hi(func_80016294)
    /* D5D54 80152C64 11C00004 */  beqz       $t6, .L80152C78
    /* D5D58 80152C68 24450050 */   addiu     $a1, $v0, 0x50
    /* D5D5C 80152C6C 27396294 */  addiu      $t9, $t9, %lo(func_80016294)
    /* D5D60 80152C70 0320F809 */  jalr       $t9
    /* D5D64 80152C74 94840058 */   lhu       $a0, 0x58($a0)
  .L80152C78:
    /* D5D68 80152C78 8FBF0014 */  lw         $ra, 0x14($sp)
    /* D5D6C 80152C7C 27BD0018 */  addiu      $sp, $sp, 0x18
    /* D5D70 80152C80 03E00008 */  jr         $ra
    /* D5D74 80152C84 00000000 */   nop
endlabel func_80152C50
