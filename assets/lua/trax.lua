
print("Trax has loaded")

state = {}
state.bpm = 120
state.beat = 0
state.sequences = {}

function add(num, func)
  if type(num) ~= "number" then
    print("no sequence number given")
    return
  end
  if type(func) == "function" then
    state.sequences[num] = func
  else
    state.sequences[num] = nil
  end
end

function run(pTime, cTime)

  local bpm = state.bpm
  local beat = state.bpm
  local tdiff = cTime - pTime
  local bdelta = (bpm / 60000) * tdiff
  local pBeat = beat
  state.beat = beat + bdelta
  local cBeat = beat

  for idx, seq in pairs(state.sequences) do
    seq(beat, pBeat, cBeat, pTime, cTime)
  end
end
