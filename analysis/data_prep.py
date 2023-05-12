import os
import sys
import ROOT
from ROOT import gStyle, gPad, gBenchmark
from array import array
import pandas as pd
import numpy as np

# execution parameters
create_files = True
check_afterpulses = False

benchmark = input(
    "Do you want to benchmark the data preparation code? (y/n)\n")
if benchmark == 'y':
    gBenchmark.Start("data_prep")

# ROOT style
gStyle.SetOptTitle(1)
gStyle.SetOptStat(1110)
gStyle.SetOptFit(111)
gStyle.SetLineScalePS(1)

# read from files and convert from hex to decimal
iron = pd.read_csv("data/new_iron.txt",
                   header=None, delim_whitespace=True)
concrete = pd.read_csv("data/new_concrete.txt",
                       header=None, delim_whitespace=True)

files = [iron, concrete]

for f in files:
    f.columns = ['tmp', 'Event', 'Trg_time', 'P1', 'P2', 'P3']
    f.drop('tmp', axis=1, inplace=True)
    f['P1'] = f['P1'].apply(int, base=16)
    f['P2'] = f['P2'].apply(int, base=16)
    f['P3'] = f['P3'].apply(int, base=16)
    for i in range(len(f['P1'])):
        if f['P1'][i] != 4095 and f['P2'][i] != 4095 and f['P3'][i] != 4095:
            f.drop(i)

if check_afterpulses == True:
    ibin_p1_iron = ROOT.TH1D(
        "ibin_iron1", "First 150 bins of P1 iron", 150, 0, 150)
    ibin_p2_iron = ROOT.TH1D(
        "ibin_iron2", "First 150 bins of P2 iron", 150, 0, 150)
    ibin_p3_iron = ROOT.TH1D(
        "ibin_iron3", "First 150 bins of P3 iron", 150, 0, 150)
    ibin_p1_and_p2_iron = ROOT.TH1D(
        "ibin_iron1_and_2", "First 150 bins of P1&P2 iron", 150, 0, 150)

    ibin_p1_concrete = ROOT.TH1D(
        "ibin_concrete1", "First 150 bins of P1 concrete", 150, 0, 150)
    ibin_p2_concrete = ROOT.TH1D(
        "ibin_concrete2", "First 150 bins of P2 concrete", 150, 0, 150)
    ibin_p3_concrete = ROOT.TH1D(
        "ibin_concrete3", "First 150 bins of P3 concrete", 150, 0, 150)
    ibin_p1_and_p2_concrete = ROOT.TH1D(
        "ibin_concrete1_and_2", "First 150 bins of P1&P2 concrete", 150, 0, 150)

    ibinshistos = [ibin_p1_iron, ibin_p2_iron, ibin_p3_iron, ibin_p1_and_p2_iron,
                   ibin_p1_concrete, ibin_p2_concrete, ibin_p3_concrete, ibin_p1_and_p2_concrete]

    for i in range(len(iron['P1'])):
        if iron['P1'][i] != 4095:
            ibin_p1_iron.Fill(iron['P1'][i])
        if iron['P2'][i] != 4095:
            ibin_p2_iron.Fill(iron['P2'][i])
        if iron['P3'][i] != 4095:
            ibin_p3_iron.Fill(iron['P3'][i])
        if iron['P1'][i] != 4095 and iron['P2'][i] != 4095 and iron['P3'][i] == 4095:
            ibin_p1_and_p2_iron.Fill(iron['P2'][i])

    for i in range(len(concrete['P1'])):
        if concrete['P1'][i] != 4095:
            ibin_p1_concrete.Fill(concrete['P1'][i])
        if concrete['P2'][i] != 4095:
            ibin_p2_concrete.Fill(concrete['P2'][i])
        if concrete['P3'][i] != 4095:
            ibin_p3_concrete.Fill(concrete['P3'][i])
        if concrete['P1'][i] != 4095 and concrete['P2'][i] != 4095 and concrete['P3'][i] == 4095:
            ibin_p1_and_p2_concrete.Fill(concrete['P2'][i])

    for h in ibinshistos:
        h.GetXaxis().SetTitle("Clock ticks")
        h.GetYaxis().SetTitle("Counts")

    first_150_bin_canvas = ROOT.TCanvas(
        "first_150_iron", "First 150 bins of iron planes", 1400, 700)
    first_150_bin_canvas.Divide(4, 1)
    first_150_bin_canvas.cd(1)
    ibin_p1_iron.Draw()
    first_150_bin_canvas.cd(2)
    ibin_p2_iron.Draw()
    first_150_bin_canvas.cd(3)
    ibin_p3_iron.Draw()
    first_150_bin_canvas.cd(4)
    ibin_p1_and_p2_iron.Draw()

    first_150_bin_canvas_concrete = ROOT.TCanvas(
        "first_150_concrete", "First 150 bins of concrete planes", 1400, 700)
    first_150_bin_canvas_concrete.Divide(4, 1)
    first_150_bin_canvas_concrete.cd(1)
    ibin_p1_concrete.Draw()
    first_150_bin_canvas_concrete.cd(2)
    ibin_p2_concrete.Draw()
    first_150_bin_canvas_concrete.cd(3)
    ibin_p3_concrete.Draw()
    first_150_bin_canvas_concrete.cd(4)
    ibin_p1_and_p2_concrete.Draw()

    first_150_bin_canvas.Print("first_150_iron_bins.pdf")
    first_150_bin_canvas_concrete.Print("first_150_concrete_bins.pdf")

for f in files:
    f['P1'] = f['P1'].apply(lambda x: x*3.98e-03)
    f['P2'] = f['P2'].apply(lambda x: x*3.98e-03)
    f['P3'] = f['P3'].apply(lambda x: x*3.98e-03)

overflow = 4095 * 3.98e-03

# create histograms
p1_iron = ROOT.TH1D("P1_iron", "P1 iron", 128, 0, 16)
p2_iron = ROOT.TH1D("P2_iron", "P2 iron", 128, 0, 16)
p3_iron = ROOT.TH1D("P3_iron", "P3 iron", 128, 0, 16)

p1_concrete = ROOT.TH1D("P1_concrete", "P1 concrete", 128, 0, 16)
p2_concrete = ROOT.TH1D("P2_concrete", "P2 concrete", 128, 0, 16)
p3_concrete = ROOT.TH1D("P3_concrete", "P3 concrete", 128, 0, 16)

histograms = [p1_iron, p2_iron, p3_iron, p1_concrete, p2_concrete, p3_concrete]

# fill histograms with raw data (no filter)
for i in range(len(iron['P1'])):
    if iron['P1'][i] != overflow:
        p1_iron.Fill(iron['P1'][i])
    if iron['P2'][i] != overflow:
        p2_iron.Fill(iron['P2'][i])
    if iron['P3'][i] != overflow:
        p3_iron.Fill(iron['P3'][i])

for i in range(len(concrete['P1'])):
    if concrete['P1'][i] != overflow:
        p1_concrete.Fill(concrete['P1'][i])
    if concrete['P2'][i] != overflow:
        p2_concrete.Fill(concrete['P2'][i])
    if concrete['P3'][i] != overflow:
        p3_concrete.Fill(concrete['P3'][i])

for h in histograms:
    h.GetXaxis().SetTitle("Stop time #mus")
    h.GetYaxis().SetTitle("Counts")

# display raw histograms
canvas = ROOT.TCanvas("canvas", "Histograms not filtered", 1400, 700)
canvas.Divide(3, 2)
for i in range(len(histograms)):
    canvas.cd(i+1)
    histograms[i].Draw("E,H")

canvas.Print("histograms.pdf")


# create filtered histograms
p1_filtered_iron = ROOT.TH1D(
    "P1_filtered_iron", "P1 filtered iron", 128, 0, 16)
p2_filtered_iron = ROOT.TH1D(
    "P2_filtered_iron", "P2 filtered iron", 128, 0, 16)
p3_filtered_iron = ROOT.TH1D(
    "P3_filtered_iron", "P3 filtered iron", 128, 0, 16)

p1_filtered_concrete = ROOT.TH1D(
    "P1_filtered_concrete", "P1 filtered concrete", 128, 0, 16)
p2_filtered_concrete = ROOT.TH1D(
    "P2_filtered_concrete", "P2 filtered concrete", 128, 0, 16)
p3_filtered_concrete = ROOT.TH1D(
    "P3_filtered_concrete", "P3 filtered concrete", 128, 0, 16)


filtered_histograms = [p1_filtered_iron, p2_filtered_iron, p3_filtered_iron,
                       p1_filtered_concrete, p2_filtered_concrete, p3_filtered_concrete]

# fill filtered histograms: P1&P2 coincidences, only P3
for i in range(len(iron['P1'])):
    if iron['P1'][i] != overflow and iron['P2'][i] != overflow and iron['P3'][i] == overflow:
        p1_filtered_iron.Fill(iron['P1'][i])
        p2_filtered_iron.Fill(iron['P2'][i])

    if iron['P1'][i] == overflow and iron['P2'][i] == overflow and iron['P3'][i] != overflow:
        p3_filtered_iron.Fill(iron['P3'][i])

for i in range(len(concrete['P1'])):
    if concrete['P1'][i] != overflow and concrete['P2'][i] != overflow and concrete['P3'][i] == overflow:
        p1_filtered_concrete.Fill(concrete['P1'][i])
        p2_filtered_concrete.Fill(concrete['P2'][i])

    if concrete['P1'][i] == overflow and concrete['P2'][i] == overflow and concrete['P3'][i] != overflow:
        p3_filtered_concrete.Fill(concrete['P3'][i])

for h in filtered_histograms:
    h.GetXaxis().SetTitle("Stop time #mus")
    h.GetYaxis().SetTitle("Counts")

# display filtered histograms
filtered_canvas = ROOT.TCanvas(
    "filtered_canvas", "Filtered histograms", 1400, 700)
filtered_canvas.Divide(3, 2)
for i in range(len(filtered_histograms)):
    filtered_canvas.cd(i+1)
    filtered_histograms[i].Draw("E,H")

filtered_canvas.Print("filtered_histograms.pdf")

# create root files
if create_files:
    all_data = ROOT.TFile("all_data.root", "RECREATE")
    for i in range(len(histograms)):
        histograms[i].Write()
    canvas.Write()
    all_data.Close()

    filtered_data = ROOT.TFile("filtered_data.root", "RECREATE")
    for i in range(len(filtered_histograms)):
        filtered_histograms[i].Write()
    filtered_canvas.Write()
    filtered_data.Close()

if benchmark == 'y':
    gBenchmark.Show("data_prep")


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
