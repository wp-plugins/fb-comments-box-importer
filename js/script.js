function CheckComNumFbBox(id){
    jQuery.ajax({
        type: "GET",
        url: "https://graph.facebook.com/"+id+"/comments?limit=5000&fields=id",
        async: true,
        success: function(resp) {
            
            num = resp.data.length;
            jQuery("#countcomm_"+id).html('['+num+']');
        }
    });
}

function FBCommentsBoxManualCheck(id){
    
    var current_html = jQuery( "#check_comment_"+id ).html();
    jQuery( "#check_comment_"+id ).hide();
    jQuery("#loader_id_"+id).show();
    var data = {
        action: 'fb_comments_box_manual_check',
        link_id: id 
    };
    jQuery.post(ajaxurl, data, function(response) {
        jQuery("#loader_id_"+id).hide();
        jQuery( "#check_comment_"+id ).show();
        jQuery( "#check_comment_"+id ).append("<br>"+response+" comments imported");
    }).done(function() {
        jQuery("#loader_id_"+id).hide();
        jQuery( "#check_comment_"+id ).show();
        jQuery( "#check_comment_"+id ).append("<br>"+response+" comments imported");
    });

}