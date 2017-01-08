(function($){

  Drupal.behaviors.supportgroups_node_user_groups =  {
    attach: function (context, settings) {

      var settings = Drupal.settings.supportgroups_node.node;
      var html = '';
      // Change each join button to a leave button
      for (x in settings) {
        console.log(settings[x]);
        html = '<a href="/group/node/' + settings[x] + '/unsubscribe/og_group_ref" title="Leave" class="group unsubscribe">Leave</a>';
        $('.views-field-nothing .gid-' + settings[x]).html(html);
      }
    } // close attach: function (context, settings)
  }; // close Drupal.behaviors.supportgroups_node_user_groups


}(jQuery));

