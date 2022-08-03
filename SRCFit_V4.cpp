#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "TGraph.h"
#include "string"
#include "TGraphErrors.h"
using namespace std;

double myhist_ratio(TString SPE_run, TString SPE_ch, TString SRC_run, TString SRC_ch){
    //create a histogram
    TFile *f0 = TFile::Open(SPE_run+ ".root");
    TFile *f1 = TFile::Open(SRC_run+".root");
    TTree * spe= (TTree *) f0->Get("Events");
    TTree * mod_spe= (TTree *) f0->Get("Events");
    TTree * src= (TTree *) f1->Get("Events");
    src->Draw("vMax_3044_" +SPE_ch +">>file1(200,0,940)", "width_3044_"+SPE_ch+">70", "");
    spe->Draw("vMax_3044_"+SRC_ch+">>file0(200,0,940)", "width_3044_"+SRC_ch+">70", "");
    TH1F * hist_spe = (TH1F *)  gDirectory->Get("file0");
    TH1F * hist_spr = (TH1F *)  gDirectory->Get("file1");

    //find the bin value of the maximum of each
    double spe_binmax = hist_spe->GetBinContent(hist_spe->GetMaximumBin()); //this is roughly the spe peak location
    double spr_binmax = hist_spr->GetBinContent(hist_spr->GetMaximumBin()); //this is roughly the spe peak location
    double my_scale = spr_binmax/spe_binmax;


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
    low_bound.Form("%d", 940/200*(low_bin));
    TString high_bound;
    high_bound.Form("%d", 940/200*(high_bin));
    mod_spe->Draw("vMax_3044_"+SPE_ch+">>file2(25,"+low_bound+","+high_bound+")", "width_3044_"+SPE_ch+">70", "");
    TH1F * hist_mod_spe = (TH1F *)  gDirectory->Get("file2");

    cout << "\n\n\n NO SOURCE INFORMATION\n";
    hist_mod_spe->Fit("gaus");

    //scale the spe histogram and create a new one which is the subtracted version
    hist_spe->Scale(-my_scale);
    hist_spr->Add(hist_spe);

    //Find the lower bound
    current_bin = hist_spr->GetMaximumBin()*2; // temp variable
    bool continue_loop = true;


    //find the vMax with brute force to spite Isaac Newton
    double totaldif = 0.0;
    double dif(0);
    int nbins = hist_spr->GetXaxis()->GetNbins();
    for (int i = 0; i < nbins; i++){
        dif = hist_spr->GetBinContent(i);
	    if(dif < 0){
            hist_spr->SetBinContent(i,0);
        }
    }
    
    double my_mean = hist_spr->GetMean();
    hist_spr->SetAxisRange(my_mean/2.5,my_mean*1.75);

    //old code

    /*
    while(current_bin > 0 && continue_loop){
        current_bin -= 1;
        if(hist_spr->GetBinContent(current_bin)<0){
            continue_loop= false;
        }
    }
    double spr_low_bound = current_bin*hist_spr->GetBinWidth(4);
    
    //Find the high bound
    current_bin = hist_spr->GetMaximumBin()*4; // temp variable
    continue_loop = true;
    while(current_bin < 150 && continue_loop){
        current_bin++;
        if(hist_spr->GetBinContent(current_bin)<0){
            continue_loop= false;
        }
    }
    double spr_high_bound = current_bin*hist_spr->GetBinWidth(4);
    cout << spr_high_bound;

    //draw new histogram with set perameters
    cout <<current_bin << "\n\n\n";
    hist_spr->SetAxisRange(spr_low_bound,spr_high_bound);

    */

    //print values
    cout << "\n\n\n CD109 SOURCE INFORMATION\n";
    hist_spr->Fit("gaus");
    //double my_mean = hist_spr->GetFunction("gaus")->GetParameter(1);
    return hist_spr->GetFunction("gaus")->GetParameter(1);

}


double SRCFit_V4(TString SPE_run, TString SPE_ch, TString SRC_run, TString SRC_ch){
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
    return myhist_ratio(SPE_run, SPE_ch, SRC_run, SRC_ch);
}
