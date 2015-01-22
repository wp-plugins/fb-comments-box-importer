<div class="wrap">
    
    
    <div class="infodiv">
        <h3><?php echo $msg;?></h3>
        <a href="?page=fb_comments_box_importer&action=auth"><button class="button button-primary button-large">Click Here</button></a> to renew your token (use only if you have problem with publishing posts)
    </div>
    <div class="infodiv">
        <table width="100%">
            <tr>
                <td width="50%">
                    <h3>Config connection details</h3>
                    <form action ="?page=fb_comments_box_importer&action=save_data" method="POST">
                        <table>
                            <tr>
                                <td><a href="https://developers.facebook.com/apps" target="_blank">APP ID:</a></td>
                                <td><input name="appID" type="text" value="<?php echo $appID;?>" class="regular-text"></td>
                            </tr>
                            <tr>
                                <td><a href="https://developers.facebook.com/apps" target="_blank">APP Secret Code:</a></td>
                                <td><input name="appSecret" type="text" value="<?php echo $appSecret;?>" class="regular-text"></td>
                            </tr>
                            <tr>
                                <td><a id="screen_show" href="#">Login redirect URL:</a></td>
                                <td><input name="postLoginURL" type="text" value="<?php echo $postLoginURL;?>" class="regular-text"></td>
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
                                <td><input type="submit" class="button button-primary button-large" name="submit" value="Save"></td>
                            </tr>
                        </table>
                    </form>
                </td>
                <td valign="top" width="50%">
                    <b>Plugin requirements:</b><br>
                        
                    - Curl enabled<br>
                    - available file_get_contents() function<br>
                    - PHP 5.1 +<br>
                </td>
           </tr>
        </table>
        
    </div>
    
    
</div>