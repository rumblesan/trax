
Trax = {}
Trax.bpm = 120
Trax.beat = 0
Trax.sequences = {}

function Trax.seq(t, num, func)
  if type(num) ~= "number" then
    print("no sequence number given")
    return
  end
  if type(func) == "function" then
    t.sequences[num] = func
  else
    t.sequences[num] = nil
  end
end

function Trax.run(t, pTime, cTime)

  local bpm = t.bpm
  local tdiff = cTime - pTime
  local bdelta = (bpm / 60000) * tdiff
  local pBeat = t.beat
  t.beat = t.beat + bdelta
  local cBeat = t.beat

  for idx, seq in pairs(t.sequences) do
    seq(pBeat, cBeat, pTime, cTime)
  end
end

print([[
**************
     TRAX
    LOADED
**************
]])
