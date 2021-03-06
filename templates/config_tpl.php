<?php  
if(!function_exists('curl_version')){
    echo '<div style="border: 1px solid #000000; padding: 5px; background: #FF0D00; border-radius: 5px; color: #fff">';
    echo "<b>ERROR: cURL is NOT installed on this server. Please enable curl to use this plugin.</b>";
    echo '</div>';
} 
?>
<div class="wrap">
    <h2>Import Facebook Box Comments</h2>
            
    <div class="postbox">
        <div class="inside">
            <table width="100%">
                <tr>
                    <td width="50%" valign="top">
                        <h2>Configuration:</h2>
                        <form action ="?page=fb_comments_box_importer&action=save_data" method="POST">
                            <table>
                                <tr>
                                    <td><a href="https://developers.facebook.com/apps" target="_blank">APP ID:</a></td>
                                    <td><input name="appID" type="text" value="<?php echo $appID; ?>" class="regular-text"></td>
                                </tr>
                                <tr>
                                    <td><a href="https://developers.facebook.com/apps" target="_blank">APP Secret Code:</a></td>
                                    <td><input name="appSecret" type="text" value="<?php echo $appSecret; ?>" class="regular-text"></td>
                                </tr>
                                <tr>
                                    <td>Comments status</td>
                                    <td>
                                        <input type="radio" <?php if($comments_status_value==1){echo "checked";}?> name="comments_status" value="1"> Approved 
                                        <input type="radio" <?php if($comments_status_value==0){echo "checked";}?> name="comments_status" value="0"> Not approved
                                    </td>
                                </tr>
                                <tr>
                                    <td></td>
                                    <td><input type="submit" name="submit" value="Save"></td>
                                </tr>
                            </table>
                        </form>
                    </td>
                    <td valign="top">
                        <h2><a href="http://wp-resources.com/facebook-comments-box-importer/">Buy Pro version for only 24,99 usd</a></h2>
                        <ul>
                            <li>- Import comments from external URLs</li>
                            <li>- Import comments automatically (with WP cron)</li>
                            <li>- Faster and more efficient support</li>
                            <li>... and much more...</li>
                        </ul>
                        
                    </td>
                </tr>
            </table>
        </div>
    </div>
</div>