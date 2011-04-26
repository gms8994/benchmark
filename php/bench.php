<?php

function timeit($code, $times, $name = '') {
    global $globals;

	run_code_block($code, $times, $name);
}

function cmp_these($codes, $times) {
    global $globals;

    foreach ($codes as $name => $code) {
		run_code_block($code, $times, $name);
    }
}

function run_code_block($code, $times, $name = '') {
	global $globals;

	if ($name === '' || is_int($name)) { $name = $code; }

	$total_time = 0;

	ob_start();
	for($i=0;$i<$times;$i++) {
		$st = microtime(true);
		eval($code);
		$total_time += microtime(true) - $st;
	}
	ob_end_clean();

	echo sprintf("%s: %s iterations took %.02f%s at %.03f%s\n", $name, number_format($times, 0, '.', ','), $total_time, "s", $times / $total_time, '/s');
}
