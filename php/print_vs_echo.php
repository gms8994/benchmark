#!/usr/bin/php
<?php

require_once "bench.php";

cmp_these(
    array(
        'p();',
        'e();' 
    ), 100000
);

function p() {
    print "test";
}
function e() {
    echo  "test";
}
