(function($){
  Drupal.behaviors.supportgroups_pm =  {
    attach: function(context, settings){
      // This function will replace the live feed block with the an updated version of it

      // This function is to create a message on a message list page
      $("#post-pm:not(.processed)").addClass("processed").click( function(){
      var messagebody = $("#messagebody").val();
      var urlCreate = '/ajax/createmessages';
      $.ajax({
        url: urlCreate,
        type: 'POST',
        data: {
      	  thread_id: Drupal.settings.supportgroups_pm.messagevalue,
    	  discussion: messagebody,
        },
        dataType: 'json',
        cache: false,
        success: function(data) {
          if(parseInt(data)!=0){
            // If a Json response is returned, run postHandler
              // if post went through, then update the message list
              if( data.status == 'true'){
                $('#messagepage').html(data.message_list);
                $("#messagebody").val('');                
                Drupal.attachBehaviors();
              }
          }
        },
        error: function() {
        },
        complete: function() {
        }
      });
      });



      $(".blockuser:not(.processed)").addClass("processed").click( function(){

        var uid =  $(this).attr( "uid" );
        var urlUser = '/ajax/actionblock/'+uid;
        $.ajax({  
          url: urlUser,  
          type: 'POST',  
          data: 'js=1',
          dataType: 'json',  
          cache: false,  
          success: function(data) {
            if(parseInt(data)!=0){
              // if post went through, then update the message list
              if( data.status){
                // will alert the user the status of the action
                $('#privatemessage-response').html(data.message);
                //$('#threadpage').html(data.thread_list);
                // Remove block container message
                $("#blockcontainer-"+uid).css('display', 'none');
                Drupal.attachBehaviors();
              }       
            }
          },  
          error: function() {  
          },  
          complete: function() {  
          }  
        });
        return false;
      });


      // Opens delete confirmation
      $(".blockuserbutton:not(.processed)").addClass('processed').click( function(){
        var uid = $(this).attr( "uid" );
        $("#blockcontainer-"+uid).css('display', 'block');
      });

      // Cancel delete confirmation
      $(".cancelblock:not(.processed)").addClass('processed').click( function(){
        var uid = $(this).attr( "uid" );
        $("#blockcontainer-"+uid).css('display', 'none');
      });



      $(".deletemessage:not(.processed)").addClass('processed').click( function(){

        var mid =  $(this).attr( "mid" );
        var urlDelete = '/ajax/deletemessages/'+mid+'/'+Drupal.settings.supportgroups_pm.messagevalue;
        $.ajax({
          url: urlDelete,
          type: 'POST',
          data: 'js=1',
          dataType: 'json',
          cache: false,
          success: function(data) {
            if(parseInt(data)!=0){
              // if post went through, then update the message list
              if( data.status){
                $('#messagepage').html(data.message_list);
                Drupal.attachBehaviors();
              }
            }
          },
          error: function() {
          },
          complete: function() {
          }
        });
        return false;
      });

    }
  };
}(jQuery));