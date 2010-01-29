#!/usr/bin/php
<?php

class globals {
    public $x = array();

    public function setupArray($count) {
        for($i=0;$i<$count;$i++) {
            $this->x[] = $i;
        }
    }
}

global $globals;
$globals = new globals;
$globals->setupArray(1000000);

timeit('sizeof($globals->x);', 1000000);
timeit('count($globals->x);', 1000000);

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
