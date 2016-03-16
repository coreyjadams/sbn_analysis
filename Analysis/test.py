import ROOT
from ROOT import sbn

er = sbn.EventRate()
er.mainLoop()

import time
time.sleep(10)