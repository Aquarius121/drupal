(function($){

  // we initiate this here because we do not want to reset it with each
  var activity_notification_counter = 0;

  Drupal.behaviors.supportgroups_feed_notifications =  {
    attach: function (context, settings) {
      if('object' == typeof(Drupal.settings.supportgroups_feed.notifications)) {        
        // set timeout to query for recent activity items
        // check for the interval having been set to avoid creating multiple internvals on attachBehaviors

//		    if(!Drupal.settings.supportgroups_feed.notifications.running) {
//		      // set the interval id to running
//		      Drupal.settings.supportgroups_feed.notifications.running = setInterval('_get_recent_activity_notifications()', Drupal.settings.supportgroups_feed.notifications.interval);
//
//		      // call the function manually for the first time
//		      _get_recent_activity_notifications(0));
//		    }
    	  

        var notification_list = $("#activity-item-list ul");
    		// Clicking on the icon will do any of the following:
        // First time, notifications will load
        // If notifications already loaded, then will hide notification list
        // If notifications loaded and hidden, then will show notification list
        $("#activity-notifications-icon:not(.processed)").addClass('processed').click(function(){

    		  if( activity_notification_counter == 0 || Drupal.settings.supportgroups_feed.notifications.unseen.notifications_unseen > 0){
            // Clear any current items and get the list
            $('#activity-item-list ul li:not(#no-notifications)').remove();
            _get_recent_activity_notifications(0);
    			  activity_notification_counter = 1;
            $('#activity-item-list').show();
    			  // remove activity count
    			  $("#activity-notifications-count").html('');

    		  } else {
            console.log($("#activity-item-list").is(':visible'));
            if ($("#activity-item-list").is(':visible')) {
              $('#activity-item-list').hide();
            }
    			  else {
              $('#activity-item-list').show();
            }
    			  //activity_notification_counter = 0;

    		  }
    		});

        $('#more-notifications').click(function() {
          // Get last timestamp
          var timestamp = $('#activity-item-list li').last().attr('timestamp');
          _get_recent_activity_notifications(timestamp);
        });

        update_activity_count();

        console.log('notif counter ' + activity_notification_counter);

        $(document).click(function(e) {
          // check that your clicked
          // element has no id=info
          // && !$('#activity-item-list').find(e.target).length
          if (e.target.id != 'activity-notifications-icon' && e.target.className != 'notification-item'
            && e.target.className != 'notification-helper') {
            $("#activity-item-list").hide();
          }

        });
      } 
    } // close attach: function (context, settings) {
  }; // close Drupal.behaviors.supportgroups_feed_notifications =  {


  function _get_recent_activity_notifications(timestamp) {
    var wrapper = $('#'+Drupal.settings.supportgroups_feed.notifications.wrapper);

    // get the time and set the new time to advance the query
    if (timestamp > 0) {
      current_time = timestamp;
    }
    else {
      current_time = ($.now() / 1000).toFixed(0);
    }
    $.ajax({
      url: '/ajax/sg_f/notifications',
      data: {
        max: Drupal.settings.supportgroups_feed.notifications.max,
        since: current_time,
        id: Drupal.settings.supportgroups_feed.notifications.id
      },
      beforeSend: function(xhr) {
        $("#activity-item-list").addClass('loading');
      },
      success: function(data) {
        var activity_list  = $('#activity-item-list');

        if('object' == typeof(data.themed_output)) {
          if(Object.keys(data.themed_output).length > 0) {
            // empty the store
            $('#activity-item-store ul').empty();
            // if the no notifications list is there then rmeove it.
            if($('li#no-notifications', activity_list).length) {
              $('li#no-notifications', activity_list).remove();
            }

            // remove any more list items sitting around
            $('#more-notifications').remove();
            var list = _sortObject(data.themed_output);

            // prepend the return content to the store
            for(row in list) {
              $('#activity-item-store ul').prepend(list[row]);
            }
            // append the content to the list
            $('#activity-item-list ul').append($('#activity-item-store ul').html());
            // empty the store
            $('#activity-item-store ul').empty();
            $("#activity-notifications-count").html('');
            // We clear the count settings here and redo the count
            Drupal.settings.supportgroups_feed.notifications.unseen.notifications_unseen = 0;
            console.log('getting new notifications');
            update_activity_count();
          }
          else {
            $('li#no-notifications').css('color', '#666').show();
          }
        }

        // remove loading class
        $("#activity-item-list").removeClass('loading');

        // add event behaviors to new markup
        Drupal.attachBehaviors();
      }
    });
  }

  function _sortObject(o) {
    var sorted = {},
      key, a = [];

    for (key in o) {
      if (o.hasOwnProperty(key)) {
        a.unshift(key);
      }
    }

    // sort the array to rebuild
    // reverse the array so most recent is at the top
    a.sort().reverse();

    for (key = 0; key < a.length; key++) {
      sorted[a[key]] = o[a[key]];
    }

    return sorted;
  }

  // Update the activity count
  function update_activity_count(){

    $.ajax({
      url: '/ajax/sg_f/notifications_unseen',
      data: {
        id: Drupal.settings.supportgroups_feed.notifications.id
      },
      success: function(data) {
        if(data.notifications_unseen > 0) {
          Drupal.settings.supportgroups_feed.notifications.unseen.notifications_unseen = data.notifications_unseen;
          // If there are new notifications we want them to load
          activity_notification_counter = 0;
          $("#activity-notifications-icon").removeClass('processed');
        }
      }
    });
    var activity_count = Drupal.settings.supportgroups_feed.notifications.unseen.notifications_unseen;
    // Remove the notification icon if the count is 0
    // Else set the new count
    if( activity_count == 0 ){
      $("#activity-notifications-count").html('');
    }
    else{
      $("#activity-notifications-count").html(activity_count);
    }
  }

  function set_count_cookie(count) {
    var exdate=new Date();
    exdate.setDate(exdate.getDate() + 1);
    document.cookie="__unseen_count=" + count+";expires="+exdate.toUTCString()+";domain=.supportgroups.com;path=/";
  }

}(jQuery));

