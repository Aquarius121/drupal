Mechanism for notifications

Notifications are stored in redis with key SUPPORTGROUPS_FEED_RECENT_ACTIVITY_NOTIFICATIONS_CID . '-' . $uid
We are only really interested in users who are active

When a user first logs in we rebuild their notifications cache

When we load the notification block we check that a notification cache exists for the user,
(in case they are already logged in when returning to the website) and rebuild it before obtaining recent
notifications.

Cron run deletes notification caches if user not active after a period

Notifications are not stored for a user who does not have an active cache.  This is to prevent the cache from only
being partially filled, with most recent notifications.  The cache will be fully filled when the user logs in.