#!/usr/bin/php
<?php

require_once "bench.php";

cmp_these(
    array(
        'substr_in_array1("abc", array("abcdef", "acbdef", "acdbef", "acebdf", "adebcf"));',
        'substr_in_array2("abc", array("abcdef", "acbdef", "acdbef", "acebdf", "adebcf"));',
        'substr_in_array3("abc", array("abcdef", "acbdef", "acdbef", "acebdf", "adebcf"));',
    ), 100000
);

function substr_in_array1($needle, array $haystack)
{
    $filtered = array_filter($haystack, function ($item) use ($needle) {
        return false !== strpos($item, $needle);
    });

    return !empty($filtered);
}
function substr_in_array2($needle, array $haystack)
{
	foreach ($haystack as $item) {
		if (false !== strpos($item, $needle)) {
			return true;
		}
	}

	return false;
}
function substr_in_array3($needle, array $haystack)
{
	return array_reduce($haystack, function ($inArray, $item) use ($needle) {
		return $inArray || false !== strpos($item, $needle);
	}, false);
}
