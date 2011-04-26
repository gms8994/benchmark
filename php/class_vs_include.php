#!/usr/bin/php
<?php

require_once "bench.php";

cmp_these(
    array(
        'classer();',
        'includer();',
    ), 5000
);

function classer() {
	shell_exec("php tmp/class_vs_include/1.php");
}
function includer() {
	shell_exec("php tmp/class_vs_include/2.php");
}
