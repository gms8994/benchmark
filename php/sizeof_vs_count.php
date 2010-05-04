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

require_once "bench.php";

global $globals;
$globals = new globals;
$globals->setupArray(1000000);

cmp_these(
    array(
        'sizeof($globals->x);',
        'count($globals->x);'
    ), 1000000
);
