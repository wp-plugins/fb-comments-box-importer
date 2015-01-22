<?php
class FBConnect{
    
    private $AppID;
    private $AppSecret;
    private $PostLoginURL;
    
    /*
     * Construct function
     */
    function __construct($AppID,$AppSecret,$PostLoginURL) {
        
        $this->AppID = $AppID;
        $this->AppSecret = $AppSecret;
        $this->PostLoginURL = $PostLoginURL;

    }
    
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
    
    
    /*//////////////////////////////////////////////////////////////////////////
     * TOKEN FUNCTIONS (auth, refresh...) //////////////////////////////////////
     *//////////////////////////////////////////////////////////////////////////
    
    // this function only create url for authorization
    public function AuthorizeUserURL(){
        $dialog_url= "http://www.facebook.com/dialog/oauth?"
               . "client_id=" .  $this->AppID 
               . "&redirect_uri=" . urlencode( $this->PostLoginURL);
        return $dialog_url;
    }
    
    public function GenerateAccessToken(){
        $fullurl = "https://graph.facebook.com/oauth/access_token?type=client_cred&client_id=$this->appID&client_secret=$this->appSecret";
        echo $fullurl;
        $ret = $this->GetHTTPSData($fullurl);
        return $ret;
    }
    
    // get token from code
    public function GetTokenFromCode($code) {

        if ($code) {

            $token_url = "https://graph.facebook.com/oauth/access_token?"
                    . "client_id=" . $this->AppID
                    . "&redirect_uri=" . urlencode($this->PostLoginURL)
                    . "&client_secret=" . $this->AppSecret
                    . "&code=" . $code;

            $response = $this->GetHTTPSData($token_url);
            $params = null;
            parse_str($response, $params);
            $access_token = $params['access_token'];

            update_option('fb_comments_box_access_token', $access_token);

            return $access_token;

        }
        else {
            return false;
        }
    }
    
    // check token (expire time and code)
    public function IsTokenValid($access_token){
        if($access_token){
            $content = $this->GetHTTPSData('https://graph.facebook.com/oauth/access_token_info?client_id='.$this->AppID.'&access_token='.$access_token);
            $json_object = json_decode($content);
            $data["access_token"] = $json_object->access_token;
            $data["expires"] = $json_object->expires_in;
            
            return $data;
        }
        else{
            return false;
        }
    }
    
    // get new token
    public function RenewToken($old_token){
        if($old_token){
            $content = $this->GetHTTPSData("https://graph.facebook.com/oauth/access_token?client_id=".$this->AppID."&client_secret=".$this->AppSecret."&grant_type=fb_exchange_token&fb_exchange_token=".$old_token);
            $token = str_replace("access_token=","",$content);
            $token = explode("&",$token);
            if($token[0]){
                update_option('fb_comments_box_access_token', $token[0]);
                return $token[0];
            }
            else {
                return false;
            }
        }
        else{
            return false;
        }
    }
    
    
}
