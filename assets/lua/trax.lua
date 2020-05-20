
print("sequencer has loaded")

sequencer = {}

sequencer[0] = function ()
  print("track 0")
end

function run()
  for idx, seq in pairs(sequencer) do
    seq()
  end
end

run()
