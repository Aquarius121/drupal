<?php
 /**
  * This template is used to print a single field in a view. It is not
  * actually used in default Views, as this is registered as a theme
  * function which has better performance. For single overrides, the
  * template is perfectly okay.
  *
  * Variables available:
  * - $view: The view object
  * - $field: The field handler object that can process the input
  * - $row: The raw SQL result that can be used
  * - $output: The processed output that will normally be used.
  *
  * When fetching output from the $row, copy and paste the following snippet:
  * $data = $row->{$field->field_alias}
  *
  * The above will guarantee that you'll always get the correct data,
  * regardless of any changes in the aliasing that might happen if
  * the view is modified.
  */
?>
<?php
  global $user;
  $total;
  // Get the total member count
  $sql = "select count(etid) as user FROM {og_membership} og_membership INNER JOIN {users} users ON og_membership.etid = users.uid WHERE entity_type = 'user' AND state=1 AND status=1 AND gid= ".$row->nid;
  $result = db_query($sql);
  foreach( $result AS $row){
    $total = $row->user;
  }
  // Show group post if user is logged in
  if($user->uid > 0){
    print  $total." Members";
  } else{
    //Anonymous user. ONly show count if greater than 1000
    if( $total > 1000){
      print $total." Members Strong and Growing!";
    }
  }
?>
