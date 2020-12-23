----------------
-- Melody
--
-- Functions for doing melody related things
----------------

Util = require "trax.util"
moduloIdx = Util.moduloIdx

local M = {}

local scales = {
  major = {0, 2, 4, 5, 7, 9, 11},
  minor = {0, 2, 4, 5, 7, 9, 11},
}

function M.scale(name)
  return scales[name]
end


function M.setscale(scale)
  if not scale then
    print("no scale given")
    return function (v) return v end
  end

  return function(v)
    return moduloIdx(scale, v)
  end
end

return M
