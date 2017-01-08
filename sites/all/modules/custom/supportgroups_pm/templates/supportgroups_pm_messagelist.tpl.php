<?php // template for private message page - individual message
/**
*  Variables
*  $message_list = All threads
*  Need to create a variable to post a message
* $blockuser
* $recipients
*/
?>
<h1> Conversation with <?php print $recipients; ?></h1>
<div id='messagelist' class='messagelist'>
    <a class="button gray" href="/privatemessages">&#8656; All Messages</a>
  <div id='privatemessage-response'></div>
    <div class="block-user ind"><?php print $blockuser; ?></div>
		<ul id='messagepage'>
			<?php print $message_list; ?>
		</ul>
	</div>
<form action='' method='POST'>
    <textarea id='messagebody' placeholder='Enter your message here'></textarea>
    <input id='post-pm' type='button' value='Submit Message'/>
</form>

