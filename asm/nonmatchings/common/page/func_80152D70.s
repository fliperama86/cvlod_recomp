nonmatching func_80152D70, 0x70

glabel func_80152D70
    /* D5E60 80152D70 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* D5E64 80152D74 AFBF0014 */  sw         $ra, 0x14($sp)
    /* D5E68 80152D78 8483000E */  lh         $v1, 0xE($a0)
    /* D5E6C 80152D7C 24630001 */  addiu      $v1, $v1, 0x1
    /* D5E70 80152D80 00031C00 */  sll        $v1, $v1, 16
    /* D5E74 80152D84 00031C03 */  sra        $v1, $v1, 16
    /* D5E78 80152D88 00037040 */  sll        $t6, $v1, 1
    /* D5E7C 80152D8C 008E1021 */  addu       $v0, $a0, $t6
    /* D5E80 80152D90 A483000E */  sh         $v1, 0xE($a0)
    /* D5E84 80152D94 904F0008 */  lbu        $t7, 0x8($v0)
    /* D5E88 80152D98 25F80001 */  addiu      $t8, $t7, 0x1
    /* D5E8C 80152D9C A0580008 */  sb         $t8, 0x8($v0)
    /* D5E90 80152DA0 AFA40018 */  sw         $a0, 0x18($sp)
    /* D5E94 80152DA4 90590009 */  lbu        $t9, 0x9($v0)
    /* D5E98 80152DA8 00194080 */  sll        $t0, $t9, 2
    /* D5E9C 80152DAC 3C198019 */  lui        $t9, %hi(D_80190830)
    /* D5EA0 80152DB0 0328C821 */  addu       $t9, $t9, $t0
    /* D5EA4 80152DB4 8F390830 */  lw         $t9, %lo(D_80190830)($t9)
    /* D5EA8 80152DB8 0320F809 */  jalr       $t9
    /* D5EAC 80152DBC 00000000 */   nop
    /* D5EB0 80152DC0 8FA40018 */  lw         $a0, 0x18($sp)
    /* D5EB4 80152DC4 8489000E */  lh         $t1, 0xE($a0)
    /* D5EB8 80152DC8 252AFFFF */  addiu      $t2, $t1, -0x1
    /* D5EBC 80152DCC A48A000E */  sh         $t2, 0xE($a0)
    /* D5EC0 80152DD0 8FBF0014 */  lw         $ra, 0x14($sp)
    /* D5EC4 80152DD4 27BD0018 */  addiu      $sp, $sp, 0x18
    /* D5EC8 80152DD8 03E00008 */  jr         $ra
    /* D5ECC 80152DDC 00000000 */   nop
endlabel func_80152D70
