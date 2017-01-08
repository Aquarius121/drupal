(function($){
  Drupal.behaviors.supportgroups_live =  {
    attach: function(context, settings){
      // This function will replace the live feed block with the an updated version of it
      var postHandler = function(postsJSON) {  
        $.each(postsJSON,function(i,post) {  
          $('#block-supportgroups-live-supportgroups-live').html(post[0]);
        });   
      }; 
      
      // path that ajax will all to
      var urlLink = '/ajax/loadmoreactivity';


    if(!Drupal.settings.supportgroups_live.loadmore.running) {


      // update the block every 30 seconds
      Drupal.settings.supportgroups_live.loadmore.running = setInterval( function(){

      $.ajax({  
        url: urlLink,  
        type: 'POST',  
        data: 'js=1',
        dataType: 'json',  
        cache: false,  
        success: function(responseJSON) {
          if(parseInt(responseJSON)!=0){
            // If a Json response is returned, run postHandler
            postHandler(responseJSON);
          }
          Drupal.attachBehaviors();
        },  
        error: function() {  
        },  
        complete: function() {  
        }  
      });    
      }, 30000);

      }// End of Drupal.settings.supportgroups_live.loadmore.running
       
    }
  };
}(jQuery));