<?php // template for private message page
/**
*  Variables
*  $thread_list = All threads
*  Need to create a variable to post a message
*/
?>
<div id='threadlist'>
  <div id='privatemessage-response'></div>
  <span class="greenbutton" id='createpm'>+ New Message</span>
  <span id='privatemessage-form'><?php print render($privatemessage_form); ?></span>
  <ul  id='threadpage'>
    <?php print $thread_list; ?>
  </ul>
</div>
  