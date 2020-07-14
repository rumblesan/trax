
local function reload(modulepath)
  if package.loaded[modulepath] then
    package.loaded[modulepath] = nil
  end
  return require(modulepath)
end

return {
  reload = reload,
}
