----------------
-- Seq
--
-- A looping sequence of events with
-- the same duration
----------------

local function moduloIdx(array, idx)
  local v = idx % #array
  if v == 0 then
    v = #array
  end
  return array[v]
end

local S = {}

local function runMods(s, evt)
  local out = evt
  for _, f in ipairs(s.mappingFunctions) do
    out = f(out)
  end
  return out
end

local function getEvents(s, prevBeat, curBeat)
  local startpoint = math.fmod(prevBeat + s.offset, s.length)
  local endpoint = math.fmod(curBeat + s.offset, s.length)

  print("start", startpoint, "end", endpoint, prevBeat, curBeat, s.length)
  local events = {}

  if endpoint > startpoint then
    for idx, v in pairs(s.offsets) do
      if v >= endpoint then
        break
      elseif v >= startpoint then
        events[#events+1] = runMods(s, s.elems[idx])
      end
    end
  else
    for idx, v in pairs(s.offsets) do
      if v < endpoint then
        events[#events+1] = runMods(s, s.elems[idx])
      elseif v >= startpoint then
        events[#events+1] = runMods(s, s.elems[idx])
      end
    end
  end

  return events
end

local function map(seq, func)
  seq.mappingFunctions[#seq.mappingFunctions+1] = func
  return seq
end

function S.new(elems, steplengths, offset)
  local seq = {}
  seq.getEvents = getEvents
  seq.map = map
  seq.mappingFunctions = {}

  seq.elems = elems
  if type(steplengths) == "number" then
    seq.steplengths = {steplengths}
  else
    seq.steplengths = steplengths
  end

  seq.offset = offset or 0

  seq.offsets = {}
  seq.length = 0

  for idx, v in pairs(elems) do
    seq.offsets[idx] = seq.length
    seq.length = seq.length + moduloIdx(seq.steplengths, idx)
  end

  return seq
end

return S
