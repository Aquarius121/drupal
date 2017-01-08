<?php
  include('initialize.php');
  $cat_one = "";
  $cat_two = "";
  $cat_three = "";
  $cat_four = "";
  $cat_five = "";

  for($i = 1; $i <= QUESTION_MAX; $i++){
    if($questions[$i]['category'] == 1){
        $cat_one +=  $_SESSION['answers'][$i];
    }
    elseif($questions[$i]['category'] == 2){
        $cat_two +=  $_SESSION['answers'][$i];
    }    
    elseif($questions[$i]['category'] == 3){
        $cat_three +=  $_SESSION['answers'][$i];
    }
    elseif($questions[$i]['category'] == 4){
        $cat_four +=  $_SESSION['answers'][$i];
    }
    elseif($questions[$i]['category'] == 5){
        $cat_five +=  $_SESSION['answers'][$i];
    }
  }
  print "<div class='your-scores'><h2>Your scores for each category are:</h2>";
  print "<ul class='answers'><li><strong>Extroversion</strong>: ".$cat_one."</li>";
  print "<li><strong>Agreeableness</strong>: ".$cat_two."</li>";
  print "<li><strong>Conscientiousness</strong>: ".$cat_three."</li>";
  print "<li><strong>Emotional Stability</strong>: ".$cat_four."</li>";
  print "<li><strong>Imagination/Intellect</strong>: ".$cat_five."</li></ul></div>";
  
  

// Second Page - Don't include base url
for($i = 1; $i < QUESTION_MAX +1 ; $i++){
  if(!isset($_SESSION['answers'][$i])){
    print "<a href='/self-tests/personality-test/start?question=".$i."'> Please answer question ".$i." to get an accurate result! </a><br/>";
  }
}

?>

<div class="interpretation">
<h2>What the Scores Mean:</h2>
<p>Your score is broken down into five categories based on the Big Five traits which are broad dimensions used to describe personality. These categories are not definitive and are not intended to pigeonhole anyone into a "personality type". Please remember that these results are not scientific or definitive and are for your own entertainment.</p>
<dl>
<dt>Extroversion</dt>
<dd>
<em>High (41-50):</em> People who score high in this domain tend to be upbeat, energetic, assertive, and friendly. They tend to like people and have an optimistic outlook.
</dd>
<dd>
<em>Low (0-22):</em> People who score low in this domain tend to be reserved, formal and aloof. They tend to be independent rather than following the crowd.
</dd>
<dd>
<em>Middle (23-40):</em> People who score in the middle range tend to exhibit a balanced range of these characteristics.
</dd>

<dt>Agreeableness</dt>
<dd>
<em>High (45-50):</em> People who score high in this domain tend to be trusting, sympathetic and generous. They are often helpful and cooperative.
</dd>
<dd>
<em>Low (0-27):</em> People who score low in this domain tend to be assertive and competitive. They are often demanding, impatient and skeptical of others.
</dd>
<dd>
<em>Middle (28-44):</em> People who score in the middle range tend to exhibit a balanced range of these characteristics.
</dd>

<dt>Conscientiousness</dt>
<dd>
<em>High (41-50):</em> People who score high in this domain tend to have strong impulse control. They tend to be effective planners and high achievers. They may also tend towards being a workaholic and being overly fastidious.
</dd>

<dd>
<em>Low (0-24):</em> People who score low in this domain tend to be moody and spontaneous. They may be hasty and not think things through before acting.
</dd>

<dd>
<em>Middle (25-40):</em> People who score in the middle range tend to exhibit a balanced range of these characteristics.
</dd>

<dt>Emotional Stability</dt>
<dd>
<em>High (41-50):</em> People who score high in this domain tend to be emotionally stable. They tend to handle stressful situations calmly and adapt well to difficult situations.
</dd>

<dd>
<em>Low (0-20):</em> People who score low in this domain tend to exhibit emotional distress. They tend to be easily rattled and have trouble coping with stress.
</dd>

<dd>
<em>Middle (21-40):</em> People who score in the middle range tend to exhibit a balanced range of these characteristics.
</dd>

<dt>Imagination/Intellect</dt>
<dd>
<em>High (44-50):</em> People who score high in this domain tend to be curious about the world and emotionally responsive to aesthetics. They tend to be open to a wide range of ideas and opinions.
</dd>

<dd>
<em>Low (0-28):</em> People who score low in this domain tend to be more conservative and prefer familiar ideas and surroundings. They tend to have a more cautious outlook.
</dd>

<dd>
<em>Middle (29-43):</em> People who score in the middle range tend to exhibit a balanced range of these characteristics.
</dd>
</dl>
</div>
