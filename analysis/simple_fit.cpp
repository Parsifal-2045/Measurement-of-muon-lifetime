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

void simple_fit()
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
        histograms[i]->GetXaxis()->SetTitleSize(0.045);
        histograms[i]->GetYaxis()->SetTitle("Counts");
        histograms[i]->GetYaxis()->SetTitleSize(0.045);

    }

    // simple exponential - free parameters: N(u, 0), lifetime and background
    auto simple_exp = new TF1("Simple exp", "[0]*TMath::Exp(-x/[1]) + [2]");
    simple_exp->SetParameters(270, 2, 5);
    simple_exp->SetParNames("N(#mu, 0)", "#tau_{0}", "b");

    ////////////////////////////////////////////////
    /////////////////// IRON ///////////////////////
    ////////////////////////////////////////////////

    // P1 iron fit and parameters
    p1_iron->Fit(simple_exp, "E,M,Q", "", 1, 12);
    auto one_over_tau = simple_exp->GetParameter(1);
    auto err_one_over_tau = simple_exp->GetParError(1);
    std::cout << red << "P1 Iron fit parameters:" << reset << '\n';
    std::cout << cyan << simple_exp->GetParameter(1) << " +/- " << simple_exp->GetParError(1) << '\n';
    std::cout << "chi2 / NDF: " << simple_exp->GetChisquare() << "/" << simple_exp->GetNDF() << reset << std::endl;

    // P2 iron fit and parameters
    p2_iron->Fit(simple_exp, "E,M,Q", "", 1, 12);
    one_over_tau = simple_exp->GetParameter(1);
    err_one_over_tau = simple_exp->GetParError(1);
    std::cout << red << "P2 Iron fit parameters:" << reset << '\n';
    std::cout << cyan << simple_exp->GetParameter(1) << " +/- " << simple_exp->GetParError(1) << '\n';
    std::cout << "chi2 / NDF: " << simple_exp->GetChisquare() << "/" << simple_exp->GetNDF() << reset << std::endl;

    // P3 iron fit and parameters
    p3_iron->Fit(simple_exp, "E,M,Q", "", 0.5, 10);
    one_over_tau = simple_exp->GetParameter(1);
    err_one_over_tau = simple_exp->GetParError(1);
    std::cout << red << "P3 Iron fit parameters:" << reset << '\n';
    std::cout << cyan << simple_exp->GetParameter(1) << " +/- " << simple_exp->GetParError(1) << '\n';
    std::cout << "chi2 / NDF: " << simple_exp->GetChisquare() << " / " << simple_exp->GetNDF() << reset << std::endl;

    /*
        ////////////////////////////////////////////////
        ////////////////// CEMENT //////////////////////
        ////////////////////////////////////////////////

        // P1 cement fit and parameters
        p1_cement->Fit(simple_exp, "E,M,Q", "", 1, 12);
        one_over_tau = simple_exp->GetParameter(1);
        err_one_over_tau = simple_exp->GetParError(1);
        std::cout << red << "P1 Cement fit parameters:" << reset << '\n';
        std::cout << cyan << simple_exp->GetParameter(1) << " +/- " << simple_exp->GetParError(1) << '\n';
        std::cout << "chi2 / NDF: " << simple_exp->GetChisquare() << "/" << simple_exp->GetNDF() << reset << std::endl;

        // P2 cement fit and parameters
        p2_cement->Fit(simple_exp, "E,M,Q", "", 1, 12);
        one_over_tau = simple_exp->GetParameter(1);
        err_one_over_tau = simple_exp->GetParError(1);
        std::cout << red << "P2 Cement fit parameters:" << reset << '\n';
        std::cout << cyan << simple_exp->GetParameter(1) << " +/- " << simple_exp->GetParError(1) << '\n';
        std::cout << "chi2 / NDF: " << simple_exp->GetChisquare() << "/" << simple_exp->GetNDF() << reset << std::endl;

        // P3 cement fit and parameters
        p3_cement->Fit(simple_exp, "E,M,Q", "", 1, 12);
        one_over_tau = simple_exp->GetParameter(1);
        err_one_over_tau = simple_exp->GetParError(1);
        std::cout << red << "P3 Cement fit parameters:" << reset << '\n';
        std::cout << cyan << simple_exp->GetParameter(1) << " +/- " << simple_exp->GetParError(1) << '\n';
        std::cout << "chi2 / NDF: " << simple_exp->GetChisquare() << "/" << simple_exp->GetNDF() << reset << std::endl;
    */

    auto simple_exp_canvas = new TCanvas("simple_fit_display", "Fit iron and cement with a simple exponential");
    simple_exp_canvas->Divide(3, 2);

    for (int i = 0; i != 6; i++)
    {
        simple_exp_canvas->cd(i + 1);
        gPad->SetLogy();
        gPad->Modified();
        histograms[i]->Draw("E,H");
    }

    auto iron_canvas = new TCanvas("iron_canvas", "Iron Fit");
    iron_canvas->SetCanvasSize(600, 800);
    iron_canvas->Divide(1, 3);

    for (int i = 0; i != 3; i++)
    {
        iron_canvas->cd(i + 1);
        gPad->SetLogy();
        gPad->Modified();
        histograms[i]->Draw("E,H");
    }

    gPad->Update();
    TPaveStats *stats[3];
    TPaveStats *st1 = (TPaveStats *)p1_iron->FindObject("stats");
    stats[0] = st1;
    st1->SetName("p1_iron_stats");
    TPaveStats *st2 = (TPaveStats *)p2_iron->FindObject("stats");
    stats[1] = st2;
    st2->SetName("p2_iron_stats");
    TPaveStats *st3 = (TPaveStats *)p3_iron->FindObject("stats");
    stats[2] = st3;
    st3->SetName("p3_iron_stats");
    for (int i = 0; i != 3; i++)
    {
        stats[i]->SetX1NDC(0.581633);
        stats[i]->SetY1NDC(0.549202);
        stats[i]->SetX2NDC(0.89966);
        stats[i]->SetY2NDC(0.900266);
    }

    iron_canvas->Print("iron_fit.pdf");

    if (0) // benchmark == 'y')
    {
        gBenchmark->Show("Analysis Benchmark");
    }
}