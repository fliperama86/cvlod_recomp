nonmatching func_801523B4, 0x1C

glabel func_801523B4
    /* D54A4 801523B4 27BDFFE8 */  addiu      $sp, $sp, -0x18
    /* D54A8 801523B8 AFBF0014 */  sw         $ra, 0x14($sp)
    /* D54AC 801523BC C4860048 */  lwc1       $ft1, 0x48($a0)
    /* D54B0 801523C0 3C014044 */  lui        $at, (0x40440000 >> 16)
    /* D54B4 801523C4 44812800 */  mtc1       $at, $ft0f
    /* D54B8 801523C8 44802000 */  mtc1       $zero, $ft0
    /* D54BC 801523CC 46003221 */  cvt.d.s    $ft2, $ft1
endlabel func_801523B4
