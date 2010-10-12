#!/usr/bin/php
<?php

ob_start();

require_once "bench.php";

cmp_these(
    array(
        'p();',
        'e();' 
    ), 1000000
);

$results = ob_get_contents();
ob_end_clean();
$results = preg_replace('/test/', '', $results);

print $results;
print "\n";

function p() {
    print "test";
}
function e() {
    echo  "test";
}
