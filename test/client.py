import memcache
import random 
mc = memcache.Client(['127.0.0.1:8080'], debug=0)

mc.set("some_key", "some bad w2333")

mc.set("a","abcdefg")
mc.set("b","abcdefg")
mc.set("c","abcdefg")
mc.set("d","abcdefg")
mc.append("d","da")

# value = mc.get("some_key")
v2 = mc.get("some_key")
v3 = mc.get("a")
v4 = mc.get("b")
v5 = mc.get("c")
v6 = mc.get("d")
print v2,v3,v4,v5,v6
# 

key = "dan1" + str(random.randint(0, 1000000))
v = mc.add(key,"da")
v1 = mc.get(key)


assert v1 == "da"


v = mc.replace(key,"d111a")
v1 = mc.get(key)
assert v1 == "d111a"


v = mc.append(key, "132312312312312312312312")
v1 = mc.get(key)
assert v1 == "d111a132312312312312312312312"


v = mc.prepend(key, "132312312312312312312312")
v1 = mc.get(key)
assert v1 == "132312312312312312312312d111a132312312312312312312312"





# v2 = mc.replace("dan", "!23213")
# print v2;
# assert v2 == "!23213"
