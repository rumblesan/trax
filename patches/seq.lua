
local S = {}

local function runMods(s, evt)
  local out = evt
  for _, f in ipairs(s.mappingFunctions) do
    out = f(out)
  end
  return out
end

local function next(s, prevBeat, curBeat)
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

local function map(s, func)
  s.mappingFunctions[#s.mappingFunctions+1] = func
  return s
end

function S.new(elems, steplength)
  local seq = {}
  seq.next = next
  seq.map = map
  seq.mappingFunctions = {}

  seq.elems = elems
  seq.steplength = steplength
  return seq
end

return S
