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
    TH1D *histograms[8];

    TH1D *p1_iron = (TH1D *)file->Get("P1_iron");
    histograms[0] = p1_iron;
    TH1D *p2_iron = (TH1D *)file->Get("P2_iron");
    histograms[1] = p2_iron;
    TH1D *p3_iron = (TH1D *)file->Get("P3_iron");
    histograms[2] = p3_iron;
    auto all_iron = new TH1D("all_iron", "Cumulative iron", 128, 0, 16);
    histograms[3] = all_iron;
    all_iron->Sumw2();
    all_iron->Add(p1_iron, p2_iron);
    all_iron->Add(p3_iron);

    TH1D *p1_concrete = (TH1D *)file->Get("P1_concrete");
    histograms[4] = p1_concrete;
    TH1D *p2_concrete = (TH1D *)file->Get("P2_concrete");
    histograms[5] = p2_concrete;
    TH1D *p3_concrete = (TH1D *)file->Get("P3_concrete");
    histograms[6] = p3_concrete;
    auto all_concrete = new TH1D("all_concrete", "Cumulative concrete", 128, 0, 16);
    histograms[7] = all_concrete;
    all_concrete->Sumw2();
    all_concrete->Add(p1_concrete, p2_concrete);
    all_concrete->Add(p3_concrete);

    for (int i = 0; i != 8; i++)
    {
        histograms[i]->GetXaxis()->SetTitle("Stop time (#mus)");
        histograms[i]->GetXaxis()->SetTitleSize(0.05);
        histograms[i]->GetYaxis()->SetTitle("Counts");
        histograms[i]->GetYaxis()->SetTitleSize(0.05);
        histograms[i]->GetYaxis()->SetTitleOffset(0.6);
    }

    // simple exponential - free parameters: N(u, 0), lifetime and background
    auto simple_exp = new TF1("Simple exp", "[0]*TMath::Exp(-x/[1]) + [2]");
    simple_exp->SetParameters(270, 2, 5);
    simple_exp->SetParNames("N(#mu, 0)", "#tau_{0}", "b");

    ////////////////////////////////////////////////
    /////////////////// IRON ///////////////////////
    ////////////////////////////////////////////////

    // P1 iron fit
    p1_iron->Fit(simple_exp, "E,M,Q", "", 0.4, 16);

    // P2 iron fit
    p2_iron->Fit(simple_exp, "E,M,Q", "", 0.4, 16);

    // P3 iron fit
    p3_iron->Fit(simple_exp, "E,M,Q", "", 0.4, 16);

    // Cumulative iron fit
    all_iron->Fit(simple_exp, "E,M,Q", "", 0.4, 16);

    auto iron_canvas = new TCanvas("iron_canvas", "Iron Fit");
    iron_canvas->SetCanvasSize(600, 800);
    iron_canvas->Divide(1, 4);

    for (int i = 0; i != 4; i++)
    {
        iron_canvas->cd(i + 1);
        gPad->SetLogy();
        gPad->Modified();
        histograms[i]->Draw("E,H");
    }
    gPad->Update();

    TPaveStats *stats_iron[4];
    TPaveStats *st1_iron = (TPaveStats *)p1_iron->FindObject("stats");
    stats_iron[0] = st1_iron;
    st1_iron->SetName("p1_iron_stats");
    TPaveStats *st2_iron = (TPaveStats *)p2_iron->FindObject("stats");
    stats_iron[1] = st2_iron;
    st2_iron->SetName("p2_iron_stats");
    TPaveStats *st3_iron = (TPaveStats *)p3_iron->FindObject("stats");
    stats_iron[2] = st3_iron;
    st3_iron->SetName("p3_iron_stats");
    TPaveStats *st_cumulative_iron = (TPaveStats *)all_iron->FindObject("stats");
    stats_iron[3] = st_cumulative_iron;
    st_cumulative_iron->SetName("all_iron_stats");
    for (int i = 0; i != 4; i++)
    {
        stats_iron[i]->SetX1NDC(0.581633);
        stats_iron[i]->SetY1NDC(0.549202);
        stats_iron[i]->SetX2NDC(0.89966);
        stats_iron[i]->SetY2NDC(0.900266);
    }

    iron_canvas->Print("iron_fit.pdf");

    ////////////////////////////////////////////////
    ///////////////// CONCRETE /////////////////////
    ////////////////////////////////////////////////

    // P1 concrete fit
    p1_concrete->Fit(simple_exp, "E,M,Q", "", 1, 16);

    // P2 concrete fit
    p2_concrete->Fit(simple_exp, "E,M,Q", "", 1, 16);

    // P3 concrete fit
    p3_concrete->Fit(simple_exp, "E,M,Q", "", 1, 16);

    // Cumulative concrete fit
    all_concrete->Fit(simple_exp, "E,M,Q", "", 1, 16);

    auto concrete_canvas = new TCanvas("concrete_canvas", "concrete Fit");
    concrete_canvas->SetCanvasSize(600, 800);
    concrete_canvas->Divide(1, 4);
    int j = 1;
    for (int i = 4; i != 8; i++)
    {

        concrete_canvas->cd(j);
        gPad->SetLogy();
        gPad->Modified();
        histograms[i]->Draw("E,H");
        j++;
    }
    gPad->Update();

    TPaveStats *stats_concrete[4];
    TPaveStats *st1_concrete = (TPaveStats *)p1_concrete->FindObject("stats");
    stats_concrete[0] = st1_concrete;
    st1_concrete->SetName("p1_concrete_stats");
    TPaveStats *st2_concrete = (TPaveStats *)p2_concrete->FindObject("stats");
    stats_concrete[1] = st2_concrete;
    st2_concrete->SetName("p2_concrete_stats");
    TPaveStats *st3_concrete = (TPaveStats *)p3_concrete->FindObject("stats");
    stats_concrete[2] = st3_concrete;
    st3_concrete->SetName("p3_concrete_stats");
    TPaveStats *st_cumulative_concrete = (TPaveStats *)all_concrete->FindObject("stats");
    stats_concrete[3] = st_cumulative_concrete;
    st_cumulative_concrete->SetName("all_concrete_stats");
    for (int i = 0; i != 4; i++)
    {
        stats_concrete[i]->SetX1NDC(0.581633);
        stats_concrete[i]->SetY1NDC(0.549202);
        stats_concrete[i]->SetX2NDC(0.89966);
        stats_concrete[i]->SetY2NDC(0.900266);
    }

    concrete_canvas->Print("concrete_fit.pdf");

    auto simple_exp_canvas = new TCanvas("simple_exp_canvas", "Fit iron and concrete with a simple exponential");
    simple_exp_canvas->Divide(4, 2);

    for (int i = 0; i != 8; i++)
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
    gStyle->SetLineScalePS(1);

    // retrieve histograms
    TFile *file = new TFile("filtered_data.root", "READ");
    TH1D *filtered_histograms[6];

    TH1D *p1_and_p2_iron = (TH1D *)file->Get("P2_filtered_iron");
    filtered_histograms[0] = p1_and_p2_iron;
    p1_and_p2_iron->SetTitle("P1 and P2 coincidences iron");
    TH1D *p3_filtered_iron = (TH1D *)file->Get("P3_filtered_iron");
    filtered_histograms[1] = p3_filtered_iron;
    auto all_iron_filtered = new TH1D("all_iron_filtered", "Cumulative filtered iron", 128, 0, 16);
    filtered_histograms[2] = all_iron_filtered;
    all_iron_filtered->Sumw2();
    all_iron_filtered->Add(p1_and_p2_iron, p3_filtered_iron);

    TH1D *p1_and_p2_concrete = (TH1D *)file->Get("P2_filtered_concrete");
    filtered_histograms[3] = p1_and_p2_concrete;
    p1_and_p2_concrete->SetTitle("P1 and P2 coincidences concrete");
    TH1D *p3_filtered_concrete = (TH1D *)file->Get("P3_filtered_concrete");
    filtered_histograms[4] = p3_filtered_concrete;
    auto all_concrete_filtered = new TH1D("all_concrete_filtered", "Cumulative filtered concrete", 128, 0, 16);
    filtered_histograms[5] = all_concrete_filtered;
    all_concrete_filtered->Sumw2();
    all_concrete_filtered->Add(p1_and_p2_concrete, p3_filtered_concrete);

    for (int i = 0; i != 6; i++)
    {
        filtered_histograms[i]->GetXaxis()->SetTitle("Stop time (#mus)");
        filtered_histograms[i]->GetXaxis()->SetTitleSize(0.05);
        filtered_histograms[i]->GetYaxis()->SetTitle("Counts");
        filtered_histograms[i]->GetYaxis()->SetTitleSize(0.05);
        filtered_histograms[i]->GetYaxis()->SetTitleOffset(0.6);
    }

    // simple exponential - free parameters: N(u, 0), lifetime and background
    auto simple_exp = new TF1("Simple exp", "[0]*TMath::Exp(-x/[1]) + [2]");
    simple_exp->SetParameters(270, 2, 5);
    simple_exp->SetParNames("N(#mu, 0)", "#tau_{0}", "b");

    ////////////////////////////////////////////////
    /////////////////// IRON ///////////////////////
    ////////////////////////////////////////////////

    // P1 and P2 coincidences iron fit
    p1_and_p2_iron->Fit(simple_exp, "E,M,Q", "", 0.4, 16);

    // P3 filtered iron fit
    p3_filtered_iron->Fit(simple_exp, "E,M,Q", "", 0.4, 16);

    // Cumulative filtered iron fit
    all_iron_filtered->Fit(simple_exp, "E,M,Q", "", 0.4, 16);

    gPad->Update();
    TPaveStats *stats_iron[3];
    TPaveStats *st1_iron = (TPaveStats *)p1_and_p2_iron->FindObject("stats");
    stats_iron[0] = st1_iron;
    st1_iron->SetName("p1_and_p2_iron_stats");
    TPaveStats *st3_iron = (TPaveStats *)p3_filtered_iron->FindObject("stats");
    stats_iron[1] = st3_iron;
    st3_iron->SetName("p3_filtered_iron_stats");
    TPaveStats *st_all_iron_filtered = (TPaveStats *)all_iron_filtered->FindObject("stats");
    stats_iron[2] = st_all_iron_filtered;
    st_all_iron_filtered->SetName("all_iron_filtered_stats");

    for (int i = 0; i != 3; i++)
    {
        stats_iron[i]->SetX1NDC(0.582544);
        stats_iron[i]->SetY1NDC(0.545131);
        stats_iron[i]->SetX2NDC(0.901034);
        stats_iron[i]->SetY2NDC(0.901298);
    }

    ////////////////////////////////////////////////
    ///////////////// CONCRETE /////////////////////
    ////////////////////////////////////////////////

    // P1 and P2 coincidences concrete fit
    p1_and_p2_concrete->Fit(simple_exp, "E,M,Q", "", 1, 16);

    // P3 filtered concrete fit
    p3_filtered_concrete->Fit(simple_exp, "E,M,Q", "", 1, 16);

    // Cumulative concrete fit
    all_concrete_filtered->Fit(simple_exp, "E,M,Q", "", 1, 16);

    gPad->Update();
    TPaveStats *stats_concrete[3];
    TPaveStats *st1_concrete = (TPaveStats *)p1_and_p2_concrete->FindObject("stats");
    stats_concrete[0] = st1_concrete;
    st1_concrete->SetName("p1_and_p2_concrete_stats");
    TPaveStats *st3_concrete = (TPaveStats *)p3_filtered_concrete->FindObject("stats");
    stats_concrete[1] = st3_concrete;
    st3_concrete->SetName("p3_filtered_concrete_stats");
    TPaveStats *st_all_concrete_filtered = (TPaveStats *)all_concrete_filtered->FindObject("stats");
    stats_concrete[2] = st_all_concrete_filtered;
    st_all_concrete_filtered->SetName("all_concrete_filtered_stats");

    for (int i = 0; i != 3; i++)
    {
        stats_concrete[i]->SetX1NDC(0.582544);
        stats_concrete[i]->SetY1NDC(0.545131);
        stats_concrete[i]->SetX2NDC(0.901034);
        stats_concrete[i]->SetY2NDC(0.901298);
    }

    // Visualize all histograms
    auto filtered_canvas = new TCanvas("filtered_canvas", "Filtered iron and concrete fit with a simple exponential");
    filtered_canvas->Divide(3, 2);
    for (int i = 0; i != 6; i++)
    {
        filtered_canvas->cd(i + 1);
        filtered_histograms[i]->Draw("E,H");
    }

    auto iron_filtered_canvas = new TCanvas("iron_filtered_canvas", "Iron filtered fit - simple exponential");
    iron_filtered_canvas->Divide(1, 3);
    for (int i = 0; i != 3; i++)
    {
        iron_filtered_canvas->cd(i + 1);
        filtered_histograms[i]->Draw("E,H");
    }

    auto concrete_filtered_canvas = new TCanvas("concrete_filtered_canvas", "concrete filtered fit - simple exponential");
    concrete_filtered_canvas->Divide(1, 3);
    int j = 1;
    for (int i = 3; i != 6; i++)
    {
        concrete_filtered_canvas->cd(j);
        filtered_histograms[i]->Draw("E,H");
        j++;
    }

    iron_filtered_canvas->Print("iron_filtered_fit.pdf");
    concrete_filtered_canvas->Print("concrete_filtered_fit.pdf");
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
    gStyle->SetLineScalePS(1);

    // retrieve histograms
    TFile *file = new TFile("filtered_data.root", "READ");
    TH1D *filtered_histograms[6];

    TH1D *p1_and_p2_iron = (TH1D *)file->Get("P2_filtered_iron");
    filtered_histograms[0] = p1_and_p2_iron;
    p1_and_p2_iron->SetTitle("P1 and P2 coincidences iron");
    TH1D *p3_filtered_iron = (TH1D *)file->Get("P3_filtered_iron");
    filtered_histograms[1] = p3_filtered_iron;
    auto all_iron_filtered = new TH1D("all_iron_filtered", "Cumulative filtered iron", 128, 0, 16);
    filtered_histograms[2] = all_iron_filtered;
    all_iron_filtered->Sumw2();
    all_iron_filtered->Add(p1_and_p2_iron, p3_filtered_iron);

    TH1D *p1_and_p2_concrete = (TH1D *)file->Get("P2_filtered_concrete");
    filtered_histograms[3] = p1_and_p2_concrete;
    p1_and_p2_concrete->SetTitle("P1 and P2 coincidences concrete");
    TH1D *p3_filtered_concrete = (TH1D *)file->Get("P3_filtered_concrete");
    filtered_histograms[4] = p3_filtered_concrete;
    auto all_concrete_filtered = new TH1D("all_concrete_filtered", "Cumulative filtered concrete", 128, 0, 16);
    filtered_histograms[5] = all_concrete_filtered;
    all_concrete_filtered->Sumw2();
    all_concrete_filtered->Add(p1_and_p2_concrete, p3_filtered_concrete);

    for (int i = 0; i != 6; i++)
    {
        filtered_histograms[i]->GetXaxis()->SetTitle("Stop time (#mus)");
        filtered_histograms[i]->GetXaxis()->SetTitleSize(0.05);
        filtered_histograms[i]->GetYaxis()->SetTitle("Counts");
        filtered_histograms[i]->GetYaxis()->SetTitleSize(0.05);
        filtered_histograms[i]->GetYaxis()->SetTitleOffset(0.6);
    }

    // 1/R exponential -> free parameters: N(u+, 0), lifetime, charge rate R, capture time, background
    auto adv_exp = new TF1("adv_exp", "[0]*TMath::Exp(-x/[1])*(1 + 1/[2]*TMath::Exp(-x/[3])) + [4]");
    adv_exp->SetParameters(200, 2, 1, 0.2, 6);
    adv_exp->SetParNames("N(#mu^{+}, 0)", "#tau_{0}", "R", "#tau_{c}", "b");

    ////////////////////////////////////////////////
    /////////////////// IRON ///////////////////////
    ////////////////////////////////////////////////

    // P1 and P2 coincidences iron fit
    p1_and_p2_iron->Fit(adv_exp, "E,M,Q", "", 0.07, 16);

    // P3 filtered iron fit
    p3_filtered_iron->Fit(adv_exp, "E,M,Q", "", 0.07, 16);

    // Cumulative filtered iron fit
    all_iron_filtered->Fit(adv_exp, "E,M,Q", "", 0.07, 16);

    gPad->Update();
    TPaveStats *stats_iron[3];
    TPaveStats *st1_iron = (TPaveStats *)p1_and_p2_iron->FindObject("stats");
    stats_iron[0] = st1_iron;
    st1_iron->SetName("p1_and_p2_iron_stats");
    TPaveStats *st3_iron = (TPaveStats *)p3_filtered_iron->FindObject("stats");
    stats_iron[1] = st3_iron;
    st3_iron->SetName("p3_filtered_iron_stats");
    TPaveStats *st_all_iron_filtered = (TPaveStats *)all_iron_filtered->FindObject("stats");
    stats_iron[2] = st_all_iron_filtered;
    st_all_iron_filtered->SetName("all_iron_filtered_stats");
    for (int i = 0; i != 3; i++)
    {
        stats_iron[i]->SetX1NDC(0.582544);
        stats_iron[i]->SetY1NDC(0.545131);
        stats_iron[i]->SetX2NDC(0.901034);
        stats_iron[i]->SetY2NDC(0.901298);
    }

    ////////////////////////////////////////////////
    ///////////////// CONCRETE /////////////////////
    ////////////////////////////////////////////////

    // P1 and P2 coincidences concrete fit
    p1_and_p2_concrete->Fit(adv_exp, "E,M,Q", "", 0.1, 16);

    // P3 filtered concrete fit
    p3_filtered_concrete->Fit(adv_exp, "E,M,Q", "", 0.1, 16);

    // Cumulative concrete filtered fit
    all_concrete_filtered->Fit(adv_exp, "E,M,Q", "", 0.1, 16);

    gPad->Update();
    TPaveStats *stats_concrete[3];
    TPaveStats *st1_concrete = (TPaveStats *)p1_and_p2_concrete->FindObject("stats");
    stats_concrete[0] = st1_concrete;
    st1_concrete->SetName("p1_and_p2_concrete_stats");
    TPaveStats *st3_concrete = (TPaveStats *)p3_filtered_concrete->FindObject("stats");
    stats_concrete[1] = st3_concrete;
    st3_concrete->SetName("p3_filtered_concrete_stats");
    TPaveStats *st_all_concrete_filtered = (TPaveStats *)all_concrete_filtered->FindObject("stats");
    stats_concrete[2] = st_all_concrete_filtered;
    st_all_concrete_filtered->SetName("all_iron_filtered_stats");
    for (int i = 0; i != 3; i++)
    {
        stats_concrete[i]->SetX1NDC(0.582544);
        stats_concrete[i]->SetY1NDC(0.545131);
        stats_concrete[i]->SetX2NDC(0.901034);
        stats_concrete[i]->SetY2NDC(0.901298);
    }

    // Visualize all histograms
    auto filtered_canvas = new TCanvas("filtered_canvas", "Filtered iron and concrete fit with with 1/R exponential");
    filtered_canvas->Divide(3, 2);
    for (int i = 0; i != 6; i++)
    {
        filtered_canvas->cd(i + 1);
        filtered_histograms[i]->Draw("E,H");
    }

    auto iron_filtered_canvas = new TCanvas("iron_filtered_canvas", "Iron filtered fit - 1/R exponential fit");
    iron_filtered_canvas->SetCanvasSize(600, 800);
    iron_filtered_canvas->Divide(1, 3);
    for (int i = 0; i != 3; i++)
    {
        iron_filtered_canvas->cd(i + 1);
        filtered_histograms[i]->Draw("E,H");
    }

    auto concrete_filtered_canvas = new TCanvas("concrete_filtered_canvas", "concrete filtered - 1/R exponential fit");
    concrete_filtered_canvas->SetCanvasSize(600, 800);
    concrete_filtered_canvas->Divide(1, 3);
    int j = 1;
    for (int i = 3; i != 6; i++)
    {
        concrete_filtered_canvas->cd(j);
        filtered_histograms[i]->Draw("E,H");
        j++;
    }

    iron_filtered_canvas->Print("1_over_r_iron_fit.pdf");
    concrete_filtered_canvas->Print("1_over_r_concrete_fit.pdf");
}

void fixed_lifetime_fit()
{
    // console colours
    const std::string red("\033[0;31m");
    const std::string cyan("\033[0;36m");
    const std::string reset("\033[0m");

    // style
    gStyle->SetOptTitle(1);
    gStyle->SetOptStat(1110);
    gStyle->SetOptFit(111);
    gStyle->SetLineScalePS(1);

    // retrieve histograms
    TFile *file = new TFile("filtered_data.root", "READ");
    TH1D *filtered_histograms[6];

    TH1D *p1_and_p2_iron = (TH1D *)file->Get("P2_filtered_iron");
    filtered_histograms[0] = p1_and_p2_iron;
    p1_and_p2_iron->SetTitle("P1 and P2 coincidences iron");
    TH1D *p3_filtered_iron = (TH1D *)file->Get("P3_filtered_iron");
    filtered_histograms[1] = p3_filtered_iron;
    auto all_iron_filtered = new TH1D("all_iron_filtered", "Cumulative filtered iron", 128, 0, 16);
    filtered_histograms[2] = all_iron_filtered;
    all_iron_filtered->Sumw2();
    all_iron_filtered->Add(p1_and_p2_iron, p3_filtered_iron);

    TH1D *p1_and_p2_concrete = (TH1D *)file->Get("P2_filtered_concrete");
    filtered_histograms[3] = p1_and_p2_concrete;
    p1_and_p2_concrete->SetTitle("P1 and P2 coincidences concrete");
    TH1D *p3_filtered_concrete = (TH1D *)file->Get("P3_filtered_concrete");
    filtered_histograms[4] = p3_filtered_concrete;
    auto all_concrete_filtered = new TH1D("all_concrete_filtered", "Cumulative filtered concrete", 128, 0, 16);
    filtered_histograms[5] = all_concrete_filtered;
    all_concrete_filtered->Sumw2();
    all_concrete_filtered->Add(p1_and_p2_concrete, p3_filtered_concrete);

    for (int i = 0; i != 6; i++)
    {
        filtered_histograms[i]->GetXaxis()->SetTitle("Stop time (#mus)");
        filtered_histograms[i]->GetXaxis()->SetTitleSize(0.05);
        filtered_histograms[i]->GetYaxis()->SetTitle("Counts");
        filtered_histograms[i]->GetYaxis()->SetTitleSize(0.05);
        filtered_histograms[i]->GetYaxis()->SetTitleOffset(0.6);
    }

    // 1/R exponential -> free parameters: N(u+, 0), lifetime, charge rate R, capture time, background
    auto adv_exp = new TF1("adv_exp", "[0]*TMath::Exp(-x/[1])*(1 + 1/[2]*TMath::Exp(-x/[3])) + [4]");
    adv_exp->SetParNames("N(#mu^{+}, 0)", "#tau_{0}", "R", "#tau_{c}", "b");

    ////////////////////////////////////////////////
    /////////////////// IRON ///////////////////////
    ////////////////////////////////////////////////

    // P1 and P2 coincidences iron fit
    adv_exp->SetParameters(300, 2, 1, 0.2, 6);
    adv_exp->FixParameter(1, 2.197);
    p1_and_p2_iron->Fit(adv_exp, "E,M,Q", "", 0.07, 16);

    // P3 filtered iron fit
    adv_exp->SetParameters(300, 2, 1, 0.2, 6);
    adv_exp->FixParameter(1, 2.197);
    p3_filtered_iron->Fit(adv_exp, "E,M,Q", "", 0.07, 16);

    // Cumulative filtered iron fit
    adv_exp->SetParameters(300, 2, 1, 0.2, 6);
    adv_exp->FixParameter(1, 2.197);
    all_iron_filtered->Fit(adv_exp, "E,M,Q", "", 0.07, 16);

    gPad->Update();
    TPaveStats *stats_iron[3];
    TPaveStats *st1_iron = (TPaveStats *)p1_and_p2_iron->FindObject("stats");
    stats_iron[0] = st1_iron;
    st1_iron->SetName("p1_and_p2_iron_stats");
    TPaveStats *st3_iron = (TPaveStats *)p3_filtered_iron->FindObject("stats");
    stats_iron[1] = st3_iron;
    st3_iron->SetName("p3_filtered_iron_stats");
    TPaveStats *st_all_iron_filtered = (TPaveStats *)all_iron_filtered->FindObject("stats");
    stats_iron[2] = st_all_iron_filtered;
    st_all_iron_filtered->SetName("all_iron_filtered_stats");
    for (int i = 0; i != 3; i++)
    {
        stats_iron[i]->SetX1NDC(0.582544);
        stats_iron[i]->SetY1NDC(0.545131);
        stats_iron[i]->SetX2NDC(0.901034);
        stats_iron[i]->SetY2NDC(0.901298);
    }

    ////////////////////////////////////////////////
    ///////////////// CONCRETE /////////////////////
    ////////////////////////////////////////////////

    // P1 and P2 coincidences concrete fit
    adv_exp->SetParameters(300, 2, 1, 1, 6);
    adv_exp->FixParameter(1, 2.197);
    p1_and_p2_concrete->Fit(adv_exp, "E,M,Q", "", 0.1, 16);

    // P3 filtered concrete fit
    adv_exp->SetParameters(300, 2, 1, 1, 6);
    adv_exp->FixParameter(1, 2.197);
    p3_filtered_concrete->Fit(adv_exp, "E,M,V", "", 0.1, 16);

    // Cumulative filtered concrete fit
    adv_exp->SetParameters(300, 2, 1, 1, 6);
    adv_exp->FixParameter(1, 2.197);
    all_concrete_filtered->Fit(adv_exp, "E,M,V", "", 0.1, 16);

    gPad->Update();
    TPaveStats *stats_concrete[3];
    TPaveStats *st1_concrete = (TPaveStats *)p1_and_p2_concrete->FindObject("stats");
    stats_concrete[0] = st1_concrete;
    st1_concrete->SetName("p1_and_p2_concrete_stats");
    TPaveStats *st3_concrete = (TPaveStats *)p3_filtered_concrete->FindObject("stats");
    stats_concrete[1] = st3_concrete;
    st3_concrete->SetName("p3_filtered_concrete_stats");
    TPaveStats *st_all_concrete_filtered = (TPaveStats *)all_concrete_filtered->FindObject("stats");
    stats_concrete[2] = st_all_concrete_filtered;
    st_all_concrete_filtered->SetName("all_concrete_filtered_stats");
    for (int i = 0; i != 3; i++)
    {
        stats_concrete[i]->SetX1NDC(0.582544);
        stats_concrete[i]->SetY1NDC(0.545131);
        stats_concrete[i]->SetX2NDC(0.901034);
        stats_concrete[i]->SetY2NDC(0.901298);
    }

    // Visualize all histograms
    auto filtered_canvas = new TCanvas("filtered_canvas", "Filtered iron and concrete fit with with 1/R exponential");
    filtered_canvas->Divide(3, 2);
    for (int i = 0; i != 6; i++)
    {
        filtered_canvas->cd(i + 1);
        filtered_histograms[i]->Draw("E,H");
    }

    auto iron_filtered_canvas = new TCanvas("iron_filtered_canvas", "Iron filtered fit - 1/R exponential fit");
    iron_filtered_canvas->SetCanvasSize(800, 600);
    iron_filtered_canvas->Divide(1, 3);
    for (int i = 0; i != 3; i++)
    {
        iron_filtered_canvas->cd(i + 1);
        filtered_histograms[i]->Draw("E,H");
    }

    auto concrete_filtered_canvas = new TCanvas("concrete_filtered_canvas", "concrete filtered - 1/R exponential fit");
    concrete_filtered_canvas->SetCanvasSize(800, 600);
    concrete_filtered_canvas->Divide(1, 3);
    int j = 1;
    for (int i = 3; i != 6; i++)
    {
        concrete_filtered_canvas->cd(j);
        filtered_histograms[i]->Draw("E,H");
        j++;
    }

    iron_filtered_canvas->Print("fixed_lifetime_fit_iron.pdf");
    concrete_filtered_canvas->Print("fixed_lifetime_fit_concrete.pdf");
}