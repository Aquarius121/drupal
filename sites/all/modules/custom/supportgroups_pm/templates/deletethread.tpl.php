<?php
/**
*  Variables
*  $thread_id 
*/
?>
<span id='deletethreadbutton-<?php print $thread_id; ?>' class='deletethreadbutton' title='Delete' thread_id='<?php print $thread_id; ?>'>X</span>
<?php
// This is the form for the private message confirmation
?>
<div id='deletecontainer-<?php print $thread_id; ?>' class='deletecontainer'>
  <p>Are you sure you want to delete this message?</p>
  <span id='deletethread-<?php print $thread_id; ?>' class='deletethread orangebutton' thread_id='<?php print $thread_id; ?>'>Delete</span>
  <span id='canceldelete-<?php print $thread_id; ?>' class='canceldelete graybutton' thread_id='<?php print $thread_id; ?>'>Cancel</span>
</div>