<?php
/**
*  Variables
*  $uid 
* $block_text
*/
?>
<span id='blockuserbutton-<?php print $uid; ?>' class='blockuserbutton orangebutton' title='Block this member' uid='<?php print $uid; ?>'><?php print $block_text; ?></span>
<?php
// This is the form for the private message confirmation
?>
<div id='blockcontainer-<?php print $uid; ?>' class='blockcontainer'>
  <p>Are you sure you want to block this member from sending you any more messages?</p>
  <span id='blockuser-<?php print $uid; ?>' class='blockuser orangebutton popup' uid='<?php print $uid; ?>'>Block</span>
  <span id='cancelblock-<?php print $uid; ?>' class='cancelblock graybutton' uid='<?php print $uid; ?>'>Cancel</span>
</div>