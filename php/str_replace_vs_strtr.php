#!/usr/bin/php
<?php

require_once "bench.php";

$key = 'A_B_C_D_E_F_G_H_I_J_K_L_M_N_O_P_Q_R_S_T_U_V_W_X_Y_Z_';

cmp_these(
    [
        'replace_str_replace();',
        'replace_strtr();',
    ],
    100000
);

function replace_str_replace()
{
    global $key;
    str_replace('_', '-', strtolower($key));
}
function replace_strtr()
{
    global $key;
    strtr($key, '_', '-');
}
