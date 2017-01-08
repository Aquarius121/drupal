<?php
/**
* $uid
*/
?>
<div id="sendpm">
    <div id='response'></div>
    <a href='' class='sendbutton button gray' uid='<?php print $uid; ?>'>Message</a>
    <form id='sendform-<?php print $uid; ?>' class='sendformclass' action='' method='post'>
        <textarea id='sendmessagebody-<?php print $uid; ?>' placeholder='Enter your message here'></textarea>
        <input id='sendmessage-<?php print $uid; ?>' class='sendmessage' type='button' uid='<?php print $uid; ?>' value='Send'/>
    </form>
</div>