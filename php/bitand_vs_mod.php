#!/usr/bin/php
<?php

require_once "bench.php";

cmp_these(
    array(
        '$r=rand(); $r & 1 == 0 ? 1 : 0;',
        '$r=rand(); $r % 2 == 0 ? 1 : 0;',
    ), 1000000
);
