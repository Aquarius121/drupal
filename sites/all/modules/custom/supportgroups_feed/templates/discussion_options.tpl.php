<?php
/*
 *  Template for discussion options
 * Variables
 * $nid
 */
?>
<span id='activity-item-options-delete-<?php print $nid; ?>' nid='<?php print $nid ?>' class='activity-item-options-delete' title="Delete this post">
  &nbsp;
</span>
<div id='deletediscussioncontainer-<?php print $nid; ?>' class='deletediscussioncontainer'>
  <p>Are you sure you want to delete this discussion?</p>
  <span id='activity-item-options-delete-confirm-<?php print $nid; ?>' nid='<?php print $nid ?>' class='activity-item-options-delete-confirm orangebutton' title="Delete this post">Delete</span>
  <span id='activity-item-options-delete-cancel-<?php print $nid; ?>' nid='<?php print $nid ?>' class='activity-item-options-delete-cancel graybutton' title="Delete this post">Cancel</span>
</div>
<span id='activity-item-options-edit-<?php print $nid; ?>' nid='<?php print $nid ?>' class='activity-item-options-edit' title="Edit this post">&nbsp;</span>
