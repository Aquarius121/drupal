(function($){
  Drupal.behaviors.supportgroups_profile_edit =  {
    attach: function(context, settings){
      // Check the remove image checkbox when this id is clicked
      $("#removepicture:not(.processed)").addClass("processed").click(
        function(){
          $("#edit-picture-delete").attr('checked', 'checked');
          // Submit the edit form
          // This submit id might change, edit-submit--2
          // So be aware of this. We might have to find a better solution
          $("#edit-submit--2").trigger('click');
        }
      );
      // Changed Password default text
      $(".form-item-pass-pass1 label:not(.processed)").addClass("processed").html("New Password");

    }
  };
  
  $( document).ready( function(){
    // Change mailchimp submit button text
    $(".mailchimp-lists-user-subscribe-form .form-submit").val("Save Email Settings");  
  });

}(jQuery));