#!/usr/bin/php
<?php

require_once "bench.php";

$data = array();
for ($i = 32; $i <= 126; $i++) {
	$data[$i] = array();
	$data[$i][0] = str_repeat(chr($i), $i);
}

global $serialize, $json;
$serialize = serialize($data);
$json = json_encode($data);

cmp_these(
	array(
		'unserialize_data();',
		'jsondecodedata();',
		), 100000,
		array( 'report_mem' => true )
	);

function unserialize_data() {
	global $serialize;

	unserialize($serialize);
}

function jsondecodedata() {
	global $json;

	json_decode($json);
}
