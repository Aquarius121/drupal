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
//$row->nid;
	   $results = db_query('SELECT SUM(ncs.comment_count) as ccount FROM {node} n INNER JOIN {og_ancestry oga ON n.nid = oga.nid LEFT JOIN {node_comment_statistics} ncs ON n.nid = ncs.nid INNER JOIN {node} node_group_nid ON oga.group_nid = node_group_nid.nid INNER JOIN {users} u ON node_group_nid.uid = u.uid WHERE n.status = 1 AND oga.group_nid = %d GROUP BY oga.group_nid, node_group_nid.title, node_group_nid.uid, u.name', $row->nid);
  $row = db_fetch_object($results);
  $count = preg_replace( '/[^0-9]/', '' ,$output)+$row->ccount;
?>
<?php print "Posts: ".$count; ?>
