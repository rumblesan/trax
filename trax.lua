
pmidi = require('portmidi')

pmidi.initialize()

pmidi.countdevices()

d1 = pmidi.getdeviceinfo(2)

d1:isoutput()

d1:id()

stream = d1:openoutput()

stream:direction()

stream:noteon(2, 80, 100)

stream:noteoff(2, 80, 100)
