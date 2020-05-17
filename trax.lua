
pmidi = require('portmidi')

pmidi.initialize()

pmidi.countdevices()

d1 = pmidi.getdeviceinfo(2)

d1:isoutput()

stream = pmidi.openoutput(2)

stream:direction()

pmidi.noteon(stream, 2, 80, 100)

pmidi.noteoff(stream, 2, 80, 100)

d1:isoutput()

pmidi.devicename(pmidi.getdeviceinfo(3))

pmidi.terminate()
