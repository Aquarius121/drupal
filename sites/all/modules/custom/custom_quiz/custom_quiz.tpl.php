<?php

  include('initialize.php');

 if(!isset($_GET['question'])){
   $question_number = 1;
 }
 else{
 $question_number = $_GET['question'];
 }
    if($question_number <= QUESTION_MAX){
      $next_question = $question_number + 1;
    }
    else{
        $next_question = "results";
    }
 
?>

<p><em>&nbsp;&nbsp;<?php print $question_number;  ?> of  <?php print QUESTION_MAX;?> </em></p>
<?php //Second page url before ?question ?>
<form class="selftest" method="post" action="/self-tests/personality-test/start?question=<? print $next_question; ?>">
    <h3> <?php print $questions[$question_number]['title']; ?></h3>
    <label>Very Inaccurate</label>                 <input name="choice" type="radio" value="<?php if($questions[$question_number]['type'] == '+'){print '1';}elseif($questions[$question_number]['type'] == '-'){print '5';} ?>" <?php if($questions[$question_number]['type'] == '+'){if($_SESSION['answers'][$question_number] == "1"){print "CHECKED";}}elseif($questions[$question_number]['type'] == '-'){if($_SESSION['answers'][$question_number] == "5"){print "CHECKED";}} ?> /><br/>
    <label>Moderately Inaccurate</label>           <input name="choice" type="radio" value="<?php if($questions[$question_number]['type'] == '+'){print '2';}elseif($questions[$question_number]['type'] == '-'){print '4';} ?>"  <?php if($questions[$question_number]['type'] == '+'){if($_SESSION['answers'][$question_number] == "2"){print "CHECKED";}}elseif($questions[$question_number]['type'] == '-'){if($_SESSION['answers'][$question_number] == "4"){print "CHECKED";}} ?>/><br/>
    <label>Neither Accurate nor Inaccurate</label> <input name="choice" type="radio" value="3" <?php if($_SESSION['answers'][$question_number] == "3"){print "CHECKED";}  ?>/><br/>
    <label>Moderately Accurate</label>             <input name="choice" type="radio" value="<?php if($questions[$question_number]['type'] == '+'){print '4';}elseif($questions[$question_number]['type'] == '-'){print '2';} ?>" <?php if($questions[$question_number]['type'] == '+'){if($_SESSION['answers'][$question_number] == "4"){print "CHECKED";}}elseif($questions[$question_number]['type'] == '-'){if($_SESSION['answers'][$question_number] == "2"){print "CHECKED";}} ?> /><br/>
    <label>Very Accurate</label>                   <input name="choice" type="radio" value="<?php if($questions[$question_number]['type'] == '+'){print '5';}elseif($questions[$question_number]['type'] == '-'){print '1';} ?>" <?php if($questions[$question_number]['type'] == '+'){if($_SESSION['answers'][$question_number] == "5"){print "CHECKED";}}elseif($questions[$question_number]['type'] == '-'){if($_SESSION['answers'][$question_number] == "1"){print "CHECKED";}} ?> /><br/>
    <input type="submit" name="submit" value="Next"/>
</form>


