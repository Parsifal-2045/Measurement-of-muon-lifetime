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

void all_data_fit()
{
    // console colours
    const std::string red("\033[0;31m");
    const std::string cyan("\033[0;36m");
    const std::string reset("\033[0m");

    // style
    gStyle->SetOptTitle(1);
    gStyle->SetOptStat(1110);
    gStyle->SetOptFit(111);

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
    std::cout << red << "P1 Iron fit parameters:" << reset << '\n';
    std::cout << cyan << simple_exp->GetParameter(1) << " +/- " << simple_exp->GetParError(1) << '\n';
    std::cout << "chi2 / NDF: " << simple_exp->GetChisquare() << "/" << simple_exp->GetNDF() << reset << std::endl;

    // P2 iron fit and parameters
    p2_iron->Fit(simple_exp, "E,M,Q", "", 1, 12);
    std::cout << red << "P2 Iron fit parameters:" << reset << '\n';
    std::cout << cyan << simple_exp->GetParameter(1) << " +/- " << simple_exp->GetParError(1) << '\n';
    std::cout << "chi2 / NDF: " << simple_exp->GetChisquare() << "/" << simple_exp->GetNDF() << reset << std::endl;

    // P3 iron fit and parameters
    p3_iron->Fit(simple_exp, "E,M,Q", "", 0.5, 10);
    std::cout << red << "P3 Iron fit parameters:" << reset << '\n';
    std::cout << cyan << simple_exp->GetParameter(1) << " +/- " << simple_exp->GetParError(1) << '\n';
    std::cout << "chi2 / NDF: " << simple_exp->GetChisquare() << " / " << simple_exp->GetNDF() << reset << std::endl;

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

    TPaveStats *stats_iron[3];
    TPaveStats *st1_iron = (TPaveStats *)p1_iron->FindObject("stats");
    stats_iron[0] = st1_iron;
    st1_iron->SetName("p1_iron_stats");
    TPaveStats *st2_iron = (TPaveStats *)p2_iron->FindObject("stats");
    stats_iron[1] = st2_iron;
    st2_iron->SetName("p2_iron_stats");
    TPaveStats *st3_iron = (TPaveStats *)p3_iron->FindObject("stats");
    stats_iron[2] = st3_iron;
    st3_iron->SetName("p3_iron_stats");
    for (int i = 0; i != 3; i++)
    {
        stats_iron[i]->SetX1NDC(0.581633);
        stats_iron[i]->SetY1NDC(0.549202);
        stats_iron[i]->SetX2NDC(0.89966);
        stats_iron[i]->SetY2NDC(0.900266);
    }

    iron_canvas->Print("iron_fit.pdf");

    ////////////////////////////////////////////////
    ////////////////// CEMENT //////////////////////
    ////////////////////////////////////////////////

    // P1 cement fit and parameters
    p1_cement->Fit(simple_exp, "E,M,Q", "", 1, 12);
    std::cout << red << "P1 Cement fit parameters:" << reset << '\n';
    std::cout << cyan << simple_exp->GetParameter(1) << " +/- " << simple_exp->GetParError(1) << '\n';
    std::cout << "chi2 / NDF: " << simple_exp->GetChisquare() << "/" << simple_exp->GetNDF() << reset << std::endl;

    // P2 cement fit and parameters
    p2_cement->Fit(simple_exp, "E,M,Q", "", 1, 12);
    std::cout << red << "P2 Cement fit parameters:" << reset << '\n';
    std::cout << cyan << simple_exp->GetParameter(1) << " +/- " << simple_exp->GetParError(1) << '\n';
    std::cout << "chi2 / NDF: " << simple_exp->GetChisquare() << "/" << simple_exp->GetNDF() << reset << std::endl;

    // P3 cement fit and parameters
    p3_cement->Fit(simple_exp, "E,M,Q", "", 1, 12);
    std::cout << red << "P3 Cement fit parameters:" << reset << '\n';
    std::cout << cyan << simple_exp->GetParameter(1) << " +/- " << simple_exp->GetParError(1) << '\n';
    std::cout << "chi2 / NDF: " << simple_exp->GetChisquare() << "/" << simple_exp->GetNDF() << reset << std::endl;

    auto cement_canvas = new TCanvas("cement_canvas", "Cement Fit");
    cement_canvas->SetCanvasSize(600, 800);
    cement_canvas->Divide(1, 3);
    int j = 1;
    for (int i = 3; i != 6; i++)
    {

        cement_canvas->cd(j);
        gPad->SetLogy();
        gPad->Modified();
        histograms[i]->Draw("E,H");
        j++;
    }
    gPad->Update();

    TPaveStats *stats_cement[3];
    TPaveStats *st1_cement = (TPaveStats *)p1_cement->FindObject("stats");
    stats_cement[0] = st1_cement;
    st1_cement->SetName("p1_cement_stats");
    TPaveStats *st2_cement = (TPaveStats *)p2_cement->FindObject("stats");
    stats_cement[1] = st2_cement;
    st2_cement->SetName("p2_cement_stats");
    TPaveStats *st3_cement = (TPaveStats *)p3_cement->FindObject("stats");
    stats_cement[2] = st3_cement;
    st3_cement->SetName("p3_cement_stats");
    for (int i = 0; i != 3; i++)
    {
        stats_cement[i]->SetX1NDC(0.581633);
        stats_cement[i]->SetY1NDC(0.549202);
        stats_cement[i]->SetX2NDC(0.89966);
        stats_cement[i]->SetY2NDC(0.900266);
    }

    cement_canvas->Print("cement_fit.pdf");
    auto simple_exp_canvas = new TCanvas("simple_exp_canvas", "Fit iron and cement with a simple exponential");
    simple_exp_canvas->Divide(3, 2);

    for (int i = 0; i != 6; i++)
    {
        simple_exp_canvas->cd(i + 1);
        gPad->SetLogy();
        gPad->Modified();
        histograms[i]->Draw("E,H");
    }
}

void filtered_data_fit()
{
    // console colours
    const std::string red("\033[0;31m");
    const std::string cyan("\033[0;36m");
    const std::string reset("\033[0m");

    // style
    gStyle->SetOptTitle(1);
    gStyle->SetOptStat(1110);
    gStyle->SetOptFit(111);

    TFile *file = new TFile("filtered_data.root", "READ");
    TH1D *filtered_histograms[6];

    TH1D *p1_filtered_iron = (TH1D *)file->Get("P1_filtered_iron");
    filtered_histograms[0] = p1_filtered_iron;
    TH1D *p2_filtered_iron = (TH1D *)file->Get("P2_filtered_iron");
    filtered_histograms[1] = p2_filtered_iron;
    TH1D *p3_filtered_iron = (TH1D *)file->Get("P3_filtered_iron");
    filtered_histograms[2] = p3_filtered_iron;

    TH1D *p1_filtered_cement = (TH1D *)file->Get("P1_filtered_cement");
    filtered_histograms[3] = p1_filtered_cement;
    TH1D *p2_filtered_cement = (TH1D *)file->Get("P2_filtered_cement");
    filtered_histograms[4] = p2_filtered_cement;
    TH1D *p3_filtered_cement = (TH1D *)file->Get("P3_filtered_cement");
    filtered_histograms[5] = p3_filtered_cement;

    for (int i = 0; i != 6; i++)
    {
        filtered_histograms[i]->GetXaxis()->SetTitle("Stop time (#mus)");
        filtered_histograms[i]->GetXaxis()->SetTitleSize(0.045);
        filtered_histograms[i]->GetYaxis()->SetTitle("Counts");
        filtered_histograms[i]->GetYaxis()->SetTitleSize(0.045);
    }

    // simple exponential - free parameters: N(u, 0), lifetime and background
    auto simple_exp = new TF1("Simple exp", "[0]*TMath::Exp(-x/[1]) + [2]");
    simple_exp->SetParameters(270, 2, 5);
    simple_exp->SetParNames("N(#mu, 0)", "#tau_{0}", "b");

    ////////////////////////////////////////////////
    /////////////////// IRON ///////////////////////
    ////////////////////////////////////////////////

    // P1 filtered iron fit and parameters
    p1_filtered_iron->Fit(simple_exp, "E,M,Q", "", 1, 12);
    std::cout << red << "Filtered P1 Iron fit parameters:" << reset << '\n';
    std::cout << cyan << simple_exp->GetParameter(1) << " +/- " << simple_exp->GetParError(1) << '\n';
    std::cout << "chi2 / NDF: " << simple_exp->GetChisquare() << "/" << simple_exp->GetNDF() << reset << std::endl;

    // P2 filtered iron fit and parameters
    p2_filtered_iron->Fit(simple_exp, "E,M,Q", "", 1, 12);
    std::cout << red << "Filtered P2 Iron fit parameters:" << reset << '\n';
    std::cout << cyan << simple_exp->GetParameter(1) << " +/- " << simple_exp->GetParError(1) << '\n';
    std::cout << "chi2 / NDF: " << simple_exp->GetChisquare() << "/" << simple_exp->GetNDF() << reset << std::endl;

    // P3 filtered iron fit and parameters
    p3_filtered_iron->Fit(simple_exp, "E,M,Q", "", 0.5, 10);
    std::cout << red << "Filtered P3 Iron fit parameters:" << reset << '\n';
    std::cout << cyan << simple_exp->GetParameter(1) << " +/- " << simple_exp->GetParError(1) << '\n';
    std::cout << "chi2 / NDF: " << simple_exp->GetChisquare() << " / " << simple_exp->GetNDF() << reset << std::endl;

    auto iron_filtered_canvas = new TCanvas("iron_filtered_canvas", "Iron filtered fit");
    iron_filtered_canvas->SetCanvasSize(600, 800);
    iron_filtered_canvas->Divide(1, 3);

    for (int i = 0; i != 3; i++)
    {
        iron_filtered_canvas->cd(i + 1);
        gPad->SetLogy();
        gPad->Modified();
        filtered_histograms[i]->Draw("E,H");
    }
    gPad->Update();

    TPaveStats *stats_iron[3];
    TPaveStats *st1_iron = (TPaveStats *)p1_filtered_iron->FindObject("stats");
    stats_iron[0] = st1_iron;
    st1_iron->SetName("p1_filtered_iron_stats");
    TPaveStats *st2_iron = (TPaveStats *)p2_filtered_iron->FindObject("stats");
    stats_iron[1] = st2_iron;
    st2_iron->SetName("p2_filtered_iron_stats");
    TPaveStats *st3_iron = (TPaveStats *)p3_filtered_iron->FindObject("stats");
    stats_iron[2] = st3_iron;
    st3_iron->SetName("p3_filtered_iron_stats");
    for (int i = 0; i != 3; i++)
    {
        stats_iron[i]->SetX1NDC(0.582809);
        stats_iron[i]->SetY1NDC(0.55001);
        stats_iron[i]->SetX2NDC(0.900615);
        stats_iron[i]->SetY2NDC(0.901298);
    }

    iron_filtered_canvas->Print("iron_filtered_fit.pdf");

    ////////////////////////////////////////////////
    ////////////////// CEMENT //////////////////////
    ////////////////////////////////////////////////

    // P1 filtered cement fit and parameters
    p1_filtered_cement->Fit(simple_exp, "E,M,Q", "", 1, 12);
    std::cout << red << "Filtered P1 Cement fit parameters:" << reset << '\n';
    std::cout << cyan << simple_exp->GetParameter(1) << " +/- " << simple_exp->GetParError(1) << '\n';
    std::cout << "chi2 / NDF: " << simple_exp->GetChisquare() << "/" << simple_exp->GetNDF() << reset << std::endl;

    // P2 filtered cement fit and parameters
    p2_filtered_cement->Fit(simple_exp, "E,M,Q", "", 1, 12);
    std::cout << red << "Filtered P2 Cement fit parameters:" << reset << '\n';
    std::cout << cyan << simple_exp->GetParameter(1) << " +/- " << simple_exp->GetParError(1) << '\n';
    std::cout << "chi2 / NDF: " << simple_exp->GetChisquare() << "/" << simple_exp->GetNDF() << reset << std::endl;

    // P3 filtered cement fit and parameters
    p3_filtered_cement->Fit(simple_exp, "E,M,Q", "", 1, 12);
    std::cout << red << "Filtered P3 Cement fit parameters:" << reset << '\n';
    std::cout << cyan << simple_exp->GetParameter(1) << " +/- " << simple_exp->GetParError(1) << '\n';
    std::cout << "chi2 / NDF: " << simple_exp->GetChisquare() << "/" << simple_exp->GetNDF() << reset << std::endl;

    auto cement_filtered_canvas = new TCanvas("cement_filtered_canvas", "Cement Fit");
    cement_filtered_canvas->SetCanvasSize(600, 800);
    cement_filtered_canvas->Divide(1, 3);
    int j = 1;
    for (int i = 3; i != 6; i++)
    {

        cement_filtered_canvas->cd(j);
        gPad->SetLogy();
        gPad->Modified();
        filtered_histograms[i]->Draw("E,H");
        j++;
    }

    gPad->Update();
    TPaveStats *stats_cement[3];
    TPaveStats *st1_cement = (TPaveStats *)p1_filtered_cement->FindObject("stats");
    stats_cement[0] = st1_cement;
    st1_cement->SetName("p1_filtered_cement_stats");
    TPaveStats *st2_cement = (TPaveStats *)p2_filtered_cement->FindObject("stats");
    stats_cement[1] = st2_cement;
    st2_cement->SetName("p2_filtered_cement_stats");
    TPaveStats *st3_cement = (TPaveStats *)p3_filtered_cement->FindObject("stats");
    stats_cement[2] = st3_cement;
    st3_cement->SetName("p3_filtered_cement_stats");
    for (int i = 0; i != 3; i++)
    {
        stats_cement[i]->SetX1NDC(0.582809);
        stats_cement[i]->SetY1NDC(0.55001);
        stats_cement[i]->SetX2NDC(0.900615);
        stats_cement[i]->SetY2NDC(0.901298);
    }

    cement_filtered_canvas->Print("cement_filtered_fit.pdf");

    auto filtered_canvas = new TCanvas("filtered_canvas", "Filtered iron and cement fit with a simple exponential");
    filtered_canvas->Divide(3, 2);

    for (int i = 0; i != 6; i++)
    {
        filtered_canvas->cd(i + 1);
        filtered_histograms[i]->Draw("E,H");
    }
}

void advanced_fit()
{
    // console colours
    const std::string red("\033[0;31m");
    const std::string cyan("\033[0;36m");
    const std::string reset("\033[0m");

    // style
    gStyle->SetOptTitle(1);
    gStyle->SetOptStat(1110);
    gStyle->SetOptFit(111);

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