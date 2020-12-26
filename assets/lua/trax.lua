
Trax = {}
Trax.bpm = 120
Trax.beat = 0
Trax.syncoffset = 0
Trax.syncinterval = 1/24
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
  t.syncoffset = t.syncoffset + bdelta
  local cBeat = t.beat

  if t.midiout and t.syncout then
    if t.syncoffset > t.syncinterval then
      local syncmessages = math.floor(t.syncoffset / t.syncinterval)
      print(t.syncoffset, syncmessages)
      t.syncoffset = math.fmod(t.syncoffset, t.syncinterval)
      for i = 1, syncmessages do
        print(bpm, t.beat, bdelta, syncmessages)
        t.midiout:byte(0xF8)
      end
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
