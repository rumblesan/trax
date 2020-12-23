----------------
-- Util
--
-- utility functions
----------------

local U = {}

function U.moduloIdx(array, idx)
  local v = idx % #array
  if v == 0 then
    v = #array
  end
  return array[v]
end

return U
