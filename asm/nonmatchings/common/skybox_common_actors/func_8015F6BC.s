nonmatching func_8015F6BC, 0x178

glabel func_8015F6BC
    /* E27AC 8015F6BC 27BDFFB8 */  addiu      $sp, $sp, -0x48
    /* E27B0 8015F6C0 AFBF001C */  sw         $ra, 0x1C($sp)
    /* E27B4 8015F6C4 AFB00018 */  sw         $s0, 0x18($sp)
    /* E27B8 8015F6C8 8C820070 */  lw         $v0, 0x70($a0)
    /* E27BC 8015F6CC 3C0E801D */  lui        $t6, %hi(D_801CAB90)
    /* E27C0 8015F6D0 85CEAB90 */  lh         $t6, %lo(D_801CAB90)($t6)
    /* E27C4 8015F6D4 94590018 */  lhu        $t9, 0x18($v0)
    /* E27C8 8015F6D8 3C188019 */  lui        $t8, %hi(D_80191A44)
    /* E27CC 8015F6DC 000E7880 */  sll        $t7, $t6, 2
    /* E27D0 8015F6E0 030FC021 */  addu       $t8, $t8, $t7
    /* E27D4 8015F6E4 8F181A44 */  lw         $t8, %lo(D_80191A44)($t8)
    /* E27D8 8015F6E8 00194080 */  sll        $t0, $t9, 2
    /* E27DC 8015F6EC 01194023 */  subu       $t0, $t0, $t9
    /* E27E0 8015F6F0 8C830034 */  lw         $v1, 0x34($a0)
    /* E27E4 8015F6F4 00084080 */  sll        $t0, $t0, 2
    /* E27E8 8015F6F8 03084821 */  addu       $t1, $t8, $t0
    /* E27EC 8015F6FC AFA90030 */  sw         $t1, 0x30($sp)
    /* E27F0 8015F700 00808025 */  or         $s0, $a0, $zero
    /* E27F4 8015F704 3C05801A */  lui        $a1, %hi(D_8019E5F0)
    /* E27F8 8015F708 AC600008 */  sw         $zero, 0x8($v1)
    /* E27FC 8015F70C 8CA5E5F0 */  lw         $a1, %lo(D_8019E5F0)($a1)
    /* E2800 8015F710 0C00168C */  jal        func_80005A30
    /* E2804 8015F714 24040001 */   addiu     $a0, $zero, 0x1
    /* E2808 8015F718 00402825 */  or         $a1, $v0, $zero
    /* E280C 8015F71C 14400007 */  bnez       $v0, .L8015F73C
    /* E2810 8015F720 AE020024 */   sw        $v0, 0x24($s0)
    /* E2814 8015F724 8E190010 */  lw         $t9, 0x10($s0)
    /* E2818 8015F728 02002025 */  or         $a0, $s0, $zero
    /* E281C 8015F72C 0320F809 */  jalr       $t9
    /* E2820 8015F730 00000000 */   nop
    /* E2824 8015F734 1000003B */  b          .L8015F824
    /* E2828 8015F738 8FBF001C */   lw        $ra, 0x1C($sp)
  .L8015F73C:
    /* E282C 8015F73C 0C050C73 */  jal        func_801431CC
    /* E2830 8015F740 02002025 */   or        $a0, $s0, $zero
    /* E2834 8015F744 3C198000 */  lui        $t9, %hi(func_80001CE8)
    /* E2838 8015F748 27391CE8 */  addiu      $t9, $t9, %lo(func_80001CE8)
    /* E283C 8015F74C 26070008 */  addiu      $a3, $s0, 0x8
    /* E2840 8015F750 2605000E */  addiu      $a1, $s0, 0xE
    /* E2844 8015F754 AFA50024 */  sw         $a1, 0x24($sp)
    /* E2848 8015F758 00E02025 */  or         $a0, $a3, $zero
    /* E284C 8015F75C 0320F809 */  jalr       $t9
    /* E2850 8015F760 AFA70028 */   sw        $a3, 0x28($sp)
    /* E2854 8015F764 8FAA0030 */  lw         $t2, 0x30($sp)
    /* E2858 8015F768 8FA70028 */  lw         $a3, 0x28($sp)
    /* E285C 8015F76C 954B0000 */  lhu        $t3, 0x0($t2)
    /* E2860 8015F770 2D61000A */  sltiu      $at, $t3, 0xA
    /* E2864 8015F774 10200024 */  beqz       $at, .L8015F808
    /* E2868 8015F778 000B5880 */   sll       $t3, $t3, 2
    /* E286C 8015F77C 3C01801A */  lui        $at, %hi(jtbl_8019ABF8)
    /* E2870 8015F780 002B0821 */  addu       $at, $at, $t3
    /* E2874 8015F784 8C2BABF8 */  lw         $t3, %lo(jtbl_8019ABF8)($at)
    /* E2878 8015F788 01600008 */  jr         $t3
    /* E287C 8015F78C 00000000 */   nop
    /* E2880 8015F790 3C05801A */  lui        $a1, %hi(D_8019E5F0)
    /* E2884 8015F794 8CA5E5F0 */  lw         $a1, %lo(D_8019E5F0)($a1)
    /* E2888 8015F798 24040001 */  addiu      $a0, $zero, 0x1
    /* E288C 8015F79C 0C00168C */  jal        func_80005A30
    /* E2890 8015F7A0 AFA70028 */   sw        $a3, 0x28($sp)
    /* E2894 8015F7A4 8FA70028 */  lw         $a3, 0x28($sp)
    /* E2898 8015F7A8 14400007 */  bnez       $v0, .L8015F7C8
    /* E289C 8015F7AC AE020028 */   sw        $v0, 0x28($s0)
    /* E28A0 8015F7B0 8E190010 */  lw         $t9, 0x10($s0)
    /* E28A4 8015F7B4 02002025 */  or         $a0, $s0, $zero
    /* E28A8 8015F7B8 0320F809 */  jalr       $t9
    /* E28AC 8015F7BC 00000000 */   nop
    /* E28B0 8015F7C0 10000018 */  b          .L8015F824
    /* E28B4 8015F7C4 8FBF001C */   lw        $ra, 0x1C($sp)
  .L8015F7C8:
    /* E28B8 8015F7C8 3C198000 */  lui        $t9, %hi(func_80001E30)
    /* E28BC 8015F7CC 27391E30 */  addiu      $t9, $t9, %lo(func_80001E30)
    /* E28C0 8015F7D0 00E02025 */  or         $a0, $a3, $zero
    /* E28C4 8015F7D4 8FA50024 */  lw         $a1, 0x24($sp)
    /* E28C8 8015F7D8 0320F809 */  jalr       $t9
    /* E28CC 8015F7DC 24060002 */   addiu     $a2, $zero, 0x2
    /* E28D0 8015F7E0 1000000D */  b          .L8015F818
    /* E28D4 8015F7E4 00000000 */   nop
    /* E28D8 8015F7E8 3C198000 */  lui        $t9, %hi(func_80001E30)
    /* E28DC 8015F7EC 27391E30 */  addiu      $t9, $t9, %lo(func_80001E30)
    /* E28E0 8015F7F0 00E02025 */  or         $a0, $a3, $zero
    /* E28E4 8015F7F4 8FA50024 */  lw         $a1, 0x24($sp)
    /* E28E8 8015F7F8 0320F809 */  jalr       $t9
    /* E28EC 8015F7FC 24060003 */   addiu     $a2, $zero, 0x3
    /* E28F0 8015F800 10000005 */  b          .L8015F818
    /* E28F4 8015F804 00000000 */   nop
  .L8015F808:
    /* E28F8 8015F808 8E190010 */  lw         $t9, 0x10($s0)
    /* E28FC 8015F80C 02002025 */  or         $a0, $s0, $zero
    /* E2900 8015F810 0320F809 */  jalr       $t9
    /* E2904 8015F814 00000000 */   nop
  .L8015F818:
    /* E2908 8015F818 0C050C8D */  jal        func_80143234
    /* E290C 8015F81C 02002025 */   or        $a0, $s0, $zero
    /* E2910 8015F820 8FBF001C */  lw         $ra, 0x1C($sp)
  .L8015F824:
    /* E2914 8015F824 8FB00018 */  lw         $s0, 0x18($sp)
    /* E2918 8015F828 27BD0048 */  addiu      $sp, $sp, 0x48
    /* E291C 8015F82C 03E00008 */  jr         $ra
    /* E2920 8015F830 00000000 */   nop
endlabel func_8015F6BC
