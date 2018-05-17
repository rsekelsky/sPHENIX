/*
  Author: Rourke Sekelsky
  This code is similar to ptheta_dist.C, except it takes the average of the means that were the data points, and changes the error bars into the standard
  deviation of these means.
  This one is a little more user-friendly.
*/

int ptheta_dist_mean(){

  bool draw_electron = false;
  bool draw_pion = true;
  bool draw_kaon = true;
  bool draw_proton = true;
  
  bool draw_theory = true;

  // Input file //
  // From RICH evaluator, particle 1 //
  TString file_name_1 = "/sphenix/user/sekelsky/particle_simulations/electron_nofield/evals/allmerged.root";
  TFile* root_file_1 = new TFile(file_name_1,"OPEN");
  TNtuple* tree_1 = (TNtuple*)root_file_1->Get("eval_rich_small");

  // Input file 2 //
  // From RICH evaluator, particle 2 //
  TString file_name_2 = "/sphenix/user/sekelsky/particle_simulations/pion_nofield/evals/allmerged.root";
  TFile* root_file_2 = new TFile(file_name_2,"OPEN");
  TNtuple* tree_2 = (TNtuple*)root_file_2->Get("eval_rich_small");

  // Input file 3 //
  // From RICH evaluator //
  TString file_name_3 = "/sphenix/user/sekelsky/particle_simulations/kaon_nofield/evals/allmerged.root";
  TFile* root_file_3 = new TFile(file_name_3,"OPEN");
  TNtuple* tree_3 = (TNtuple*)root_file_3->Get("eval_rich_small");

  // Input file 4 //
  // From RICH evaluator //
  TString file_name_4 = "/sphenix/user/sekelsky/particle_simulations/proton_nofield/evals/allmerged.root";
  TFile* root_file_4 = new TFile(file_name_4,"OPEN");
  TNtuple* tree_4 = (TNtuple*)root_file_4->Get("eval_rich_small");
  

  TCanvas *c1 = new TCanvas();
  c1->cd();

  const Double_t pmax = 70;
  const Double_t pmin = 2;
  const Double_t interval = 2;
  const Int_t M = 35; // (pmax-pmin)/interval + 1;
  Double_t pnew1[M], tmnew1[M], trnew1[M];
  Double_t pnew2[M], tmnew2[M], trnew2[M];
  Double_t pnew3[M], tmnew3[M], trnew3[M];
  Double_t pnew4[M], tmnew4[M], trnew4[M];
  TString base("mptot>");

  h1 = new TH1F("h1","h1",1000,0,0.1);
  for( int j=0; j<M; j++){
    
    TString restrict = base;
    restrict += interval*(j+1)-1;
    restrict.Append(" && mptot<");
    restrict += interval*(j+1)+1;
    restrict.Append(" &&theta_mean>0 && theta_mean<0.035");
 
    pnew1[j] = interval*(j+1);
    pnew2[j] = interval*(j+1);
    pnew3[j] = interval*(j+1);
    pnew4[j] = interval*(j+1);

    tree_1->Draw("theta_mean>>h1",restrict,"");
    tmnew1[j] = h1->GetMean();
    trnew1[j] = h1->GetRMS();
    h1->Reset();

    tree_2->Draw("theta_mean>>h1",restrict,"");
    tmnew2[j] = h1->GetMean();
    trnew2[j] = h1->GetRMS();    
    h1->Reset();
    
    tree_3->Draw("theta_mean>>h1",restrict,"");
    tmnew3[j] = h1->GetMean();
    trnew3[j] = h1->GetRMS();    
    h1->Reset();

    tree_4->Draw("theta_mean>>h1",restrict,"");
    tmnew4[j] = h1->GetMean();
    trnew4[j] = h1->GetRMS();    
    h1->Reset();
    
  }


  gr1 = new TGraphErrors(M,pnew1,tmnew1,0,trnew1);
  gr1->SetMarkerSize(0.8);
  gr1->SetMarkerStyle(3);
  gr1->SetMarkerColor(kRed);
  gr1->SetLineColor(45);

  gr2 = new TGraphErrors(M,pnew2,tmnew2,0,trnew2);
  gr2->SetMarkerSize(0.8);
  gr2->SetMarkerStyle(3);
  gr2->SetMarkerColor(kBlue);
  gr2->SetLineColor(38);

  gr3 = new TGraphErrors(M,pnew3,tmnew3,0,trnew3);
  gr3->SetMarkerSize(0.8);
  gr3->SetMarkerStyle(3);
  gr3->SetMarkerColor(kGreen);
  gr3->SetLineColor(kGreen-7);

  gr4 = new TGraphErrors(M,pnew4,tmnew4,0,trnew4);
  gr4->SetMarkerSize(0.8);
  gr4->SetMarkerStyle(3);
  gr4->SetMarkerColor(kOrange-3);
  gr4->SetLineColor(kOrange);


  mg = new TMultiGraph();
  legend = new TLegend(0.5,0.3,0.9,0.5);


  // Draw theoretical lines //
  if (draw_theory){
    
    double m[4] = {0.000511, 0.13957018, 0.493677, 0.938272};
    double y[4] = {0.,0.,0.,0.};
    double index = 1.000526;
    
    th1 = new TGraph();
    th2 = new TGraph();
    th3 = new TGraph();
    th4 = new TGraph();

    th1->SetMarkerSize(0.6);
    th2->SetMarkerSize(0.6);
    th3->SetMarkerSize(0.6);
    th4->SetMarkerSize(0.6);

    double dx = 70./200.;
    double x = 0;

    for (int i; i<200; i++){
      x = x+dx;
      y[0] = sqrt(m[0]*m[0]+x*x)/index/x;
      y[1] = sqrt(m[1]*m[1]+x*x)/index/x;
      y[2] = sqrt(m[2]*m[2]+x*x)/index/x;
      y[3] = sqrt(m[3]*m[3]+x*x)/index/x;
      
      if ( y[0] > -1 && y[0] < 1 )
	th1->SetPoint(i,x,acos(y[0]));
      if ( y[1] > -1 && y[1] < 1 )
	th2->SetPoint(i,x,acos(y[1]));
      if ( y[2] > -1 && y[2] < 1 )
	th3->SetPoint(i,x,acos(y[2]));
      if ( y[3] > -1 && y[3] < 1 )
	th4->SetPoint(i,x,acos(y[3]));
    }
  
    if (draw_electron){
      mg->Add(th1);
      legend->AddEntry(th1,"e- Theory","lep");
    }
    if (draw_pion){
      mg->Add(th2);
      legend->AddEntry(th2,"#pi- Theory","lep");
    }
    if (draw_kaon){
      mg->Add(th3);
      legend->AddEntry(th3,"k- Theory","lep");
    }
    if (draw_proton){
      mg->Add(th4);
      legend->AddEntry(th4,"p Theory","lep");
    }  
      
  }

  
  // Draw simulated lines //

  if (draw_electron){
    mg->Add(gr1);
    legend->AddEntry(gr1,"e- Emission Momentum","lep");
  }
  if (draw_pion){
    mg->Add(gr2);
    legend->AddEntry(gr2,"#pi- Reconstructed Momentum","lep");
  }
  if (draw_kaon){
    mg->Add(gr3);
    legend->AddEntry(gr3,"k- Reconstructed Momentum","lep");
  }
  if (draw_proton){
    mg->Add(gr4);
    legend->AddEntry(gr4,"p Reconstructed Momentum","lep");
  }  
  
  mg->SetTitle("; Momentum [GeV]; Cherenkov Angle [rad]");
  mg->Draw("AP");
  mg->GetXaxis()->SetLimits(0,80);
  legend->Draw();


  c1->Update();
  c1->Print("ptheta_dist_mean.pdf","pdf");

  return 0;
}
