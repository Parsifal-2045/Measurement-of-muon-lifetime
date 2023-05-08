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

void advanced_fit()
{
    // console colours
    const std::string red("\033[0;31m");
    const std::string cyan("\033[0;36m");
    const std::string reset("\033[0m");

    // char benchmark;
    // std::cout << "Do you want to benchmark the analysis code? (y/n)" << '\n';
    // std::cin >> benchmark;
    if (0) // benchmark == 'y')
    {
        gBenchmark->Start("Analysis Benchmark");
    }

    TFile *file = new TFile("all_data.root", "READ");
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
        histograms[i]->GetXaxis()->SetTitle("Stop time (#mus)");
        histograms[i]->GetYaxis()->SetTitle("Counts");
    }

    // 1/R exponential -> free parameters: N(u+, 0), lifetime, charge rate R, capture time, background
    auto adv_exp = new TF1("adv_exp", "[0]*TMath::Exp(-x*[1])*(1 + 1/[2]*TMath::Exp(-x*[3])) + [4]");
    adv_exp->SetParameters(200, 0.5, 1.27, 5, 6);

    // P3 cement fit and parameters
    p3_cement->Fit(adv_exp, "E,M,Q", "", 0.1, 12);
    auto one_over_tau = adv_exp->GetParameter(1);
    auto err_one_over_tau = adv_exp->GetParError(1);
    auto tau = std::abs(1 / one_over_tau);
    auto err_tau = tau * (err_one_over_tau / one_over_tau);
    std::cout << red << "P3 Cement fit parameters:" << reset << '\n';
    std::cout << cyan << tau << " +/- " << err_tau << '\n';
    std::cout << "chi2 / NDF: " << adv_exp->GetChisquare() << "/" << adv_exp->GetNDF() << reset << std::endl;

    auto adv_exp_canvas = new TCanvas("advanced_fit_display", "Fit iron and cement exponential");
    adv_exp_canvas->Divide(3, 2);
    for (int i = 0; i != 6; i++)
    {
        adv_exp_canvas->cd(i + 1);
        histograms[i]->Draw("E,H,SAME");
    }
    if (0) // benchmark == 'y')
    {
        gBenchmark->Show("Analysis Benchmark");
    }
}