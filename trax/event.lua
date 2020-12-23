
local E = {}


function E.tostring(evt)
  local s = "Event: " .. evt.type
  return s
end

E.mt = {}

E.mt.__tostring = E.tostring

function E.note(value, velocity)
  local n = {}

  setmetatable(n, E.mt)

  n.type = "note"
  n.value = value
  n.velocity = velocity
  n.channel = 0

  return n
end

return E
