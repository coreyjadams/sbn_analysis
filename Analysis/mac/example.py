import sys
from ROOT import gSystem
gSystem.Load("libsbn_analysis_Analysis")
from ROOT import sample

try:

    print "PyROOT recognized your class %s" % str(sample)

except NameError:

    print "Failed importing Analysis..."

sys.exit(0)

