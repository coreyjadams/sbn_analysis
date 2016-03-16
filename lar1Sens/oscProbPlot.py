from matplotlib import pyplot as plt
import numpy as np
import math


# In meters!!!!!
def oscProb(L, E, sin22th, dm2):
    return sin22th * math.sin(1.267 * (L/1000) * (1.0/E) * dm2)


def makePlot():

    eLow = 0.7
    eMean = 0.8
    eHigh = 0.9

    lsndBF = [0.003,1.2]
    koppBF = [0.013, 0.43]
    giuntiBF = [0.002, 1.8]

    # Make a set of points, the xVals:
    x = np.linspace(0, 800, 800)

    # Make the set of y points:
    ykoppLow = np.ndarray(800)
    ykoppMean = np.ndarray(800)
    ykoppHigh = np.ndarray(800)
    ygiuntiLow = np.ndarray(800)
    ygiuntiMean = np.ndarray(800)
    ygiuntiHigh = np.ndarray(800)
    ylsndLow = np.ndarray(800)
    ylsndMean = np.ndarray(800)
    ylsndHigh = np.ndarray(800)
    for i in xrange(len(x)):
        ygiuntiLow[i] = oscProb(x[i], eLow, giuntiBF[0], giuntiBF[1])
        ygiuntiMean[i] = oscProb(x[i], eMean, giuntiBF[0], giuntiBF[1])
        ygiuntiHigh[i] = oscProb(x[i], eHigh, giuntiBF[0], giuntiBF[1])
        ykoppLow[i] = oscProb(x[i], eLow, koppBF[0], koppBF[1])
        ykoppMean[i] = oscProb(x[i], eMean, koppBF[0], koppBF[1])
        ykoppHigh[i] = oscProb(x[i], eHigh, koppBF[0], koppBF[1])
        ylsndLow[i] = oscProb(x[i], eLow, lsndBF[0], lsndBF[1])
        ylsndMean[i] = oscProb(x[i], eMean, lsndBF[0], lsndBF[1])
        ylsndHigh[i] = oscProb(x[i], eHigh, lsndBF[0], lsndBF[1])

    # Now actually draw things:
    #
    fig, ax = plt.subplots()

    # for dset,style,lbl in zip(dataSets,styles,legends):
    #     ax.plot(xVals,dset,style,label=lbl,markersize=10)
    #     ax.grid(True,which='major')
    #     ax.legend(numpoints=1)

    # print y
    # print x

    ax.plot(x,ykoppMean,color="green",label="Kopp et. al. BF")
    ax.plot(x,ygiuntiMean,color="red",label="Giunti et. al. BF")
    # ax.plot(x,ylsndMean,color="blue",label="LSND BF")

    ax.fill_between(x, ykoppLow, ykoppHigh,facecolor="green",label="Kopp. et al BF")
    ax.fill_between(x, ygiuntiLow, ygiuntiHigh,facecolor="red",label="test")
    # ax.fill_between(x, ylsndLow, ylsndHigh,facecolor="blue",label="test")
    # ax.plot(x,ygiuntiLow)
    ax.grid(True, which='major')

    ax.axis([0, 800, 0, 0.009])
    plt.xlabel("Distance from Proton Target [m]", fontsize=20)
    plt.ylabel("Oscillation Prob.", fontsize=20)
    plt.title("SBN Oscillation Probability", fontsize=20, y=1.02)

    plt.tick_params(axis='both', which='major', labelsize=14)
    plt.tick_params(axis='both', which='minor', labelsize=14)

    for item in [fig]:
        item.patch.set_visible(False)



    # Get teh slope of the lines at 750:
    slopekoppHigh = math.degrees(math.atan2(ykoppHigh[750] - ykoppHigh[700],x[750]-x[700]))
    print slopekoppHigh

    plt.text(700,ykoppHigh[700]+0.00027,"900 MeV",rotation=25)
    plt.text(650,ykoppLow[700]+0.00041,"700 MeV",rotation=30)
    plt.text(700,ygiuntiHigh[700]+0.0001,"900 MeV",rotation=-5)
    plt.text(680,ygiuntiLow[700]-0.00025,"700 MeV",rotation=-17)

    leg = plt.legend(loc = 2)
    for legobj in leg.legendHandles:
        legobj.set_linewidth(6.0)

    plt.show()


    # name = matrixFileName.rstrip('.root') + '_fracUncert.png'

    # with open(name, 'w') as outfile:
    # fig.canvas.print_png(outfile)

if __name__ == '__main__':
    makePlot()
