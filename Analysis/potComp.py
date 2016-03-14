import math
import os
from ROOT import lar1
import matplotlib.pyplot as plt

# This script reads in files that contain the chi2 and churns out
# a list of chi2 at a point as a function of POT
# 
# Can be used to do systematics vs. statistics comparisons.


def getIndexes(dm2, sin22th, npoints=500):
  # Given dm2, sin22th, find the closest
  # value of the indexes to match them
  dm2min = 0.01    
  dm2max = 100.
  sin22thmin = 0.0001
  sin22thmax = 1.0

  # dm2 = pow(10.,(TMath::Log10(dm2min)+(dm2point*1./npoints)*TMath::Log10(dm2max/dm2min)));
  # sin22th = pow(10.,(TMath::Log10(sin22thmin)+(sin22thpoint*1./npoints)*TMath::Log10(sin22thmax/sin22thmin)))

  i_dm2 = npoints * (math.log(dm2, 10) - math.log(dm2min, 10)) / math.log(dm2max/dm2min, 10)
  i_sin22th = npoints * (math.log(sin22th, 10) - math.log(sin22thmin, 10)) / math.log(sin22thmax/sin22thmin, 10)

  return round(i_dm2), round(i_sin22th)


def my_range(start, end, step):
    while start <= end:
        yield start
        start += step


def getChi2AtPoint(file, x_index, y_index):
  # This function opens the file, reads in the matrix,
  # and returns the value at the point.
  # It uses c++ code to read in the whole chi2 map so that it is fast

  if not os.path.isfile(file):
    return -1

  putils = lar1.PlotUtils()
  chi2Vec = putils.getChi2MapFromFile(file)

  x_index = int(x_index)
  y_index = int(y_index)

  return chi2Vec.at(x_index).at(y_index)

def getPlotValues(dm2, sin22th):

  dm2_index, sin22th_index = getIndexes(dm2,sin22th)

  x_axis = []
  y_axis_nominal = []
  y_axis_with_systematics = []
  y_axis_upgrade = []
  y_axis_upgrade_syst = []

  filePathNominal = "/data_linux/nominal_ntuples/chi2/"
  filePathUpgrade = "/data_linux/horn_upgrade_repeat/chi2/"

  fileEnding = "_xsec_0_flux_6_ecalo2_nu_vePhot0.05_gap3_ND_100m_uB_T600_onaxis_chi2.root"
  fileStart = "chi2_nue_"

  for i in my_range(0.25,15.0,0.5):
    # Build the file name:
    name = filePathNominal + fileStart + "nominal_{k}".format(k=i) + fileEnding
    x_axis.append(i*2.2e20)
    # print "On POT = {X}".format(X=i*2.2e20)
    y_axis_nominal.append(math.sqrt(getChi2AtPoint(name,dm2_index,sin22th_index)))
    name = filePathNominal + fileStart + "nominal_{k}_syst_0.03".format(k=i) + fileEnding
    y_axis_with_systematics.append(math.sqrt(getChi2AtPoint(name,dm2_index,sin22th_index)))
    name = filePathUpgrade + fileStart + "upgrade_{k}".format(k=i) + fileEnding
    y_axis_upgrade.append(math.sqrt(getChi2AtPoint(name,dm2_index,sin22th_index)))
    name = filePathUpgrade + fileStart + "upgrade_{k}_syst_0.03".format(k=i) + fileEnding
    y_axis_upgrade_syst.append(math.sqrt(getChi2AtPoint(name,dm2_index,sin22th_index)))


  return x_axis, y_axis_nominal, y_axis_with_systematics, y_axis_upgrade, y_axis_upgrade_syst


if __name__ == '__main__':
  print "Making plots or some shit."

  directory="/home/cadams/Dropbox/Talks/BeamUpgradeMeeting/HornUpgradePlots3/POT/"

  # Giunti Best Fit Points:    
  dm2_giunti = 1.8
  sin22th_giunti = 0.002 
  
  # Kopp Best Fit Points:
  dm2_kopp = 0.43
  sin22th_kopp = 0.013

  # LSND Best Fit Points:
  # dm2_lsnd = 1.2
  # sin22th_lsnd = 0.003
  dm2_lsnd = 2.0
  sin22th_lsnd = 0.001

  dm2_low = 2.0
  sin22th_low = 0.0005

  dm2_points = [dm2_giunti, dm2_kopp, dm2_lsnd, dm2_low]
  sin22th_points = [sin22th_giunti, sin22th_kopp, sin22th_lsnd, sin22th_low]

  for dm2, sin22th in zip(dm2_points,sin22th_points):


      # Make a plot of the points of interest:
      x_axis, y_axis_nominal, y_axis_syst, y_axis_upgrade, y_axis_upgrade_syst = getPlotValues(dm2,sin22th)


      deltachisq5s = 23.40
      five_sigma_line = [math.sqrt(deltachisq5s)]*len(x_axis)

# savefig('foo.png')
      fig, ax = plt.subplots()
  
      ax.plot(x_axis,y_axis_nominal, label="Nominal $\sqrt{\Delta \chi^2}$")
      ax.plot(x_axis,y_axis_upgrade, label="Upgrade $\sqrt{\Delta \chi^2}$")
      ax.plot(x_axis,five_sigma_line,label="5 $\sigma$")
      ax.grid(True, which='major')
      ax.legend(loc=2)

      title = r"Nominal vs. Upgrade at ($\Delta m^2$, sin$^22\theta$) = ({dm2},{sin22th})".format(dm2=dm2,sin22th=sin22th)
      plt.title(title, fontsize=20, y=1.02)
  
      plt.savefig(directory + "pot_normal_{dm}_{sin}.png".format(dm=dm2,sin=sin22th))
      plt.savefig(directory + "pot_normal_{dm}_{sin}.pdf".format(dm=dm2,sin=sin22th))

      fig, ax = plt.subplots()
      ax.plot(x_axis,y_axis_syst, label="Nominal 3% $\sqrt{\Delta \chi^2}$")
      ax.plot(x_axis,y_axis_upgrade_syst, label="Upgrade 3% $\sqrt{\Delta \chi^2}$")
      ax.plot(x_axis,five_sigma_line,label="5 $\sigma$")
      ax.grid(True, which='major')

      title = r"Nominal vs. Upgrade at ($\Delta m^2$, sin$^22\theta$) = ({dm2},{sin22th})".format(dm2=dm2,sin22th=sin22th)
      plt.title(title, fontsize=20, y=1.02)

      leg = plt.legend(loc = 2)

      plt.ylabel("$\sqrt{\Delta \chi^2}$")
      plt.xlabel("POT")
      
      plt.savefig(directory + "pot_0.03syst_{dm}_{sin}.png".format(dm=dm2,sin=sin22th))
      plt.savefig(directory + "pot_0.03syst_{dm}_{sin}.pdf".format(dm=dm2,sin=sin22th))

  # plt.show()