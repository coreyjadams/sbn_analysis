import sys
from ROOT import gSystem
gSystem.Load("libsbn_analysis_Reprocessing")
from ROOT import sample

try:

    print "PyROOT recognized your class %s" % str(sample)

except NameError:

    print "Failed importing Reprocessing..."

sys.exit(0)

