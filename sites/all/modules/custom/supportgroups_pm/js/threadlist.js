(function($){
  Drupal.behaviors.thread_list =  {
    attach: function(context, settings){

      // show new message form
      $("#createpm:not(.processed)").addClass('processed').click(function(){
        $("#privatemessage-form").css('display', 'block');
      });
      
      // Show the user options when a thread is hovered
      // Remove the user options when you leave the thread
      $(".threadrow:not(.processed)").addClass('processed').hover( 
        function(){
          var tid = $(this).attr("thread_id");
          $("#threadoptions-"+tid).css('display', 'block');
        },
        function(){
          var tid = $(this).attr("thread_id");
          $("#threadoptions-"+tid).css('display', 'none');
        }      
      );

      // Opens delete confirmation
      $(".deletethreadbutton:not(.processed)").addClass('processed').click( function(){
        var thread_id = $(this).attr( "thread_id" );
        $("#deletecontainer-"+thread_id).css('display', 'block');
      });

      // Cancel delete confirmation
      $(".canceldelete:not(.processed)").addClass('processed').click( function(){
        var thread_id = $(this).attr( "thread_id" );
        $("#deletecontainer-"+thread_id).css('display', 'none');
      });


      // Delete thread
      // Need to create a confirmation popup
      $(".deletethread:not(.processed)").addClass('processed').click( function(){
        var thread_id =  $(this).attr( "thread_id" );
        var urlDelete = '/ajax/deletethread/'+thread_id;
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
                $('#threadpage').html(data.thread_list);
                // Not working
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



      // Block a user
      // Need to create a confirmation popup
      $(".blockuser:not(.processed)").addClass('processed').click( function(){
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
                //$('#threadpage').html(data.thread_list);
                // Add response text    
                $("#privatemessage-response").html(data.message);
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
      // Mark thread as unread
      $(".markthread:not(.processed)").addClass('processed').click( function(){
        var thread_id =  $(this).attr( "thread_id" );
        var urlMark = '/ajax/actionmark/'+thread_id;
        $.ajax({  
          url: urlMark,  
          type: 'POST',  
          data: 'js=1',
          dataType: 'json',  
          cache: false,  
          success: function(data) {
            if(parseInt(data)!=0){
              // if post went through, then update the message list
              if( data.status){
                $('#threadpage').html(data.thread_list);
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
    
    
  };  // End of drupal behavior thread list
  
  
  // Add custom clear command for discussion post form
  Drupal.ajax.prototype.commands.supportgroups_pm_clear = function(ajax, response, status) {
    selector = jQuery(response.data.selector);
    
    console.log("SELECTOR IS: "+response.data.selector);    
    if( response.data.status == 'true' ){
      // clear the values
      $('input[type=text], input[type=file], textarea, select', selector).val('');
      // update thread
      $('#threadpage').html(response.data.thread_list);
      // Hide form
      $("#privatemessage-form").css('display','none');
    }
    // Add response text    
    $("#privatemessage-response").html(response.data.message);
  };

}(jQuery));