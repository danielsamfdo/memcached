import memcache

mc = memcache.Client(['127.0.0.1:8080'], debug=0)

mc.set("some_key", "some bad");
value = mc.get("some_key");

print value