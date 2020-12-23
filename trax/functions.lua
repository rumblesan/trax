----------------
-- Funcs
--
-- Functions for actually doing stuff
-- really need a better name for this module
----------------

local F = {}

function F.offset(offset)
  return function(v)
    if type(v) == "number" then
      return v + offset
    elseif type(v) == "table" and v.value then
      v.value = v.value + offset
      return v
    else
      return v
    end
  end
end

return F
