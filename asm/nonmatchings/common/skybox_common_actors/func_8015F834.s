nonmatching func_8015F834, 0xD8

glabel func_8015F834
    /* E2924 8015F834 27BDFFE0 */  addiu      $sp, $sp, -0x20
    /* E2928 8015F838 AFBF0014 */  sw         $ra, 0x14($sp)
    /* E292C 8015F83C 8C860024 */  lw         $a2, 0x24($a0)
    /* E2930 8015F840 8C870028 */  lw         $a3, 0x28($a0)
    /* E2934 8015F844 AFA40020 */  sw         $a0, 0x20($sp)
    /* E2938 8015F848 AFA6001C */  sw         $a2, 0x1C($sp)
    /* E293C 8015F84C 24C50050 */  addiu      $a1, $a2, 0x50
    /* E2940 8015F850 0C050C53 */  jal        func_8014314C
    /* E2944 8015F854 AFA70018 */   sw        $a3, 0x18($sp)
    /* E2948 8015F858 8FA40020 */  lw         $a0, 0x20($sp)
    /* E294C 8015F85C 8FA6001C */  lw         $a2, 0x1C($sp)
    /* E2950 8015F860 10400006 */  beqz       $v0, .L8015F87C
    /* E2954 8015F864 8FA70018 */   lw        $a3, 0x18($sp)
    /* E2958 8015F868 8C990010 */  lw         $t9, 0x10($a0)
    /* E295C 8015F86C 0320F809 */  jalr       $t9
    /* E2960 8015F870 00000000 */   nop
    /* E2964 8015F874 10000022 */  b          .L8015F900
    /* E2968 8015F878 8FBF0014 */   lw        $ra, 0x14($sp)
  .L8015F87C:
    /* E296C 8015F87C 3C05801D */  lui        $a1, %hi(D_801C82C0)
    /* E2970 8015F880 24A582C0 */  addiu      $a1, $a1, %lo(D_801C82C0)
    /* E2974 8015F884 8CAE2B0C */  lw         $t6, 0x2B0C($a1)
    /* E2978 8015F888 3C198019 */  lui        $t9, %hi(D_80191B54)
    /* E297C 8015F88C ACCE0018 */  sw         $t6, 0x18($a2)
    /* E2980 8015F890 8CAF007C */  lw         $t7, 0x7C($a1)
    /* E2984 8015F894 ACCF0024 */  sw         $t7, 0x24($a2)
    /* E2988 8015F898 8CB82B0C */  lw         $t8, 0x2B0C($a1)
    /* E298C 8015F89C ACF80018 */  sw         $t8, 0x18($a3)
    /* E2990 8015F8A0 8CA8007C */  lw         $t0, 0x7C($a1)
    /* E2994 8015F8A4 ACE80024 */  sw         $t0, 0x24($a3)
    /* E2998 8015F8A8 8483000E */  lh         $v1, 0xE($a0)
    /* E299C 8015F8AC 24630001 */  addiu      $v1, $v1, 0x1
    /* E29A0 8015F8B0 00031C00 */  sll        $v1, $v1, 16
    /* E29A4 8015F8B4 00031C03 */  sra        $v1, $v1, 16
    /* E29A8 8015F8B8 00034840 */  sll        $t1, $v1, 1
    /* E29AC 8015F8BC 00891021 */  addu       $v0, $a0, $t1
    /* E29B0 8015F8C0 A483000E */  sh         $v1, 0xE($a0)
    /* E29B4 8015F8C4 904A0008 */  lbu        $t2, 0x8($v0)
    /* E29B8 8015F8C8 254B0001 */  addiu      $t3, $t2, 0x1
    /* E29BC 8015F8CC A04B0008 */  sb         $t3, 0x8($v0)
    /* E29C0 8015F8D0 AFA40020 */  sw         $a0, 0x20($sp)
    /* E29C4 8015F8D4 904C0009 */  lbu        $t4, 0x9($v0)
    /* E29C8 8015F8D8 000C6880 */  sll        $t5, $t4, 2
    /* E29CC 8015F8DC 032DC821 */  addu       $t9, $t9, $t5
    /* E29D0 8015F8E0 8F391B54 */  lw         $t9, %lo(D_80191B54)($t9)
    /* E29D4 8015F8E4 0320F809 */  jalr       $t9
    /* E29D8 8015F8E8 00000000 */   nop
    /* E29DC 8015F8EC 8FA40020 */  lw         $a0, 0x20($sp)
    /* E29E0 8015F8F0 848E000E */  lh         $t6, 0xE($a0)
    /* E29E4 8015F8F4 25CFFFFF */  addiu      $t7, $t6, -0x1
    /* E29E8 8015F8F8 A48F000E */  sh         $t7, 0xE($a0)
    /* E29EC 8015F8FC 8FBF0014 */  lw         $ra, 0x14($sp)
  .L8015F900:
    /* E29F0 8015F900 27BD0020 */  addiu      $sp, $sp, 0x20
    /* E29F4 8015F904 03E00008 */  jr         $ra
    /* E29F8 8015F908 00000000 */   nop
endlabel func_8015F834
