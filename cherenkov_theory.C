// Author: Rourke Sekelsky
// This code produces a plot of the theoretical Cherenkov angles for the particles:
// electron, charged pion, charged kaon, proton.
// No input necessary, this is just a useful graphic.

int cherenkov_theory(){

  TCanvas *c1 = new TCanvas();
  c1->cd();

  double index = 1.000526;
  double m[4] = {0.000511,0.139570,0.493677,0.938272};
  double t1[200], t2[200], t3[200], t4[200];
  double m1[200], m2[200], m3[200], m4[200];

  double Npoints = 200;
  double pmax = 70;
  double j=0;
  double p1, p2, p3, p4;
  for (int i=0; i<Npoints; i++) {

    j = pmax/Npoints*i;
    m1[i] = j;
    m2[i] = j;
    m3[i] = j;
    m4[i] = j;

    if (j != 0){
      p1 = sqrt(m[0]*m[0] + j*j) / index / j;
      p2 = sqrt(m[1]*m[1] + j*j) / index / j;
      p3 = sqrt(m[2]*m[2] + j*j) / index / j;
      p4 = sqrt(m[3]*m[3] + j*j) / index / j;

      if (p1 < 1)
	t1[i] = acos(p1);
      else
	t1[i] = 0;
      
      if (p2 < 1)
	t2[i] = acos(p2);
      else
	t2[i] = 0;
      
      if (p3 < 1)
	t3[i] = acos(p3);
      else
	t3[i] = 0;
      
      if (p4 < 1)
	t4[i] = acos(p4);
      else
	t4[i] = 0;
    }
    
  }

  TGraph *g1 = new TGraph(Npoints,m1,t1); g1->SetLineColor(kBlue); g1->SetLineWidth(3);
  TGraph *g2 = new TGraph(Npoints,m2,t2); g2->SetLineColor(kRed);  g2->SetLineWidth(3);
  TGraph *g3 = new TGraph(Npoints,m3,t3); g3->SetLineColor(kGreen);  g3->SetLineWidth(3);
  TGraph *g4 = new TGraph(Npoints,m4,t4); g4->SetLineColor(kOrange);  g4->SetLineWidth(3);

  TMultiGraph *mg = new TMultiGraph();
  mg->Add(g1);
  mg->Add(g2);
  mg->Add(g3);
  mg->Add(g4);

  mg->SetTitle(";Momentum [GeV];Cherenkov Angle [rad]");
  mg->Draw("a");

  TLegend *l = new TLegend(0.7,0.3,0.9,0.5);
  l->AddEntry(g1,"Electron (Theory)","l");
  l->AddEntry(g2,"Pion (Theory)","l");
  l->AddEntry(g3,"Kaon (Theory)","l");
  l->AddEntry(g4,"Proton (Theory)","l");
  l->Draw();

  c1->Print("cherenkov_theory.pdf","pdf");
  
  return 0;
}
