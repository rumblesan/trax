----------------
-- Notes
--
-- Note functions
----------------

local N = {}

local function play(note, stream)
  stream:noteon(note.channel, note.value, note.velocity)
end

local function stop(note, stream)
  stream:noteoff(note.channel, note.value, note.velocity)
end

function N.new(value, velocity)
  local n = {}
  n.value = value
  n.velocity = velocity
  n.channel = 0

  n.play = play
  n.stop = stop
  return n
end

function N.tonote(velocity)

  v = velocity or 100
  return function (n)
    return N.new(n, v)
  end
end


return N
