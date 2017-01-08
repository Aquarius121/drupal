(function($){
  Drupal.behaviors.supportgroups_node =  {
    attach: function (context, settings) {
      // setup the form selector for all the calls
      var selector = Drupal.settings.supportgroups_node.group_post.selector;
      var selector_class = '.'+selector;
      
      // add click to custom upload icon (camera)
      $(".group-post-image-button:not(.processed)", selector_class).addClass("processed").click(function(){
        $(this).parent().parent().find('.field-name-field-discussion-image .form-file').trigger('click');
        return false;
      });
      
      // add mousedown event auto upload image on file change
      $(".form-file:not(.processed)", selector_class).addClass("processed").change(function(){
        $(this).siblings('.form-submit[value=Upload]').mousedown();
        $(this).closest('.field-name-field-discussion-image').show();
        return false;
      });
      
      // if a remove button is on the page add a click event to hide removed image wrapper
      // this hides the returned upload button
      $(".form-submit[value=Remove]:not(.processed)", selector_class).addClass("processed").mousedown(function(){
    	  $(this).closest('.field-name-field-discussion-image').hide();
      });
      
      // on textarea foucs enable the editing bar
      $("textarea:not(.processed)", selector_class).addClass("processed").focus(function(){
        form = $(this).closest('form');
      
        // keep group selection hidden when we are on a group_post node page
        if(!$('body').hasClass('disable-group-selection')) {
            $(".groups-selection", form).css('display','block');
        }
        
        $(".group-post-image-button", form).css('display','block');
        $(".form-submit", form).css('display','block');
        $(".post-functions", form).css('display','block');
        $(".group-post-node-form").addClass('open');
        return false;
      });
            
    } // close attach: function (context, settings) {
  }; // close Drupal.behaviors.supportgroups_node =  {


  if('undefined' == typeof(Drupal.ajax) ) {
    console.log("Drupal AJAX undefined");
  } else{
  // Add custom clear command for discussion post form
  Drupal.ajax.prototype.commands.supportgroups_node_reload_form = function(ajax, response, status) {
    // remove all messages
    setTimeout(function(){$('.messages').remove();}, 3000);
    $('.error').remove();

    var selector = jQuery('#'+jQuery(ajax.selector).closest('form').attr('id'));
    var wrapper = selector.parent();
    
    // place the form on the page
    wrapper.html(response.data.markup);
    
    // need to reselect the new DOM element
    selector = jQuery('#'+response.data.selector);
    
    // count the number of current buttons
    //TODO need to update method of counting objects as it is not supported in IE8
    var num_of_buttons = Object.keys(Drupal.settings.ajax).length
    
    // add non ajax forms
    num_of_buttons = num_of_buttons + _count_non_ajax_forms();

    // we need to take into account multiple submit buttons on the node
    //discussion form
    submit_buttons = jQuery('.form-submit', selector);
    new_ids = [];
    old_ids = [];
    
    submit_buttons.each(function(x){
      
      id = jQuery(this).attr('id');
      
      // create new id based on what needs to happen
       new_id = id + '--' + (num_of_buttons+(1+x));
       
       // record new ids to loop again velow
       new_ids.push(new_id);
       
       // record old id to mimic the indexes in order to clone previous object
       old_ids.push(id);
       
       // replace the id in the clone
       jQuery('#'+id, selector).attr('id', new_id);
      
    });
    
    // loop the ids to get their elements and load them onto the ajax settings
    jQuery.each(new_ids, function(i, value){
      // get discussion object for Drupal.settings.ajax; we still need to override the element
      discussion_obj = _spoof_ajax_settings_object_for_discussion(value, old_ids[i]);
      
      // discussion form needs an element attached
      discussion_obj.element = jQuery('#'+value);

      // we need to update the Drupal.settings.ajax object with the new form
      Drupal.settings.ajax[value] = discussion_obj;
    });
     
    // load new activity supportfeed.js and subsequently call Drupal.attachBehaviors()
    _get_recent_activity();
  };
  }// end of outer else statement

  if('undefined' == typeof(Drupal.ajax) ) {     
  } else{
  // Add custom clear command for inline editing of discussions
  Drupal.ajax.prototype.commands.supportgroups_node_replace = function(ajax, response, status) {
    // remove all messages
    setTimeout(function(){$('.messages').remove();}, 3000);
    $('.error').remove();
    
    $('#'+response.data.selector).replaceWith(response.data.markup);
  };
  }// end of else object statement
}(jQuery));