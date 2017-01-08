(function($){

  Drupal.behaviors.supportgroups_flag_unflag =  {
    attach: function (context, settings) {

      // This function decrements unseen when an item is unflagged
      $('.flag-like .flagged, .flag-like-comments .flagged').click( function() {

        $.ajax({
          url: '/ajax/sg_f/unflag',
          data: {
            uid: $(this).attr('uid'),
            timestamp: $(this).attr('timestamp')
          }
        });

      });

    } // close attach: function (context, settings) {

  }; // close Drupal.behaviors.supportgroups_flag_unflag =  {
}(jQuery));
