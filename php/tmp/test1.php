<?php

/**
 * Zend Framework
 *
 * LICENSE
 *
 * This source file is subject to the new BSD license that is bundled
 * with this package in the file LICENSE.txt.
 * It is also available through the world-wide-web at this URL:
 * http://framework.zend.com/license/new-bsd
 * If you did not receive a copy of the license and are unable to
 * obtain it through the world-wide-web, please send an email
 * to license@zend.com so we can send you a copy immediately.
 *
 * @category   Zend
 * @package    Zend_Db
 * @copyright  Copyright (c) 2005-2010 Zend Technologies USA Inc. (http://www.zend.com)
 * @license    http://framework.zend.com/license/new-bsd     New BSD License
 * @version    $Id: Db.php 20096 2010-01-06 02:05:09Z bkarwin $
 */


/**
 * Class for connecting to SQL databases and performing common operations.
 *
 * @category   Zend
 * @package    Zend_Db
 * @copyright  Copyright (c) 2005-2010 Zend Technologies USA Inc. (http://www.zend.com)
 * @license    http://framework.zend.com/license/new-bsd     New BSD License
 */
    /**
     * Use the PROFILER constant in the config of a Zend_Db_Adapter.
     */

    /**
     * Use the CASE_FOLDING constant in the config of a Zend_Db_Adapter.
     */

    /**
     * Use the AUTO_QUOTE_IDENTIFIERS constant in the config of a Zend_Db_Adapter.
     */

    /**
     * Use the ALLOW_SERIALIZATION constant in the config of a Zend_Db_Adapter.
     */

    /**
     * Use the AUTO_RECONNECT_ON_UNSERIALIZE constant in the config of a Zend_Db_Adapter.
     */

    /**
     * Use the INT_TYPE, BIGINT_TYPE, and FLOAT_TYPE with the quote() method.
     */

    /**
     * PDO constant values discovered by this script result:
     *
     * $list = array(
     *    'PARAM_BOOL', 'PARAM_NULL', 'PARAM_INT', 'PARAM_STR', 'PARAM_LOB',
     *    'PARAM_STMT', 'PARAM_INPUT_OUTPUT', 'FETCH_LAZY', 'FETCH_ASSOC',
     *    'FETCH_NUM', 'FETCH_BOTH', 'FETCH_OBJ', 'FETCH_BOUND',
     *    'FETCH_COLUMN', 'FETCH_CLASS', 'FETCH_INTO', 'FETCH_FUNC',
     *    'FETCH_GROUP', 'FETCH_UNIQUE', 'FETCH_CLASSTYPE', 'FETCH_SERIALIZE',
     *    'FETCH_NAMED', 'ATTR_AUTOCOMMIT', 'ATTR_PREFETCH', 'ATTR_TIMEOUT',
     *    'ATTR_ERRMODE', 'ATTR_SERVER_VERSION', 'ATTR_CLIENT_VERSION',
     *    'ATTR_SERVER_INFO', 'ATTR_CONNECTION_STATUS', 'ATTR_CASE',
     *    'ATTR_CURSOR_NAME', 'ATTR_CURSOR', 'ATTR_ORACLE_NULLS',
     *    'ATTR_PERSISTENT', 'ATTR_STATEMENT_CLASS', 'ATTR_FETCH_TABLE_NAMES',
     *    'ATTR_FETCH_CATALOG_NAMES', 'ATTR_DRIVER_NAME',
     *    'ATTR_STRINGIFY_FETCHES', 'ATTR_MAX_COLUMN_LEN', 'ERRMODE_SILENT',
     *    'ERRMODE_WARNING', 'ERRMODE_EXCEPTION', 'CASE_NATURAL',
     *    'CASE_LOWER', 'CASE_UPPER', 'NULL_NATURAL', 'NULL_EMPTY_STRING',
     *    'NULL_TO_STRING', 'ERR_NONE', 'FETCH_ORI_NEXT',
     *    'FETCH_ORI_PRIOR', 'FETCH_ORI_FIRST', 'FETCH_ORI_LAST',
     *    'FETCH_ORI_ABS', 'FETCH_ORI_REL', 'CURSOR_FWDONLY', 'CURSOR_SCROLL',
     *    'ERR_CANT_MAP', 'ERR_SYNTAX', 'ERR_CONSTRAINT', 'ERR_NOT_FOUND',
     *    'ERR_ALREADY_EXISTS', 'ERR_NOT_IMPLEMENTED', 'ERR_MISMATCH',
     *    'ERR_TRUNCATED', 'ERR_DISCONNECTED', 'ERR_NO_PERM',
     * );
     *
     * $const = array();
     * foreach ($list as $name) {
     *    $const[$name] = constant("PDO::$name");
     * }
     * var_export($const);
     */

    /**
     * Factory for Zend_Db_Adapter_Abstract classes.
     *
     * First argument may be a string containing the base of the adapter class
     * name, e.g. 'Mysqli' corresponds to class Zend_Db_Adapter_Mysqli.  This
     * name is currently case-insensitive, but is not ideal to rely on this behavior.
     * If your class is named 'My_Company_Pdo_Mysql', where 'My_Company' is the namespace
     * and 'Pdo_Mysql' is the adapter name, it is best to use the name exactly as it
     * is defined in the class.  This will ensure proper use of the factory API.
     *
     * First argument may alternatively be an object of type Zend_Config.
     * The adapter class base name is read from the 'adapter' property.
     * The adapter config parameters are read from the 'params' property.
     *
     * Second argument is optional and may be an associative array of key-value
     * pairs.  This is used as the argument to the adapter constructor.
     *
     * If the first argument is of type Zend_Config, it is assumed to contain
     * all parameters, and the second argument is ignored.
     *
     * @param  mixed $adapter String name of base adapter class, or Zend_Config object.
     * @param  mixed $config  OPTIONAL; an array or Zend_Config object with adapter parameters.
     * @return Zend_Db_Adapter_Abstract
     * @throws Zend_Db_Exception
     */

        /*
         * Convert Zend_Config argument to plain string
         * adapter name and separate config object.
         */

        /*
         * Verify that adapter parameters are in an array.
         */
            /**
             * @see Zend_Db_Exception
             */

        /*
         * Verify that an adapter name has been specified.
         */
            /**
             * @see Zend_Db_Exception
             */

        /*
         * Form full adapter class name
         */

        // Adapter no longer normalized- see http://framework.zend.com/issues/browse/ZF-5606

        /*
         * Load the adapter class.  This throws an exception
         * if the specified class cannot be loaded.
         */

        /*
         * Create an instance of the adapter class.
         * Pass the config to the adapter class constructor.
         */

        /*
         * Verify that the object created is a descendent of the abstract adapter type.
         */
            /**
             * @see Zend_Db_Exception
             */

