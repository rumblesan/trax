----------------
-- Simple Sequence
--
-- the basic mechanics of sequences
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
  local s = {}

  s.run = run

  s.elements = elements
  if type(durations) == "number" then
    s.durations = {durations}
  else
    s.durations = durations
  end

  s.offset = offset or 0

  s.offsets = {}
  s.length = 0

  for idx, v in pairs(s.elements) do
    s.offsets[idx] = s.length
    s.length = s.length + moduloIdx(s.durations, idx)
  end

  return s
end

return S
