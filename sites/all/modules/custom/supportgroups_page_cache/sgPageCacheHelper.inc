<?php

/**
 * @author craigmc
 *
 */
class sgPageCacheHelper {
  static function addExtraCidInfo($cid) {
    return $cid . $_SERVER['HTTP_HOST'];
  }
}
