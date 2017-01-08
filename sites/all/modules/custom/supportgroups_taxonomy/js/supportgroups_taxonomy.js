(function($){
  Drupal.behaviors.supportgroups_taxonomy =  {
    attach: function (context, settings) {
  
      $("#group-taxonomy:not(.processed)").addClass('processed').click(function(){
        var nid = $(this).attr('nid');      
        var urlCreate = '/ajax/joingroup/'+nid;
        // add load icon
        jQuery("#group-taxonomy").addClass('load-icon');
        $.ajax({  
          url: urlCreate,  
          type: 'POST',  
          data: 'js=1',
          dataType: 'json',  
          cache: false,  
          success: function(data) {
            if(parseInt(data)!=0){
              // if post went through, then update the message list
              if( data.status == 'true'){
                // Get group name before it is replaced                
                var group_name = $("#group-taxonomy-join").text();
                // replace group block
                $("#group-taxonomy").html(data.content);
                var group_url = "<a href='"+data.url+"'>Go to "+group_name+"</a>";  
                $("#group-taxonomy-response").html(group_url);              
                Drupal.attachBehaviors();                
              }       
            }
          },  
          error: function() {  
          },  
          complete: function() {  
            // add load icon
            jQuery("#group-taxonomy").removeClass('load-icon');

          }  
        });

      });
      
      
    } // close attach: function (context, settings) {
  }; // close Drupal.behaviors.supportgroups_node =  {
    
    
}(jQuery));