(function($){
  // override ajax form submits with custom messages and styles
  if('undefined' != typeof(Drupal.ajax)) {
    Drupal.ajax.prototype.beforeSubmit = function (form_values, element, options) {
      // only hook main form submit and not image uploads
      if(options.url.indexOf('field_discussion_image') <= 0) {
        var message = "";
        if($(element).hasClass('comment-form')) {
          message = "<div class='message' style='width: 100%; text-align: center; padding: 3px 0;'>Thank you for your comment; it will appear shortly</div>";
        } else {
          message = "<div class='message' style='width: 100px; text-align: left; padding: 3px 0;'>Thank you for your post; it will appear shortly.</div>"
        }

        $(element).find('.form-actions input[type="submit"]').attr('disabled', 'disabled').addClass('disabled').val("Submitting...").after(message);
      }
    
    };
  }
  
  Drupal.behaviors.supportgroups_feed_activity =  {
    attach: function (context, settings) {
      // only run the activity functions if the page is setup to do so
      if('object' == typeof(Drupal.settings.supportgroups_feed) && 'object' == typeof(Drupal.settings.supportgroups_feed.activity)) {
        // add listener to get-past-activity button
    	  $("#get-past-activity:not(.processed)").addClass('processed').click(function(){
    		 _get_past_activity();
    		 return false;
    	  });
    	  
    	  // check the past activity flag
    	  if('object' == typeof(Drupal.settings.supportgroups_feed.activity.past) && Drupal.settings.supportgroups_feed.activity.past.check) {
    	    _check_supportfeed_minimum();
    	  }

    	  // add listener to get-recent-activity button
    	  $("#get-recent-activity:not(.processed)").addClass('processed').click(function(){
    		  _insert_recent_activity();
    		  return false;
    	  });


    	  // set interval to query for recent activity items
    	  // check for interval already running to avoid attachBehaviors
    	  if(!Drupal.settings.supportgroups_feed.activity.recent.running) {
    	    // set the recent interval id to running
    	    Drupal.settings.supportgroups_feed.activity.recent.running = setInterval('_get_recent_activity()', Drupal.settings.supportgroups_feed.activity.recent.interval); 
    	  }
        
      }// end of if statement
          
      // show more text for discussions
      $(".showmorebutton:not(.processed)").addClass("processed").click(function(){
        var nid = $(this).attr('nid');
        $("#showmorebutton-"+nid).remove();
        $("#activity-item-body-text-"+nid).css('height', 'auto');
      });

      // show more text for comments
      $(".showmorebutton-comment:not(.processed)").addClass("processed").click(function(){
        var cid = $(this).attr('cid');
        $("#showmorebutton-comment-"+cid).remove();
        $("#activity-item-comment-body-"+cid).css('height', 'auto');
      });

      
      // add listener to get comments for a nid
      $(".show-more-comments:not(.processed)").addClass('processed').click(function(){
          var nid = $(this).attr('nid');
          _insert_comments(nid, 0);          
          return false;
      });

  	  // add listener to activity-item-options-delete button
  	  $(".activity-item-options-delete:not(.processed)").addClass('processed').click(function(){
            var nid = $(this).attr('nid');
            // Display confirmation block
            $("#deletediscussioncontainer-"+nid).css('display', 'block');
  		  return false;
  	  });


  	  // add listener to activity-item-options-delete button
  	  $(".activity-item-options-delete-confirm:not(.processed)").addClass('processed').click(function(){
            var nid = $(this).attr('nid');
            // remove discussion
  		  _remove_discussion(nid);
  		  
  		  // hide the delete contianer
  		  $("#deletediscussioncontainer-"+nid).hide();
  		  return false;
  	  });

  	  // add listener to activity-item-options-delete button
  	  $(".activity-item-options-delete-cancel:not(.processed)").addClass('processed').click(function(){
            var nid = $(this).attr('nid');
            // Hide confirmation block
            $("#deletediscussioncontainer-"+nid).css('display', 'none');
  		  return false;
  	  });

  	  // add listener to activity-item-comments-options-delete button
  	  $(".activity-item-comments-options-delete:not(.processed)").addClass('processed').click(function(){
            var cid = $(this).attr('cid');
            // Display confirmation block
            $("#deletecommentcontainer-"+cid).css('display', 'inline-block');
  		  return false;
  	  });


  	  // add listener to activity-item-comments-options-delete button
  	  $(".activity-item-comments-options-delete-confirm:not(.processed)").addClass('processed').click(function(){
        var cid = $(this).attr('cid');
        
        // remove discussion
        _remove_comment(cid);
  		  
  		  // hide delete container    
        $("#deletecommentcontainer-"+cid).hide('fast');
        
  		  return false;
  	  });

  	  // add listener to activity-item-comments-options-delete button
  	  $(".activity-item-comments-options-delete-cancel:not(.processed)").addClass('processed').click(function(){
        var cid = $(this).attr('cid');
        
        // Hide confirmation block
        $("#deletecommentcontainer-"+cid).css('display', 'none');
  		  return false;
  	  });

  	  // add listener to show comment form
  	  $(".comment-form-reply-button:not(.processed)").addClass('processed').click(function(){
            var username = $(this).attr('username');
            var nid = $(this).attr('nid');
            var pid = $(this).attr('pid');
            var cid = $(this).attr('cid');
            // remove duplicate forms
            if( $("#comment-node-group-post-form-pid-"+pid).length == 0 ){
            }
            else{
             $("#comment-node-group-post-form-pid-"+pid).remove();
            }
            // need to add username to the form
            // need to produce comment form
            _get_comment_form(nid,pid,cid,username);
            // Show comment form              
            $("#comment-form-container-"+cid).css('display', 'block');
  		  return false;
  	  });
      
      // TODO create edit discussion code
  	  // add listener to activity-item-options-delete button
  	  $(".activity-item-options-edit:not(.processed)").addClass('processed').click(function(){
            var nid = $(this).attr('nid');
            _edit_discussion(nid);
            // TODO create function
  		  return false;
  	  });

        // TODO create edit comment code
  	  // add listener to 
  	  $(".activity-item-comments-options-edit:not(.processed)").addClass('processed').click(function(){
            var cid = $(this).attr('cid');
            var nid = $(this).attr('nid');
            var pid = $(this).attr('pid');
            _edit_comment(cid,nid,pid);
  		  return false;
  	  });

  	  // add listener when hovering username ON a activity item
  	  $(".user-profile-container:not(.processed)").addClass('processed').hover(
  	    function(){
            var nid = $(this).attr('nid');
            $("#user-profile-mini-block-"+nid).css('display', 'block');
  	    },
  	    function(){
            var nid = $(this).attr('nid');
            $("#user-profile-mini-block-"+nid).css('display', 'none');
  	    }    	  
  	  );

  	  // add listener when hovering over discussion options
  	  $(".activity-item-wrapper:not(.processed)").addClass('processed').hover(    	  
  	    function(){
            var nid = $(this).attr('nid');
            $("#activity-item-options-"+nid).css('display', 'block');
  	    },
  	    function(){
            var nid = $(this).attr('nid');
            $("#activity-item-options-"+nid).css('display', 'none');
  	    }
  	  );


  	  // add listener when hovering over discussion options
  	  $(".activity-item-comment-wrapper:not(.processed)").addClass('processed').hover(    	  
  	    function(){
            var cid = $(this).attr('cid');
            $("#comment-options-"+cid).css('display', 'block');
  	    },
  	    function(){
            var cid = $(this).attr('cid');
            $("#comment-options-"+cid).css('display', 'none');
  	    }
  	  );

  	  // add listener when hovering username ON a comment item
  	  $(".user-profile-container-comment:not(.processed)").addClass('processed').hover(
  	    function(){
            var cid = $(this).attr('cid');
            $("#user-profile-mini-block-comment-"+cid).css('display', 'block');
  	    },
  	    function(){
            var cid = $(this).attr('cid');
            $("#user-profile-mini-block-comment-"+cid).css('display', 'none');
  	    }    	  
  	  );


  	  // add listener when userpoints on a comment item
  	  $(".userpoints-container-comment:not(.processed)").addClass('processed').hover(
  	    function(){
            var cid = $(this).attr('cid');
            $("#userpoints-info-"+cid).css('display', 'block');
  	    },
  	    function(){
            var cid = $(this).attr('cid');
            $("#userpoints-info-"+cid).css('display', 'none');
  	    }    	  
  	  );

    	// add listener when user clicks on the comment link on the discussion
    	$(".comment-postbutton:not(.processed)").addClass('processed').click(
    	    function(){
              var nid = $(this).attr('nid');
              // trigger the add a comment form load
              $("#comment-form-"+nid).find('a.add-a-comment').trigger('click');
              
              // scroll to the top
              $(window).scrollTop($("#comment-form-"+nid).position().top);
    	    }
    	 );
    
      // convert object to array
      if('object' == typeof(Drupal.settings.supportgroups_feed) && 'object' == typeof(Drupal.settings.supportgroups_feed.activity) && 'object' == typeof(Drupal.settings.supportgroups_feed.activity.nids)) {
        nids = []
        for(var nid in Drupal.settings.supportgroups_feed.activity.nids) {
          nids.push(Drupal.settings.supportgroups_feed.activity.nids[nid]);
        }
        // change nids object to array
        Drupal.settings.supportgroups_feed.activity.nids = nids;
      }        
        
      // clone the node block form if it is present on the page
      if('undefined' != typeof(Drupal.settings.supportgroups_node) && Drupal.settings.supportgroups_node.group_post.selector.length) {
        form = $('#'+Drupal.settings.supportgroups_node.group_post.selector);
        if(form.length) {
          Drupal.settings.supportgroups_node.group_post.form = form.clone();
        }
      }
      
      // sweep the page for comments. Mark as ajax processed if id exists in Drupal.settings.ajax
      comment_forms_to_be_processed = $('div.comment-form:not(.processed)');
      if(comment_forms_to_be_processed.length) {
        _attach_ajax_settings_for_lazy_loaded_comments(comment_forms_to_be_processed);
      } // close if(comment_forms_to_be_processed.length) {
    } // close attach: function (context, settings) {
                
  }; // close Drupal.behaviors.supportgroups_feed_activity =  {
}(jQuery));


/**
 * Helper function to remove discussion
 */
function _remove_discussion(nid) {
  var query_uri = '/ajax/sg_f/discussion_remove/'+nid;
  // icon while waiting for the response  
  jQuery("#activity-item-"+nid).addClass('load-icon').prepend('<div class="message" style="padding: 5px 0; text-align:center;">Deleting will take a few moments. Sorry for the delay.</div>');
  
  // copied from recent activity function. need to change
 jQuery.ajax({
	  url: query_uri,
        data: 'js=1',
	success: function(data) {	  
	  if( data.status == 'true'){
        jQuery("#activity-item-"+nid).remove();
	  }
	  else{
	  
	  }    
	},
	error: function(data) {
	  console.log("There was an error removing the entry");
	},
	complete: function(){
      jQuery("#activity-item-"+nid).removeClass('load-icon');
	}
  });
  
}

/**
 * Helper function to remove discussion
 */
function _remove_comment(cid) {
  var query_uri = '/ajax/sg_f/comment_remove/'+cid;
  // icon while waiting for the response  
  jQuery("#activity-item-comment-"+cid).addClass('load-icon').prepend('<div class="message" style="padding: 5px 0; text-align:center;">Deleting will take a few moments. Sorry for the delay.</div>');
  // copied from recent activity function. need to change
 jQuery.ajax({
	  url: query_uri,
        data: 'js=1',
	success: function(data) {	  
	  if( data.status == 'true'){
        jQuery("#activity-item-comment-"+cid).remove();
	  }
	  else{
	  
	  }    
	},
	error: function(data) {
	  console.log("There was an error removing the entry");
	},
	complete: function(){
      jQuery("#activity-item-comment-"+cid).removeClass('load-icon');
	}
  });
  
}


/**
 * Helper function to edit comment
 */
function _edit_comment(cid,nid,pid) {
  var query_uri = '/ajax/sg_f/comment_edit/'+cid+'/'+nid+'/'+pid;
  // icon while waiting for the response  
  jQuery("#activity-item-comment-body-"+cid).addClass('load-icon');
  // copied from recent activity function. need to change
 jQuery.ajax({
	  url: query_uri,
        data: 'js=1',
	success: function(data) {	  
	  if( data.status == 'true'){        
	    var container = jQuery("#activity-item-comment-body-"+cid);
	    
	    // get the returned form
	    var comment_form = data.commentform;
	    
	    // find the button within the form
	    var comment_form_submit = jQuery('.form-submit', comment_form);
	          
	    id = jQuery('.form-submit', comment_form).attr('id');
      
	    // count the number of current buttons
	    //TODO need to update method of counting objects as it is not supported in IE8
	    var num_of_buttons = Object.keys(Drupal.settings.ajax).length
	    
	    // add non ajax forms
      num_of_buttons = num_of_buttons + _count_non_ajax_forms();
            
	    // create new id based on what needs to happen
	    new_id = id + '--' + (num_of_buttons+1);
      
	    // we need to update the Drupal.settings.ajax object with the new form
	    Drupal.settings.ajax[new_id] = _spoof_ajax_settings_object_for_comment(new_id);
      
	    // alter the button with the new id
	    new_comment_form = jQuery(comment_form).clone();
	    jQuery('.form-submit', new_comment_form).attr('id', new_id);
                  
	    // add the form to the container
	    container.html(new_comment_form);
	    
	    // focus the cursor on the textarea
	    ta = jQuery('textarea', container);
	    ta_val = ta.val() + " ";
	    ta.focus().val("").val(ta_val);
        
	    // finally ajaxify the form by attachingBehaviors          
	    Drupal.attachBehaviors();
	  }
	  else{
	    console.log("THERE WAS AN ERROR");
	  }    
	},
	error: function(data) {
	  console.log("There was an error trying to edit the comment");
	},
	complete: function(){
     jQuery("#activity-item-comment-body-"+cid).removeClass('load-icon');
	
	}
  });
  
}



/**
 * Helper function to get comment form
 */
function _get_comment_form(nid,pid,cid,username) {
  var query_uri = '/ajax/sg_f/comment_form/'+nid+'/'+pid+'/'+cid;
  // icon while waiting for the response  
  
  if(cid == '0') {
    var container = jQuery("#comment-form-container-"+nid+"-"+cid).addClass('load-icon');
  } else {
    var container = jQuery("#comment-form-container-"+cid).addClass('load-icon');
  }

  // copied from recent activity function. need to change
 jQuery.ajax({
	  url: query_uri,
        data: 'js=1',
	success: function(data) {	 
	  if( data.status == 'true' ){
	    // throw in an override for 
	    if(cid == '0') {
	      var container = jQuery("#comment-form-container-"+nid+"-"+cid);
	    } else {
	      var container = jQuery("#comment-form-container-"+cid);
	    }
	    
	    // get the returned form
	    var comment_form = data.commentform;
	    
	    // find the button within the form
	    var comment_form_submit = jQuery('.form-submit', comment_form);
	          
	    id = jQuery('.form-submit', comment_form).attr('id');
      
	    // count the number of current buttons
	    //TODO need to update method of counting objects as it is not supported in IE8
	    if(typeof(Drupal.settings.ajax) != 'undefined' && typeof(Drupal.settings.ajax) != null) {
	      var num_of_buttons = Object.keys(Drupal.settings.ajax).length
	    } else {
	      var num_of_buttons = 0
	      Drupal.settings.ajax = {};
	    }
	    
	    // add non ajax forms
      num_of_buttons = num_of_buttons + _count_non_ajax_forms();
            
	    // create new id based on what needs to happen
	    new_id = id + '--' + (num_of_buttons+1);
      
	    // we need to update the Drupal.settings.ajax object with the new form
	    Drupal.settings.ajax[new_id] = _spoof_ajax_settings_object_for_comment(new_id);
      
	    // alter the button with the new id
	    new_comment_form = jQuery(comment_form).clone();
	    jQuery('.form-submit', new_comment_form).attr('id', new_id);
                  
	    // add the form to the container
	    container.html(new_comment_form);
	    container.removeClass('load-icon');

      // Add @ username tag
      jQuery('#comment-node-group-post-form-pid-'+pid+' textarea').val('@'+username);
      
	    // focus the cursor on the textarea
	    ta = jQuery('textarea', container);
	    ta_val = ta.val() + " ";
	    ta.focus().val("").val(ta_val);
        
	    // finally ajaxify the form by attachingBehaviors          
	    Drupal.attachBehaviors();
	  }
	  else{
	  
	  }    
	},
	error: function(data) {
	  console.log("There was an error trying to load the comment form.");
	  container.html("<div class='comment-error'>There was an error trying to load the comment form.</div>");
	},
	complete: function() {
     container.removeClass('load-icon');
	},
  });
  
}




/**
 * Helper function to edit discussion
 */
function _edit_discussion(nid) {
  var query_uri = '/ajax/sg_f/discussion_edit/'+nid;
  // icon while waiting for the response  
  jQuery("#activity-item-body-"+nid).addClass('load-icon');
  // copied from recent activity function. need to change
 jQuery.ajax({
	  url: query_uri,
        data: 'js=1',
	success: function(data) {	  
	  if( data.status == 'true'){
	      var container = jQuery("#activity-item-body-"+nid);        
        var discussion_form = data.discussion;
        
        // count the number of current buttons
        //TODO need to update method of counting objects as it is not supported in IE8
        var num_of_buttons = Object.keys(Drupal.settings.ajax).length
        
        // add non ajax forms
        num_of_buttons = num_of_buttons + _count_non_ajax_forms();
        
        // add the discussion form with new_id to the page
        new_discussion_form = jQuery(discussion_form).clone();
 
        // we need to take into account multiple submit buttons on the node
        //discussion form
        submit_buttons = jQuery('.form-submit', discussion_form);
        new_ids = [];
        old_ids = [];
        
        submit_buttons.each(function(x){
          id = jQuery(this).attr('id');          
          
          // test whether the id is numerice
          if(!(new RegExp('[0-9]')).test(id)) {
            // create new id based on what needs to happen
            new_id = id + '--' + (num_of_buttons+(1+x));
          } else {
            new_id = id;
          }          
           
          // record new ids to loop again velow
          new_ids.push(new_id);
           
          // record old id to mimic the indexes in order to clone previous object
          old_ids.push(id);
          
          // always change the id
          jQuery('#'+id, new_discussion_form).attr('id', new_id);
          
        });
        
        // add the form to the container
        container.html(new_discussion_form);
        
        // loop the ids to get their elements and load them onto the ajax settings
        jQuery.each(new_ids, function(i, value){
          // get discussion object for Drupal.settings.ajax; we still need to override the element
          discussion_obj = _spoof_ajax_settings_object_for_discussion(value, old_ids[i]);
          
          // we need to make sure a valid object is returned
          // otherwise do not append to Drupal.settings.ajax to prevent breaking Drupal.ajax
          if('object' == typeof(discussion_obj)) {
              // discussion form needs an element attached
              discussion_obj.element = jQuery('#'+value);

              // we need to update the Drupal.settings.ajax object with the new form
              Drupal.settings.ajax[value] = discussion_obj;
          }
        });
        
        // for discussion edits only, show the image-preview
        if(jQuery('.image-preview', container).length > 0) {
          jQuery('.field-name-field-discussion-image', container).show();
        }

        // finally ajaxify the form by attachingBehaviors          
        Drupal.attachBehaviors();
	  }   
	},
	error: function(data) {
	  console.log("There was an error trying to edit the discussion");
	},
	complete: function(){
      jQuery("#activity-item-body-"+nid).removeClass('load-icon');	
	}
  });
  
}


/**
 * Helper function to get all comments for a node
 */
function _insert_comments(nid, cid) {
  // build url based on type
  query_uri = '/ajax/sg_f/comments';
  // icon while waiting for the response  
  jQuery("#show-more-comments-container-"+nid).addClass('loading');
  var discussion_page;
  var pathname = window.location.pathname;
  // If this is not the groups page or the user page, then this has to be the discussion page
  if( pathname != '/' && window.location.href.indexOf(window.location.origin+"/user") == -1 ){
    discussion_page = 1;
  } else{
    discussion_page = 0;
  }
  jQuery.ajax({
    url: query_uri,
    data: {
    	id: nid,
    	discussion_page: discussion_page,
    },
    success: function(data) {      
        jQuery("#activity-item-comments-"+nid).html(data.themed_output);
        Drupal.attachBehaviors(); 
        jQuery("#show-more-comments-"+nid).remove();
        
        // focus the element
        if(cid != 0) {
          console.log("Focusing to comment #activity-item-comment-"+cid);
          jQuery(window).scrollTop(jQuery("#activity-item-comment-"+cid).position().top);
        }
      
    },
    complete: function(data){
      jQuery("#show-more-comments-container-"+nid).removeClass('loading');
    }
  });
}

/**
 * Helper function to run interval callback for current activity and prepend results to the page
 */
function _get_recent_activity() {
  var wrapper = jQuery('#'+Drupal.settings.supportgroups_feed.activity.wrapper);

  // get the time and set the new time to advance the query
  current_time = Drupal.settings.supportgroups_feed.activity.time
  
  // set the time rounded to the minute
  Drupal.settings.supportgroups_feed.activity.time = _get_current_time_rounded_to_coeff(60 * 1000);
  
  // build url based on type
  query_uri = '/ajax/sg_f/';
  if(Drupal.settings.supportgroups_feed.activity.type == 'node') {
    query_uri += 'group_';
  }
  query_uri += 'recent_activity';
      // add load icon
  jQuery("#get-recent-activity").addClass('load-icon');

  jQuery.ajax({
    url: query_uri,
    data: {
    	max: Drupal.settings.supportgroups_feed.activity.max,
    	from: current_time,
    	id: Drupal.settings.supportgroups_feed.activity.id,
    },
    success: function(data) {      
      if(data.themed_output) {
        jQuery.each(data.themed_output, function(index, value){
          // override the index to prevent v8 in chrome and opera from sorting the indexed node ids
          new_index = index.replace(Drupal.settings.supportgroups_feed.index_override, '');

          console.log("Index was: "+index+" . New index is: "+new_index);
          
          // is the node already on the page?
          if(jQuery.inArray(new_index, Drupal.settings.supportgroups_feed.activity.nids) > -1) {
            //TODO Remove logging message
            console.log(new_index+" matches and is already on the page");

            // replace the existing content with the new
            // the returned elements are nested arrays so,
            // we need to loop the array and break on the first, "most recent", item
            jQuery.each(value, function(x, markup) {
              // add a check to see if the content exists in the new array rather than on the page
              if('undefined' != typeof(Drupal.settings.supportgroups_feed.activity.recent.new[new_index])) {
                // override the existing content waiting to be inserted with new
                Drupal.settings.supportgroups_feed.activity.recent.new[new_index] = markup;
              } else {            	  
              	// skip the item if an element within has focus
              	activity_item = jQuery("#activity-item-"+new_index);
            	
              	//TODO Clean up focused text into one line. Remove log message
              	// if(jQuery('textarea:focus', activity_item).length) {
              	focused = jQuery('textarea:focus', activity_item);
            	
              	if(!focused.length) {
              	  activity_item.replaceWith(markup);
              	}
              }

          	  // break the loop after the first one
          	  return false;
            });

          } else {
            //TODO Remove logging message
            console.log(new_index+" does not match and will be prepended to the top");

            // append the nid to the existing array so we now it is there for future calls
            Drupal.settings.supportgroups_feed.activity.nids.push(new_index);

            // save the output for prepending to the page


            // save the output for prepending to the page
            // the returned elements are nested arrays so,
            // we need to loop the array and break on the first, "most recent", item
            jQuery.each(value, function( x, markup ) {
          	  Drupal.settings.supportgroups_feed.activity.recent.new[new_index] = markup;

          	  // break the loop after the first one
          	  return false;
            });
          } // end if
        }); // end each
      }  // end if
      
      // after we have finished this loop decide if we are at the top of the page and should prepend content
      // or if we should hold it and signal the user that we have some new content for them
      _show_recent_activity();
      
      // add event behaviors to new markup
      Drupal.attachBehaviors();
    },
    complete: function(data){
      // add load icon
      jQuery("#get-recent-activity").removeClass('load-icon');    
    }
  });
}

/**
 * Helper function to run ajax callback for past activity and prepend them to the page
 */
function _get_past_activity() {  
  var wrapper = jQuery('#'+Drupal.settings.supportgroups_feed.activity.wrapper);
  
  // get timestamp of last feed item on the page from the last comment or the discussion if no comments have been made
  last_comment = jQuery('.activity-item-wrapper:last .activity-item-comments .activity-item-comment-wrapper:last', wrapper);
  if(last_comment.length) {
    last_item_time = last_comment.attr('time');
  } else {
    last_item_time = jQuery('.activity-item-wrapper:last', wrapper).attr('time');
  }
    
  // if there is no time then set it to the last minute
  //(Math.round(date.getTime() / coeff) * coeff) / 1000
  var isadmin = false;
  var isArchive = false;
  if(Drupal.settings.currentUser != undefined && Drupal.settings.currentUser.uid == "1")
    isadmin = true;
  if(window.location.pathname == "/archive")
    isArchive = true;
  var twoYB = (new Date(new Date().setFullYear(new Date().getFullYear() - 2))).getTime();
  var limitTime = (Math.round(twoYB / 60000) * 60000) / 1000;
  if(!isArchive && last_item_time && last_item_time < limitTime || !isadmin && isArchive && last_item_time && last_item_time < limitTime){
    return false;
  }
  
  if(!last_item_time) {
    last_item_time = _get_current_time_rounded_to_coeff(60 * 1000);
  }
  if(isadmin && isArchive && last_item_time > limitTime){
      last_item_time = limitTime;
  }
  
  // build url based on type
  query_uri = '/ajax/sg_f/';
  if(Drupal.settings.supportgroups_feed.activity.type == 'node') {
    query_uri += 'group_';
  }
  query_uri += 'past_activity';
  // add load icon
  jQuery(".past-activity-wrapper-container").addClass('loading');
    
  jQuery.ajax({
	  url: query_uri,
	  crossDomain: true,
    data: {
      max: Drupal.settings.supportgroups_feed.activity.max,
      before: last_item_time,
      id: Drupal.settings.supportgroups_feed.activity.id
    },
	success: function(data) {	  
	  if(data.themed_output) {
      // loop the response and append to the bottom
      jQuery.each(data.themed_output, function(index, value){        
        // override the index to prevent v8 in chrome and opera from sorting the indexed node ids
        new_index = index.replace(Drupal.settings.supportgroups_feed.index_override, '');
                
        // is the node already on the page?            
        if(jQuery.inArray(new_index, Drupal.settings.supportgroups_feed.activity.nids) > -1) {
          //TODO Remove logging message
          console.log(new_index+" matches and is already on the page");
        
          // replace the existing content with the new
          // the returned elements are nested arrays so,
          // we need to loop the array and break on the first, "most recent", item
          jQuery.each(value, function(x, markup) {
        	  jQuery("#activity-item-"+new_index).replaceWith(markup);
      	  
        	  // break the loop
        	  return false;
          });

        } else {
          //TODO Remove logging message
          console.log(new_index+" does not match and will be appended to the bottom");
        
          // append the nid to the existing array so we now it is there for future calls
          // this also runs in timestamp sorted order so the most recent nid gets added first
          // and subsequest matching nids are ignored
          Drupal.settings.supportgroups_feed.activity.nids.push(new_index);
        
          // grab the wrapper
          var wrapper = jQuery('#'+Drupal.settings.supportgroups_feed.activity.wrapper);
        
          // save the output for prepending to the page
          // the returned elements are nested arrays so,
          // we need to loop the array and break on the first, "most recent", item
          jQuery.each(value, function( x, markup ) {
          	wrapper.append(markup);
      	  
          	// break the loop
        	  	return false;
          });
        }
      });
    }
    
    // add event behaviors to new markup
    Drupal.attachBehaviors();
	},
	error: function(data) {
	  console.log("There was an error retrieving past activity");
	},
	complete: function(data){
      // add load icon
      jQuery(".past-activity-wrapper-container").removeClass('loading');
	
	}
  });
}

/**
 * Helper function to determine whether or not to show content
 **/
function _show_recent_activity(){
  // if there is content to append
  if(Drupal.settings.supportgroups_feed.activity.recent.new.length) {
    // if we are at the top of the page then insert
    if(!jQuery(window).scrollTop()) {
      _insert_recent_activity();
    } else {
      // show recent activity button for user to request new content
      jQuery("#recent-activity-wrapper").fadeIn();
    }
  } else {
    // there is no recent content so hide the recent activity wrapper
    jQuery("#recent-activity-wrapper").fadeOut('slow');
  }
}

function _count_non_ajax_forms() {
  return jQuery('.form-submit:not(.ajax-processed)').length;
}

/**
 * Helper function to insert new content
 **/
function _insert_recent_activity(){
  if(window.location.pathname == "/archive")
    return false;
  var wrapper = jQuery('#'+Drupal.settings.supportgroups_feed.activity.wrapper);
  var new_content = Drupal.settings.supportgroups_feed.activity.recent.new;
  
  // prepend the new activity
  jQuery.each(new_content, function(i, value) {
    wrapper.prepend(value);
  });
  
  // hide the new activity wrapper
  jQuery("#recent-activity-wrapper").fadeOut('slow');
  
  // set the new activity empty
  Drupal.settings.supportgroups_feed.activity.recent.new = [];
}

/**
 * Helper function to check for support feed minimum
 **/
function _check_supportfeed_minimum() {
  // we only want to run this once so set the check to false
  Drupal.settings.supportgroups_feed.activity.past.check = false;
  
  // count th activity items on the page
  count = jQuery('.activity-item-wrapper', '#activity-items').length
  if(count < Drupal.settings.supportgroups_feed.activity.past.minimum) {
    console.log("Getting past activity to fulfill minimum");
    _get_past_activity();
  }
} 


function _attach_ajax_settings_for_lazy_loaded_comments(comment_forms_to_be_processed) {
  // add processed class and get ids
  ids = [];
  comment_forms_to_be_processed.addClass('processed').each(function(index, value){
    submit_button = jQuery(this).find('div.form-actions input[type="submit"]');
    
    // push button id onto ids array
    ids.push(submit_button.attr('id'));
  });

  // if we have ids, intersect them with existing Drupal.settings.ajax keys
  if(ids.length) {
    //  if Drupal.settings.ajax exists get the keys else create the object
    existing_keys = Array();
    if('object' == typeof(Drupal.settings.ajax)) {
      existing_keys = Object.keys(Drupal.settings.ajax);
    } else {
      Drupal.settings.ajax = {};
    }
    
    intersection = ids.filter(function(el) {
        return existing_keys.indexOf(el) != -1
    });
    
    // if an intersection exists, set new ids on the form submit buttons
    if(intersection.length) {
      num_of_buttons = existing_keys.length

      // add non ajax forms
      num_of_buttons += _count_non_ajax_forms();
      
      // add one more for a fresh id
      num_of_buttons += 1;
      jQuery.each(intersection, function(index, value){
        // set new id
        new_id = 'edit-submit--'+ num_of_buttons;
        
        // set the new values
        jQuery('#'+value).attr('id', new_id);
        
        // pop the old id from the ids array
        x = ids.indexOf(value);
        if(x > -1) {
          ids.splice(x, 1);
        }
        
        // add the new id to the array
        ids.push(new_id);
        
        // increment the button cound
        num_of_buttons += 1;
      })
    }
    
    // once we have checked for intersection we can spoof the ajax settings for comments
    jQuery.each(ids, function(index, value){
       // get discussion object for Drupal.settings.ajax; we still need to override the element
       comment_obj = _spoof_ajax_settings_object_for_comment(value);

       // we need to make sure a valid object is returned
       // otherwise do not append to Drupal.settings.ajax to prevent breaking Drupal.ajax
       if('object' == typeof(comment_obj)) {
           // discussion form needs an element attached
           comment_obj.element = jQuery('#'+value);

           // we need to update the Drupal.settings.ajax object with the new form
           Drupal.settings.ajax[value] = comment_obj;
       }
     });
  } // close if(ids.length) {
}

function _spoof_ajax_settings_object_for_comment(id) {
  obj = {
    callback: 'supportgroups_comment_group_node_callback',
    event: 'mousedown',
    keypress: true,
    method: 'replaceWith',
    prevent: 'click',
    progress: {
      message: 'Submitting you post'
    },
    selector: '#'+id,
    submit: {
      _triggering_element_name: 'op',
      _triggering_element_value: 'Post',
    },
    url: '/system/ajax',
    wrapper: 'comment-node-group-post-form'
  };
  return obj;
}

function _spoof_ajax_settings_object_for_discussion(new_id, old_id) {
  // we need special overrides for upload and remove fields
  if(new_id.indexOf('upload-button') > -1) {
    elem = jQuery('#'+new_id);
    upload_input_name = elem.attr('name')
    path = elem.attr('path');
    
    //TODO Need a better way to get the wrapper element of the form
    wrapper_id = elem.parent().parent().parent().parent().attr('id');
    
    // there will always be an upload object on the page
    if('object' == typeof(Drupal.settings.ajax['edit-field-discussion-image-und-0-upload-button'])) {
        new_obj = jQuery.extend(true, {}, Drupal.settings.ajax['edit-field-discussion-image-und-0-upload-button']);
        new_obj.selector = '#'+new_id;
        new_obj.url = '/'+path;
        new_obj.wrapper = wrapper_id;
    } else {
    	new_obj = _spoof_ajax_settings_object_for_upload_button(new_id, upload_input_name, wrapper_id, path);
    }
    return new_obj;
  }
  
  // setup remove buttons
  if(new_id.indexOf('remove-button') > -1) {
    elem = jQuery('#'+new_id);
    remove_input_name = elem.attr('name');
    path = elem.attr('path');
    
    //TODO Need a better way to get this
    wrapper_id = elem.parent().parent().parent().parent().attr('id');
    remove_obj = _spoof_ajax_settings_object_for_remove_button(remove_input_name, wrapper_id, path);
    remove_obj.wrapper = wrapper_id;
    
    return remove_obj;
  }
  
  // get the old object and clone
  if('object' == typeof(Drupal.settings.ajax[old_id])) {
	  old_obj = Drupal.settings.ajax[old_id];
	  
	  // clone the object
	  new_obj = jQuery.extend(true, {}, old_obj);
	  new_obj.selector = '#'+new_id;
    
	  return new_obj;
  } else {
	elem = jQuery('#'+new_id);
	  
	  //TODO Need a better way to get this
	wrapper_id = elem.parent().parent().parent().parent().attr('id');
	  
	  new_obj = _spoof_ajax_settings_object_for_post_button(new_id, wrapper_id);
	  return new_obj;
  }
  
  // if nothing is found returning empty object
  return false;
}


/**
 * Helper function to spoof a submit button for easy appending to the Drupal.ajax object
 * @param string id
 * @param string path
 **/
function _spoof_ajax_settings_object_for_post_button(button_id, wrapper_id) {
  obj = {
	callback: 'supportgroups_node_group_node_callback',
    event: 'mousedown',
    keypress: true,
    method: 'replaceWith',
    prevent: 'click',
    progress: {
      message: 'Submitting your post'
    },
    selector: '#'+button_id,
    submit: {
      _triggering_element_name: 'op',
      _triggering_element_value: 'Post'
    },
    url: '/system/ajax',
    wrapper: wrapper_id
  };
  return obj;
}

/**
 * Helper function to spoof an upload button for easy appending to the Drupal.ajax object
 * @param string id
 * @param string path
 **/
function _spoof_ajax_settings_object_for_upload_button(button_id, button_name, wrapper_id, path) {
  obj = {
    effect: 'fade',
    event: 'mousedown',
    keypress: true,
    prevent: 'click',
    progress: {
      message: null,
      type: 'throbber',
    },
    selector: '#'+button_id,
    submit: {
      _triggering_element_name: button_name,
      _triggering_element_value: 'Upload'
    },
    url: '/'+path,
    wrapper: wrapper_id
  };
  return obj;
}

/**
 * Helper function to spoof an remove button for easy appending to the Drupal.ajax object
 * @param string id
 * @param string path
 **/
function _spoof_ajax_settings_object_for_remove_button(button_name, wrapper_id, path) {
  obj = {
    effect: 'none',
    event: 'mousedown',
    keypress: true,
    prevent: 'click',
    progress: {
      message: null,
      type: 'throbber',
    },
    submit: {
      _triggering_element_name: button_name,
      _triggering_element_value: 'Remove'
    },
    url: '/'+path,
    wrapper: wrapper_id
  };
  return obj;
}