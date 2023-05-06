import os
import sys
import ROOT
from ROOT import gStyle, gPad
from array import array
import pandas as pd
import numpy as np

# read from files and convert from hex to decimal
iron = pd.read_csv("data/ferro_26042023.txt",
                   header=None, delim_whitespace=True)
cement = pd.read_csv("data/cemento_21042023.txt",
                     header=None, delim_whitespace=True)

files = [iron, cement]

for f in files:
    f.columns = ['tmp', 'Event', 'Trg_time', 'P1', 'P2', 'P3']
    f.drop('tmp', axis=1, inplace=True)
    f['P1'] = f['P1'].apply(int, base=16)
    f['P2'] = f['P2'].apply(int, base=16)
    f['P3'] = f['P3'].apply(int, base=16)

# ROOT style
gStyle.SetOptTitle(1)
gStyle.SetOptStat(1110)
gStyle.SetOptFit(111)

# create and fill iron histograms
p1_iron = ROOT.TH1D("h1_iron", "P1 iron", 100, 0, 4095)
p2_iron = ROOT.TH1D("h2_iron", "P2 iron", 100, 0, 4095)
p3_iron = ROOT.TH1D("h3_iron", "P3 iron", 100, 0, 4095)


for i in range(len(iron['P1'])):
    p1_iron.Fill(iron['P1'][i])
    p2_iron.Fill(iron['P2'][i])
    p3_iron.Fill(iron['P3'][i])

# create and fill cement histograms
p1_cement = ROOT.TH1D("h1_cement", "P1 cement", 100, 0, 4095)
p2_cement = ROOT.TH1D("h2_cement", "P2 cement", 100, 0, 4095)
p3_cement = ROOT.TH1D("h3_cement", "P3 cement", 100, 0, 4095)

for i in range(len(cement['P1'])):
    p1_cement.Fill(cement['P1'][i])
    p2_cement.Fill(cement['P2'][i])
    p3_cement.Fill(cement['P3'][i])

# display iron histograms
canv_iron = ROOT.TCanvas("display_canvas_1", "Histograms with iron")
canv_iron.SetCanvasSize(1331, 475)
canv_iron.SetWindowSize(1333, 500)
canv_iron.Divide(3,1)
canv_iron.cd(1)
p1_iron.Draw("E,H")
canv_iron.cd(2)
p2_iron.Draw("E,H")
canv_iron.cd(3)
p3_iron.Draw("E,H")


# display cement histograms
canv_cement = ROOT.TCanvas("display_canvas_2", "Histograms with cement")
canv_cement.SetCanvasSize(1331, 475)
canv_cement.SetWindowSize(1333, 500)
canv_cement.Divide(3,1)
canv_cement.cd(1)
p1_cement.Draw("E,H")
canv_cement.cd(2)
p2_cement.Draw("E,H")
canv_cement.cd(3)
p3_cement.Draw("E,H")

canv_iron.Print("Histograms with iron.pdf")
canv_cement.Print("Histograms with cement.pdf")

# fit TODO
#fit_func = ROOT.TF1("Exponential", "[0]*TMath::Exp([1]*x)")
#fit_func.SetParameters(1, 2)
#
#p1_iron.Fit("Exponential")
#
#
## display fitted histograms
#canv_iron.cd(1)
#p1_iron.Draw("E,H,SAME")
#canv_iron.cd(2)
#p2_iron.Draw("E,H,SAME")
#canv_iron.cd(3)
#p3_iron.Draw("E,H,SAME")


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
