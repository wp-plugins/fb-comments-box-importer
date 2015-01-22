<?php
Class FBCommentsBox{
    
    public function __construct(){
        $this->appID = get_option('fb_comments_box_appID');
        $this->appSecret = get_option('fb_comments_box_appSecret');
    }
    
    
    /**
     * Generate avatar image
     * @param type $avatar
     * @param type $id_or_email
     * @param type $size
     * @return type
     */
    public function GenerateAvatar($avatar, $id_or_email, $size = '50'){
        if (!is_object($id_or_email)) {
            $id_or_email = get_comment($id_or_email);
        }

        if (is_object($id_or_email)) {
            
            $ag_part = explode("__",$id_or_email->comment_agent); /// get only agent, remove hash
            if ($ag_part[0] == 'facebook-comment-importer-plugin' OR $id_or_email->comment_agent == 'facebook-comment-importer plugin') {
                $alt = '';
                $fb_url = $id_or_email->comment_author_url;
                $fb_array = split("/", $fb_url);
                $fb_id = $fb_array[count($fb_array) - 1];
                if (strlen($fb_id) > 1) {
                    $img = "http://graph.facebook.com/" . $fb_id . "/picture";
                    return "<img alt='{$alt}' src='{$img}' class='avatar avatar-{$size} photo' height='{$size}' width='{$size}' />";
                }
            }
        }
        return $avatar;
    }
    
    /**
     * Curl get content - https enabled
     * @param type $fullurl
     * @return type
     */
    public function GetHTTPSData($fullurl) {
        $ch = curl_init();
        curl_setopt($ch, CURLOPT_HEADER, 0);
        curl_setopt($ch, CURLOPT_VERBOSE, 0);
        curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, 0);
        curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, 0);
        curl_setopt($ch, CURLOPT_FAILONERROR, 0);
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
        curl_setopt($ch, CURLOPT_USERPWD, "$username:$password");
        curl_setopt($ch, CURLOPT_URL, $fullurl);
        $returned = curl_exec($ch);

        return $returned;
    }
    
    
    
    /**
     * Return total number of comments
     * @global type $wpdb
     * @param type $post_id
     * @return int
     */
    function total_comments_f($post_id){
        if($post_id == "-"){
            return 0;
        }
        else {
            global $wpdb;
            $commentes_num = $wpdb->get_var( "SELECT COUNT(*) FROM ".$wpdb->prefix."comments WHERE comment_post_ID = '".$post_id."'" );
            return $commentes_num;
        }
    }
    
    
    
    
    
    /////////// WORKING WITH FB API ////////////////////////////////////////////
    
    
    /**
     * Genrate token
     * @return type
     */
    public function GenerateAccessToken(){
        $fullurl = "https://graph.facebook.com/oauth/access_token?type=client_cred&client_id=$this->appID&client_secret=$this->appSecret";
        $ret = $this->GetHTTPSData($fullurl);
        $ret = str_replace("access_token=","",$ret);
        return $ret;
    }
    
    /**
     * Get username from facebook
     * @param type $userid
     * @return type
     */
    public function FBUsername($userid){
        $file = $this->GetHTTPSData("http://graph.facebook.com/$userid");
        $obj = json_decode($file);
        $username = $obj->username;
        return $username;
    }
    
    /**
     * Generate array with all comments for single post
     * @param type $postid
     * @return type
     */
    public function GenerateCommentsArrayForPost($postid){
        
        $all_my_comments = array();
        
        $my_comments = get_comments( array("post_id"=>$postid) );
        foreach($my_comments as $comment){
            $all_my_comments[] = $comment->comment_content;
        }
        return $all_my_comments;
    }
    public function GenerateCommentsArrayForPostAgent($postid){
        
        $all_my_comments = array();
        
        $my_comments = get_comments( array("post_id"=>$postid) );
        foreach($my_comments as $comment){
            $all_my_comments[] = $comment->comment_agent;
        }
        return $all_my_comments;
    }
    
    /**
     * Check is this double comment
     * @global type $wpdb
     * @param type $cid
     * @param type $mess
     * @return type
     */
    public function IsDoubleComment($cid, $user_agent,$mess,$comments_array){
        //$mess = addslashes($mess);
        
        if(in_array($user_agent,$comments_array)){
            return "1";
        }
        else {
            $mess = addslashes($mess);
            
            global $wpdb;
            $num = $wpdb->get_var( "SELECT COUNT(*) FROM ".$wpdb->prefix."comments WHERE comment_post_ID = '" . $cid . "' AND comment_content = '" . $mess . "'");
            return $num;

        }
    }
    
    /**
     * Get comment ID from useragent
     * @global type $wpdb
     * @param type $postid
     * @param type $user_agent
     * @return type
     */
    public function GetTopCommentID($postid, $user_agent){
        global $wpdb;
        $num = $wpdb->get_var( "SELECT comment_ID FROM ".$wpdb->prefix."comments WHERE comment_post_ID = '" . $postid . "' AND comment_agent = '" . $user_agent . "'");
        return $num;
    }
    

    /**
     * Generate time from wp string
     * @param type $created_time
     * @return type
     */
    private function GenerateTimeFromString($created_time){
        $date = explode("T", $created_time);
        $d = $date[0];
        $t = explode("+", $date[1]);
        $t = $t[0];
        $dt = "$d $t";
        return $dt;
    }
    
    public function GetURLFBid($url,$token){
        $file = $this->GetHTTPSData("https://graph.facebook.com/v2.1/?id=$url&access_token=$token");
        $FBobject = json_decode($file);

        return $FBobject->og_object->id;
    }
    
    /**
     * get comments and replies for single post
     * @param type $id
     * @param type $post_id
     * @return type
     */
    public function GetFBComments($post_id,$url,$token){
        
        $post_id = trim($post_id);

        $id = $this->GetURLFBid($url, $token);
        $api_url = 'https://graph.facebook.com/'.$id.'/comments?access_token='.$token.'&limit=1000&fields=id,from,message,created_time,comments.limit(1000).summary(true).fields(id,from,message,created_time)&filter=stream';

        $file = $this->GetHTTPSData($api_url);
        $FBobject = json_decode($file);
        if(!$FBobject){
            return false;
        }

        $FBCommentsArray = array_reverse($FBobject->data);
        
        // get all comments for this post
        $all_comments_for_post = $this->GenerateCommentsArrayForPostAgent($post_id);

        // parse FB object, and create array for db insert
        if($FBCommentsArray){
            foreach($FBCommentsArray as $SingleCommentData){
                
                $data['comment_id'] = $SingleCommentData->id;
                $data['from_name'] = $SingleCommentData->from->name;
                $data['from_id'] = $SingleCommentData->from->id;
                $data['message'] = $SingleCommentData->message;
                $data['created_time'] = $SingleCommentData->created_time;
                $data['timestamp'] = strtotime($SingleCommentData->created_time);
                $data['wp_date'] = $this->GenerateTimeFromString($SingleCommentData->created_time);
                $data['fb_username'] = $this->FBUsername($SingleCommentData->from->id);
                $data['profile_link'] = 'http://www.facebook.com/'.$SingleCommentData->from->id;
                $data['double_comment_check'] = $this->IsDoubleComment($post_id, 'facebook-comment-importer-plugin__'.$SingleCommentData->id,$SingleCommentData->message,$all_comments_for_post);
                if($data['double_comment_check'] == "1"){
                    $data['current_comment_id'] = $this->GetTopCommentID($post_id, 'facebook-comment-importer-plugin__'.$SingleCommentData->id);
                }
                $data['replies'] = $this->GetCommentsRepliesFromObject($SingleCommentData->comments->data,$post_id,$all_comments_for_post);
                $Comments[] = $data;
                unset($SingleCommentData);
            }
        }
        
        // get comments from next page (max limit is 2000)
        if($FBobject->paging->next){
            
            $file_next = $this->GetHTTPSData($FBobject->paging->next);
            $FBobject_next = json_decode($file_next);
            $FBCommentsArray_next = array_reverse($FBobject_next->data);
            
            if($FBCommentsArray_next){
                foreach($FBCommentsArray_next as $SingleCommentData){

                    $data['comment_id'] = $SingleCommentData->id;
                    $data['from_name'] = $SingleCommentData->from->name;
                    $data['from_id'] = $SingleCommentData->from->id;
                    $data['message'] = $SingleCommentData->message;
                    $data['created_time'] = $SingleCommentData->created_time;
                    $data['timestamp'] = strtotime($SingleCommentData->created_time);
                    $data['wp_date'] = $this->GenerateTimeFromString($SingleCommentData->created_time);
                    $data['fb_username'] = $this->FBUsername($SingleCommentData->from->id);
                    $data['profile_link'] = 'http://www.facebook.com/'.$SingleCommentData->from->id;
                    $data['double_comment_check'] = $this->IsDoubleComment($post_id, 'facebook-comment-importer-plugin__'.$SingleCommentData->id,$SingleCommentData->message,$all_comments_for_post);
                    if($data['double_comment_check'] == "1"){
                        $data['current_comment_id'] = $this->GetTopCommentID($post_id, 'facebook-comment-importer-plugin__'.$SingleCommentData->id);
                    }
                    $data['replies'] = $this->GetCommentsRepliesFromObject($SingleCommentData->comments->data,$post_id,$all_comments_for_post);
                    $Comments[] = $data;
                    unset($SingleCommentData);
                }
            }
            
        }
        
        // return complete array with replays
        return $Comments;
        
    }
    
    /**
     * Get only object with replies and create array
     * @param type $FBCommentsArray
     * @return boolean
     */
    public function GetCommentsRepliesFromObject($FBCommentsArray,$post_id,$all_comments_for_post){
        if($FBCommentsArray){
            foreach($FBCommentsArray as $SingleCommentData){
                
                $data['comment_id'] = $SingleCommentData->id;
                $data['from_name'] = $SingleCommentData->from->name;
                $data['from_id'] = $SingleCommentData->from->id;
                $data['message'] = $SingleCommentData->message;
                $data['created_time'] = $SingleCommentData->created_time;
                $data['timestamp'] = strtotime($SingleCommentData->created_time);
                $data['wp_date'] = $this->GenerateTimeFromString($SingleCommentData->created_time);
                $data['fb_username'] = $this->FBUsername($SingleCommentData->from->id);
                $data['profile_link'] = 'http://www.facebook.com/'.$SingleCommentData->from->id;
                $data['double_comment_check'] = $this->IsDoubleComment($post_id, 'facebook-comment-importer-plugin__'.$SingleCommentData->id,$SingleCommentData->message,$all_comments_for_post);
                $Comments[] = $data;
                unset($SingleCommentData);
            }
            return $Comments;
        }
        else{
            return false;
        }
    }
    
    /**
     * Save comments to database
     * @param type $comments
     * @param type $post_id
     */
    public function SaveCommentsToDatabase($comments,$post_id){
        
        $comments_status_value = get_option('fb_commentes_box_comments_status');
        if(!$comments_status_value){
            $comments_status_value = "0";
        }
        $i=0;
        if($comments){
            // parse comments
            foreach($comments as $comment){

                // check is this double comment, if yes skip it
                if($comment['double_comment_check']=="0"){

                    $ag = "facebook-comment-importer-plugin__".$comment['comment_id']; // set comment agent

                    $data = array(
                        'comment_post_ID' => $post_id,
                        'comment_author' => $comment['from_name'],
                        'comment_author_url' => $comment['profile_link'],
                        'comment_content' => $comment['message'],
                        'comment_agent' => $ag,
                        'comment_date' => $comment['wp_date'],
                        'comment_date_gmt' =>$comment['wp_date'], 
                        'comment_approved' => $comments_status_value,
                    );
                    $parent_comment = wp_insert_comment($data);
                    $i++;
                }
                else {
                    $parent_comment = $comment['current_comment_id'];
                }

                // insert replies if exist
                if($comment['replies']){
                    // parse replies
                    foreach($comment['replies'] as $replay){

                        if($replay['double_comment_check']=="0"){
                            $ag = "facebook-comment-importer-plugin__".$replay['comment_id']; // set comment agent

                            $data = array(
                                'comment_post_ID' => $post_id,
                                'comment_author' => $replay['from_name'],
                                'comment_author_url' => $replay['profile_link'],
                                'comment_content' => $replay['message'],
                                'comment_parent'=> $parent_comment,
                                'comment_agent' => $ag,
                                'comment_date' => $replay['wp_date'],
                                'comment_date_gmt' => $replay['wp_date'], 
                                'comment_approved' => $comments_status_value,
                            );
                            wp_insert_comment($data);
                            $i++;

                        }

                    }

                }




            }
        }
        // update comment count
        wp_update_comment_count($post_id);

        return $i;
    }
    
    
    
    
    
}

