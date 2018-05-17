/*
  Author: Rourke Sekelsky
  This macro creates a reconstructed angle vs. momentum plot for each particle (if you include it in the multigraph).
  This takes a file with varying momentum values, preferably multiple fixed-momentum evaluators merged into a single file.
*/

int ptheta_dist(){

  // Input file //
  // From RICH evaluator //
  TString file_name_1 = "/sphenix/user/sekelsky/particle_simulations/electron_nofield/emission_evals/allmerged.root";
  TFile* root_file_1 = new TFile(file_name_1,"OPEN");
  TNtuple* tree_1 = (TNtuple*)root_file_1->Get("eval_rich_small");

  // Input file 2 //
  // From RICH evaluator //
  TString file_name_2 = "/sphenix/user/sekelsky/particle_simulations/pion_nofield/emission_evals/allmerged.root";
  TFile* root_file_2 = new TFile(file_name_2,"OPEN");
  TNtuple* tree_2 = (TNtuple*)root_file_2->Get("eval_rich_small");

  // Input file 3 //
  // From RICH evaluator //
  TString file_name_3 = "/sphenix/user/sekelsky/particle_simulations/kaon_nofield/emission_evals/allmerged.root";
  TFile* root_file_3 = new TFile(file_name_3,"OPEN");
  TNtuple* tree_3 = (TNtuple*)root_file_3->Get("eval_rich_small");

  // Input file 4 //
  // From RICH evaluator //
  TString file_name_4 = "/sphenix/user/sekelsky/particle_simulations/proton_nofield/emission_evals/allmerged.root";
  TFile* root_file_4 = new TFile(file_name_4,"OPEN");
  TNtuple* tree_4 = (TNtuple*)root_file_4->Get("eval_rich_small");


  TCanvas *c1 = new TCanvas();

  // Create first plot  //
  c1->cd();

  const Int_t N = tree_1->GetEntries();
  Double_t x1, x2, x3;
  Double_t ptot1[N], tmean1[N], trms1[N];
  Double_t y1, y2, y3;
  Double_t ptot2[N], tmean2[N], trms2[N];
  Double_t z1, z2, z3;
  Double_t ptot3[N], tmean3[N], trms3[N];
  Double_t w1, w2, w3;
  Double_t ptot4[N], tmean4[N], trms4[N];
   
  tree_1->SetBranchAddress("mptot",&x1);
  tree_1->SetBranchAddress("theta_mean",&x2);
  tree_1->SetBranchAddress("theta_rms",&x3);

  tree_2->SetBranchAddress("mptot",&y1);
  tree_2->SetBranchAddress("theta_mean",&y2);
  tree_2->SetBranchAddress("theta_rms",&y3);

  tree_3->SetBranchAddress("mptot",&z1);
  tree_3->SetBranchAddress("theta_mean",&z2);
  tree_3->SetBranchAddress("theta_rms",&z3);

  tree_4->SetBranchAddress("mptot",&w1);
  tree_4->SetBranchAddress("theta_mean",&w2);
  tree_4->SetBranchAddress("theta_rms",&w3);


  for( int i=0; i<N; i++ ){
    tree_1->GetEntry(i);
    tree_2->GetEntry(i);
    tree_3->GetEntry(i);
    tree_4->GetEntry(i);
    
    ptot1[i] = x1;
    tmean1[i] = x2;
    trms1[i] = x3;
    
    ptot2[i] = y1;
    tmean2[i] = y2;
    trms2[i] = y3;
    
    ptot3[i] = z1;
    tmean3[i] = z2;
    trms3[i] = z3;
    
    ptot4[i] = w1;
    tmean4[i] = w2;
    trms4[i] = w3;
    
  }

  gr1 = new TGraphErrors(N,ptot1,tmean1,0,trms1);
  gr1->SetMarkerSize(0.8);
  gr1->SetMarkerStyle(3);
  gr1->SetMarkerColor(kRed);
  gr1->SetLineColor(45);

  gr2 = new TGraphErrors(N,ptot2,tmean2,0,trms2);
  gr2->SetMarkerSize(0.8);
  gr2->SetMarkerStyle(3);
  gr2->SetMarkerColor(kBlue);
  gr2->SetLineColor(38);

  gr3 = new TGraphErrors(N,ptot3,tmean3,0,trms3);
  gr3->SetMarkerSize(0.8);
  gr3->SetMarkerStyle(3);
  gr3->SetMarkerColor(kGreen);
  gr3->SetLineColor(kGreen-7);

  gr4 = new TGraphErrors(N,ptot4,tmean4,0,trms4);
  gr4->SetMarkerSize(0.8);
  gr4->SetMarkerStyle(3);
  gr4->SetMarkerColor(kOrange-3);
  gr4->SetLineColor(kOrange);


  mg = new TMultiGraph();
  //  mg->Add(gr1);
  mg->Add(gr2);
  mg->Add(gr3);
  mg->Add(gr4);

  mg->Draw("AP");
  mg->GetXaxis()->SetLimits(0,80);
  mg->GetYaxis()->SetRangeUser(0,0.035);
  mg->GetXaxis()->SetTitle("Momentum [GeV]");
  mg->GetYaxis()->SetTitle("Cherenkov angle [rad]");


  legend = new TLegend(0.5,0.3,0.9,0.5);
  //legend->AddEntry(gr1,"e- Reconstructed Momentum","lep");
  legend->AddEntry(gr2,"#pi- Emission Momentum","lep");
  legend->AddEntry(gr3,"K- Emission Momentum","lep");
  legend->AddEntry(gr4,"p Emission Momentum","lep");
  legend->Draw();

  c1->Update();
  c1->Print("ptheta_dist.pdf","pdf");

  return 0;
}
