#!/usr/bin/php
<?php

require_once "bench.php";

require_once "Zend/Cache.php";

$frontendOptions = array(
        'lifetime' => 7200, // cache lifetime of 2 hours
        'automatic_serialization' => true
        );

$backendOptions = array(
        'cache_dir' => './tmp/' // Directory where to put the cache files
        );

// getting a Zend_Cache_Core object
$cache = Zend_Cache::factory('Core',
        'File',
        $frontendOptions,
        $backendOptions);

global $file;

$file = "simplexml_vs_cachedxml.php.xml";

cmp_these(
    array(
        'load_always();',
        'load_cached();',
    ), 100000
);

function load_always() {
    global $file;

    $res = simplexml_load_file($file);
}
function load_cached() {
    global $file;
    global $cache;

    $s_file = preg_replace('/\W/', '_', $file);
    if (! $res = $cache->load($s_file)) {
        $res = simplexml_load_file($file);
        $cache->save($res, $s_file);
    }
}
