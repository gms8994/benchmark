<?php

define('GLOBAL_INCLUDE_DIR', '/home/glens/scripts/benchmark/php/includes');

define('GLOBAL_BASE_URL', 'http://global.networldalliance.com');
define('GLOBAL_LEADGEN', 'http://leadgen.networldalliance.com');
define('GLOBAL_SERVICE_DIR', '/home/glens/scripts/benchmark/php');
define('GLOBAL_XML_DIR', GLOBAL_BASE_DIR . '/etc');
define('GLOBAL_USER_XML_DIR', GLOBAL_XML_DIR . '/page_user');
define('GLOBAL_WIDGET_XML_DIR', GLOBAL_XML_DIR . '/widgets');
define('GLOBAL_FORM_XML_DIR', GLOBAL_XML_DIR . '/forms');
define('GLOBAL_FORMFIELD_XML_DIR', GLOBAL_FORM_XML_DIR . '/fields');
define('GLOBAL_DEF_XML_DIR', GLOBAL_XML_DIR . '/page_default');
define('GLOBAL_CONTENT_DIR', GLOBAL_BASE_DIR . '/includes/content');
define('GLOBAL_JS_DIR', GLOBAL_SERVICE_DIR . '/new/js');

/**
 * The javascript url
 * @uses GLOBAL_BASE_DIR
 * @global constant GLOBAL_JS_URL
 * @name GLOBAL_JS_URL
 */
define('GLOBAL_JS_URL', GLOBAL_BASE_URL . '/new/js');
define('GLOBAL_CSS_DIR', GLOBAL_SERVICE_DIR . '/new/css');
define('GLOBAL_CSS_URL', GLOBAL_BASE_URL . '/new/css');
define('GLOBAL_IMAGE_DIR', GLOBAL_SERVICE_DIR . '/new/images');
define('GLOBAL_IMAGE_URL', GLOBAL_BASE_URL . '/new/images');
define('ASSET_IMAGE_DIR', GLOBAL_IMAGE_DIR . '/assets');
define('ASSET_IMAGE_URL', GLOBAL_IMAGE_URL . '/assets');
define('DIVISION_IMAGE_DIR', GLOBAL_IMAGE_DIR . '/companies');
define('DIVISION_IMAGE_URL', GLOBAL_LEADGEN . '/images/companies');


// GRC: the following are defined to resolve conflicts when using GLOBAL_BASE_URL/DIR
// new code should use the following
define( 'CDN_BASE_URL',          'http://global.networldalliance.com');
define( 'CDN_BASE_DIR',       '/home/glens/scripts/benchmark/php');
define( 'CDN_RFI_XML_DIR',       '/home/glens/scripts/benchmark/php/xml/rfi');

define('CDN_IMAGE_DIR', CDN_BASE_DIR . '/new/images');
define('CDN_IMAGE_URL', CDN_BASE_URL . '/new/images');

define('CDN_ARTICLE_COVER_DIR', CDN_IMAGE_DIR . '/article/covers');
define('CDN_ARTICLE_COVER_URL', CDN_IMAGE_URL . '/article/covers');

define('CDN_ASSET_DIR', CDN_BASE_DIR . '/downloads/white_papers');
define('CDN_ASSET_URL', CDN_BASE_URL . '/downloads/white_papers');

define('CDN_ASSET_COVER_DIR', CDN_IMAGE_DIR . '/assets/covers');
define('CDN_ASSET_COVER_URL', CDN_IMAGE_URL . '/assets/covers');

define('CDN_PRODUCT_DIR', CDN_IMAGE_DIR . '/products');
define('CDN_PRODUCT_URL', CDN_IMAGE_URL . '/products');

define('CDN_COMPANY_DIR', CDN_IMAGE_DIR . '/companies');
define('CDN_COMPANY_URL', CDN_IMAGE_URL . '/companies');

define('GLOBAL_SLIDESHOW_URL', CDN_BASE_URL . '/new/images/slideshows');
define('GLOBAL_SLIDESHOW_DIR', CDN_BASE_DIR . '/new/images/slideshows');
define('GLOBAL_AVATAR_URL', CDN_BASE_URL . '/new/images/avatars');
define('GLOBAL_AVATAR_DIR', CDN_BASE_DIR . '/new/images/avatars');

define('GLOBAL_CSSJS_FILE', GLOBAL_BASE_DIR . '/etc/global_css_js.xml');
define('GLOBAL_VIDEO_THUMB', 'http://video.networldalliance.com/thumb');

define('GLOBAL_CAROUSEL_DIR', CDN_BASE_DIR . '/new/images/new_carousel');
define('GLOBAL_CAROUSEL_URL', CDN_BASE_URL . '/new/images/new_carousel');




set_include_path(get_include_path() . PATH_SEPARATOR . GLOBAL_INCLUDE_DIR);

?>
