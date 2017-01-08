<?php
// $Id$

require_once dirname(__FILE__) .'/sgPageCacheHelper.inc';

/**
 * @file
 * Wrapper class to handle page cache CID overrides for SG
 */
class sgPageCacheMemcache extends MemCacheDrupal {
  function get($cid) {
    return parent::get(sgPageCacheHelper::addExtraCidInfo($cid));
  }

  function getMultiple(&$cids) {
    foreach ($cids as $index => $cid) {
      $cids[$index] = sgPageCacheHelper::addExtraCidInfo($cid);
    }

    return parent::getMultiple($cids);
  }

  function set($cid, $data, $expire = CACHE_PERMANENT, array $headers = NULL) {
    $cid = sgPageCacheHelper::addExtraCidInfo($cid);
    parent::set($cid, $data, $expire, $headers);
  }
}