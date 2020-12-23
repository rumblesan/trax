----------------
-- Ixi
--
-- Ixi-lang style things
----------------

Seq = require "trax.seq"

local I = {}

function I.seq(seq)
  local values = {}
  local durations = {}
  local offset = 0

  local current = ''
  local spaces = 0

  for i = 1,seq:len() do
    local c = seq:sub(i, i)
    if c == ' ' then
      spaces = spaces + 1
    else
      if current == '' then
        offset = spaces
      else
        values[#values+1] = tonumber(current)
        durations[#durations+1] = spaces
      end
      current = c
      spaces = 1
    end
  end

  if current ~= '' then
    values[#values+1] = tonumber(current)
    durations[#durations+1] = spaces + offset
  end

  return Seq.new(values, durations, offset)
end

return I
