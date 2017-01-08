<?php
  include('initialize.php');
?>
<div id="quiz-start-page">
<div class="quiz-description">
<p><strong>The following 50-item self test presents statements that describe behaviors. Read each statement, then select the item on the rating scale that most accurately describes you as you are now. Try to be as honest as possible.</strong></p>

<p class="acknowledge">This questionnaire is an adaptation of the IPIP Big Five Factor Markers. It is presented solely for entertainment purposees and cannot be considered as or substituted for a diagnosis of any description. Your responses will not be stored, shared, or otherwise reproduced or distributed.</p>

<p class="acknowledge">By clicking on "Take The Quiz" I acknowledge that this self test is for personal use only and cannot be used for any professional or diagnostic purpose whatsoever.</p>

</div>

<?php if(($check == 'yes') && ($_SESSION['answers'][1] > 0)): ?>
    <h3 class="startover">It appears that you have a session in progress. Would you like to start over?</h3>
    <br/>
<?php // second page url - don't include the base url ?>
    <form action='/self-tests/personality-test/start' method='post'>
    <input type='submit' value='Start Over' name='startover' class="startover"/>
    </form>
    <br/>

<?php
// second page url - don't include the base url - There are two places to change this in the for loop
for($i = 2; $i < QUESTION_MAX +1 ; $i++){
  if(!isset($_SESSION['answers'][$i])){
    print "<span class='continue'><a href='/self-tests/personality-test/start?question=".$i."'> Continue to question ".$i." &raquo;</a></span>";
    break;
  }
 if($i == QUESTION_MAX){
   print "<span class='continue'><a href='/self-tests/personality-test/start?question=results'> Continue to your results! </a></span>";
 }
}
?>
<?php else: ?>
<?php
// second page url - don't include the base url
?>
<span class="start-quiz"><a href="/self-tests/personality-test/start">Take the quiz</a></span>
<?php endif; ?>
</div>
