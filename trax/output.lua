----------------
-- Output
--
-- Handles MIDI output things
----------------

local O = {}

local function send(o, evt)
  if evt.type == "note" then
    o.stream:noteon(o.channel, evt.value, evt.velocity)
  else
    print("unknown midi event", evt.type)
  end
end

local function sender(o)
  return function (evt)
    o:send(evt)
  end
end

function O.new(stream, channel)
  local o = {}

  o.channel = channel
  o.stream = stream

  o.send = send
  o.sender = sender

  return o
end

return O
