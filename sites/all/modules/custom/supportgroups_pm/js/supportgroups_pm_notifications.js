(function($){
  Drupal.behaviors.supportgroups_pm_notification =  {
    attach: function(context, settings){
      // This function will replace the live feed block with the an updated version of it
    var pm_notification = 1;
    
    // If the icon is clicked, then we will either show or hide the message list 
    $("#notifications-icon:not(.processed)").addClass("processed").click( function(){
      if( pm_notification == 1 ){
        $("#notifications-messagelist").css('display','block');  
        pm_notification  = 0;
        // recreate cookie with new timestamp
        recreate_cookie("__private_message_timestamp");
        // remove pm count                
        $("#notifications-icon").html('');
      }
      else if( pm_notification == 0 ){
        $("#notifications-messagelist").css('display','none');        
        pm_notification  = 1;
      }
    });

    /** TODO WRAP this is a function */
    // check to see if cookie was created
    // if not, then we have to create the cookie
    var private_message_timestamp = get_cookie("__private_message_timestamp");
    if( private_message_timestamp != null & private_message_timestamp != '' ){
    }
    else{
      // create cookie on page load if it doesn't exist
      set_cookie("__private_message_timestamp");
    }
    
    update_pm_count();

    if(!Drupal.settings.supportgroups_pm_notification.loadmore.running) {

      // check to see if there are new notifications
      Drupal.settings.supportgroups_pm_notification.loadmore.running = setInterval( function(){
      var urlNotifications = '/ajax/loadmorenotifications';

      $.ajax({  
        url: urlNotifications,  
        type: 'GET',  
        data: {
        	js: 1
        },
        dataType: 'json',  
        cache: false,  
        success: function(data) {
          if(parseInt(data)!=0){
            if( data.status == 'true' ){
               // update message thread
               $("#notifications-message").html(data.thread_list);
               // update pm count
               update_pm_count();
            }
            Drupal.attachBehaviors();
          }
        },  
        error: function() {  
        },  
        complete: function() {  
        }  
      });    
      }, Drupal.settings.supportgroups_feed.notifications.interval);

    }// end of Drupal.settings.supportgroups_pm_notification.loadmore.running
    
    setTimeout('clearInterval(Drupal.settings.supportgroups_pm_notification.loadmore.running)', Drupal.settings.supportgroups_feed.notifications.interval);
             
    }
  };
}(jQuery));

// Update the pm count 
function update_pm_count(){    
    var pm_count = 0;
  // Loop through each message list and count the messages that is greater than
  private_message_timestamp = get_cookie("__private_message_timestamp");
  if( private_message_timestamp != null & private_message_timestamp != '' ){
  }
  else{
    // if cookie is not set, then create the cookie and create the timestamp
    // create cookie on page load if it doesn't exist
    set_cookie("__private_message_timestamp");
    private_message_timestamp = Math.round(new Date().getTime() / 1000) ; 
  }
  // the current timestamp        
  jQuery("#notifications-message li").each(function(index){
    if( private_message_timestamp < jQuery(this).attr('timestamp') ){      
      pm_count++;      
    }
    else{
  
    }
  });
  if( pm_count == 0 ){
    jQuery("#notifications-icon .alert").html('');
  }
  else{
    jQuery("#notifications-icon .alert").html(pm_count);    
  }
}