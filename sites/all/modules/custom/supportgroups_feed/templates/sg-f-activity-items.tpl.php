<?php
// $Id$
/**
 * $user_picture
 * $date
 * @file
 * Template for listing of multiple activity items
 */
?>

<div id='activity-items-wrapper' class='activity-items-wrapper' time='<?php print time(); ?>'>
  <div id='recent-activity-wrapper'>
    <p><?php print $recent_activity_link; ?></p>
  </div>
  <div id='activity-items' class='activity-items'><?php print $items; ?></div>
</div>

<div class='past-activity-wrapper-container'>
  <div class='past-activity-wrapper'>
    <p><?php print $past_activity_link; ?></p>
  </div>
  <div class='waiting-icon'>
  </div>
</div>