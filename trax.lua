
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

pmidi = require 'luapmidi'

print(pmidi.mysin(2))
