
print("Hello, World!")

function fact (n)
  if n == 0 then
    return 1
  else
    return n * fact(n - 1)
  end
end

print(fact(3))


a = {}

a["x"] = 10
a[3] = 20

print(a)

print(a[3])

os.date()

pmidi = require('portmidi')

pmidi.initialize()

pmidi.countdevices()

d1 = pmidi.getdeviceinfo(1)

d1

d1:isopen()

pmidi.devicename(pmidi.getdeviceinfo(3))

pmidi.terminate()
