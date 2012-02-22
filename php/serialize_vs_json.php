#!/usr/bin/php
<?php

require_once "bench.php";

global $data;
$data = array();

for ($i = 32; $i <= 126; $i++) {
	$data[$i] = array();
	$data[$i][0] = str_repeat(chr($i), $i);
}

cmp_these(
	array(
		'serialize_data();',
		'jsonencodedata();',
		), 100000,
		array( 'report_mem' => true )
	);

function serialize_data() {
	global $data;

	serialize($data);
}

function jsonencodedata() {
	global $data;

	json_encode($data);
}
