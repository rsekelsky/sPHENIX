// Author: Rourke Sekelsky
// This code will display photon hits (ideally Cherenkov rings) from the RICH readout planes.
// Make sure you choose the correct evaluator file, and the correct tree name.
// Restrictions on the photon hits can be implemented as needed.

int cherenkov_ring(){

  TString file_name = "/sphenix/user/sekelsky/particle_simulations/pion_nofield/evals/eval_p30.root";
  TFile* file = new TFile(file_name,"OPEN");
  TNtuple* tree = (TNtuple*)file->Get("eval_rich");

  TCanvas* c1 = new TCanvas();
  c1->cd();
  
  tree->Draw("hit_y:hit_x","event==1","");
  TH2F *htemp = (TH2F*)gPad->GetPrimitive("htemp"); 
  htemp->GetXaxis()->SetTitle("X [cm]"); 
  htemp->GetYaxis()->SetTitle("Y [cm]");

  c1->Print("cherenkov_ring.pdf","pdf");
  
  return 0;

}
