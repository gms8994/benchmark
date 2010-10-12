<?php

/**
 * Miscellaneous functions
 *
 * @package Utils
 * @author  Todd Tucker <toddt@networldalliance.com>
 * @author  Glen Solsberry <glens@networldalliance.com>
 * @author  Gary Conway <garyc@networldallianc.com>
 * @version 20100302-01
 */

/**
 * input vars
 * @global array $input_vars
 */
include_once GLOBAL_INCLUDE_DIR . "/utils.inc";

global $input_vars, $my_user, $widgetList;

$widgetList = array();

// GRC
// this function will take data from a class and
// format NumCols <ul>'s horizontally
// CSS file: site_styles.css

function DisplayColumns($ClassData,$NumCols,$BlockWidth,$element,$directory,$tags='',$idfield='id') {
	// element is the element of the class to access
	// directory is the base link adress to the datafile

    echo "<div class='SClistwrapper';>";
    $RowsPerCol = count($ClassData)/$NumCols;
    $Index=0;
    $LastIndex=$Index+$RowsPerCol;

    $colwidth=($BlockWidth/$NumCols)-45;

    for ($columns=0; $columns<$NumCols; $columns++) {
        echo "<ul class='SClist square' style='width:{$colwidth}px'>";
        for ($n=$Index; $n < $LastIndex; $n++) {
//           $link = $directory . urlify(strip_tags($ClassData[$n]->$element));

           $link = $directory. $ClassData[$n]->$idfield."/"  . urlify(strip_tags($ClassData[$n]->$element));

			if ($tags !='')
				$link .= "?tag=".$ClassData[$n]->id;
            echo "<li><a href='{$link}'><span class='SClista'>". strip_tags($ClassData[$n]->$element) ."</span></a></li>";
            $Index++;
        }
        echo "</ul>";
        $LastIndex = $LastIndex+$RowsPerCol;
    }

    echo "</div>";

}

/**
 * Displays an asset summary such as Features, Blog, Video, Whitepaper
 *
 * added imageLink for the blogger image link to their blogs
 *
 * @uses urlify()
 * @uses ellipsis()
 *
 * @param array $assetData
 *
 * @author Ben Kuhl <benk@networldalliance.com>
 * @author Gary Conway <garyc@networldalliance.com>
 * @author Glen Solsberry <glens@networldalliance.com>
 */
function displayAssetSummary ($assetData) {
	$title = urlify($assetData['title']);
	$assetData['author'] = trim($assetData['author']);
	
	echo "<li class='asset'>\n";
	
	$image = '';
	if (array_key_exists('thumb', $assetData) && strlen($assetData['thumb']) > 0) {
		$image = 'thumb';
	} elseif (array_key_exists('image', $assetData) && strlen($assetData['image']) > 0) {
		$image = 'image';
		if (!array_key_exists('class', $assetData)) {
			$assetData['class'] = 'summary-small';
		}
	}

	if (strlen($image) > 0) {
		echo "	 <div class='".$image."'>";
		if (strlen($assetData[$image]) > 0) {
			if (array_key_exists('imageLink', $assetData)) {
				// GRC: we have a special link for the image (blogs)
				echo "
                <a href='".$assetData['imageLink']."'>
                <img src='".$assetData[$image.'Url'].$assetData[$image]."' style=\"{$assetData[$image.'Style']}\">
                </a>";
 			} else {
				echo "
				<a href='".$assetData['assetUrl'].$title."'>
				<img src='".$assetData[$image.'Url'].$assetData[$image]."' style=\"{$assetData[$image.'Style']}\">
				</a>";
			}	
		}
		echo "	</div>
	<div class='summary";
		if (array_key_exists('class', $assetData) && strlen($assetData['class']) > 0) {
			echo ' '.$assetData['class'];
		}
		echo "'>";
		$showThumb = 1;
	}
	
	if (array_key_exists('sponsor', $assetData) && is_array($assetData['sponsor'])) {
		echo "
		<div class='left sponsored'>";
		$showSponsor = 1;	
		$sponsorClass = ' class="sponsored"';
	}
	echo "
		<h2{$sponsorClass}>
			<a href='".$assetData['assetUrl'].$title."'>".$assetData['title']."</a>
		</h2>";
	if ($showSponsor > 0) {
		echo "
		</div>
		<div class='right'>
			<span class='sponsored-by'>Sponsored by:</span><br/>
			<a href='".$assetData['sponsor']['linkUrl']."'>
				<img src='".$assetData['sponsor']['imageUrl']."'/>
			</a>
		</div>
		<div class='clear'></div>";
	}
		
	if ($assetData['synopsis'] === '' || $assetData['synopsis'] === null) {
		$assetData['synopsis'] = trim(ellipsis($assetData['body'], 200));
	} else {
		$assetData['synopsis'] = ellipsis($assetData['synopsis'], 200);
	}
	
	if ((!isset($assetData['displaySynopsis']) || $assetData['displaySynopsis'] === true) && strlen($assetData['synopsis']) > 0) {
		echo "
		<div class='synopsisrc'>
			".strip_tags($assetData['synopsis'])."
		</div>";
	}
	
	if (is_string($assetData['tagList']) && strlen($assetData['tagList']) > 0) {
		$assetData['tagList'] = explode(',', $assetData['tagList']);
		sort($assetData['tagList']);
	}
	$tagCount = count($assetData['tagList']);
	if ($tagCount > 0) 
	{
		echo "
		<div class='tag-list-rc'>
			<span class='label'>Tags:</span> ";
		$x = 1;
		foreach ($assetData['tagList'] as $tag) 
		{

			if (is_object($tag))
			{ 
				$tagName = $tag->name;
				$urlID = "";
				if (isset($tag->id))
					$urlID = $tag->id ."/";
			}
			else 
				$tagName = $tag;
	
			echo "<a href='".$assetData['tagUrl'].$urlID .urlify($tagName)."'>".html_safe($tagName)."</a>";
			if ($x != $tagCount) {
				echo ", ";
			}
			$x++;
		}	

		echo "
		</div>";
	}
	if (array_key_exists('type', $assetData)) {
		echo "
		<div class='type'>
			<span class='label'>Type:</span> ".$assetData['type']."
		</div>";
	}
	if (array_key_exists('sponsor', $assetData) && !is_array($assetData['sponsor'])) {
		echo "
		<div class='sponsor'>
			<span class='label'>Sponsored by:</span> ".$assetData['sponsor']."
		</div>";
	}
	if (!empty($assetData['posted']) || !empty($assetData['author'])) {
		echo "
		<span class='by'>
			".$assetData['posted'];
		if (!empty($assetData['posted']) && !empty($assetData['author'])) {
			echo " - ";
		}
			echo $assetData['author']."
		</span>";
	}
	
	if ($showThumb == 1) {
		echo "
	</div>
	<div class='clear'></div>";
	}
	echo "</li>";
}

//============================================================
//
// read a parameter from GET,POST or COOKIE
//
//============================================================

// call with param("id");
// and routine will return the value regardless of how it
// was passed
// return a string, having removed any HTML or PHP tags and converting
// any special characters to the proper HTML syntax



function param($Name)
{
	global $_GET;
	global $_POST;
	global $_COOKIE;

	if (isset($_POST[$Name])) {
		$temp = strip_tags($_POST[$Name]);
		return(htmlspecialchars($temp));
	}

	if (isset($_COOKIE[$Name])) {
		$temp = strip_tags($_COOKIE[$Name]);
		return(htmlspecialchars($temp));
	}


	if (isset($_GET[$Name] )) {
		$temp = strip_tags($_GET[$Name]);
		return(htmlspecialchars($temp));
	}


	return("");
}





function create_gradient($my_columns, $my_rows, $my_orientation, $my_start_color, $my_end_color) {

	if (! $my_columns || ! $my_rows) {
		error_log("Gradient: Columns or Rows not specified.");
		return false;
	}

	if (! $my_orientation) {
		error_log("Gradient: Orientation not specified; assuming portrait.");
		$my_orientation = "portrait";
	}

	if (! $my_start_color || ! $my_end_color) {
		error_log("Gradient: Start or End color not specified; using black to white.");
		$my_start_color = "ffffff";
		$my_end_color   = "000000";
	}

	$my_image_filename = $my_columns . '_' . $my_rows . '_' .
		$my_start_color . '_' . $my_end_color . '_' .
		$my_orientation . '.png';

	if (file_exists(GRAD_DIR . '/' . $my_image_filename)) {
		return $my_image_filename;
	} else {
		$my_image = new Imagick();
		$my_grad_string = 'gradient:#' . $my_start_color . '-#' . $my_end_color;
		$my_image->newPseudoImage($my_columns, $my_rows, $my_grad_string);
		if ($my_orientation == 'landscape') {
			$my_image->rotateImage(new ImagickPixel(), 270);
		}
		$my_image->setImageFormat('png');
		$my_image_filename = $my_columns . '_' . $my_rows . '_' .
			$my_start_color . '_' . $my_end_color . '_' .
			$my_orientation . '.png';
		$my_image_path = GRAD_DIR . '/' . $my_image_filename;
		$my_image->writeImage($my_image_path);

		return $my_image_filename;
	}
}


/**
 * load data into $input_vars
 *
 * @uses $input_vars
 * @return bool Success/failure
 * @author Todd Tucker <toddt@networldalliance.com>
 */
function get_input_vars() {
	global $input_vars;

	if (count($input_vars) == 0) {
		$input_vars = array();
	}

	if (! empty($_GET)) {
		foreach ($_GET as $key => $value) {
			if (get_magic_quotes_gpc() == 1) {
				while (strstr($value, '\\')) {
					$value = stripslashes($value);
				}
			}
			$input_vars[$key] = $value;
		}
	}


	if (! empty($_POST)) {
		foreach ($_POST as $key => $value) {
			if (get_magic_quotes_gpc() == 1) {
				while (strstr($value, '\\')) {
					$value = stripslashes($value);
				}
			}
			$input_vars[$key] = $value;
		}
	}


	if (empty($input_vars)) {
		return false;
	} else {
		return true;
	}
}



function check_input_vars() {
	global $input_vars;
	return true;
}



function is_good_captcha() {
	global $input_vars;

	$my_sid = session_id();

	if (isset($input_vars['image_verification'])) {
		$my_result = file_get_contents("http://global.networldalliance.com/verifier.php?c={$input_vars['image_verification']}&sid={$my_sid}");
		if ($my_result === "1") {
			return true;
		}
	}

	// @todo Add logic to store number of bad captcha attempts in session variable
	return false;
}

function resized_image_exists( $width, $height, $image_filename, $image_path = IMAGE_DIR ) {
	if (! $image_filename || $image_filename == "") {
		return false;
	}
	$my_pattern = "/" . $width . "x" . $height . "/";
	if (preg_match($my_pattern, $image_filename)) {
		return $image_filename;
	} else {
		$my_name_parts = explode('_', $image_filename);
		$my_resized_name = $image_path . "/resized/" . $my_name_parts[0] . "_" .  $width . "x" . $height . "_" .  substr($my_name_parts[2], 0, -4) . ".png";
		if (file_exists($my_resized_name)) {
			return "resized/" . $my_name_parts[0] . "_" . $width . "x" .  $height . "_" . substr($my_name_parts[2], 0, -4) . ".png";
		} else {
			if (resize_and_name_image($image_path . "/" . $image_filename, $my_resized_name, $height, $width)) {
				return "resized/" . $my_name_parts[0] . "_" . $width . "x" .  $height . "_" . substr($my_name_parts[2], 0, -4) . ".png";
			} else {
				return false;
			}
		}
	}
}

/**
 * Resizes and renames an image file
 *
 * @param string $source    Path to source image
 * @param string $dest      Path to output image
 * @param int    $n_height  New height to set image to
 * @param int    $n_width   New width to set image to
 * @param bool   $overwrite Allow overwrite of image file
 * @param bool   $backup    Create a backup copy of the imagein "source/" folder under current folder
 *
 * @return  resource An image identifier
 * @return  false on error
 * @author  Glen Solsberry <glens@networldalliance.com>
 * @author  Gary Conway <garyc@networldalliance.com>
 * @version 20100101-01
 *
 * @todo Handle converting images to a known "good" type (tiff -> jpg, etc)
 * @todo Automatically rename images (remove punctuation, etc)
 */
function resize_and_name_image($source, $dest, $n_height, $n_width, $overwrite = false, $backup = true) {
	if (!file_exists($source)) {
		error_log("$source doesn't exist");
		return false;
	}
	if (file_exists($dest) && !$overwrite) {
		error_log("$dest exists, and we're not overwriting");
		return false;
	}

	list($width, $height, $type, $attr) = image_info($source);
	list($n_height, $n_width) = resize_with_aspect($height, $width, $n_height, $n_width);

	$mime_type = image_type_to_mime_type($type);
	$mime_type = preg_replace("/^.*?\//", "", $mime_type);

	// call the appropriate imagecreatefrom function for this image
	if (function_exists("imagecreatefrom" . $mime_type)) {
		$img = call_user_func("imagecreatefrom" . $mime_type, $source);
	} else {
		display_error("invalid_image_format");
		error_log("imagecreatefrom{$mime_type} doesn't exist");
		return false;
	}

	$new_img = imagecreatetruecolor($n_width, $n_height);
	// imagecolorallocatealpha($new_img, 0, 0, 0, 0);

	// get and reallocate transparency-color
	$transindex = imagecolortransparent($img);
	if ($transindex >= 0) {
		$transcol = imagecolorsforindex($img, $transindex);
		$transindex = imagecolorallocatealpha($new_img, $transcol['red'], $transcol['green'], $transcol['blue'], 127);
		imagefill($new_img, 0, 0, $transindex);
	} else if ($transindex == -1) {
		imagecolortransparent($new_img, imagecolorallocate($new_img, 0, 0, 0));
		imagealphablending($new_img, false);
		imagesavealpha($new_img, true);
	}

	imagecopyresampled($new_img, $img, 0, 0, 0, 0, $n_width, $n_height, $width, $height);


	// stuff the existing file off in to a backup directory
	if (function_exists("image" . $mime_type) && $backup === true) {
		// we need to get the end path
		$my_local_dest = preg_replace("/^(.*)\/(.*?)$/", "$1/source/$2", $dest);
		$my_local_dest = preg_replace("/^(.*)_.*/", "$1.$mime_type", $my_local_dest);

		// make sure that the directory exists
        if (! is_dir(dirname($my_local_dest))) {
            $res = mkdir(dirname($my_local_dest));
        }
		$res = call_user_func("image" . $mime_type, $img, $my_local_dest);
		if (!$res) {
			error_log("image{$mime_type} failed!!!!");
		}
	}

	imagedestroy($img);


	// write the image out to the new file, using imagepng/imagegif/etc
	$my_dest_type = preg_replace("/^.*\.(.*?)$/", "$1", $dest);

	if ($my_dest_type === 'jpg') {
		$my_dest_type = 'jpeg'; // handle the fact that imagejpeg exists, but not imagejpg
	}

	if (function_exists("image" . $my_dest_type)) {
		$res = call_user_func("image" . $my_dest_type, $new_img, $dest);
		imagedestroy($new_img);
		return $res;
	} else {
		error_log("image{$my_dest_type} doesn't exist");
		imagedestroy($new_img);
		return false;
	}

}



/**
 * Creates function imagecreatefrombmp, since PHP doesn't have one
 *
 * @return resource An image identifier, similar to imagecreatefrompng
 * @param  string $filename Path to the BMP image
 * @see    imagecreatefrompng
 * @author Glen Solsberry <glens@networldalliance.com>
 */
if (!function_exists("imagecreatefrombmp")) {
	function imagecreatefrombmp( $filename ) {
		$file = fopen($filename, "rb");
		$read = fread($file, 10);
		while (! feof($file) && $read != "") {
			$read .= fread($file, 1024);
		}
		$temp = unpack("H*", $read);
		$hex = $temp[1];
		$header = substr($hex, 0, 104);
		$body = str_split(substr($hex, 108), 6);
		if (substr($header, 0, 4) == "424d") {
			$header = substr($header, 4);
			// Remove some stuff?
			$header = substr($header, 32);
			// Get the width
			$width = hexdec(substr($header, 0, 2));
			// Remove some stuff?
			$header = substr($header, 8);
			// Get the height
			$height = hexdec(substr($header, 0, 2));
			unset($header);
		}
		$x = 0;
		$y = 1;
		$image = imagecreatetruecolor($width, $height);
		foreach ($body as $rgb) {
			$r = hexdec(substr($rgb, 4, 2));
			$g = hexdec(substr($rgb, 2, 2));
			$b = hexdec(substr($rgb, 0, 2));
			$color = imagecolorallocate($image, $r, $g, $b);
			imagesetpixel($image, $x, $height-$y, $color);
			$x++;
			if ($x >= $width) {
				$x = 0;
				$y++;
			}
		}
		return $image;
	}
}

function get_random_str($length = 10, $seed = 'abcdefghijklmnopqrstuvwxyz0123456789') {
	for ($str = '', $i = 0; $i < $length; ++$i)
		$str .= $seed{rand(0, strlen($seed) - 1)};

	return $str;
}

function update_constant_contact($my_site_id, $my_user_id)  {

	global $db;

	$my_user = $db->get_row("SELECT a.email,a.first_name,a.last_name,a.job_title,a.company,b.category AS nice_bizcat,c.type AS nice_biztype,d.size AS nice_bizsize from users a JOIN business_categories b ON a.business_category=b.id JOIN business_types c ON a.business_type=c.id JOIN company_sizes d ON a.company_size=d.id WHERE a.id=" . $my_user_id);
	$my_concon = $db->get_results("SELECT * from constant_contact WHERE site_id=" . $my_site_id);

	foreach ($my_concon as $cc) {
		$login = "loginName=" . $cc->login . "&loginPassword=" .
			$cc->password;
		$info = "&ea=" . $my_user->email .
			"&First_Name=" . $my_user->first_name .
			"&Last_Name=" . $my_user->last_name .
			"&job_title=" . $my_user->job_title .
			"&company_name=" . $my_user->company .
			"&custom_field_1=" . $my_user->nice_bizcat .
			"&custom_field_2=" . $my_user->nice_biztype .
			"&custom_field_3=" . $my_user->nice_bizsize;
			// "&Work_Phone=" . $input_vars['phone'] .
			// "&Address_Line_1=" . $input_vars['address1'] .
			// "&Address_Line_2=" . $input_vars['address2'] .
			// "&City=" . $input_vars['city'] .
			// "&US_State/CA_Province=" . $nice_state .
			// "&Country" . $nice_country .
			// "&Zip/Postal_Code" . $input_vars['zip'] .

		include_once INCLUDE_DIR . "/Spider.inc";
		Spider::getGETPage("http://ccprod.roving.com/roving/wdk/API_AddSiteVisitor.jsp?" . $login . "&ic=" . $cc->list . $info);
	}
}

/**
 * Function to ellipse-ify text to a specific length
 *
 * @param string $text   The text to be ellipsified
 * @param int    $max    The maximum number of characters (to the word) that should be allowed
 * @param string $append The text to append to $text
 * @return string The shortened text
 * @author Brenley Dueck
 * @link   http://www.brenelz.com/blog/2008/12/14/creating-an-ellipsis-in-php/
 */
function ellipsis($text, $max=100, $append='&hellip;') {
	if (strlen($text) <= $max) return $text;
	$text = strip_tags($text); // remove any html.  we *only* want text
	$text = preg_replace('/&(.*?);/', '', $text); // strip out any html entities
	$out = substr($text, 0, $max);
	if (strpos($text, ' ') === false) return $out.$append;
	return preg_replace('/\W+$/', ' ', preg_replace('/\w+$/', '', $out)) . $append;
}

function get_page_sponsor($query = false) {

	if ($query && $query != "") {
		global $db;
		$my_results = $db->get_col($query);
		$my_count = count($my_results);
		if ($my_count == 0) {
			return false;
		} else if ($my_count == 1) {
			return $my_results[0];
		} else {
			return $my_results;
		}
	} else {
		global $my_page_name;
		if (defined('SPONSOR_DIR')) { // this checks the defined-ness of the constant
			$my_filename = SPONSOR_DIR . "/" . $my_page_name . ".xml";
			if (file_exists( $my_filename)) {
				include_once $my_filename;
				return $sponsor;
			} else {
				return false;
			}
		} else {
			return false;
		}
	}
}

function log_ad_view($ad_id, $page_name, $site_id) {
	global $db;

	$site_id = (int)$site_id;
	$page_name = (string)$page_name;
	if (is_null($ad_id)) return;
	if (!is_numeric($ad_id)) return;
    if (is_null($site_id)) return;
    if (!is_numeric($site_id)) return;
    if (is_null($page_name)) return;
    if (!is_string($page_name)) return;

	$page_name = preg_replace('/^ /', '', str_replace('"', '', $page_name));

	$db->query("INSERT INTO ad_views (ad_id, page_name, site_id, log_time) VALUES ({$ad_id}, '{$page_name}', {$site_id}, NOW())");
}

function archive_ad_view() {
	global $db;

	$series = array(
		"day" => array(
			"start_time"	=> mktime(0, 0, 0, date("m"), date("d")-1, date("Y")),
			"end_time"	=> mktime(23, 59, 59, date("m"), date("d")-1, date("Y")),
		),
	);

	$last_ts_lenth = 0;
	foreach ($series as $item => $parts) {
		$start_time = date("Y-m-d H:i:s", $parts['start_time']);
		$end_time = date("Y-m-d H:i:s", $parts['end_time']);

		if ($item == "day") {
			$results = $db->query("INSERT INTO ad_views_archive SELECT ad_id, page_name, site_id, DATE('{$start_time}'), COUNT(*) FROM ad_views WHERE log_time BETWEEN '{$start_time}' AND '{$end_time}' GROUP BY ad_id, page_name, site_id ON DUPLICATE KEY UPDATE counter = counter + VALUES(counter);"); 
			if ($results > 0) {
				$db->query("DELETE FROM ad_views WHERE log_time BETWEEN '{$start_time}' AND '{$end_time}'");
			}
		}
		$last_ts_length = $parts['end_time'] - $parts['start_time'];
	}

}

function log_ad_click($ad_id, $page_name, $site_id) {
	global $db;

	$site_id = (int)$site_id;
	$page_name = (string)$page_name;
    if (is_null($ad_id)) return;
    if (!is_numeric($ad_id)) return;
    if (is_null($site_id)) return;
    if (!is_numeric($site_id)) return;
    if (is_null($page_name)) return;
    if (!is_string($page_name)) return;

	$page_name = preg_replace('/^ /', '', str_replace('"', '', $page_name));
	$db->query("INSERT INTO ad_clicks (ad_id, page_name, site_id, log_time) VALUES ({$ad_id}, '{$page_name}', {$site_id}, NOW())");
}

function archive_ad_click() {
	global $db;

	$series = array(
		"day" => array(
			"start_time"	=> mktime(0, 0, 0, date("m"), date("d")-1, date("Y")),
			"end_time"	=> mktime(23, 59, 59, date("m"), date("d")-1, date("Y")),
		),
	);

	$last_ts_lenth = 0;
	foreach ($series as $item => $parts) {
		$start_time = date("Y-m-d H:i:s", $parts['start_time']);
		$end_time = date("Y-m-d H:i:s", $parts['end_time']);

		if ($item == "day") {
			$results = $db->query("INSERT INTO ad_clicks_archive SELECT ad_id, page_name, site_id, DATE('{$start_time}'), COUNT(*) FROM ad_clicks WHERE log_time BETWEEN '{$start_time}' AND '{$end_time}' GROUP BY ad_id, page_name, site_id ON DUPLICATE KEY UPDATE counter = counter + VALUES(counter);"); 
			if ($results) {
				$db->query("DELETE FROM ad_views WHERE log_time BETWEEN '{$start_time}' AND '{$end_time}'");
			}
		}
		$last_ts_length = $parts['end_time'] - $parts['start_time'];
	}

}

// $the_id is normally $input_vars['id']
// $url = "blog" or "article"
// $title is the article or blog title

function socialize($query, $the_id, $url = "", $title = "") {

    include_once GLOBAL_INCLUDE_DIR . "/inflect.class.inc";

	GLOBAL $db, $my_site_config;

    $inflect = new Inflect();

    $res = $db->get_results($query);
    $comments = count($res);

    list($my_vote_score, $my_vote_count) = get_vote_score_and_count($url, $the_id);

    $escaped_title = urlify($title);

    $image_url = GLOBAL_IMAGE_URL . "/socialize/";
    $entry_url = $my_site_config->url . "/" . $url . "/" . $escaped_title;

    $title .= " | "  . $my_site_config->name;

    $comment_text = preg_replace("/^(\d+)/", '<span class="commentcount">$1</span>', $inflect->pluralize_if($comments, "comment"));
    $comment_text = $comments > 0 ? $comment_text : 'Be the <span class="commentcount">first!</span>';

	$title = urlencode($title);


$sign="";
if ($my_vote_score >0)
    $sign="+";
if ($my_vote_score<0)
    $sign="-";


    $output = <<<SOCIALIZE
<div class="socialize">
	<div class="comments" style="background-image: url({$image_url}opinion.png); background-repeat: no-repeat;">
    	<small>{$comment_text}</small>
	</div>
    <div class="email">
        <a id ='emailbox' href="/send_to_friend.php?url={$entry_url}" class="emailer iframe">&nbsp;</a>
    </div>
    <div class="print">
        <a href="{$my_site_config->url}/{$url}_print/{$escaped_title}" target="_blank">&nbsp;</a>
    </div>
    <div class="tweet">
        <a href="/tweet.php?url={$entry_url}&amp;title={$title}" target="_blank">&nbsp;</a>
    </div>
    <div class="share">
        <div class="shareLinks">
			<ul>
				<li>
					<a href="http://www.facebook.com/share.php?u={$entry_url}&amp;t={$title}?TB_iframe=true&amp;height=500&amp;width=900">
						<img width="16" height="16" style="background: transparent url({$image_url}services-sprite.png) no-repeat scroll -343px -1px; -moz-background-clip: border; -moz-background-origin: padding; -moz-background-inline-policy: continuous;" src="{$image_url}services-sprite.gif"/>
					</a>
				</li>
				<li>
					<a href="http://digg.com/submit?phase=2&amp;url={$entry_url}&amp;title={$title}&amp;bodytext=EXCERPT?TB_iframe=true&amp;height=500&amp;width=900">
						<img width="16" height="16" style="background: transparent url({$image_url}services-sprite.png) no-repeat scroll -235px -1px; -moz-background-clip: border; -moz-background-origin: padding; -moz-background-inline-policy: continuous;" src="{$image_url}services-sprite.gif"/>
					</a>
				</li>
				<li>
					<a href="http://delicious.com/post?url={$entry_url}&amp;title={$title}&amp;notes=EXCERPT?TB_iframe=true&amp;height=500&amp;width=900">
						<img width="16" height="16" style="background: transparent url({$image_url}services-sprite.png) no-repeat scroll -199px -1px; -moz-background-clip: border; -moz-background-origin: padding; -moz-background-inline-policy: continuous;" src="{$image_url}services-sprite.gif"/>
					</a>
				</li>
				<li>
					<a href="http://www.google.com/bookmarks/mark?op=edit&amp;bkmk={$entry_url}&amp;title={$title}&amp;annotation=EXCERPT?TB_iframe=true&amp;height=500&amp;width=900">
						<img width="16" height="16" style="background: transparent url({$image_url}services-sprite.png) no-repeat scroll -90px -19px; -moz-background-clip: border; -moz-background-origin: padding; -moz-background-inline-policy: continuous;" src="{$image_url}services-sprite.gif"/>
					</a>
				</li>
				<li>
					<a href="http://www.diigo.com/post?url={$entry_url}&amp;title={$title}?TB_iframe=true&amp;height=500&amp;width=900">
						<img width="16" height="16" style="background: transparent url({$image_url}services-sprite.png) no-repeat scroll -253px -1px; -moz-background-clip: border; -moz-background-origin: padding; -moz-background-inline-policy: continuous;" src="{$image_url}services-sprite.gif"/>
					</a>
				</li>
				<li>
					<a href="http://www.friendfeed.com/share?title={$title}&amp;link={$entry_url}?TB_iframe=true&amp;height=500&amp;width=900">
						<img width="16" height="16" style="background: transparent url({$image_url}services-sprite.png) no-repeat scroll -36px -19px; -moz-background-clip: border; -moz-background-origin: padding; -moz-background-inline-policy: continuous;" src="{$image_url}services-sprite.gif"/>
					</a>
				</li>
				<li>
					<a href="http://www.netvibes.com/share?title={$title}&amp;url={$entry_url}?TB_iframe=true&amp;height=500&amp;width=900">
						<img width="16" height="16" style="background: transparent url({$image_url}services-sprite.png) no-repeat scroll -198px -37px; -moz-background-clip: border; -moz-background-origin: padding; -moz-background-inline-policy: continuous;" src="{$image_url}services-sprite.gif"/>
					</a>
				</li>
				<li>
					<a href="http://ping.fm/ref/?link={$entry_url}&amp;title={$title}&amp;body=EXCERPT?TB_iframe=true&amp;height=500&amp;width=900">
						<img width="16" height="16" style="background: transparent url({$image_url}services-sprite.png) no-repeat scroll -270px -37px; -moz-background-clip: border; -moz-background-origin: padding; -moz-background-inline-policy: continuous;" src="{$image_url}services-sprite.gif"/>
					</a>
				</li>
				<li>
					<a href="http://reddit.com/submit?url={$entry_url}&amp;title={$title}?TB_iframe=true&amp;height=500&amp;width=900">
						<img width="16" height="16" style="background: transparent url({$image_url}services-sprite.png) no-repeat scroll -55px -55px; -moz-background-clip: border; -moz-background-origin: padding; -moz-background-inline-policy: continuous;" src="{$image_url}services-sprite.gif"/>
					</a>
				</li>
				<li>
					<a href="http://technorati.com/faves?add={$entry_url}?TB_iframe=true&amp;height=500&amp;width=900">
						<img width="16" height="16" style="background: transparent url({$image_url}services-sprite.png) no-repeat scroll -271px -55px; -moz-background-clip: border; -moz-background-origin: padding; -moz-background-inline-policy: continuous;" src="{$image_url}services-sprite.gif"/>
					</a>
				</li>
			</ul>
       </div>
    </div>
    <div class="voteLinks">
        <div class="voteUp"><img src="{$image_url}thumbsUp.png" /></div>
        <div class="voteDown"><img src="{$image_url}thumbsDn.png" /></div>
        <div class="voteScore">{$sign}{$my_vote_score}</div>
        <div class="voteCount">{$my_vote_count}</div>
		<div style='clear: both;'></div>
    </div>
</div>
SOCIALIZE;

    echo $output;

}

/**
 * paginate an object
 *
 * @param mixed $paginate_this  The object to paginate
 * @param int   $page_number    The page to paginate "to"
 * @param int   $items_per_page The number of items allowed on a page
 * @return mixed The paginated object, at page $page_number
 * @author Glen Solsberry <glens@networldalliance.com>
 * @author Gary Conway <garyc@networldalliance.com>
 * @version 20100101-01
 */
function paginate($paginate_this, $page_number = 1, $items_per_page = 15) {
	include_once GLOBAL_INCLUDE_DIR . "/Zend/Paginator.php";

	//GRC handle null
	if (is_null($paginate_this)) {
		return $paginate_this;
	}
	$paginator = Zend_Paginator::factory($paginate_this);
	$paginator->setCurrentPageNumber($page_number);
	$paginator->setItemCountPerPage($items_per_page);
	$paginator->setDefaultScrollingStyle('Sliding');

	return $paginator;
}


function pagination_links($paginate_this, $url_format = "", $page_number = 1, $items_per_page = 15) {
    include_once GLOBAL_INCLUDE_DIR . "/Zend/Paginator.php";


        //GRC handle null
	if (is_null($paginate_this)) {
		echo "<div class=\"paginationControl\">";
		echo "No Data Available.";
		echo "</div>";
		return; 
	}

    $paginator = Zend_Paginator::factory($paginate_this);
    $paginator->setCurrentPageNumber($page_number);
    $paginator->setItemCountPerPage($items_per_page);
    $paginator->setDefaultScrollingStyle('Elastic');
    $paginator->setPageRange(5);

    $p = $paginator->getPages();

    if ($url_format === "") {
        $url_format = "?page=";
    }

    echo "<div class=\"paginationControl\">";
    if ($p->pageCount) {
        echo "<div class=\"paginationLinks\">";

        // First page link
        if (isset($p->previous)) {
            echo "<a class=\"firstPage\" href=\"{$url_format}{$p->first}\">&laquo;</a> ";
        }

        if ($p->pageCount > 1) {
            foreach ($p->pagesInRange as $p_num) {
                if ($p_num == $p->current) {
                    echo "<span class=\"selectorPage\">{$p_num}</span> ";
                } else {
                    echo "<a class=\"selectorPage\" href=\"{$url_format}{$p_num}\">{$p_num}</a> ";
                }
            }
        }

        // Last page link
        if (isset($p->next)) {
            echo " <a class=\"lastPage\" href=\"{$url_format}{$p->last}\">&raquo;</a>";
        }
        echo "</div>";

        echo "<span class=\"itemList\">Showing ({$p->firstItemNumber} - ". number_format($p->lastItemNumber) .") of ". number_format($p->totalItemCount) ."</span>";

    } else {
        echo "No records...";
    }
    echo "</div>";
}

function url_to_parts($url) {
    if ($url === "") return false;

    $my_parts = parse_url($url);
    $path = $my_parts['path'];

    list($a, $section, $title) = explode('/', $path);
    $title = preg_replace('/-/', ' ', $title);
    $title = preg_replace('/[^\dA-Za-z ]/', '%', $title);

    return array($section, $title);
}

/**
 * Correctly adjust a string for use with URLs
 *
 * @link http://monitor.networldalliance.com/phpunit/globaltest/testdox.html Test output for this function
 * @param string $field The text to check
 *
 * @return string The correctly adjusted string
 *
 * @author Glen Solsberry <glens@networldalliance.com>
 */
function urlify($field) {
	$field = strip_tags($field);
	$field = preg_replace('/&(.*?);/', '-', $field);
	$field = preg_replace('/\W/', '-', $field);
	$field = preg_replace('/-+/', '-', $field);
	$field = preg_replace('/-$/', '', $field);
	
	//BK: added remove of '
	$field = str_replace("'", '', $field);
	
	$field = urlencode($field);
	return $field;
}

function deurlify($field) {
	return str_replace('-', ' ', urldecode($field));
}

/**
 * makes the text string safe for display within html
 *
 * @param  string $text The text to clean
 * @return string The html-safe text
 * @author Glen Solsberry <glens@networldalliance.com>
 * @version 20100101-01
 */
function html_safe($text) {
	return preg_replace('/&(?!(amp|nbsp|hellip|reg);)/', '&amp;', $text);
}

/**
 * get the vote score and vote count for a specific article/blog
 *
 * @uses $db
 * @param string $section Article/Blog
 * @param int $id The id of the aforementioned $section
 * @return array \$score and $count
 * @author Glen Solsberry <glens@networldalliance.com>
 */
function get_vote_score_and_count($section, $id) {
    global $db;

    $score = $db->get_var("SELECT IFNULL(SUM(score), 0), COUNT(*) FROM votes WHERE {$section}_id = {$id}");
    $count = $db->get_var(null, 1);

    return array($score, $count);
}

/**
 * Replace values with respective values from input_vars
 *
 * The values in $search_string are surrounded by [ ].
 * No checking is done to make sure that the values exist in input_vars
 *
 * @uses $input_vars
 * @param string $search_string The string to check
 * @return string The updated string
 * @author Glen Solsberry <glens@networldalliance.com>
 */
function replace_with_values($search_string) {
    global $input_vars, $my_site_config;

    $search_string = preg_replace('/\[(.+?)\]/e', 'get_values(\'$1\')', $search_string);

    return $search_string;
}

function get_values($key) {
	global $input_vars, $my_site_config;

	if (array_key_exists($key, $input_vars)) { error_log(sprintf("iv: %s: %s", $key, $input_vars[$key])); return $input_vars[$key]; }
	else if (isset($my_site_config->$key)) { error_log(sprintf("msc: %s: %s", $key, $my_site_config->$key)); return $my_site_config->$key; }
	else { error_log(sprintf("undefined `%s`", $key)); return ''; }
}

/**
 * display a relative time, instead of an absolute date
 *
 * @uses inflect.class.inc
 * @param date $date The datetime to convert
 * @return string The relative time
 * @author Glen Solsberry <glens@networldalliance.com>
 */
function getRelativeTime($date) {

    include_once GLOBAL_INCLUDE_DIR . "/inflect.class.inc";

    $inflect = new Inflect();

    $diff = time() - strtotime($date);
    if ($diff<60)
        return "about " . $inflect->pluralize_if($diff, "second") . " ago";
    $diff = round($diff/60);
    if ($diff<60)
        return "about " . $inflect->pluralize_if($diff, "minute") . " ago";
    $diff = round($diff/60);
    if ($diff<24)
        return "about " . $inflect->pluralize_if($diff, "hour") . " ago";
    $diff = round($diff/24);
    if ($diff<7)
        return "about " . $inflect->pluralize_if($diff, "day") . " ago";
    $diff = round($diff/7);
    if ($diff<4)
        return "about " . $inflect->pluralize_if($diff, "week") . " ago";
    $diff = round($diff/4.34); // 30.5 days per month, 7 days per week
    return "about " . $inflect->pluralize_if($diff, "month") . " ago";
}

/**
 * wrapper around getimagesize
 *
 * @param string $path_to_image location of file
 * @return array The results of getimagesize
 * @author Glen Solsberry <glens@networldalliance.com>
 * @version 20100101-01
 */
function image_info($path_to_image) {
	list($width, $height, $type, $attr) = getimagesize($path_to_image);

	return array($width, $height, $type, $attr);
}

/**
 * "Resize" an image, keeping the aspect ratio
 *
 * @author Glen Solsberry <glens@networldalliance.com>
 * @var    $height int The current height
 * @var    $width int The current width
 * @var    $n_height int The new height to use; 0 to calculate on the fly
 * @var    $n_width int The new width to use; 0 to calculate on the fly
 * @return array $n_height, $n_width of "resized" image
 */
function resize_with_aspect($height, $width, $n_height, $n_width) {
	if ($n_height == 0 && $n_width == 0) { return array($height, $width); }
	if ($n_height > $height || $n_width > $width) { return array($height, $width); }

	if ($n_height == 0) { // passing 0 implies a calculation directly
		$n_height = (int)floor(($n_width / $width) * $height);
	} elseif ($n_width == 0) {
		$n_width = (int)floor(($n_height / $height) * $width);
	} else {
		if ($width > $height) {
			$n_height = (int)floor(($n_width / $width) * $height);
		} else {
			$n_width = (int)floor(($n_height / $height) * $width);
		}
	}

	return array($n_height, $n_width);
}

/**
 * Subscribes $user_id to $thing $id
 *
 * @uses $db
 * @author Glen Solsberry <glens@networldalliance.com>
 */
function subscribe_to_this($thing, $id, $user_id) {
	global $db;

	$query = <<< SQL
	INSERT INTO post_subscriptions
	(type, id, user_id, date_created, hash)
	VALUES
	('{$thing}', {$input_vars['id']}, {$my_user['id']}, NOW(), MD5(CONCAT('{$thing}', {$input_vars['id']}, {$my_user['id']})))
	ON DUPLICATE KEY UPDATE user_id = VALUES(user_id)
SQL;

	$db->query($query);
}

/**
 * displays the on/off links in the admin
 *
 * @uses $my_hidden_items
 * @uses $my_div_id
 * @param string $field The field to search through
 * @return void
 * @author Glen Solsberry <glens@networldalliance.com>
 */
function displayOnOff($field) {
	global $my_hidden_items, $my_div_id;

	if (in_array($field, $my_hidden_items)) { // if it exists, we should display "Turn On"
		$val = "On";
	} else {
		$val = "Off";
	}

	echo "<a href=\"/utils/turn.php?div_id={$my_div_id}&field={$field}&value={$val}\" class=\"turn_{$val}\" style=\"position: absolute; left: 520px;\">Turn {$val}</a>";

}

/**
 * remove strings of funky characters
 *
 * @author  Glen Solsberry <glens@networldalliance.com>
 * @version 20100101-01
 */
function funky_html_to_sane_html($string) {

	$search = array(
		'&Atilde;&cent;&acirc;&sbquo;&not;&quot;',
		'&Atilde;&cent;&acirc;&sbquo;&not;&acirc;&bdquo;&cent;',
		'&Atilde;&cent;&acirc;&sbquo;&not;&Euml;&oelig;',
		'&Atilde;&fnof;&Acirc;&copy;',
		'&Atilde;&cent;&acirc;&euro;Å¾&Acirc;&cetn;',
		'&Atilde;&cent;&acirc;&sbquo;&not;&Acirc;&uml;',
		'&Atilde;&sbquo;&Acirc;',
		'&Acirc;&reg;',
		'&acirc;&bdquo;&cent;',
		'&Acirc;&frac12;',
		'&#39;',
		'&#153;',
		'<br>',
		'&nbsp;&frac12;',
	);

	$replace = array(
		'&mdash;',
		'\'',
		'\'',
		'&eacute;',
		'&trade;',
		'',
		'',
		'&reg;',
		'&trade;',
		'&frac12;',
		'\'',
		'&trade;',
		' ',
		'&frac12;',
	);

	$string = str_replace($search, $replace, $string);
	$string = preg_replace('/\s+/', ' ', $string);
	$string = trim($string);

	return $string;
}

/**
 * generates a list of widgets based on an xml name
 *
 * @author  Glen Solsberry <glens@networldalliance.com>
 * @version 20100217-01
 */
function getWidgetsFromPageXML($filename) {
	
	global $widgetList;

	if (file_exists($filename)) {
		// read the file
		$object = simplexml_load_file($filename);

		if (isset($object->layout->content)) {
			foreach ($object->layout->content as $content) {
				if ((string)($content->type) === 'widget') {
					$widgetName = (string)($content->name);

					array_push($widgetList, $widgetName);

					getWidgetsFromPageXML($widgetName);

				}
			}
		}
	} else {
		// this is a widget file.  load it up
		$widget = loadWidgetXML($filename);

		if (isset($widget->content)) {
			foreach ($widget->content as $w_content) {
				if ((string)($w_content->type) === 'widget') {
					$widgetName = (string)($w_content->name);

					array_push($widgetList, $widgetName);

					getWidgetsFromPageXML($widgetName);
				}
			}
		}
	}
}

/**
 * correctly load a widget xml
 *
 * @author  Glen Solsberry <glens@networldalliance.com>
 * @version 20100217-01
 */
function loadWidgetXML($widgetName) {
	
	$widget = Object;

	if (file_exists(WIDGET_XML_DIR . "/{$widgetName}.xml")) {
		if (! $widget = parse_xml_file(WIDGET_XML_DIR . "/{$widgetName}.xml")) {
			// do nothing.  attempt to load the global widget
		}
	}

	if (! isset($widget) || ! is_object($widget)) {
		if (file_exists(GLOBAL_WIDGET_XML_DIR . "/{$widgetName}.xml")) {
			if (! $widget = parse_xml_file(GLOBAL_WIDGET_XML_DIR . "/{$widgetName}.xml")) {
				// throw an error
			}
		}
	}

	return $widget;
}

/**
 * gets the sponsor for a widget
 *
 * @uses $db
 * @uses $my_site_config
 * @param string $includeName The file to check
 * @return array The link values
 * @author Glen Solsberry <glens@networldalliance.com>
 * @version 20100201-01
 */
function getWidgetSponsor($includeName) {
	global $db, $my_site_config;

	$widgetName = basename($includeName, ".inc");


	$sql = <<< SQL
	SELECT url,	target,	image 
	FROM division_urls du
	INNER JOIN widget_sponsors AS ws ON du.id = ws.url_id AND ws.site_id={$my_site_config->site_id}
	INNER JOIN _division_images AS di ON ws.image_id = di.id
	INNER JOIN division_sites AS ds ON di.division_id = ds.division_id
	WHERE ws.widget_name = '{$widgetName}' 
	AND ds.site_id = {$my_site_config->site_id}
SQL;
	$linkUrl = $db->get_var($sql, 0);
	$linkTarget = $db->get_var(null, 1);
	$imageUrl = CDN_BASE_URL . '/new/images/companies/' . $db->get_var(null, 2);

//echo $sql. $widgetName;

	if (strlen($linkUrl) > 0) {
		$_SESSION['sponsored_widget'] = (string)$widgetName;
	}

	return array(
		'linkUrl' => $linkUrl,
		'linkTarget' => $linkTarget,
		'imageUrl' => $imageUrl
	);

}

/**
 * wrapper function for file_get_contents
 *
 * @author  Glen Solsberry <glens@networldalliance.com>
 * @version 20100228-01
 * @param   string $url The url to fetch
 * @param   int $max_age The age in seconds that we'll keep the file in cache
 * @return  string The contents of the fetched file (cached version if available)
 */
function fetch_and_save($url, $max_age = 300) {
	$new_url = urlify($url);

	$url_file = '/tmp/' . $_SERVER['SERVER_NAME'] . '/' . $new_url;

	if (! file_exists($url_file) || (time() - filemtime($url_file)) > $max_age || filesize($url_file) == 0) {
		$opts = array('http' =>
				array(
					'method'  => 'GET',
					'timeout' => 20,
					)
				);

		$r = stream_context_create($opts);
		$data = file_get_contents($url, false, $r);
		file_put_contents($url_file, $data, LOCK_EX);
	} else {
		$data = file_get_contents($url_file);
	}

	return $data;
}

function showFragment($fragmentName, $object, $link = 'article') {
	global $pluralize;

	$fragId = getFragmentName($fragmentName);

	$inflect = new Inflect();

	if (! isset($pluralize) || $pluralize === true) {
		$pl_frag = $inflect->pluralize($fragmentName);
	} else {
		$pl_frag = $fragmentName;
	}

	echo <<< HTML
		<div id="{$fragId}">
		<ul>
HTML;
	if (! $object) {
		echo "<li>No data available.</li>";
	} else 
	{
		$orgLink = $link;
		foreach ($object as $article) 
		{
			$url = urlify($article->title);
			$safe_title = htmlentities($article->title);
			$title = $article->title;

			if (isset($article->id)) {
				$link = $orgLink . '/' . $article->id;
			}

			echo <<< HTML
				<li style="list-style: disc;"><a href="/{$link}/{$url}" title="{$safe_title}">{$title}</a></li>
HTML;
		}
	}
	echo <<< HTML
		</ul>
		</div>
HTML;
}

function getFragmentName($section) {
	global $current_widget_name;

	$inflect = new Inflect();

	$section = str_replace(' ', '_', $section);
	$pl_frag = $inflect->pluralize($section);

	return $current_widget_name . '_' . $pl_frag;
}

/**
 * flush the query cache
 *
 * modified to use find instead of rm; argument list was becoming too long
 *
 * @param string|array $site The site(s) to clear the cache for
 * @param string|array $section The section(s) to clear the cache for
 * @return void
 * @author Glen Solsberry <glens@networldalliance.com>
 * @version 20100621-01
 */
function flushQueryCache($site = '*', $section = '*') {
	$status = 0;
	if (is_array($site)) {
		if (is_array($section)) {
			foreach($site as $s) {
				foreach($section as $s2) {
					$cmd = "find /tmp/{$s}/{$s2} -type f -not -name \*\.\* -not -name sess_\* -name [0123456789abcdef]\* | xargs rm";
					$message = system($cmd, $status);
				}
			}
		} else {
			foreach($site as $s) {
				$cmd = "find /tmp/{$s}/{$section} -type f -not -name \*\.\* -not -name sess_\* -name [0123456789abcdef]\* | xargs rm";
				$message = system($cmd, $status);
			}
		}
	} else {
		if (is_array($section)) {
			foreach($section as $s2) {
				$cmd = "find /tmp/{$site}/{$s2} -type f -not -name \*\.\* -not -name sess_\* -name [0123456789abcdef]\* | xargs rm";
				$message = system($cmd, $status);
			}
		} else {
			$cmd = "find /tmp/{$site}/{$section} -type f -not -name \*\.\* -not -name sess_\* -name [0123456789abcdef]\* | xargs rm";
			$message = system($cmd, $status);
		}
	}
}

/*

*/


function ShowGreg($txt)
{
	GLOBAL $my_user;

	if ($my_user['id'] == '5639')
	    echo "<script type=text/javascript>alert('{$txt}');</script>";

}


function ShowViper($arg,$status=1)
{
	GLOBAL $my_user;

switch ($my_user['id'])
{
	case 22790:
	case 11:
				break;
	default:	return;
}

	switch($status)
	{
    case 1:
        // do an echo
        echo $arg;
        return;

	case 2:
		// do a var_dump()
		var_dump($arg);
		return;
	}

}

/**
 * Generate a list page.  Allows sorting via column headers
 *
 * @uses $db
 * @uses $input_vars
 * @param string $sql 				This is the sql statement for the list page (no limits, etc)
 *									NOTE: GROUP BY clause is required by the alpha sort
 *										  ORDER BY clause required by column sort
 *										  must include an `id` field to be used for delete fn
 * @param int $limit 				The number of items to display per page
 * @param string $char_list_field 	Which field in the sql statement to use for the alpha link list (the A-Z list atop)
 * @param array $columns 			The column mapping for sql fields to HTML column headers

		$columns is an array which may be formatted in one of two ways

		There are three special keys that cause specific behavior.
		(active and abbr apply to both formats)
		'active' - causes the column to become a Status column, displaying the 'active' DBF field
		'abbr'
		(view applies to only the enhanced format)
		'view' - causes the column to become a View field using the 'link' provided


		1. Original Format
            NOTE: keys are the DBF fieldnames, values are column headers

            $columns = array(
                'active' => 'Status',
                'title' => 'Blog Title',
                'name' => 'Updated By'
            );


		2. Enhanced format
			The first key/value pair is a DBF field, same as above
			The second set represents the link parms for the field above
			NOTE: 'link' MUST be the key for the second element in this array
				you may use DBF field names enclosed in [] in the links, they will be replaced with $record->[value]
				see '/blogs/blog_list.inc' for usage
			'class' allows you to specify the CSS class(es) for the link

			$columns = array(
		        array('active' => 'Status', 'link' => '/blogs/blog_list?author_id=[user_id]'),
		        array('title' => 'Blog Title','link' => 'http://blogs'),
		        array('name' => 'Updated By','link' => '', 'class' => 'class1 class2')
			);

 * @param string $delete_tag_field 	Which field will contain the text that appears in the delete alert box
									NOTE: query must include the `id` field as this is passed in the URL to delete module
 * @param string $table 			The table to update for setting the active flag
 * @param string $edit_url 			The url to the edit page
 * @param string $delete_url 		The url to the delete page (value of 'none' causes this field to be omitted)
 * @param string $edit_view			Default ='Edit", header txt for the clickable column, normally 'Edit' or 'View'
									a value of 'none' will cause this column to be omitted
 * @param string $edit_class		Default ='edit', this is the CSS class assigned to the Edit link
 									the default will cause the new window to come up in a fancybox
 * @param string $additional_edit_field 		DBF fieldname of an additional column linked to $edit_url
 * @return void
 * @author Glen Solsberry <glens@networldalliance.com>
 * @author Gary Conway <garyc@networldalliance.com>
	PREREQUISITES:	<TABLE> is opened
					<THEAD> is opened
    example:
                echo "<table id='list' align='center' width='100%'>";
                echo "<thead>";

	POST-OPS:		</TABLE> close the table

 * @version 20100516-01
 */
function displayDataList($sql = '', $limit = 15, $char_list_field = '', $columns = array(), $delete_tag_field = '', $table = '', $edit_url = '', $delete_url = '', $edit_view='Edit', $edit_class='edit', $additional_edit_field = '') {
	global $db, $input_vars;

	if (strtolower($edit_view) === 'none') 	{ $edit_view = 'none'; }
    if (strtolower($delete_url) === 'none')  { $delete_url = 'none'; }

	// calculate the number of columns
	$colCount = count($columns);

	// see if we have a "delete" column, if so, add that in
	if ($delete_url !== 'none') 
		$colCount++;

	// see if we have an 'active' column, if so, add that in
	$ele = array_slice($columns,0,1,true);
	if (key($ele) == 'active')
		$colCount++;
	

	if (! isset($input_vars['page'])) 
		$input_vars['page'] = 1;
	

	if (isset($input_vars['dir']) && strlen($input_vars['dir']) !== 0) 
		$dir = $input_vars['dir'];
	else 
		$dir = 'ASC';
	

	$opposite_dir = ($dir === 'ASC' ? 'DESC' : 'ASC');

	$character_result_set = $db->get_results($sql);

	$has_having = false;
	if (isset($input_vars['char']) && $input_vars['char'] !== '' && $char_list_field !== '') {
		$sql = preg_replace('/(GROUP BY.*\n)/', '$1'."\t".'HAVING LEFT('.$char_list_field.', 1) = \''.$input_vars['char']."'\n", $sql);
		$has_having = true;
	}

	if (isset($input_vars['site_id']) && (int)$input_vars['site_id'] !== 100) {
		$site_portion = "site_id = " . (int)$input_vars['site_id'] . "\n";
		if ($has_having)
			$sql = preg_replace('/(HAVING.*\n)/', '$1'."\t".'AND ' . $site_portion, $sql);
		else
			$sql = preg_replace('/(GROUP BY.*\n)/', '$1'."\t".'HAVING ' . $site_portion, $sql);
	}

	// allow sorting of data
	if (isset($input_vars['sort']) && strlen($input_vars['sort']) !== 0) {
		$sql = preg_replace('/ORDER BY /', 'ORDER BY ' . $input_vars['sort'] . ' ' . $dir . ',', $sql);
	}

	if ($char_list_field !== '') {
		$char_list_array = array();
		foreach($character_result_set as $result) {
			array_push($char_list_array, strtoupper(substr($result->$char_list_field, 0, 1)));
		}
		$char_list_array = array_unique($char_list_array);
		sort($char_list_array);
	}

	if (is_array($char_list_array)) {
		echo "<tr class='nohover'><td colspan=\"" . ($colCount) . "\" style=\"text-align: center;\">";
		echo "<a href='{$_SERVER['PHP_SELF']}'>All</a> ";
		foreach ($char_list_array as $char) {
			$links .= "<a href=\"?char=" . urlencode($char) . "\">" . htmlentities($char) . "</a>";
			$links .= " ";
		}
		echo $links;
		echo "</td></tr>";
	}

	$result_set = $db->get_results($sql);
	$current_page = paginate($result_set, $input_vars['page'], $limit);

	echo "<tr class='nohover'><td colspan=\"" . ($colCount) . "\" style=\"text-align: center;\">";
	pagination_links($result_set, "?sort={$input_vars['sort']}&dir={$input_vars['dir']}&char={$input_vars['char']}&site_id={$input_vars['site_id']}&page=", $input_vars['page'], $limit);
	echo "</td></tr>";

	echo "<tr class='nohover'>";
	if (is_array($columns)) {
		foreach($columns as $key => $value) {
			ShowHeaders($key,$value,$opposite_dir);
//			echo "<th><a href=\"?sort={$key}&dir={$opposite_dir}\">{$value}</a></th>";
		}
	}
	if ($edit_view != 'none')
		echo "<th width='5%'>{$edit_view}</th>";

	if ($delete_url !== 'none')
		echo "<th width='5%'>Delete</th>";

	echo "</tr>";
	echo "</thead>";

	echo "<tbody>";
foreach($current_page as $record) 
{
	echo "<tr>";
	if (is_array($columns)) 
	{
		foreach($columns as $key => $value) 
		{
			if (is_array($value))
			{
			// we have a multi-dimensional array that contains field values and links
				$ele = array_slice($value,0,1,true);
				$newkey= key($ele);
				$newlink="";
				$classString="";

				foreach ($value as $itm => $itmvalue)
				{
					switch ($itm)
					{
					case "link":
                        $newLink = $itmvalue;
						break;
					case "class":
						$classString = " class='{$itmvalue}' ";
						break;
					}
				}


					$matchcount=preg_match('/^.*\[(.*?)\]/',$newLink,$matches);
					$newLink = preg_replace('/\[(.+?)\]/e', '$record->$1', $newLink);

                switch($newkey) {
					case 'view':
						echo "<td align='center'>";
						//NOTE: this section was written especially for tag_mapping_list.inc
						if ($newLink)
						{
							if ($matchcount>0)
							{
								if (! is_null($record->$matches[1]))
									echo "<a $classString href='{$newLink}'>View</a>";
							}
							else
	                            echo "<a $classString href='{$newLink}'>View</a>";
						}
						echo "</td>";
						break;
                    case 'active':
                        $active = ($record->$newkey ? "Active" : "Inactive");
                        echo "<td class=\"active_{$record->$newkey}\"><a href=\"/utils/activity_edit.php?id={$record->id}&table={$table}&current={$record->$newkey}\" class=\"activity\">{$active}</a></td>";
                        break;
//                    case 'abbr':
//                        echo "<td><a $classString href=\"?page={$input_vars['page']}&site_id={$record->site_id}\">{$record->$newkey}</a></td>";
//                        break;
                    default:
                        echo "<td";
                        if ($newkey === $delete_tag_field) {  echo " class=\"delete_tag\"";   }
                        echo ">";
                        if ($newkey === $additional_edit_field) {  echo "<a href=\"{$edit_url}?directive=edit&id={$record->id}\" class=\"{$edit_class}\">";   }

						if ($newLink)
							echo "<a $classString href='{$newLink}'>{$record->$newkey}</a>";
						else
	                        echo $record->$newkey;

                        if ($newkey === $additional_edit_field) {   echo "</a>";       }
                        echo "</td>";
                        break;
                }
 

			}
			else
			{
				// we have a single dimensional array that contains a field value
				switch($key) {
					case 'active':
						$active = ($record->$key ? "Active" : "Inactive");
						echo "<td  class=\"active_{$record->$key}\"><a href=\"/utils/activity_edit.php?id={$record->id}&table={$table}&current={$record->$key}\" class=\"activity\">{$active}</a></td>";
						break;
					case 'abbr':
						echo "<td><a href=\"?page={$input_vars['page']}&site_id={$record->site_id}\">{$record->$key}</a></td>";
						break;
					default:
						echo "<td";
						if ($key === $delete_tag_field) {
							echo " class=\"delete_tag\"";
						}
						echo ">";
						if ($key === $additional_edit_field) {
							echo "<a href=\"{$edit_url}?directive=edit&id={$record->id}\" class=\"{$edit_class}\">";
						}
						echo $record->$key;
						if ($key === $additional_edit_field) {
							echo "</a>";
						}
						echo "</td>";
						break;
				}
			}
		}
	}
		if ($edit_view !== 'none')
			echo "<td style=\"width: 50px; text-align: center;\"><a href=\"{$edit_url}?directive=edit&id={$record->id}\" class='{$edit_class}'>{$edit_view}</a></td>";

		if ($delete_url !== 'none')
			echo "<td style=\"width: 50px; text-align: center;\"><a href=\"{$delete_url}?directive=delete&id={$record->id}\" class=\"delete\">Delete</a></td>";
		echo "</tr>";
}

	echo "</tbody>";

	echo "<tfoot>";
	echo "<tr class='nohover'>";
	if (is_array($columns)) {
		foreach($columns as $key => $value) {
            ShowHeaders($key,$value,$opposite_dir);
//			echo "<th><a href=\"?sort={$key}&dir={$opposite_dir}\">{$value}</a></th>";
		}
	}
	if ($edit_view !== 'none')
		echo "<th>{$edit_view}</th>";

	if ($delete_url !== 'none')
		echo "<th>Delete</th>";

	echo "</tr>";
	echo "<tr class='nohover'><td colspan=\"" . ($colCount) . "\" style=\"text-align: center;\">";
	pagination_links($result_set, "?sort={$input_vars['sort']}&dir={$input_vars['dir']}&char={$input_vars['char']}&site_id={$input_vars['site_id']}&page=", $input_vars['page'], $limit);
	echo "</td></tr>";

	if (isset($links)) {
		echo "<tr class='nohover'><td colspan=\"" . ($colCount) . "\" style=\"text-align: center;\">";
		echo "<a href='{$_SERVER['PHP_SELF']}'>All</a> ";
		echo $links;
		echo "</td></tr>";
	}

	echo "</tfoot>";
}




function ShowHeaders($key,$value,$opposite_dir)
{

    if (is_array($value))
    {
		$ele = array_slice($value,0,1,true);
		echo "<th><a href=\"?sort=". key($ele) ."&dir={$opposite_dir}\">{$value[key($ele)]}</a></th>";
    }
    else
    {
        // we have a singular array
		echo "<th><a href=\"?sort={$key}&dir={$opposite_dir}\">{$value}</a></th>";
    }

}




function show_session_vars() {
    foreach ( $_SESSION as $key => $val ) {
        echo "Session var '$key': \t'$val'<br/>";
    }
}



/*
	Log a product/service view
	Called from: products_services.inc
				showcase_mc.inc

*/

function LogProductView($division_id)
{
    GLOBAL $db,$my_site_config;

    $db->query("INSERT INTO products_services_views
        (`division_id`,`log_date`,`view_count`,`site_id`)
        VALUES('{$division_id}','".date('Y-m-d') ."','1','{$my_site_config->site_id}')
        ON DUPLICATE KEY UPDATE
        `view_count`=`view_count`+1
    ");
}


// set active=0 for all ads whose end_date is 
// prior to midnight tonight

function ExpireAds()
{
	GLOBAL $db;

	// calculate midnight tonight in SQL format
//	$midnight= strtotime("+1 day");
	$midnight = strtotime("midnight");
	$midnight= date("Y-m-d 00:00:00",$midnight);

	$sql="UPDATE _ad_map SET `active`='0'
		WHERE `end_date`<='{$midnight}'";

	$db->query($sql);
}

/**
 * checks for existance of $key in $array,
 * and makes sure that $array[$key] == $value
 *
 * @param array $array The array to search
 * @param string $key The key to search $array for
 * @param string $value The value to test against $array[$key]
 * @return bool $array[$key] exists and matches $value
 *
 * @author Glen Solsberry <glens@networldalliance.com>
 * @version 20100512-01
 */
function compareArrayItem($array, $key, $value) {
	return isset($array[$key]) && $array[$key] == $value;
}


function explainLast($db = "") {
	if ($db === "") {
		global $db;
	}

	$last_query = $db->last_query;
	$last_query = "EXPLAIN " . $last_query;

	$db->get_results($last_query);
	$db->debug();
}

function escape($var) {
	return htmlspecialchars($var);
}

/**
 * Handle errors (specifically related to ezSQL
 *
 * @author Glen Solsberry <glens@networldalliance.com>
 * @version 20100602-01
 */
function errorHandler($errno, $errstr, $errfile, $errline) {
	global $my_site_config;
	global $db;

	$subject = sprintf('Error occurred on %s', (string)$my_site_config->name);

	$message = sprintf("Error information:\nError Number: %s\nError String: %s\nError File: %s\nError Line: %s", $errno, $errstr, $errfile, $errline);

	$message .= "\n====== \$db ======\n";
	if (isset($db)) {
		$message .= print_r($db, true);
	} else {
		$message .= "\$db is not set";
	}
	$message .= "\n====== \$my_site_config ======\n";
	if (isset($my_site_config)) {
		$message .= print_r($my_site_config, true);
	} else {
		$message .= "\$my_site_config is not set";
	}
	$message .= "\n====== \$_SERVER ======\n";
	$message .= print_r($_SERVER, true);
	
	$additional_headers = '';
	$additional_parameters = '';

	// mail("glens@networldalliance.com", $subject, $message, $additional_headers, $additional_parameters);

	return true; // don't allow internal error handler to continue
}

?>
