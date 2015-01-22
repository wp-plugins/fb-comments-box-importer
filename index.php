<?php
/*
Plugin Name: FB Comments Box Importer
Plugin URI: wp-resources.com
Description: Imports comments from Facebook comments box to your Wordpress site and gives it a SEO boost.
Version: 1.0.1
Author: Ivan M
*/

/*
 * Include FBCommentsBox Class
 */
require_once 'includes/FBCommentsBox.class.inc';
require_once 'includes/FBConnect.class.inc';

/*
 *  add admin menu
 */
add_action( 'admin_menu', 'fb_comments_box_import_plugin_menu' );
function fb_comments_box_import_plugin_menu() {
    add_menu_page(__('FB Comments Box Importer', 'fb_comments_box_options_function'), __('FB Comments Box Importer', 'fb_comments_box_options_function'), 'manage_options', 'fb_comments_box_importer', 'fb_comments_box_options_function');
    
    wp_register_script( 'FBComBoxScripts', plugins_url('js/script.js?v=3.2', __FILE__) );
    wp_enqueue_script( 'FBComBoxScripts' );
    
    wp_register_style( 'FBComBoxStylesheet', plugins_url('css/css.css?v=2', __FILE__) );
    wp_enqueue_style( 'FBComBoxStylesheet' );
}

/*
 * admin page
 */
function fb_comments_box_options_function() {
    

    // save login details for FB app
    if($_GET['action']=="save_data"){

        $appID = $_POST['appID'];
        $appSecret = $_POST['appSecret'];
        $commentsStatus = $_POST['comments_status'];

        update_option('fb_comments_box_appID', $appID);
        update_option('fb_comments_box_appSecret', $appSecret);
        update_option('fb_commentes_box_comments_status', $commentsStatus);
        ?><meta http-equiv="REFRESH" content="0;url=?page=fb_comments_box_importer"><?php
    }
    
    $appID = get_option('fb_comments_box_appID');
    $appSecret = get_option('fb_comments_box_appSecret');

    
    include("templates/config_tpl.php");
    

}

/*
 *  avatars filter
 */
add_filter('get_avatar', 'fb_comments_box_avatar_filter', 10, 5);
function fb_comments_box_avatar_filter($avatar, $id_or_email, $size = '50') {
    $FBCommentsBox = new FBCommentsBox();
    $avatar = $FBCommentsBox->GenerateAvatar($avatar, $id_or_email, $size);
    return $avatar;
}

/*
 * add new column to the posts table
 */
add_filter('manage_posts_columns', 'fb_comments_box_columns_head');
add_action('manage_posts_custom_column', 'fb_comments_box_content', 10, 2);

function fb_comments_box_columns_head($defaults) {
    
    $new = array();
    foreach ($defaults as $key => $title) {
        if ($key == 'author'){
            // Put the Thumbnail column before the Author column
            $new['check_comments'] = 'Check Comments';

        }
        $new[$key] = $title;
    }
    return $new;

}
function fb_comments_box_content($column_name, $post_ID) {
    if ($column_name == 'check_comments') {
        ?>
        <span id="check_comment_<?php echo $post_ID;?>">
            <a href="javascript:FBCommentsBoxManualCheck(<?php echo $post_ID;?>);">
                <img src="<?php echo plugin_dir_url(__FILE__);?>img/reload.png" />
            </a>
        </span>
        <img src="<?php echo plugin_dir_url(__FILE__);?>img/loader_rotate.gif" id="loader_id_<?php echo $post_ID;?>" style="display: none;">
        <?php
    }
}


/*
 * Ajax functions
 */
add_action('wp_ajax_fb_comments_box_manual_check', 'my_action_comments_box_manual_check');
function my_action_comments_box_manual_check() {
    
    $link_id = intval( $_POST['link_id'] );
    // PRO: check assigned url
    $permalink =  get_permalink( $link_id ); 

    
    
    $FBCommentsBox = new FBCommentsBox();
    $access_token = $FBCommentsBox->GenerateAccessToken();
    $GetComments = $FBCommentsBox->GetFBComments($link_id,$permalink,$access_token);
    $SaveComments = $FBCommentsBox->SaveCommentsToDatabase($GetComments, $link_id);
    
    echo $SaveComments;

    die(); // required to return a proper result
}

/*
 * TODO: Cron job
 */