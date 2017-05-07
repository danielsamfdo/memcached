import memcache

mc = memcache.Client(['127.0.0.1:8080'], debug=0)

mc.set("some_key", "some bad w2333")
value = mc.get("some_key")
mc.set("a","abcdefg")
mc.set("b","abcdefg")
mc.set("c","abcdefg")

v2 = mc.get("c")
print value,v2
# 