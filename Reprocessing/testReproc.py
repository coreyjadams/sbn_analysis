from ROOT import *

# a_100 = Reprocessing("/data_linux/horn_upgrade_repeat/combined_ntuple_100m_nu.root")
# a_100_fosc = Reprocessing("/data_linux/horn_upgrade_repeat/combined_ntuple_100m_nu_fosc.root")
# a_100.UseTwoHornConfig()
# a_100.Loop("nue",0,0)
# a_100_fosc.UseTwoHornConfig();
# a_100_fosc.Loop("fosc",0,0);

# a_470 = Reprocessing("/data_linux/horn_upgrade_repeat/combined_ntuple_470m_nu.root")
# a_470_fosc = Reprocessing("/data_linux/horn_upgrade_repeat/combined_ntuple_470m_nu_fosc.root")
# a_470.UseTwoHornConfig()
# a_470.Loop("nue",1,4)
# a_470_fosc.UseTwoHornConfig();
# a_470_fosc.Loop("fosc",1,4);


# a_600 = Reprocessing("/data_linux/horn_upgrade_repeat/combined_ntuple_600m_onaxis_nu.root")
a_600_fosc = Reprocessing("/data_linux/horn_upgrade_repeat/combined_ntuple_600m_onaxis_nu_fosc.root")
# a_600.UseTwoHornConfig()
# a_600.Loop("nue",2,5)
a_600_fosc.UseTwoHornConfig();
a_600_fosc.Loop("fosc",2,5);