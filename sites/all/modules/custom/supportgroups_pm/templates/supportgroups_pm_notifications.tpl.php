<?php // template for private message notifications
/**
*  Variables
*  $threadlist = All threads
*  Need to create a variable to post a message
*/
?>
<div id='notifications-pm' class='messagelist'>
  <div id='notifications-icon'>
    <span class='alert'>
    </span>
  </div>
  <div id="notifications-messagelist">
    <ul id="notifications-message">
      <?php print $threadlist; ?>
    </ul>
    <div class="see-all"><?php print $see_all; ?></div>
  </div>
</div>
  