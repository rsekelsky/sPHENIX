/*
  Author: Rourke Sekelsky
  This code plots the reconstructed angles for multiple simulations at a time.
  This would also work with the mean angle in every event, provided the mean angle is contained in the file.
  
  Make sure you use the right file paths!
  Make sure if you comment out a particle you use the right histogram to change the axes labels!
*/

#include <cmath>

int multiple_theta_reco(){

  //// Input files ////
  /*TString file_name_1 = "/sphenix/user/sekelsky/particle_simulations/electron_nofield/evals/eval_p30.root";
  TFile* root_file_1 = new TFile(file_name_1,"OPEN");
  TNtuple* tree_1 = (TNtuple*)root_file_1->Get("eval_rich");
  */
  TString file_name_2 = "/sphenix/user/sekelsky/particle_simulations/pion_nofield/emission_evals/eval_p60.root"; 
  TFile* root_file_2 = new TFile(file_name_2,"OPEN");
  TNtuple* tree_2 = (TNtuple*)root_file_2->Get("eval_rich");
  
  TString file_name_3 = "/sphenix/user/sekelsky/particle_simulations/kaon_nofield/emission_evals/eval_p60.root"; 
  TFile* root_file_3 = new TFile(file_name_3,"OPEN");
  TNtuple* tree_3 = (TNtuple*)root_file_3->Get("eval_rich");
  
  TString file_name_4 = "/sphenix/user/sekelsky/particle_simulations/proton_nofield/emission_evals/eval_p60.root"; 
  TFile* root_file_4 = new TFile(file_name_4,"OPEN");
  TNtuple* tree_4 = (TNtuple*)root_file_4->Get("eval_rich");
  
  TCanvas *c1 = new TCanvas();
  TCanvas *c2 = new TCanvas();

  // Create plot //
  c1->cd();
  /*
  TH1F* h1 = new TH1F("h1","Reconstructed Angles (p = 50 GeV, #eta = 2.0)",200,0.01,0.04);
  h1->SetLineColor(kRed);
  tree_1->Draw("theta_reco>>h1","theta_reco>0 && theta_reco<0.04","");
  */
  TH1F* h2 = new TH1F("h2","Reconstructed Angles (p = 50 GeV, #eta = 2.0)",200,0.0,0.04);
  h2->SetLineColor(kBlue); 
  tree_2->Draw("theta_reco>>h2","theta_reco>0 && theta_reco<0.04","same");
  
  TH1F* h3 = new TH1F("h3","Reconstructed Angles (p = 50 GeV, #eta = 2.0)",200,0.0,0.04);
  h3->SetLineColor(kGreen);
  tree_3->Draw("theta_reco>>h3","theta_reco>0 && theta_reco<0.04","same");
  
  TH1F* h4 = new TH1F("h4","Reconstructed Angles (p = 50 GeV, #eta = 2.0)",200,0.0,0.04);
  h4->SetLineColor(kBlack);  
  tree_4->Draw("theta_reco>>h4","theta_reco>0 && theta_reco<0.04","same");
  
  h2->GetXaxis()->SetTitle("Cherenkov Angle [rad]");
  h2->GetYaxis()->SetTitle("Counts");

  TLegend* legend = new TLegend(0.25,0.5,0.55,0.8);
  legend->SetHeader("60 GeV Momentum");

  //legend->AddEntry(h1,"Electron Reco","l");
  legend->AddEntry(h2,"#pi- Emission Momentum","l");
  legend->AddEntry(h3,"K- Emission Momentum","l");
  legend->AddEntry(h4,"p Emission Momentum","l");
  legend->Draw();

  c1->Print("theta_plot.pdf","pdf");
  
  return 0;
}
