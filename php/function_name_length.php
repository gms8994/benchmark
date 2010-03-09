#!/usr/bin/php
<?php

require_once "bench.php";

cmp_these(
    array(
        'abcdefghijklmnopqrstuvwxyz();',
        'abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz();'
    ), 10000000
);

function abcdefghijklmnopqrstuvwxyz() {
    return false;
}
function abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz() {
    return false;
}
