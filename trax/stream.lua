----------------
-- Stream
--
-- create a continuous stream of events
----------------

Util = require "trax.util"
moduloIdx = Util.moduloIdx

local S = {}

local function run(s, prevBeat, curBeat)
  local startpoint = math.fmod(prevBeat + s.offset, s.length)
  local endpoint = math.fmod(curBeat + s.offset, s.length)

  local events = {}

  if endpoint > startpoint then
    for idx, v in pairs(s.offsets) do
      if v >= endpoint then
        break
      elseif v >= startpoint then
        events[#events+1] = s.elements[idx]
      end
    end
  else
    for idx, v in pairs(s.offsets) do
      if v < endpoint then
        events[#events+1] = s.elements[idx]
      elseif v >= startpoint then
        events[#events+1] = s.elements[idx]
      end
    end
  end

  return events
end

function S.new(elements, durations, offset)
  local stream = {}

  stream.run = run

  stream.elements = elements
  if type(durations) == "number" then
    stream.durations = {durations}
  else
    stream.durations = durations
  end

  stream.offset = offset or 0

  stream.offsets = {}
  stream.length = 0

  for idx, v in pairs(stream.elements) do
    stream.offsets[idx] = stream.length
    stream.length = stream.length + moduloIdx(stream.durations, idx)
  end

  return stream
end

return S
