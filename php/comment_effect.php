#!/usr/bin/php
<?php

require_once "bench.php";

cmp_these(
    array(
        'include "tmp/test1.php";',
        'include "tmp/test2.php";',
    ), 1000000
);
