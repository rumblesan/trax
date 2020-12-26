
Trax = {}
Trax.bpm = 120
Trax.beat = 0
Trax.sequences = {}
Trax.midiout = nil
Trax.syncout = false
Trax.running = false

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

function Trax.setmidiout(t, midiout)
  t.midiout = midiout
end

function Trax.start(t)
  print("***** Starting *****")
  t.running = true
  if t.midiout and t.syncout then
    t.midiout:byte(0xFA)
  end
end

function Trax.stop(t)
  print("***** Stopping *****")
  t.running = false
  if t.midiout and t.syncout then
    t.midiout:byte(0xFC)
  end
end

function Trax.run(t, pTime, cTime)

  if not t.running then
    return
  end

  local bpm = t.bpm
  local tdiff = cTime - pTime
  local bdelta = (bpm / 60000) * tdiff
  local pBeat = t.beat
  t.beat = t.beat + bdelta
  local cBeat = t.beat

  if t.midiout and t.syncout then
    local syncMessages = math.floor(bdelta * (1/24))
    for i = 0, syncMessages do
      t.midiout:byte(0xF8)
    end
  end

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
