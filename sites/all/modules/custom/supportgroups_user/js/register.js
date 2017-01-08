(function($){
  Drupal.behaviors.supportgroups_user_register =  {
    attach: function(context, settings){
    	
	  // Add Placeholders for password fields
	  //TODO Roll this back into the form alter
	  $("#edit-pass-pass1:not(.processed)").addClass('processed').attr( "placeholder", "Password" );
	  $("#edit-pass-pass2:not(.processed)").addClass('processed').attr( "placeholder", "Confirm Password" );

	  // Enable step-1 validations and pass through
	  $("#register-button:not(.processed)").addClass('processed').click( function(){
		// validate first page and move to next step if 
		if(_supportgroups_user_register_validate_account_fields()) {
			_supportgroups_user_register_switch_to_step(2);
		}
    	return false;
	  });
	  
	  // Enable step-2 validations and form submit
	  $("#user-register-form .form-submit[value='Save']:not(.processed)").addClass('processed').click( function(){
		if(!_supportgroups_user_register_validate_group_fields()) {			
		  return false;
	  	}
	  });
	  
	  // Enable close button on not-front pages
	  $("body.not-front #user-register-form .close-button:not(.processed)").addClass('processed').click(function(){
		  _supportgroups_user_register_hide_modal();
		 return false; 
	  });

      // if anonymous discussion or comment forms are available on not-front pages
      // add click event lister to popup register form
      $("body.not-front #anonymous-discussion-button:not(.processed)").addClass('processed').click(function(){
        // show register block
        _supportgroups_user_register_show_modal();  
    	  
    	// transfer values into register form
        $("#user-register-form input[name='node_body']").val($("#anonymous-discussion-body").val());
        $("#user-register-form input[name='node_gid']").val(Drupal.settings.supportgroups_user.register.gid);
        
        // prevent submitted
        return false;
      });

      // shows registration block if a user tries to join a group
      $("body.not-front .greenbutton a:not(.processed)").addClass('processed').click(function(){
        // show register block
        _supportgroups_user_register_show_modal();
        // strip all characters except numbers
        var group_url = $(this).attr('href');
        var group_id = group_url.replace(/[^0-9]/g, '');
        // transfer values into register form
        $("#edit-og-register-"+group_id).attr('checked', 'checked');
        return false;
      });

      
      // add listener to activity-item-comments-options-delete button
      $("body.not-front .anonymous-comment-button:not(.processed)").addClass('processed').click(function(){
    	// show register module
    	_supportgroups_user_register_show_modal();
    	  
        // get ids to pass
    	var nid = $(this).attr('nid');
        var pid = $(this).attr('pid');
        
        // transfer values into register form
        $("#user-register-form input[name='comment_body']").val($("#anonymous-comment-body-"+nid).val());
        $("#user-register-form input[name='comment_nid']").val(nid);
        $("#user-register-form input[name='comment_pid']").val(pid);
        
        // show register block
    	return false;
      });
      
      // attach jQuery validate plugin to form and construct rules
      $("#user-register-form").validate({
          onkeyup: false,
          onclick: false,
          onfocusout: false,
	      rules: {
	        "pass[pass1]": {
	          required: true
	        },
	        "pass[pass2]": {
	          required: true,
	          equalTo: "#edit-pass-pass2"
	        },
	        name: {
	        	required: true,
	        	noSpace: true,
	        	checkUsername: true,
	        },
	        mail: {
	        	required: true,
	        	email: true,
	        	checkEmail: true,
	        }
	      },
	      messages:{
	      	"pass[pass2]": {
	      		required: "Passwords do not match."
	      	}
	      }
      });
      
      //add no spaces validation method
      $.validator.addMethod("noSpace", function(value, element) { 
      		return value.indexOf(" ") < 0 && value != ""; 
      }, "No space please and don't leave it empty");
      
      $.validator.addMethod("checkUsername", function(value, element) {
      	// asume there is an error
      	var username_error = false;

      	$.ajax({
            url:'/supportgroups/user/check-username',
            data: {
          	username: value 
            },
            success:function(data){
          	  username_error = data;											
            },
            async:false
      	});
    	
    	  return username_error;
      }, "Username is not available.");
		
      $.validator.addMethod("checkEmail", function(value, element) {
      	// asume there is an error
      	var email_error = false;
    	
        $.ajax({
            url:'/supportgroups/user/check-email',
            data: {
              email: value
            },
            success:function(data){
            	email_error = data;
            },
            async:false
        });
        //this message appears only in case of user entering existing email
        return email_error;
      }, 'This email address is already in use.');

    } // attach: function(context, settings){
  }; // Drupal.behaviors.supportgroups_user_register =  {
  
  /**
   * Helper function to show register modal
   */
  function _supportgroups_user_register_show_modal() {
	  $("#block-supportgroups-user-supportgroups-register").show();
  };
  
  /**
   * Helper function to hide register modal
   */
  function _supportgroups_user_register_hide_modal() {
    $("#block-supportgroups-user-supportgroups-register").hide();
  };
  
  /**
   * Helper function validate step 1 fields
   */
  function _supportgroups_user_register_validate_account_fields() {
  	// assume everything is valid
  	validated = true;
	
  	$("#edit-account :input:not([type=hidden])").each(function(){
  		if(!validated) {
  		  $(this).valid();
  		} else {
  		  validated = $(this).valid();
  		}
  	});
	
  	//TODO jQuery.validate plugin equalTo method is not working so this is a quick fix
  	if($('#edit-pass-pass1').val() != $('#edit-pass-pass2').val()) {
  		validated = false;
		
  		// also insert the text to warn the user
  		if(!$('#user-register-form label.error[for="edit-pass-pass2"]').length) {
  		  $('#edit-pass-pass2').after('<label class="error" for="edit-pass-pass2">Passwords do not match.</label>');	
  		}
  	} else {
  		// remove the label if it exists on the page
  		$('#user-register-form label.error[for="edit-pass-pass2"]').remove();
  	}
		
	  return validated;
  };
  
  /**
   * Helper function validate step 1 fields
   */
  function _supportgroups_user_register_validate_group_fields() {
	  // assume everything is valid
  	validated = true;
  	if(!$('#edit-og-register--2 .form-checkbox:checked').length) {
  	  // set validated to false
  	  validated = false;
  	  // also insert the text to warn the user
  	  if(!$('#user-register-form .step-2 label.error[for="edit-submit"]').length) {
  		  $('#user-register-form .form-actions').before('<label class="error" for="edit-submit">You must select at least one group.</label>');	
  	  }
  	} else {
  	  // remove the label if it exists on the page
  	  $('#user-register-form .step-2 label.error[for="edit-submit"]').remove();
  	}
	
	  return validated;
  };
  
  /**
   * Helper function to switch steps
   */
  function _supportgroups_user_register_switch_to_step(step) {
  	switch(step) {
  	  case 2:
    		// hide all steps then shos the desired one
    		$('#user-register-form .step').hide('fast', 'swing', function(){
    		  $('#step-'+step).show();
    		});
  		break;
  	}
  };

  $(document).ready(function(){
    // Check to see if we are using IE 9 or lower
    if (/MSIE (\d+\.\d+);/.test(navigator.userAgent)){ //test for MSIE x.x;
      var ieversion=new Number(RegExp.$1) // capture x.x portion and store as a number
      if (ieversion<=9){
        $('[placeholder]').each(function(){
          var i = $(this);

          // add a value to the input field
          if( i.val() == ''  || i.val() == i.attr('placeholder')){
            if(i.attr('id') == 'edit-name' ){
              i.val('Username');
            } else if( i.attr('id') == 'edit-mail' ){
              i.val('Email');
            } else if( i.attr('id') == 'edit-pass-pass1' ){
              i.val('Password');
              i.addClass('password');
              this.type = 'text';
            } else if( i.attr('id') == 'edit-pass-pass2' ){
              i.val('Confirm Password');
              i.addClass('password');
              this.type = 'text';
            } else if(i.attr('id') == 'edit-name--2'){
              i.val('Username');
            } else if( i.attr('id') == 'edit-pass--2'){
              i.val('Password');
              i.addClass('password');
              this.type = 'text';
            }
            i.addClass('placeholder').val(i.attr('placeholder'));
          }
        })
        .focus(function(){
          var i = $(this);
          // remove value of the input field
          if( i.val() == i.attr('placeholder') ){
            if( i.hasClass('password')  ){
              i.removeClass('password');
              this.type='password';
            }
            i.val('').removeClass('placeholder');
          }
        })
        .blur(function(){
          var i = $(this);
          // add the placeholder text if the input field is empty
          if( i.val() == '' || i.val() == i.attr('placeholder')){
            if(this.type == 'password'){
              i.addClass('password');
              this.type='text';
            }
            i.addClass('placeholder').val(i.attr('placeholder'));
          }
        })
        .parents('form').submit(function(){
          $(this).find('[placeholder]').each(function(){
            var i = $(this);
            if( i.val() == i.attr('placeholder')){
              i.val('');
            }
          });
        });// end of parents
      } // end of IE check
    }// end of outer if statement
  });// Document ready


}(jQuery));