<?php
//error_reporting(E_ALL);
session_save_path(session_save_path()."/" . $_SERVER['SERVER_NAME']);

/**
 * Functions to generate html pages
 * @category Includes
 * @package HTML
 *
 * @uses db.inc
 * @uses utils.inc
 * @uses xmlfunc.inc
 * @uses userfunc.inc
 * @uses $db
 * @uses $input_vars
 *
 * @author Todd Tucker <toddt@networldalliance.com>
 * @author Glen Solsberry <glens@networldalliance.com>
 * @author Gary Conway <garyc@networldalliance.com>
 * modified 12-6-2009 GRC, added <ignore> tag implementation
 * v.2.0, extensively modified by GS and GRC
 * 6-23-2010 added <omit> tag funciontality, GRC
 */

/**
 * Object containing all site related configuration, as read from SITE_DIR/etc/site_setup.xml
 * @global object $my_site_config
 * @name $my_site_config
 */
global $my_site_config, $tab_index, $db, $input_vars, $expects_extra_output;
$tab_index = 1;
$expects_extra_output = 1;

# require_once GLOBAL_INCLUDE_DIR . '/db.inc';
require_once GLOBAL_INCLUDE_DIR . '/utils.inc';
require_once GLOBAL_INCLUDE_DIR . '/xmlfunc.inc';
# require_once GLOBAL_INCLUDE_DIR . '/userfunc.inc';

// set_error_handler('errorHandler');

$my_site_config = parse_xml_file( CONF_FILE );


/**
 * Generates a page based on xml file
 * @uses $db
 * @uses $my_page_config
 * @uses $my_site_config
 * @uses $input_vars
 *
 * @author Todd Tucker <toddt@networldalliance.com>
 * @author Glen Solsberry <glens@networldalliance.com>
 * @author Gary Conway <garyc@networldalliance.com>
 */
function generate_page() {

	$page_start = microtime(true);

    global $page_name, $page_sponsor, $db, $site_id, $input_vars;
    global $default_height, $default_width, $ie, $my_site_config;
	global $my_page_config, $expects_extra_output;

    get_input_vars();
    # session_start();
    ob_start();


	//GRC - set this for universal availablity and for usage in XML files as [site_id]
	if (! isset($input_vars['site_id']))
		$input_vars['site_id'] = $my_site_config->site_id;


    $ie = 0;
    if ( preg_match('/MSIE/i',$_SERVER['HTTP_USER_AGENT'])) {$ie = 1;}

	if (! $my_site_config) {
		if (! display_error('no_site_config')) { echo "Invalid error code!<br>\n"; }
		ob_end_flush();
		return;
	}


	if ( ! get_db_conn( "default" ) ) 
	{
		display_error("db_connection");
		error_log("Problem connecting to default database connection.");
		exit;
	}

	$site_id = $my_site_config->site_id;

	if (isset($my_site_config->layout->width)) {
		$default_width  = $my_site_config->layout->width;
	}
	if (isset($my_site_config->layout->height)) {
		$default_height = $my_site_config->layout->height;
	}
	if (isset($my_site_config->buffer)) {
		ob_start();
	}
	if (isset($my_site_config->dtd) && $my_site_config->dtd != '') {
		echo trim($my_site_config->dtd) . "\n";
	}

	//  HTML Header Block
	$my_page_title = "";
    // taking control of the headers sent by default from PHP
    header('Pragma: NWA',true);
    header('Vary: Accept-Encoding',true);
//  header('Cache-Control: max-age=86400, must-revalidate',true);
    header('Cache-Control: no-store, no-cache, must-revalidate',true);
    header('Cache-Control: pre-check=0, post-check=0',false);
    header('Expires: Sat, 26 Jul 1997 05:00:00 GMT',true );
    header('Last-Modified: '.gmdate('D, d M Y H:i:s') . ' GMT',true);

	header('P3P:CP="IDC DSP COR ADM DEVi TAIi PSA PSD IVAi IVDi CONi HIS OUR IND CNT"');
    if (isset( $my_site_config->dtd ) && $my_site_config->dtd != '') {
        echo "<html xmlns=\"http://www.w3.org/1999/xhtml\">";
    } else {
        echo "<html>";
    }
	echo "<head>\n";
	echo "<link rel=\"shortcut icon\" href=\"/images/favicon.ico\" />\n";

	$my_xml_filename = get_xml_filename();

	if (! $my_xml_filename) {
		echo "<title>$my_page_title</title>\n";
		echo "</head>\n";
		if (! display_error('404')) {
			echo "Invalid error code!<br />";
            echo "</html>\n";
		}
		ob_end_flush();
		return;
	}

    $my_page_config = parse_xml_file($my_xml_filename); 

    if (! $my_page_config) {
        echo "<title>$my_page_title</title>\n";
        echo "</head>\n";
        if (! display_error('page_config')) {
            echo "Invalid error code!<br>\n";
            echo "</html>\n";
        }
        error_log("Problem reading config file: " . $my_xml_filename);
        ob_end_flush();
        return;
    }

	$input_vars['_page_name'] = $page_name = $my_page_config->name;
	$page_sponsor = FALSE;

	// Check user state
	set_user_level();
	if (compareArrayItem($_SESSION, 'my_user_level', 'site_admin')) { $input_vars['godmode']= true; }


    if (! user_is_authorized($my_page_config->user_state)) {
        $my_url = $_SERVER['SCRIPT_NAME'];
        if (count($_GET) > 0) { $my_url .= "?"; }
        $parmcount = 1;
        foreach ($_GET as $key => $value) {
            if ($parmcount != 1) { $my_url .= "&"; }
            $my_url .= $key . "=" . $value;
            $parmcount++;
        }
        $my_url = urlencode($my_url);
        header("Location: /login.php?site_id={$input_vars['site_id']}&url={$my_url}");
        ob_end_flush();
        return;
    }

	// an ignore here will ignore the complete file - GRC
	if (isset($my_page_config->ignore)) { return; }

	if (CheckOmit($my_page_config,$my_site_config->site_id)) return;


	// major error traps have all been passed, now DO the page
    if (isset($my_page_config->sponsor_query)) {
        $my_page_config->sponsor_query = replace_with_values($my_page_config->sponsor_query);
        $page_sponsor = get_page_sponsor($my_page_config->sponsor_query);
    } else {
		$page_sponsor = get_page_sponsor();
	}

//GRC
// here we have an override of sponsorship
// if $input_vars['RC_sponsor'] has been set in article.php, then
// we have been called from a sponsored research center and this var holds the sponsor id's
// thus ads placed on the current page will be ads by this sponsor

	if (isset($input_vars['RC_sponsor'])) {
		$page_sponsor=$input_vars['RC_sponsor'];
		$page_name="Research Center Page";
	}


	if (isset($my_page_config->title)) {
		if (isset($my_page_config->title->sql)) {
            $my_page_config->title->sql = replace_with_values($my_page_config->title->sql);
			$my_page_config->title = $db->get_var($my_page_config->title->sql);
		}

		$my_page_title .= $my_page_config->title . " | " . $my_site_config->name;
		echo "<title>$my_page_title</title>\n";
	}

	add_meta_tag('description');
	add_meta_tag('keywords');

	echo "<meta http-equiv=\"Content-Type\" content=\"text/html;charset=UTF-8\" />\n";

	// load global CSS and JS file from GLOBAL_CSSJS_FILE
	Load_Global_CSSJS();

//	load_styles($my_page_config->stylesheet);
	load_scripts($my_site_config->js);
	load_scripts($my_page_config->js);
	load_styles($my_site_config->stylesheet);
    load_styles($my_page_config->stylesheet);

	if (isset($my_page_config->includes)
		&& $my_page_config->includes != ''
		&& count($_POST) == 0) {
		include_once GLOBAL_INCLUDE_DIR . "/" . $my_page_config->includes . ".inc";
	}

	if (isset($my_site_config->last_head_contents)
		&& $my_site_config->last_head_contents != '') {
		echo trim($my_site_config->last_head_contents) . "\n";
	}

	echo " </head>\n";

	//  HTML Body Block

	// Check inputs, error on incorrect inputs
	// @todo need to test this, see if this should be = or ==
	if ($my_width = $my_page_config->layout->width) {
		if ($my_width == 'default' || $my_width == 'container') {
			$my_width = $default_width;
		}
	} else {
		$my_width = false;
	}


	if ($my_height = $my_page_config->layout->height) {
		if ($my_height == 'default' || $my_height == 'container') {
			$my_height = $default_height;
		}
	} else {
		$my_height = false;
	}

	echo " <body";

	// yes, this is deprecated, but we use bgcolor so that we may override with CSS when necessary
	if ($my_site_config->bgcolor)
		echo " bgcolor='{$my_site_config->bgcolor}' ";


	if (isset($my_page_config->includes) && $my_page_config->includes == 'yui_editor') { echo " class=\"yui-skin-sam\""; }

	if ($my_width || $my_height) {
		echo " style=\"";
		if ($my_width)	{ echo "width:" . $my_width . "px;"; }
		if ($my_height)	{ echo "height:" . $my_height . "px;"; }
		echo "\"";
	}
	
	if (isset($my_page_config->onLoad) && $my_page_config->onLoad != '') {
		echo " onload=\"{$my_page_config->onLoad}\"";
	}
	echo ">\n";

	if (! check_input_vars()) {
		if (! display_error('invalid_parm')) { echo "Invalid error code!<br>\n"; }
	} else {
		// Body container div added for IE
		echo " <div id='body'";
		if ($my_width || $my_height) {

			$bgcolor="#ffffff";
			if ($my_site_config->bgcolor) $bgcolor=$my_site_config->bgcolor;

			echo " style=\"background-color:{$bgcolor};";
			if ( $my_width ) 	{ echo "width:" . $my_width . "px;"; }
			if ( $my_height ) 	{ echo "height:" . $my_height . "px;"; }
			echo "\"";
		}
		echo ">\n";

		if (isset($my_page_config->layout->content)) {
			foreach ($my_page_config->layout->content as $c) {
                // if an ignore tag is there, then IGNORE - GRC
				if (!isset($c->ignore)) 
				{
					if (!CheckOmit($my_page_config,$my_site_config->site_id))
						generate_content($c, $my_height, $my_width);
				}
			}
		}
		if ($expects_extra_output) {
			echo "  </div>\n";
		}
	}

	if ($expects_extra_output && isset($my_site_config->last_body_contents)
		&& $my_site_config->last_body_contents != '') {
		echo trim($my_site_config->last_body_contents) . "\n";
	}


	if ($expects_extra_output) {
		echo " </body>\n";
		echo "</html>\n";
		$page_time = microtime(true) - $page_start;
		$qps = $db->num_queries / $page_time;
		echo "<!-- {$db->num_queries} queries, {$db->num_cached_queries} queries from cache, {$page_time} seconds, {$qps} qps -->\n";
		global $my_user;
		/* if (isset($my_user) && isset($my_user['id']) && $my_user['id'] == 11) {
			mail("glens@networldalliance.com", "{$input_vars['_page_name']} ({$db->num_queries})", implode("\n\n\n\n\n", $db->query_list));
			echo "<!-- mail sent -->";
		} */
	}

//	if ( isset( $my_site_config->buffer ) ) 	{ ob_end_flush(); }
	ob_end_flush();
}




/*
	Checks the <omit> tags to see if we should ignore this widget on any sites

	RETURNS: true if we should ignore
			else flase
*/

function CheckOmit($base,$site_id)
{

	if (!isset($base->omit)) return FALSE;

	foreach ($base->omit->site as $site)
	{
		if (trim($site) == $site_id)
			return TRUE;
	}

	return FALSE;

}



/**
 * Displays an error message from error file
 *
 * @param string $my_error_code The error code to display
 * @return bool Success or failure of message display
 *
 * @author Todd Tucker <toddt@networldalliance.com>
 */
function display_error($my_error_code) {
	include_once GLOBAL_INCLUDE_DIR . '/xmlfunc.inc';
	$errors = parse_xml_file(ERROR_FILE);

	if ($errors) {
		foreach ($errors as $error) {
			if ($error->code == $my_error_code) {
				$my_error_message = $error->message;
				$my_error_message = "<p class=\"error\">{$my_error_message}<br />\n";
				echo $my_error_message;
				return true;
			}
		}
		return false;
	} else {
		error_log("Cannot read error file: " . ERROR_FILE);
		return false;
	}
}





function replace_special_chars($my_string) {
	include_once GLOBAL_INCLUDE_DIR . '/xmlfunc.inc';

	if ($my_replacements = parse_xml_file(CHAR_FILE)) {
		foreach ($my_replacements as $r) {
			$my_pattern = "/" . preg_replace('/x/', '\x', $r->hexcode) . "/i";
			$my_replacement = "&" . $r->htmlcode;
			$my_string = preg_replace($my_pattern, $my_replacement, $my_string);
		}
	} else {
		error_log("HTML character replacement file does not exist!");
	}
	return $my_string;
}

/**
 * Wrapper function.  Calls other functions based on $my_content->type
 * @param object $my_content
 * @param string $my_height
 * @param string $my_width
 * @return void
 */
function generate_content($my_content, $my_height, $my_width) {

	if (isset($my_content->ignore)) {return;} // GRC

	switch ($my_content->type) {
		case 'static':
			generate_static( $my_content->text, $my_content->style );
			break;
		case 'code':
			generate_code( $my_content->name, $my_height, $my_width );
			break;
		case 'widget':
			generate_widget( $my_content->name, $my_height, $my_width, $my_content->style );
			break;
		case 'ad':
			generate_ad( $my_content->name, $my_content->height, $my_content->width,$my_content->style, $my_content->ad_type, $my_content->ad_size );
			break;
		case 'db':
			break;
		case 'form':
			generate_form( $my_content->name );	
			break;
		default:
			break;
	}
}

function generate_static( $my_text, $my_style ) {
	global $my_site_config;

	$my_text = preg_replace( '/{/', '<', $my_text );
	$my_text = preg_replace( '/}/', '>', $my_text );
    $my_text = trim($my_text);

	$my_text = preg_replace("/\[(.*?)\]/e", '(string)($my_site_config->$1)', $my_text);

	echo "<div";
	if(isset($my_style) && $my_style != "") {
		echo " style=\"{$my_style}\"";
	}
	echo ">\n";
	echo $my_text;
	echo "</div>\n";
}



function generate_code( $my_name, $my_height, $my_width ) 
{
	global $default_height;
	global $default_width;

	// try to load the site specific code first
	$my_filename = CONTENT_DIR . '/' . $my_name . '.inc';
	if ( file_exists( $my_filename ) ) {
		$ret_val = include $my_filename;
		return (bool)$ret_val;
	}
	// if we couldn't load the site code, try to grab it from the global list
	$my_filename = GLOBAL_CONTENT_DIR . '/' . $my_name . '.inc';
	if ( file_exists( $my_filename ) ) {
		$ret_val = include $my_filename;
		return (bool)$ret_val;
	}
	
	// if we've gotten here, then there's no code file anywhere... error
	display_error( 'file_missing' );
	error_log("Content include file missing: " . $my_filename);
	return false;
}




function generate_widget( $my_widget_name, $my_height, $my_width, $my_style=false ) {

    global $my_site_config, $default_height, $default_width, $ie, $current_widget_name;
	global $db;

    $current_widget_name = $my_widget_name;
    $can_be_displayed = true;

    ob_start();

	include_once GLOBAL_INCLUDE_DIR . "/xmlfunc.inc";
	include_once GLOBAL_INCLUDE_DIR . "/utils.inc";

	$my_widget_filename = WIDGET_XML_DIR . '/' . $my_widget_name . '.xml';

	// try to load the site specific widget
	if ( file_exists( $my_widget_filename ) ) {
		if ( ! ( $my_widget = parse_xml_file( $my_widget_filename ) ) ) {
			// error so that we know that there was a problem with the XML
			error_log("Error reading site widget XML: " . $my_widget_filename);
		}
	}	

	// if we couldn't load the site specific widget, or the file didn't exist, try to load the global widget
	if  ( ! isset($my_widget) || ! is_object( $my_widget ) ) {
		$my_widget_filename = GLOBAL_WIDGET_XML_DIR . '/' . $my_widget_name . '.xml';
		if ( file_exists( $my_widget_filename ) ) {
			if ( ! ( $my_widget = parse_xml_file( $my_widget_filename ) ) ) {
				// error so that we know that there was a problem with the XML
				error_log("Error reading global widget XML: " . $my_widget_filename);
			}
		} else {
			display_error( 'file_missing' );
			error_log("Global Widget XML file missing: " . $my_widget_filename);
			return false;
		}
	}

	// if we still don't have a widget object, die
	if ( ! is_object( $my_widget ) ) {
		error_log("Error reading widget XML: " . $my_widget_filename);
		display_error( 'parse_error' );
		return false;
	}

	if ( isset( $my_widget->height ) ) { $my_height = $my_widget->height; }
	if ( isset( $my_widget->width ) ) { $my_width = $my_widget->width; }

	if ( isset( $my_widget->db ) ) {
		if ( ! get_db_conn( $my_widget->db, $my_widget_name ) ) {
			display_error("db_connection");
			error_log("Problem connecting to database for widget " . $my_widget_name . ".");
			// error_log(var_export($my_widget->db, true));
			exit;
		} 
	}


	if (isset($my_widget->ignore)) {return;}  // GRC

	// if we have set the <omit> tag, then omit for the site listed in the tag

	if (CheckOmit($my_widget,$my_site_config->site_id)) return;



    $is_sponsored=FALSE;

// we only check for sponsorship IFF the widget is sponsorable
// GS idea, coded by GRC
if ( isset($my_widget->sponsorable) && $my_widget->sponsorable === 1 )
{

	//GRC-GS caching scheme
	$db->cache_queries = true;
	$db->cache_dir = '/tmp/' . $_SERVER['SERVER_NAME'] . '/widget/'.$my_widget_name;
	$db->use_disk_cache = true;
	$db->cache_timeout = 24;


	// check to see if the widget is sponsored
	$sql = <<< SQL
	SELECT
		url,
		target,
		image 
	FROM division_urls du
	INNER JOIN widget_sponsors ws
	ON du.id = ws.url_id
	INNER JOIN _division_images di
	ON ws.image_id = di.id
	INNER JOIN division_sites ds
	ON di.division_id = ds.division_id
	WHERE
		widget_name = '{$my_widget_name}' AND
		ds.site_id = {$my_site_config->site_id}
/*	ORDER BY RAND()
	LIMIT 1*/
SQL;

	//GRC-GS caching scheme
    $results = $db->get_results($sql);
    $sponsor = $results[array_rand($results, 1)];

    if ( $sponsor ) 
	{
        if ($db->from_disk_cache === true) 
		{
            echo "<!-- widget sponsor loaded from cache -->";
        }

		$is_sponsored=TRUE;
		$linkUrl = $sponsor->url;
		$linkTarget = $sponsor->target;
		$linkImage = $sponsor->image;
	}

/*
	OLD CODE
	// 0 results means not sponsored
	$is_sponsored = (bool)($db->get_var($sql, 0));
	$linkUrl = $db->get_var(NULL, 0);
	$linkTarget = $db->get_var(NULL, 1);
	$linkImage = $db->get_var(NULL, 2);
*/
	if ((bool)$is_sponsored === true) 
	{
		$_SESSION['sponsored_widget'] = (string)$my_widget_name;
	}

} // end if (isset($my_widget->sponsorable) && $my_widget->sponsorable === 1)


	echo "<div style=\"";
	if ( $my_style ) {
		echo $my_style;
	}
	if ( $my_width || $my_height ) {
		if ( $my_width ) {
			$my_container_width = $my_width + 2;
			echo "width:" . $my_container_width . "px;";
		}
		if ( $my_height ) {
			echo "height:" . $my_height . "px;";
		}
	}
	echo "\"";
	if ($my_widget->class) {
		echo " class='{$my_widget->class}'";
	}
	echo ">";
	echo "<div id=\"" . str_replace("/", "_", $my_widget_name) . "\" class=\"widget";
	if ($my_widget->class) {
		echo " {$my_widget->class}";
	}
	echo "\"";
	echo " style=\"";

//GRC
// if the site config gradient and the wideget gradient arent set, then dont use gradient
if ( isset($my_site_config->gradient) && isset($my_widget->title->gradient) )
{
	if ( isset( $my_widget->title->gradient ) && $ie ) 
	{
		// Must put quotes around my_width for ImageMagick
		// Investigate later
		if ( isset($my_widget->title->gradient->start_color) ) 
		{
		   $my_image = create_gradient( 1, 28, 'portrait',
		   	       $my_widget->title->gradient->start_color,
			       $my_widget->title->gradient->end_color );
	    } else {
		   $my_image = create_gradient( 1, 28, 'portrait',
		   	       $my_site_config->gradient->default_start_color,
			       $my_site_config->gradient->default_end_color );
		}

		echo "background-image:url(";

		if ( isset( $my_widget->title->gradient->use_fqdn ) && $my_widget->title->gradient->use_fqdn ) {
			echo "http".(isset($_SERVER['HTTPS']) ? "s" : "")."://";
			echo $_SERVER['SERVER_NAME'];
		}
		echo GRAD_URL . "/" . $my_image . ");";
		echo "background-repeat:repeat-x;border-top: 1px solid #888;";
	}
}

	if ( $my_width || $my_height ) {
		if ( $my_width ) {
			echo "width:" . $my_width . "px;";
		}
		if ( $my_height ) {
			echo "height:" . $my_height . "px;";
		}
	}

	if ( isset( $my_widget->style ) ) {
		echo $my_widget->style;
	}
	echo "\"";
	echo ">";

	if ( isset( $my_widget->title ) ) 
	{
		echo "<div class=\"widget_title\"";
		echo " style=\"";

		//commented Jan 22, 2010 glens, was causing backgrounds to not run the full width
		// echo "width:" . ($my_width-10) . "px;";

//GRC
// if the site config and widget gradients arent set, then dont use gradient
if ( isset($my_site_config->gradient) && isset($my_widget->title->gradient) )
{
		if ( isset( $my_widget->title->gradient ) && ! $ie ) 
		{
			// Must put quotes around my_width for ImageMagick
			// Investigate later
            if ( isset($my_widget->title->gradient->start_color) ) 
			{
                $my_image = create_gradient( 1, 28, 'portrait',
                        $my_widget->title->gradient->start_color,
                        $my_widget->title->gradient->end_color );
            } else {
                $my_image = create_gradient( 1, 28, 'portrait',
                        $my_site_config->gradient->default_start_color,
                        $my_site_config->gradient->default_end_color );
            }
			echo "background-image:url(";
			if ( isset( $my_widget->title->gradient->use_fqdn ) && $my_widget->title->gradient->use_fqdn ) 
			{
				echo "http".(isset($_SERVER['HTTPS']) ? "s" : "")."://";
				echo $_SERVER['SERVER_NAME'];
			}
			echo GRAD_URL . "/" . $my_image . ");";
			echo "background-repeat:repeat-x;border-top: 1px solid #888;";
		}
}

		echo "\"";
		echo ">";

		//GRC
		// TwoToneTitle function
		//function TwoToneTitle($topcolor,$botcolor,$width,$text,$textcolor,$bordercolor)
		// both site and widget two_tone must be set, otherwise dont use two tone titles
		if (isset($my_site_config->two_tone) && isset($my_widget->title->two_tone))
		{
 			// step thru all the title declarations in site_setup.xml
			foreach ($my_site_config->two_tone->title as $title)
			{
				if (trim($my_widget->title->two_tone) === trim($title->type))
				{
					$mytext = TwoToneTitle($title->top_color,$title->bottom_color,trim($my_widget->width),$my_widget->title->text,$title->text_color,$title->border_color);
					echo $mytext;
				}
			}
		}
		else
		{
			echo $my_widget->title->text . "";
		}
	
		echo "</div>";

//var_dump($my_site_config);
//exit;


	}     
	if ( ! isset( $my_widget->nobody ) ) 
	{
		echo "<div class=\"widget_body\" style=\"";
		// Reduce by 10 to handle widget padding.  Revisit later.
		//             $my_width = $my_width - 10;
		echo "width:" . $my_width . "px;\"";
		echo ">";
	}

	global $noform;
	$noform = ( ! isset( $my_widget->noform ) ) ? 0 : 1;
	if ( ! isset( $my_widget->nocode ) ) 
	{
		$ret_val = generate_code( $my_widget_name, $my_height, $my_width-10 );
        $can_be_displayed = (bool)$ret_val;
	}

	if ( ! $noform ) 
	{
		generate_form( $my_widget_name );
	}

	if (isset($my_widget->content)) 
	{
		foreach ($my_widget->content as $c) 
		{
			// Reduce by 10 to handle widget padding.  Revisit later.
			generate_content( $c, $my_height, $my_width-5 );
		}
	}

	if (! isset($my_widget->nobody)) 
	{
		echo "</div>";
	}
	echo "</div>";

	if ( ! isset( $my_widget->nocurves ) ) 
	{
		echo "<b class=\"b4\"></b><b class=\"b3\"></b><b class=\"b2\"></b><b class=\"b1\"></b>";
	}
	echo "</div>\n";

    if ((bool)$can_be_displayed === false) { ob_get_clean(); } // dump the ob
    else { ob_flush(); }
	$db->cache_queries = false;
	$db->use_disk_cache = false;
}



function generate_form ( $my_form_name ) 
{
	global $input_vars, $tab_index, $my_site_config;

	include_once "html.class.inc";
	include_once "formfields.inc";

	$my_form_filename = FORM_XML_DIR . "/" . $my_form_name . ".xml";
	$myform = '';

	if (file_exists($my_form_filename)) {
		if ( ! ( $myform = parse_xml_file( $my_form_filename ) ) ) {
			error_log("Error parsing site form XML: {$my_form_filename}");
		}
	}

	if ( ! is_object( $myform ) ) {
		$my_form_filename = GLOBAL_FORM_XML_DIR . "/" . $my_form_name . ".xml";
		if ( file_exists( $my_form_filename ) ) 
		{
			if ( ! ( $myform = parse_xml_file( $my_form_filename ) ) ) 
			{
				error_log("Error parsing global form XML: {$my_form_name}");
			}
		}
	}

	if ( ! is_object( $myform ) ) 
	{
		display_error( 'file_missing' );
		error_log("Global form XML file missing: " . $my_form_filename);
		return false;
	}


	if ( isset( $myform->id ) ) 
	{
		echo "<div id=\"" . $myform->id . "\">\n";
	}
	echo "\n<form enctype=\"multipart/form-data\" method=\"";

    if ( isset( $myform->method ) ) 
	{
        echo strtolower($myform->method);
    } else {
        echo strtolower("post");
    }
    echo "\" name=\"" . $myform->name . "\" ";

	if ( isset( $myform->target ) ) {
		echo "target=\"" . $myform->target . "\" ";
	}

	if ( isset( $myform->action ) ) {
		echo "action=\"" . $myform->action . "\" ";
	} else {
		echo "action=\"\" ";
	}

	if ( isset( $myform->on_submit ) ) 
	{
		echo "onsubmit=\"" . $myform->on_submit . "\"";
	}
	echo ">\n";

	if (isset($myform->headline)) 
	{
		$content = getValueFromXMLOrDB($myform, 'headline');
		echo "<h2>{$content}</h2>\n";
	}

	if (isset($myform->pre_message)) 
	{
		$content = getValueFromXMLOrDB($myform, 'pre_message');
		echo "<p class=\"pre_message\">{$content}</p>\n";
	}

	if ( isset( $myform->field ) ) 
	{
		foreach ( $myform->field as $myfield ) 
		{

			if ( ! isset( $myfield->show_field_var ) ||
					( array_key_exists( (string)$myfield->show_field_var, (array)$input_vars )
					||
					( preg_match('/^\!/', $myfield->show_field_var)
						&& ! array_key_exists( (string)substr($myfield->show_field_var,1), (array)$input_vars ) ) ) ) {

				if ($myfield->accordian) 
				{
					echo "<h2 class=\"accordian\">" . $myfield->accordian . "</h2>";
					echo "<div>";
				}

				// Replace bracketed text in field name and info with input_vars
                $myfield->name = replace_with_values($myfield->name);
				if ( isset( $myfield->info ) ) 
				{
					//GRC - replace_with_values() replaces [x] with matches from $input_vars[]
                    $myfield->info = replace_with_values($myfield->info);
//					GLOBAL $my_site_config;
//				    $myfield->info=  $my_site_config->site_id ."/". $myfield->info;

				}

				// call a fn in formfields.inc
				if ( ! call_user_func( "generate_" . $myfield->type . "_field", $myfield, $myform->name ) ) {
					error_log("Invalid Form Field Type: " . $myfield-type . " for field Name: " . $myfield->name);
					if ( ! display_error( 'invalid_field' ) ) {
						echo "Invalid error code!<br>\n";
					}
				}
				if ($myfield->end_accordian) {
					echo "</div>";
				}
			}
		}
	}


	if ( isset( $myform->graphic_submit ) ) 
	{
		generate_submit_button( $myform->submit_label, $myform->name, 0, $myform->graphic_submit );
	}  
	else 
	{
		if ( ! isset( $myform->submit_label ) ) {	$myform->submit_label = "Submit";	}
		if ( isset( $myform->standard_submit ) ) 
			{	generate_submit_button( $myform->submit_label, $myform->name, $myform->standard_submit );	} 
		else 
			{	generate_submit_button( $myform->submit_label, $myform->name );		}
	}

	if ( isset( $myform->post_message ) ) 
	{
        $myform->post_message = replace_with_values($myform->post_message);
		echo "<p class=\"post_message\">" . $myform->post_message . "</p>\n";
	}

	echo "</form>\n";

	if ( isset( $myform->id ) ) {		echo "</div>\n";	}
}





/**
 * generate an ad based on parameters
 *
 * @uses $db
 * @uses $my_site_config
 *
 * @todo Convert queries to similar base
 */
function generate_ad( $my_ad_name, $my_height, $my_width, $my_style=false, $my_type, $my_ad_size ) 
{

	global $default_height, $default_width, $ie, $db;
	global $my_site_config, $page_name, $page_sponsor;

	$db->cache_queries = true;
	$db->cache_dir = '/tmp/' . $_SERVER['SERVER_NAME'] . '/ads';
	$db->use_disk_cache = true;
	$db->cache_timeout = 24;

//GRC added the date checking lines in all queries below

	if ( $page_sponsor ) 
	{
		if ( is_array( $page_sponsor ) ) 
		{
			// we have mulitple page sponsors
			$my_query = "
			SELECT ADM.id,ADM.division_id,b.image,c.url,ADM.styles,ADM.start_date,ADM.end_date, c.target,b.alt_text
			FROM _ad_map ADM 
			INNER JOIN _division_images b ON ADM.image_id=b.id 
			INNER JOIN division_urls c ON a.url_id=c.id 
			WHERE ADM.site_id=" . $my_site_config->site_id . " 
			";
			if ((string)$my_ad_name !== 'roadblock') {
				$my_query .= " AND ADM.page_name='" . $page_name . "' ";
			}
			$my_query .= "
				AND ADM.ad_name='" . $my_ad_name . "' 
				AND ADM.division_id IN (" . implode(',', $page_sponsor) . ") 
 			AND DATE(NOW()) BETWEEN ADM.start_date AND ADM.end_date
			";
		} 
		else 
		{

			// single page sponsor
			$my_query = "
				SELECT ADM.id,ADM.division_id,b.image,c.url,ADM.styles,ADM.start_date,ADM.end_date, c.target,b.alt_text
				FROM _ad_map ADM 
				INNER JOIN _division_images b ON ADM.image_id=b.id 
				INNER JOIN division_urls c ON ADM.url_id=c.id 
				WHERE ADM.site_id=" . $my_site_config->site_id . " 
			";
			if ((string)$my_ad_name !== 'roadblock') {
				$my_query .= " AND ADM.page_name='" . $page_name . "' ";
			}
			$my_query .= "
				AND ADM.ad_name='" . $my_ad_name . "' 
				AND ADM.division_id=" . $page_sponsor . " 
				AND DATE(NOW()) BETWEEN ADM.start_date AND ADM.end_date
				";
		}
	} 
	else 
	{
		// NO PAGE SPONSOR
		$my_query = "
			SELECT ADM.id,ADM.division_id,b.image,c.url,ADM.styles,ADM.start_date,ADM.end_date, c.target,b.alt_text
			FROM _ad_map ADM 
			INNER JOIN _division_images b ON ADM.image_id=b.id 
			INNER JOIN division_urls c ON ADM.url_id=c.id 
			WHERE ADM.site_id=" . $my_site_config->site_id . " 
			";
			if ((string)$my_ad_name !== 'roadblock') {
				$my_query .= " AND ADM.page_name='" . $page_name . "' ";
			}
			$my_query .= "
			AND ADM.ad_name='" . $my_ad_name . "' 
            AND ADM.active=1
			AND DATE(NOW()) BETWEEN ADM.start_date AND ADM.end_date
			";
	}

	$results = $db->get_results($my_query);

	if (! is_array($results)) {
		return false;
	}
	$my_ad = $results[array_rand($results, 1)];

	if ( $my_ad ) {
		if ($db->from_disk_cache === true) {
			echo "<!-- loaded from cache -->";
		}
		if ((string)$my_ad_name !== 'roadblock') { // special handling for roadblocks.... do *not* log until the browser actually shows it
			log_ad_view($my_ad->id,$page_name,$my_site_config->site_id);
		}
		echo "\n<div style=\"";
		if ($my_style) {
			echo $my_style;
		}

		if ((int)$my_width > 0) {
			$my_container_width = $my_width + 2;
			echo "width: {$my_container_width}px;";
		}
		if ((int)$my_height > 0) {
			echo "height: {$my_height}px;";
		}
		echo "\">";

		echo "<div id=\"{$my_ad_name}\" class=\"widget\" style=\"";
		//  IE SUCKS, so this has to be added here and tested for
		if ((int)$my_width > 0) {
			echo "width: {$my_width}px;";
		}
		if ((int)$my_height > 0) {
			echo "height: {$my_height}px;";
		}

		echo "border-style:none;";

		if (isset($my_widget->style)) {
			echo $my_widget->style;
		}
		if ($my_ad->styles != "") {
			echo $my_ad->styles;
		}
		echo "\">";

		$my_wh = preg_split("/x/", $my_ad_size );
		$my_ad_image = CDN_COMPANY_URL . '/' . $my_ad->image;


		if ( preg_match( '/\.sw/', $my_ad->image ) ) {
			// FLASH AD
			echo <<< AD
			<object type="application/x-shockwave-flash"
				data="{$my_ad_image}"
				width="{$my_wh[0]}"
				height="{$my_wh[1]}"
				class="ad"
                onclick="ad_click({$my_ad->id},'{$page_name}',{$my_site_config->site_id}); return true;">

			<param name="movie" value="{$my_ad_image}" />
			<param value="high" name="quality">
			<param value="#FFFFFF" name="bgcolor">
			<param value="always" name="allowScriptAccess">
			<param value="target={$my_ad->target}&clickTAG={$my_ad->url}&clickTag={$my_ad->url}" name="FlashVars">
			<param value="transparent" name="wmode">
			</object>
AD;
		} else {
			// IMG AD


			echo "<a href=\"{$my_ad->url}\" target=\"{$my_ad->target}\"
				class=\"ad\"
                onclick=\"ad_click({$my_ad->id}, '{$page_name}', {$my_site_config->site_id}); return true;\"";

			if ( preg_match( '/http/', $my_ad->url ) ) {
				echo " target=\"_blank\"";
			}
			echo ">";
			echo "<img src='{$my_ad_image}' alt='{$my_ad->alt_text}' style=\"";
			if ((string)$my_ad_name !== 'roadblock') { // special handling for roadblocks.... do *not* resize!!!
				echo "width: {$my_wh[0]}px;height: {$my_wh[1]}px;";
			}
			echo "\" />\n";
			echo "</a>";
		}

		echo "</div>";
		echo "</div>\n";
	}
	$db->cache_queries = false;
	$db->use_disk_cache = false;
}

/**
 * display links to prev/next pages
 *
 * @deprecated
 * @see paginate
 * @author Glen Solsberry <glens@networldalliance.com>
 */
function prev_next($offset, $limit, $sort, $total, $extra_querystring = "") 
{
	$lower_display_offset = $offset + 1;
	if ($offset > 0) 
	{
		echo "<a href=\"?offset=0&sort={$sort}{$extra_querystring}\" title=\"First Page\">&laquo;</a>&nbsp;";
		echo "<a href=\"?offset=".($offset - $limit < 0 ? "0" : ($offset - $limit))."&sort={$sort}{$extra_querystring}\" title=\"Previous Page\">&lsaquo;</a>&nbsp;&nbsp;";
	}

	$upper_display_offset = $lower_display_offset - 1 + $limit;
	if ($upper_display_offset > $total) { $upper_display_offset = $total; }

	echo "Viewing {$lower_display_offset} - {$upper_display_offset} of {$total} results";

	if (($offset + $limit) < $total) 
	{
		echo "&nbsp;&nbsp;<a href=\"?offset=".($offset + $limit)."&sort={$sort}{$extra_querystring}\" title=\"Next Page\">&rsaquo;</a>";
		echo "&nbsp;<a href=\"?offset=".($total - $limit)."&sort={$sort}{$extra_querystring}\" title=\"Last Page\">&raquo;</a>";
	}
}





/**
 * display links to sort list pages
 *
 * @deprecated
 * @todo replace with something "Correct"
 * @author Glen Solsberry <glens@networldalliance.com>
 */
function sorter($field, $extra_querystring = "", $link_title = "") 
{
	if ( $link_title == "" ) { $link_title = "Sort by {$field}"; }
	print<<<HTML
	<a class="sort_up" href="?sort={$field}&dir=ASC{$extra_querystring}" title="{$link_title}"><span style="padding-left: 11px; padding-right: 2px;">&nbsp;</span></a>
	<a class="sort_down" href="?sort={$field}&dir=DESC{$extra_querystring}" title="{$link_title}"><span style="padding-left: 11px; padding-right: 2px;">&nbsp;</span></a>
HTML;
}



function load_styles($sheet) 
{
	global $db;

	if ( isset( $sheet ) && $sheet != '' ) 
	{
		foreach ( $sheet as $s ) 
		{
			if (isset($s->attributes()->query) && preg_match("/\[(.*?)\]/", $s->attributes()->query, $matches)) 
			{
				$url = $db->get_var(preg_replace("/\[{$matches[1]}\]/", $input_vars[$matches[1]], $s->attributes()->query));
				$s = $url . $s;
			}

				$cssFile="";
                if ( preg_match('/http\:\/\//', $s) )
					$cssFile= $s;
				else
				{
					if (file_exists(CSS_DIR . "/" . $s)) {
						$cssFile= CSS_URL . "/" . $s;
					} else if (file_exists(GLOBAL_CSS_DIR . "/" . $s)) {
						$cssFile= GLOBAL_CSS_URL . "/" . $s;
					} else {
						error_log("Requesting load of CSS file {$s}.  Doesn't exist locally or globally");
					}
				}

			if (strlen($cssFile) >0)
			{
				echo "<link rel=\"stylesheet\" type=\"text/css\" href=\"";
				echo $cssFile; 
				echo "\" />\n";
			}

		}
	}
}



function load_scripts($script) {
	if ( isset( $script ) && $script != '' ) {
		foreach ( $script as $js ) {
            $js_url = (preg_match('/^http/', $js) ? $js : JS_URL . "/" . $js);
			echo "<script type=\"text/javascript\" src=\"{$js_url}\"></script>\n";
		}
	}
}

function add_meta_tag($tag) {

	global $my_page_config, $db;

	$content = getValueFromXMLOrDB($my_page_config, $tag);
	echo "<meta name=\"{$tag}\" content=\"" . trim(htmlspecialchars(strip_tags($content))) . "\" />\n";
}

function getValueFromXMLOrDB($object, $subkey) {
	/*
	 * Old function
	 * 
	 * added else condition so that $result always had a value to return.  This was broken for headlines/pre messages on forms, and possibly other places
	 */
	/*
	global $db;

	if (isset($object->$subkey)) {
		if (isset($object->$subkey->sql)) {
			$sql = replace_with_values($object->$subkey->sql);
			$result = $db->get_var($sql);
		}
	} else {
		$result = replace_with_values($object->$subkey);
	}

	return $result;*/
	global $db;

	if (isset($object->$subkey)) 
	{
		if (isset($object->$subkey->sql)) {
			$db->cache_queries = true;
			$db->cache_dir = '/tmp/' . $_SERVER['SERVER_NAME'] . '/value_from_xml_or_db';
			$db->use_disk_cache = true;
			$db->cache_timeout = 0.5;

			$sql = replace_with_values($object->$subkey->sql);
			$result = $db->get_var($sql);

			$db->cache_queries = false;
		} else {
	        $result = replace_with_values($object->$subkey);
		}
	} else 
	{
		// this should never happen
		$result = "";
	}

	return $result;
}


function Load_Global_CSSJS()
{
//GRC
// read global xml file for CSS and JS
	echo "<!--GLOBALS-->\n";
    $my_globals = parse_xml_file( GLOBAL_CSSJS_FILE );

	foreach ($my_globals->css_scripts as $css) {
		load_styles($css->stylesheet);
	}

	foreach ($my_globals->js_scripts as $js) {
		load_scripts($js->js);
	}
    echo "<!--END GLOBALS-->\n";

}

//GRC. this function will display a widget title block
// using two divs, one overlaid on the other

function TwoToneTitle($topcolor,$botcolor,$width,$text,$textcolor,$bordercolor)
{
    $shortwidth = (int)$width-10;
    $innerstr = "<div id='twotonetitle' class='top_layer' style='width:{$shortwidth}px;background:{$topcolor};color:{$textcolor};'>{$text}</div>";
    $mystr = "<div id='twotonetitle' class='bot_layer' style='border-bottom:2px solid {$bordercolor}; width:{$width}px;background:{$botcolor};'>{$innerstr}</div>";
    return $mystr;
}


?>
