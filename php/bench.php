<?php

function timeit($code, $times, $name = '') {
    global $globals;

    if ($name === '') { $name = $code; }

    $total_time = 0;

    for($i=0;$i<$times;$i++) {
        $st = microtime(true);
        eval($code);
        $total_time += microtime(true) - $st;
    }

    echo sprintf("%s: %d iterations took %.02f%s at %.03f%s\n", $name, $times, $total_time, "s", $times / $total_time, '/s');
}

function cmp_these($codes, $times) {
    global $globals;

    foreach ($codes as $name => $code) {

        if ($name === '' || is_int($name)) { $name = $code; }

        $total_time = 0;

        for($i=0;$i<$times;$i++) {
            $st = microtime(true);
            eval($code);
            $total_time += microtime(true) - $st;
        }

        echo sprintf("%s: %d iterations took %.02f%s at %.03f%s\n", $name, $times, $total_time, "s", $times / $total_time, '/s');
    }
}
