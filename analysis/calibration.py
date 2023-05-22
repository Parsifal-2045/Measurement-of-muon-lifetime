import os
import sys
import ROOT
from ROOT import gStyle, gPad
from array import array
import pandas as pd
import numpy as np

file_133 = pd.read_csv(
    "calibration_data/test_133_20042023.txt", header=None, delim_whitespace=True)
file_293 = pd.read_csv(
    "calibration_data/test_293_20042023.txt", header=None, delim_whitespace=True)
file_407 = pd.read_csv(
    "calibration_data/test_407_20042023.txt", header=None, delim_whitespace=True)
file_555 = pd.read_csv(
    "calibration_data/test_555_20042023.txt", header=None, delim_whitespace=True)
file_1021 = pd.read_csv(
    "calibration_data/test_1021_20042023.txt", header=None, delim_whitespace=True)
file_2274 = pd.read_csv(
    "calibration_data/test_2274_20042023.txt", header=None, delim_whitespace=True)
file_5507 = pd.read_csv(
    "calibration_data/test_5507_20042023.txt", header=None, delim_whitespace=True)
file_10180 = pd.read_csv(
    "calibration_data/test_10180_20042023.txt", header=None, delim_whitespace=True)
file_16200 = pd.read_csv(
    "calibration_data/test_16200_20042023.txt", header=None, delim_whitespace=True)
files = [file_133, file_293, file_407, file_555, file_1021,
         file_2274, file_5507, file_10180, file_16200]
file_133[3] = file_133[3].astype(str)

mean = [[], [], []]
std = [[], [], []]

for f in files:
    f.columns = ['tmp', 'Event', 'Trg_time', 'P1', 'P2', 'P3']
    f.drop('tmp', axis=1, inplace=True)
    # convert from hexadecimal to decimal
    f['P1'] = f['P1'].apply(int, base=16)
    f['P2'] = f['P2'].apply(int, base=16)
    f['P3'] = f['P3'].apply(int, base=16)
    # mean and std arrays
    mean[0].append(f['P1'].mean())
    std[0].append(f['P1'].std())
    mean[1].append(f['P2'].mean())
    std[1].append(f['P2'].std())
    mean[2].append(f['P3'].mean())
    std[2].append(f['P3'].std())

# convert in array to keep ROOT happy

mean_1 = array('f', mean[0])
std_1 = array('f', std[0])
mean_2 = array('f', mean[1])
std_2 = array('f', std[1])
mean_3 = array('f', mean[2])
std_3 = array('f', std[2])

y = [133.8, 293.2, 407.4, 555.0, 1021.0, 2274.0, 5507.0, 10180.0, 16200.0]
yerr = [0.450, 0.58, 0.969, 1.0, 3.0, 16.0, 18.0, 22.0, 25.0]

y = array('f', y)
yerr = array('f', yerr)

gStyle.SetOptTitle(1)
gStyle.SetOptStat(1110)
gStyle.SetOptFit(111)

graphs = [ROOT.TGraphErrors(9, mean_1, y, std_1, yerr),
          ROOT.TGraphErrors(9, mean_2, y, std_2, yerr),
          ROOT.TGraphErrors(9, mean_2, y, std_2, yerr)]

fit_func = ROOT.TF1("Linear regression", "[0]*x + [1]")

canvases = []
stats = []
i = 0

conversion_factor = []
err_conversion_factor = []

for g in graphs:
    canvases.append(ROOT.TCanvas())
    canvases[i].SetCanvasSize(900, 500)
    canvases[i].SetWindowSize(1000, 600)
    g.GetXaxis().SetTitle("Number of clock ticks")
    g.GetYaxis().SetTitle("Time [ns]")
    g.GetXaxis().SetTitleSize(0.05)
    g.GetYaxis().SetTitleSize(0.05)
    g.GetYaxis().SetTitleOffset(0.9)
    g.SetTitle("Calibration for plane " + str(i+1))
    g.Fit(fit_func)
    conversion_factor.append(fit_func.GetParameter(0))
    err_conversion_factor.append(fit_func.GetParError(0))
    g.Draw()
    gPad.Update()
    stats.append(g.FindObject("stats"))
    stats[i].SetX1NDC(0.1)
    stats[i].SetY1NDC(0.646)
    stats[i].SetX2NDC(0.517778)
    stats[i].SetY2NDC(0.9)
    gPad.Update()
    canvases[i].Print("calib_plane_" + str(i+1) + ".pdf")
    i += 1

print(conversion_factor)
print(err_conversion_factor)

# wait for input to keep the GUI (which lives on a ROOT event dispatcher) alive
if __name__ == '__main__':
    rep = ''
    while not rep in ['q', 'Q']:
        # Check if we are in Python 2 or 3
        if sys.version_info[0] > 2:
            rep = input('enter "q" to quit: ')
        else:
            rep = raw_input('enter "q" to quit: ')
        if 1 < len(rep):
            rep = rep[0]