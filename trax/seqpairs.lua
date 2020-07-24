----------------
-- SeqPairs
--
-- A looping sequence of events with
-- individual durations
----------------

local S = {}

local function runMods(s, evt)
  local out = evt
  for _, f in ipairs(s.mappingFunctions) do
    out = f(out)
  end
  return out
end

local function events(s, prevBeat, curBeat)
  local seqlen = #s.elems * s.steplength
  local startpoint = (math.fmod(prevBeat, seqlen) / s.steplength) + 1
  local endpoint = (math.fmod(curBeat, seqlen) / s.steplength) + 1

  local events = {}

  if endpoint > startpoint then
    for k, v in ipairs(s.elems) do
      if k > endpoint then
        break
      elseif k > startpoint then
        events[#events+1] = runMods(s, v)
      end
    end
  else
    for k, v in ipairs(s.elems) do
      if k < endpoint then
        events[#events+1] = runMods(s, v)
      elseif k > startpoint then
        events[#events+1] = runMods(s, v)
      end
    end
  end

  return events
end

local function map(seq, func)
  seq.mappingFunctions[#seq.mappingFunctions+1] = func
  return seq
end

function S.new(elems)
  local seq = {}
  seq.events = events
  seq.map = map
  seq.mappingFunctions = {}

  seq.elems = elems
  return seq
end

return S
