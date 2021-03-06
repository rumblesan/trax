----------------
-- Seq
--
-- A looping sequence of events with
-- the same duration
----------------

Util = require "trax.util"
moduloIdx = Util.moduloIdx

Simple = require "trax.simpleseq"

local S = {}

local function runmaps(s, evt)
  local out = evt
  for _, f in ipairs(s.mappingFunctions) do
    out = f(out)
  end
  return out
end

local function run(s, prevBeat, curBeat)
  local events = {}

  for _, parent in ipairs(s.sequences) do
    for _, evt in ipairs(parent:run(prevBeat, curBeat)) do
      events[#events+1] = runmaps(s, evt)
    end
  end

  return events
end

local function map(seq, func)
  seq.mappingFunctions[#seq.mappingFunctions+1] = func
  return seq
end

function S.combine(parents)
  local seq = {}
  seq.run = run
  seq.map = map
  seq.mappingFunctions = {}

  seq.sequences = parents

  return seq
end

function S.new(elements, durations, offset)
  local seq = {}
  seq.run = run
  seq.map = map
  seq.mappingFunctions = {}

  seq.sequences = {Simple.new(elements, durations, offset)}

  return seq
end

return S
