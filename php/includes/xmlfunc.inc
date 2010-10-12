<?php
/**
 * Functions relating to xml
 * @package Xml
 * @author Todd Tucker <toddt@networldalliance.com>
 */

/**
 * parse an xml file and return its contents
 *
 * @uses simplexml_load_file
 *
 * @param string $filename The xml file to load
 * @return SimpleXMLElement|bool
 * @author Todd Tucker <toddt@networldalliance.com>
 */
function parse_xml_file($filename) {


	if (file_exists($filename)) {
		$my_xml = simplexml_load_file($filename);

		if (! $my_xml) {
			error_log("File is blank or format unreadable: $filename");
		}
		return $my_xml;
	} else {
		error_log("Could not open file: $filename");
		return false;
	}
}

/**
 * determine the correct xml file, based on the current URL
 *
 * @uses userfunc.inc
 * @uses $my_user
 * @uses $input_vars
 *
 * @return string|bool
 *
 * @author Todd Tucker <toddt@networldalliance.com>
 */
function get_xml_filename() {
	include_once GLOBAL_INCLUDE_DIR . '/userfunc.inc';
	global $my_user;
	global $input_vars;

	if (read_user_cookie()) {
	} else {
	}

	if (array_key_exists('page_name', $input_vars)) {
		$my_basename = $input_vars['page_name'];
	} else {
		// this allows us to have pages in sub-directories
		// $my_basename = array_shift( explode( '.', array_pop( explode('/', $_SERVER['SCRIPT_NAME'] ) ) ) );
		$my_basename = array_shift(explode('.', preg_replace('@^/@', '', $_SERVER['SCRIPT_NAME'])));
	}

	$my_filename = USER_XML_DIR . "/" . $my_basename . "_" . $my_user['id'] . ".xml"; // user pages are the default
	if (file_exists($my_filename)) {
		return $my_filename;
	}
	$my_filename = DEF_XML_DIR . "/" . $my_basename . ".xml"; // the site-specific page comes next
	if (file_exists($my_filename)) {
		return $my_filename;
	}
	$my_filename = GLOBAL_DEF_XML_DIR . "/" . $my_basename . ".xml"; // global pages
	if (file_exists($my_filename)) {
		return $my_filename;
	}
	$my_filename = XML_DIR . "/" . "default.xml"; // then the default
	if (file_exists($my_filename)) {
		return $my_filename;
	}
	return false;
}

?>
