#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "TGraph.h"
#include "string"
#include "TGraphErrors.h"
using namespace std;

double myhist_ratio(TString SPE_run, TString SPE_ch){
    //create a histogram
    TFile *f0 = TFile::Open(SPE_run+ ".root");
    TTree * spe= (TTree *) f0->Get("Events");
    TTree * mod_spe= (TTree *) f0->Get("Events");
    spe->Draw("vMax_3044_"+SPE_ch+">>file0(200,0,940)", "width_3044_"+SPE_ch+">70", "");
    TH1F * hist_spe = (TH1F *)  gDirectory->Get("file0");

    //find the bin value of the maximum
    double spe_binmax = hist_spe->GetBinContent(hist_spe->GetMaximumBin()); //this is roughly the spe peak location

    //finding min bin
    int current_bin = 0;
    int low_bin = 2;
    while(current_bin < 200 and low_bin==0){
        current_bin++;
        if(hist_spe->GetBinContent(current_bin)> spe_binmax/2){
            low_bin=current_bin;
        }
    }

    int high_bin=0;
    current_bin = hist_spe->GetMaximumBin();

    while(current_bin <200 and high_bin==0){
        current_bin++;
        if(hist_spe->GetBinContent(current_bin)<spe_binmax/2){
            high_bin=current_bin;
        }
    }

    //get bin cut to desired format
    TString low_bound;
    low_bound.Form("%d", 940/200*(low_bin)); //this 940/200 just sets the bin size
    TString high_bound;
    high_bound.Form("%d", 940/200*(high_bin)); 
    mod_spe->Draw("vMax_3044_"+SPE_ch+">>file2(25,"+low_bound+","+high_bound+")", "width_3044_"+SPE_ch+">70", "");
    TH1F * hist_mod_spe = (TH1F *)  gDirectory->Get("file2");

    cout << "\n\n\n NO SOURCE INFORMATION\n";
    hist_mod_spe->Fit("gaus");

    //scale the spe histogram and create a new one which is the subtracted version

    return hist_mod_spe->GetFunction("gaus")->GetParameter(1);;
}


double SPEFit(TString SPE_run, TString SPE_ch){
    //create a Canvas: nessasay to have graph
    TCanvas *c1 = new TCanvas("c1", "c1",18,41,700,500);
    c1->Range(-63125,-10472.57,318125,94253.12);
    c1->SetFillColor(0);
    c1->SetBorderMode(0);
    c1->SetBorderSize(2);
    c1->SetFrameBorderMode(0);
    c1->SetFrameBorderMode(0);
    gStyle->SetOptStat(1);

    //find the ratio
    return myhist_ratio(SPE_run, SPE_ch);
}
