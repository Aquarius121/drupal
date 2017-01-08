(function($){
  // Add custom clear command for comment form
  Drupal.ajax.prototype.commands.supportgroups_comment_clear = function(ajax, response, status) {
    // remove all messages
    setTimeout(function(){$('.messages').remove();}, 3000);
    $('.error').remove();

    // set the value empty
    $('input[type=text], input[type=file], textarea, select', response.data.selector).val('');
    // reload comments
    // this is from supportgroups-feed.js
    _insert_comments(response.data.nid, response.data.cid);
    
    $(response.data.selector).hide();
  };

  // Add custom clear command for comment edit form
  Drupal.ajax.prototype.commands.supportgroups_editnode_clear = function(ajax, response, status) {
    // remove all messages
    setTimeout(function(){$('.messages').remove();}, 3000);
    $('.error').remove();

    // set the value empty
    $('input[type=text], input[type=file], textarea, select', response.data.selector).val('');
    // Hide the group selector
	//$(response.data.selector+" #edit-og-group-selector").css('display','none');
	$("#activity-item-comment-"+response.cid).html(response.body);
  };
}(jQuery));