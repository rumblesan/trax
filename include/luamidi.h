#pragma once

#include "portmidi.h"

#define LIBNAME "LuaMidi"

#define MIDIDEVICE_METATABLE (LIBNAME ".MidiDevice")

typedef struct MidiDevice {
  const PmDeviceInfo *info;
  int id;
} MidiDevice;

#define MIDISTREAM_METATABLE (LIBNAME ".MidiStream")

typedef enum midistream_direction {
  MidiStream_Output,
  MidiStream_Input
} MidiStream_Direction;

typedef struct MidiStream {
  PmStream *stream;
  MidiStream_Direction direction;
} MidiStream;
