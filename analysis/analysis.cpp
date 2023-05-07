#include <iostream>
#include <string>
#include <vector>
#include <TROOT.h>
#include <TStyle.h>
#include <TFile.h>
#include <TH1.h>
#include <TF1.h>
#include <TCanvas.h>
#include <TMath.h>
#include <TMinuit.h>
#include <TPaveStats.h>
#include <TBenchmark.h>

void analysis()
{
    char benchmark;
    std::cout << "Do you want to benchmark the analysis code? (y/n)" << '\n';
    std::cin >> benchmark;
    if (benchmark == 'y')
    {
        gBenchmark->Start("Analysis Benchmark");
    }

    TFile *file = new TFile("histograms.root", "READ");
    TH1D *histograms[6];

    TH1D *p1_iron = (TH1D *)file->Get("P1_iron");
    histograms[0] = p1_iron;
    TH1D *p2_iron = (TH1D *)file->Get("P2_iron");
    histograms[1] = p2_iron;
    TH1D *p3_iron = (TH1D *)file->Get("P3_iron");
    histograms[2] = p3_iron;

    TH1D *p1_cement = (TH1D *)file->Get("P1_cement");
    histograms[3] = p1_cement;
    TH1D *p2_cement = (TH1D *)file->Get("P2_cement");
    histograms[4] = p2_cement;
    TH1D *p3_cement = (TH1D *)file->Get("P3_cement");
    histograms[5] = p3_cement;

    for (int i = 0; i != 6; i++)
    {
        histograms[i]->GetXaxis()->SetTitle("Stop time (# of clock ticks)");
        histograms[i]->GetYaxis()->SetTitle("Counts");
    }

    auto fit_func = new TF1("Exponential", "[0]*TMath::Exp(x*[1]) + [2]");

    for (int i = 0; i != 6; i++)
    {
        histograms[i]->Fit(fit_func);
    }

    auto canvas = new TCanvas("fit_display", "Fit iron and cement");
    canvas->Divide(3, 2);
    for (int i = 0; i != 6; i++)
    {
        canvas->cd(i + 1);
        histograms[i]->Draw("E,H,SAME");
    }

    if (benchmark == 'y')
    {
        gBenchmark->Show("Analysis Benchmark");
    }
}