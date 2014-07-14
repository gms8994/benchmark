<?php

function display_output() {
	global $rates;

	$keys = array_keys($rates);

	print "||_. |_. Rate ";
	foreach ($keys as $key) {
		print "|_. $key ";
	}
	print "|\n";

	foreach ($keys as $key) {
		print "|_. $key | ";
		print sprintf("%d/s", $rates[$key]) . " | ";

		foreach ($keys as $key2) {
			if ($key == $key2) print " -- ";
			else print sprintf("%d%%", ($rates[$key] / $rates[$key2]) * 100);

			print " |";
		}

		print "\n";
	}
}

function timeit($code, $times, $name = '') {
    global $globals;

	run_code_block($code, $times, $name);
}

function cmp_these($codes, $times) {
    global $globals;

    foreach ($codes as $name => $code) {
		run_code_block($code, $times, $name);
    }

	display_output();
}

function run_code_block($code, $times, $name = '') {
	global $globals, $rates;

	if ($name === '' || is_int($name)) { $name = $code; }

	$total_time = 0;

	for($i=0;$i<$times;$i++) {
		ob_start();
		$st = microtime(true);
		eval($code);
		$total_time += microtime(true) - $st;
		ob_end_clean();
		echo sprintf("\r%s: %s iterations took %.06f%s at %.03f%s", $name, number_format($i, 0, '.', ','), $total_time, "s", $i / $total_time, '/s');
	}

	echo sprintf("\r%s: %s iterations took %.06f%s at %.03f%s\n", $name, number_format($times, 0, '.', ','), $total_time, "s", $times / $total_time, '/s');

	$rates[$name] = $times / $total_time;
}
