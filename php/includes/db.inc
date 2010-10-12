<?php

/**
 * @category Includes
 * @package Sql
 * @author Todd Tucker <toddt@networldalliance.com>
 */

include_once GLOBAL_INCLUDE_DIR . "/ez_sql_core.inc";
include_once GLOBAL_INCLUDE_DIR . "/ez_sql_mysql.inc";

/**
 * The primary database connection object
 * @global object $db
 * @name $db
 */
global $db;

/**
 * get a db connection by name
 *
 * @uses db_conn.xml
 * @uses xmlfunc.inc
 * @uses ez_sql_mysql.inc
 *
 * @param string $my_connection_name The connection to establish
 * @param string $my_sub_name Create a connection using a specific name
 *
 * @return bool Success/failure
 *
 * @author Todd Tucker <toddt@networldalliance.com>
 */
function get_db_conn($my_connection_name, $my_sub_name = false) {

	include_once GLOBAL_INCLUDE_DIR . "/xmlfunc.inc";

	$my_db_conns = parse_xml_file( DB_CONF_FILE );

	foreach( $my_db_conns as $con ) {
		if ( (string)$con->name == $my_connection_name ) {
			if ( $my_sub_name ) {
				global ${"db_" . $my_sub_name};
				if ( ${"db_" . $my_sub_name} = new ezSQL_mysql($con->username,$con->password,$con->dbname,$con->host) ) {
					${"db_" . $my_sub_name}->show_errors = true;
					return true;
				}
			} else {
				global $db;
				if ( $db = new ezSQL_mysql($con->username,$con->password,$con->dbname,$con->host) ) {
					$db->show_errors = true;
					return true;
				}
			}
		}
	}

	return false;
}
?>
