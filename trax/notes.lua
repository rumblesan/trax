----------------
-- Notes
--
-- Note functions
----------------

Event = require "trax.event"

local N = {}

function N.tostring(n)
  local s = "MIDI Note: " .. n.value .. " -> " .. n.velocity
  return s
end

N.mt = {}

N.mt.__tostring = N.tostring

function N.new(value, velocity)
  local evt = Event.note(value, velocity)
  setmetatable(evt, N.mt)
  return evt
end

function N.tonote(velocity)
  v = velocity or 100
  return function (n)
    return N.new(n, v)
  end
end


return N
