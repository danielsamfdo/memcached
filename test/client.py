import memcache

mc = memcache.Client(['127.0.0.1:8080'], debug=0)

mc.set("some_key", "some bad w2333")

mc.set("a","abcdefg")
mc.set("b","abcdefg")
mc.set("c","abcdefg")
mc.set("d","abcdefg")

# value = mc.get("some_key")
v2 = mc.get("some_key")
v3 = mc.get("a")
v4 = mc.get("b")
v5 = mc.get("c")
v6 = mc.get("d")
print v2,v3,v4,v5,v6
# 