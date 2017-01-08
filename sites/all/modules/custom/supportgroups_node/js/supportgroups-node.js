(function($){
  Drupal.behaviors.supportgroups_node_count =  {
    attach: function (context, settings) {

    	  // check for interval already running to avoid attachBehaviors
    	  if(!Drupal.settings.supportgroups_node_count.nodecount.running) {
    	    // set the recent interval id to running
            Drupal.settings.supportgroups_node_count.nodecount.running = 1;
            // If the discussion count is less than 10, then retrieve more content
            if( Drupal.settings.supportgroups_node_count.nodecount.discussion_count < 10){
      		  _get_past_activity();
      		}
    	  }  
    	     
    } // close attach: function (context, settings)
  }; // close Drupal.behaviors.supportgroups_node_count 
    

  
}(jQuery));

