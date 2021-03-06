#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "lua/lua.h"
#include "lua/lualib.h"
#include "lua/lauxlib.h"

#include "portmidi.h"

#include "luamidi.h"

/**
 * PortMidi Stream
 */

static MidiStream *checkmidistream(lua_State *L, int arg_num) {
  return luaL_checkudata(L, arg_num, MIDISTREAM_METATABLE);
}

// Methods

static int c_pm_MidiStreamDirection(lua_State *L) {
  MidiStream *mstream = checkmidistream(L, 1);
  switch(mstream->direction) {
    case MidiStream_Output:
      lua_pushstring(L, "output");
      break;
    case MidiStream_Input:
      lua_pushstring(L, "input");
      break;
  }
  return 1;
}

static int c_pm_MidiStreamToString(lua_State *L) {
  checkmidistream(L, 1);
  lua_pushliteral(L, "MidiStream");
  return 1;
}

static int MidiStream_WriteNoteOn(lua_State *L) {
  MidiStream *mstream = checkmidistream(L, 1);
  int channel = luaL_checkinteger(L, 2);
  int note = luaL_checkinteger(L, 3);
  int velocity = luaL_checkinteger(L, 4);

  PmMessage msg = Pm_Message(0x90 | channel, note, velocity);

  PmError err = Pm_WriteShort(mstream->stream, 0, msg);
  if (err != pmNoError) { return luaL_error(L, Pm_GetErrorText(err)); }
  lua_pushnil(L);
  return 1;
}

static int MidiStream_WriteNoteOff(lua_State *L) {
  MidiStream *mstream = checkmidistream(L, 1);
  int channel = luaL_checkinteger(L, 2);
  int note = luaL_checkinteger(L, 3);
  int velocity = luaL_checkinteger(L, 4);

  PmMessage msg = Pm_Message(0x80 | channel, note, velocity);

  PmError err = Pm_WriteShort(mstream->stream, 0, msg);
  if (err != pmNoError) { return luaL_error(L, Pm_GetErrorText(err)); }
  lua_pushnil(L);
  return 1;
}

static int MidiStream_WriteByte(lua_State *L) {
  MidiStream *mstream = checkmidistream(L, 1);
  int value = luaL_checkinteger(L, 2);

  PmMessage msg = 0xff & value;

  PmError err = Pm_WriteShort(mstream->stream, 0, msg);
  if (err != pmNoError) { return luaL_error(L, Pm_GetErrorText(err)); }
  lua_pushnil(L);
  return 1;
}

static const struct luaL_Reg portmidi_midistream_m [] = {
  {"__tostring", c_pm_MidiStreamToString},
  {"direction", c_pm_MidiStreamDirection},
  {"noteon", MidiStream_WriteNoteOn},
  {"noteoff", MidiStream_WriteNoteOff},
  {"byte", MidiStream_WriteByte},
  {NULL, NULL}  /* sentinel */
};

// Lua boilerplate

static MidiStream *create_MidiStream(lua_State *L) {
  MidiStream *mstream = lua_newuserdata(L, sizeof(MidiStream));
  luaL_getmetatable(L, MIDISTREAM_METATABLE);
  lua_setmetatable(L, -2);
  return mstream;
}

static void register_MidiStream(lua_State *L) {
  luaL_newmetatable(L, MIDISTREAM_METATABLE);
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");
  luaL_setfuncs(L, portmidi_midistream_m, 0);
}


/**
 * MidiDevice
 */

static MidiDevice *checkmididevice(lua_State *L, int arg_num) {
  return luaL_checkudata(L, arg_num, MIDIDEVICE_METATABLE);
}

// Methods

static int c_pm_DeviceInterface(lua_State *L) {
  MidiDevice *device = checkmididevice(L, 1);
  lua_pushstring(L, device->info->interf);
  return 1;
}

static int c_pm_DeviceName(lua_State *L) {
  MidiDevice *device = checkmididevice(L, 1);
  lua_pushstring(L, device->info->name);
  return 1;
}

static int c_pm_DeviceId(lua_State *L) {
  MidiDevice *device = checkmididevice(L, 1);
  lua_pushinteger(L, device->id);
  return 1;
}

static int c_pm_DeviceIsInput(lua_State *L) {
  MidiDevice *device = checkmididevice(L, 1);
  lua_pushboolean(L, device->info->input == 1);
  return 1;
}

static int c_pm_DeviceIsOutput(lua_State *L) {
  MidiDevice *device = checkmididevice(L, 1);
  lua_pushboolean(L, device->info->output == 1);
  return 1;
}

static int c_pm_DeviceIsOpen(lua_State *L) {
  MidiDevice *device = checkmididevice(L, 1);
  lua_pushboolean(L, device->info->opened == 1);
  return 1;
}

static int c_pm_DeviceOpenOutputStream(lua_State *L) {
  MidiDevice *device = checkmididevice(L, 1);
  if (device->info->output == false) {
    return luaL_error(L, "Cannot create output stream for non output device");
  }

  // Create a new MidiStream and put it on the stack
  MidiStream *mstream = create_MidiStream(L);
  mstream->direction = MidiStream_Output;

  PmError err = Pm_OpenOutput(
    &mstream->stream,
    device->id,
    NULL,
    0,
    NULL,
    NULL,
    0 // latency
  );
  if (err != pmNoError) {
    lua_pop(L, 1); // Remove the MidiStream userdata created above
    return luaL_error(L, Pm_GetErrorText(err));
  }

  return 1;
}

static int c_pm_DeviceToString(lua_State *L) {
  MidiDevice *device = checkmididevice(L, 1);

  int namelen = strlen(device->info->name);
            //  name   spc  -> spc  in/out nullterm
  int fulllen = namelen + 1 + 2 + 1 + 3 + 1;
  char dir[4];
  if (device->info->input) {
    dir[0] = 'I';
    dir[1] = 'n';
    dir[2] = '\0';
  } else {
    dir[0] = 'O';
    dir[1] = 'u';
    dir[2] = 't';
    dir[3] = '\0';
  }
  char *nameinfo = malloc(fulllen * sizeof(char));
  sprintf(nameinfo, "%s -> %s", device->info->name, dir);

  lua_pushstring(L, nameinfo);
  free(nameinfo);
  return 1;
}

static const struct luaL_Reg portmidi_mididevice_m [] = {
  {"interface", c_pm_DeviceInterface},
  {"name", c_pm_DeviceName},
  {"id", c_pm_DeviceId},
  {"isinput", c_pm_DeviceIsInput},
  {"isoutput", c_pm_DeviceIsOutput},
  {"isopen", c_pm_DeviceIsOpen},
  {"openoutput", c_pm_DeviceOpenOutputStream},
  {"__tostring", c_pm_DeviceToString},
  {NULL, NULL}  /* sentinel */
};

// Lua boilerplate

static MidiDevice *create_MidiDevice(lua_State *L) {
  MidiDevice *device = lua_newuserdata(L, sizeof(MidiDevice));
  luaL_getmetatable(L, MIDIDEVICE_METATABLE);
  lua_setmetatable(L, -2);
  return device;
}

static void register_MidiDevice(lua_State *L) {
  luaL_newmetatable(L, MIDIDEVICE_METATABLE);
  lua_pushvalue(L, -1);
  lua_setfield(L, -2, "__index");
  luaL_setfuncs(L, portmidi_mididevice_m, 0);
}



/**
 * PortMidi Functions
 */

static int c_pm_Initialize(lua_State *L) {
  PmError err = Pm_Initialize();
  if (err != pmNoError) { return luaL_error(L, Pm_GetErrorText(err)); }
  lua_pushnil(L);
  return 1;
}

static int c_pm_Terminate(lua_State *L) {
  PmError err = Pm_Terminate();
  if (err != pmNoError) { return luaL_error(L, Pm_GetErrorText(err)); }
  lua_pushnil(L);
  return 1;
}

static int c_pm_CountDevices(lua_State *L) {
  int count = Pm_CountDevices();
  lua_pushinteger(L, count);
  return 1;
}

static int c_pm_GetDeviceInfo(lua_State *L) {
  int device_id = luaL_checkinteger(L, 1);
  const PmDeviceInfo *info = Pm_GetDeviceInfo(device_id);
  if (info == NULL) {
    lua_pushnil(L);
    return 1;
  }
  MidiDevice *device = create_MidiDevice(L);
  device->info = info;
  device->id = device_id;

  return 1;
}

/*
static int c_pm_OpenInput(lua_State *L) {
  int device_id = luaL_checkinteger(L, 1);

  // Create a new MidiStream and put it on the stack
  MidiStream *mstream = create_MidiStream(L);
  mstream->direction = MidiStream_Input;

  PmError err = Pm_OpenInput(
    &mstream->stream,
    device_id,
    NULL,
    0,
    NULL,
    NULL
  );
  if (err != pmNoError) {
    lua_pop(L, 1); // Remove the MidiStream userdata created above
    return luaL_error(L, Pm_GetErrorText(err));
  }

  return 1;
}
*/

static const struct luaL_Reg portmidi [] = {
  {"initialize", c_pm_Initialize},
  {"terminate", c_pm_Terminate},
  {"countdevices", c_pm_CountDevices},
  {"getdeviceinfo", c_pm_GetDeviceInfo},
  //{"openinput", c_pm_OpenInput},
  {NULL, NULL}  /* sentinel */
};

int luaopen_portmidi (lua_State *L){
  register_MidiDevice(L);
  register_MidiStream(L);

  luaL_newlib(L, portmidi);
  return 1;
}
