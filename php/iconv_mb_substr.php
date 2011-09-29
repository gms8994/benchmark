#!/usr/bin/php
<?php

class globals {
    public $x = '';
	public $off = 1000;
	public $len = 90000;
	public $char = 'UTF-8';

    public function setupData($count) {
		$this->x = str_repeat("foo", $count);
    }
}

require_once "bench.php";

global $globals;
$globals = new globals;
$globals->setupData(100000);

cmp_these(
    array(
        'iconv_substr($globals->x, $globals->off, $globals->len, $globals->char);',
        'mb_substr($globals->x, $globals->off, $globals->len, $globals->char);',
        'substr($globals->x, $globals->off, $globals->len);',
    ), 20
);
