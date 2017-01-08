<?php

/**
 * @file
 *   Function for querying the database
 */

function supportgroups_feed_query_latest_comment_notifications_per_user() {
  $query = 'SELECT
              n.nid,
              n.uid as node_uid,
              n.title,
              IF (c.pid, c.pid, c.cid) as cid,
              IF (c.pid, 1, 0) as reply,
              IF (c.pid, c2.subject, c.subject) as comment_subject,
              c.uid as uid,
              IF (c.pid, c2.uid, 0) as orig_comment_uid,
              (1000 * c.changed) + FLOOR(RAND() * 1000) as timestamp,
              0 as "like",
              1 as comment
            FROM {node} n
            INNER JOIN {comment} c ON c.nid = n.nid
            LEFT JOIN {comment} c2 ON c.pid = c2.cid
            WHERE
              n.type IN (:types)
              AND n.status
              AND (n.uid=:uid OR c2.uid=:uid)
            ORDER BY timestamp DESC
            LIMIT :limit';
  return $query;
}

function supportgroups_feed_query_latest_node_like_notifications_per_user() {
  $query = 'SELECT
                n.nid,
                n.uid AS node_uid,
                n.title,
                0 as cid,
                NULL as comment_subject,
                0 as orig_comment_uid,
                0 as reply,
                fc.uid as uid,
                (1000 * fc.timestamp) + FLOOR(RAND() * 1000) as timestamp,
                1 AS "like",
                0 AS comment
              FROM {flag_content} fc
              INNER JOIN {flags} f ON fc.fid = f.fid
              INNER JOIN {node} n ON fc.content_id = n.nid
              WHERE
                n.type IN (:types)
                AND n.status
                AND fc.timestamp > :time_limit
                AND f.name = :flag_name
                AND n.uid=:uid';
  return $query;
}

function supportgroups_feed_query_latest_comment_like_notifications_per_user() {
  $query = 'SELECT n.nid,
              n.uid AS node_uid,
              n.title,
              c.cid,
              c.subject as comment_subject,
              0 as reply,
              fc.uid as uid,
              c.uid as orig_comment_uid,
              (1000 * fc.timestamp) + FLOOR(RAND() * 1000) AS timestamp,
              1 AS "like",
              1 AS comment
            FROM {flag_content} fc
            INNER JOIN {flags} f ON fc.fid = f.fid
            INNER JOIN {comment} c ON c.cid = fc.content_id
            INNER JOIN {node} n ON c.nid = n.nid
            WHERE
              n.type IN (:types)
              AND n.status
              AND timestamp > :time_limit
              AND f.name = :flag_name
              AND c.uid=:uid';
}