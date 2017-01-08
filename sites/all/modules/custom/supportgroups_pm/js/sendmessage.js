(function($){
  Drupal.behaviors.supportgroups_pm_sendmessage =  {
    attach: function(context, settings){
      // This function will show the private message form when clicked
      $(".sendbutton:not(.processed)").addClass('processed').click( function(){
        var uid_button =  $(this).attr( "uid" );
        
        // if we have the supporting UI data we should add messaging blocks
        if('object' == typeof(Drupal.settings.supportgroups_pm_supporting)) {
        	if(!Drupal.settings.supportgroups_pm_supporting.can_message) {
        		// insert message
        		$("#sendpm #response").html(Drupal.settings.supportgroups_pm_supporting.error_message);
        	} else {
                $("#sendform-"+uid_button).css("display", "block");
        	}
        } else { // allow the default behavior through
            $("#sendform-"+uid_button).css("display", "block");
        }        
        return false;
      });

      // This function will create the private message
      $(".sendmessage:not(.processed)").addClass('processed').click( function(){
        var uid_message =  $(this).attr( "uid" );
        var messagebody = $("#sendmessagebody-"+uid_message).val();

      var urlCreate = '/ajax/createnewmessages';
      $.ajax({  
        url: urlCreate,  
        type: 'POST',  
        data: {
      	  recipient: uid_message,
    	  discussion: messagebody,        
        },
        dataType: 'json',  
        cache: false,  
        success: function(data) {
          if(parseInt(data)!=0){
              // if post went through, then update the message list
              if( data.status == 'true'){
                  $("#sendmessagebody-"+uid_message).html('');
                  // Close the private message form
                  $("#sendform-"+uid_message).css("display", "none");
                  // Send a message to the user
                  $("#response").html(data.message);
                  $("#sendpm textarea").val('');
                  Drupal.attachBehaviors();
              }
              else if (data.status == 'false') {
                  $("#response").html(data.message);
              }
          }
        },  
        error: function() {  
        },  
        complete: function() {  
        }  
      });
      });         
    }
  };
}(jQuery));