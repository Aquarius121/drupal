This "module" is just to extend existing cache mechanisms
and to add additional CacheID (cid) to get around the base_url 
being set in settings.php.

No specific module needs to be enabled. This just needs to be added where
appropriate to settings.php

  // Add alternate handler for Page Cache
  $conf['cache_backends'][] = 'sites/all/modules/custom/supportgroups_page_cache/sgPageCacheRedis.inc';
  $conf['cache_class_cache_page'] = 'sgPageCacheRedis';

  
Because PHP can't do dynamic class extension, there is one
helper for memcached and one for Redis