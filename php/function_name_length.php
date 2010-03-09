#!/usr/bin/php
<?php

require_once "bench.php";

cmp_these(
    array(
        'a();',
        'abcdefghijklmnopqrstuvwxyz();',
        'abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz();'
    ), 10000000
);

function a() {
    return false;
}
function abcdefghijklmnopqrstuvwxyz() {
    return false;
}
function abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz() {
    return false;
}
