/*
  Author: Rourke Sekelsky
  This code calculates the average number of events around an expected angle for the desired particle, and fits a function to it.
  This comes from the collection of simulated events in /sphenix/user/sekelsky/.
  This function will be used in the particle probability functions.

  Make sure to change the eventstart parameter, which chooses which momentum to start at!
  Make sure you have the correct file path!
  Make sure the expected angle is the one you want!
*/

int pid_function(){

  double m[4] = {0.000511,0.139570,0.493677,0.938272};
  double test_p = 70;
  double index = 1.000526;

  // Angles at p=70 GeV, take difference of pion/kaon to set window //
  double maxangles[4] = {
    acos( sqrt(m[0]*m[0]+test_p*test_p) / (index * test_p) ),
    acos( sqrt(m[1]*m[1]+test_p*test_p) / (index * test_p) ),
    acos( sqrt(m[2]*m[2]+test_p*test_p) / (index * test_p) ),
    acos( sqrt(m[3]*m[3]+test_p*test_p) / (index * test_p) ),    
  };

  cout << maxangles[0] << " " << maxangles[1] << " " << maxangles[2] << " " << maxangles[3] << endl;
  double window = (maxangles[1] - maxangles[2])/2;
  //double window = 0.01;
  cout << "Angular window: " << window << endl;


  // ROOT file is dead if the momentum isn't high enough; find these values manually //
  int eventstart[4] = {2,6,16,2};
  TString base, restrict;
  double expect;
  double num, denom;
  double momentum[35];
  double count[35];

  for( int p=eventstart[2]; p<71; p = p+2 )
    {
      base = "/sphenix/user/sekelsky/particle_simulations/kaon_nofield/emission_evals/eval_p";
      base += p;
      base.Append(".root");
      
      // read input files //
      TFile *file = new TFile(base,"OPEN");
      TNtuple *tree = (TNtuple*)file->Get("eval_rich");
      TNtuple *tree2 = (TNtuple*)file->Get("eval_rich_small");
      
      // Restrict the chosen angles to within the window value //
      // Can mismatch expected value to compare separation //
      expect = acos( sqrt(m[1]*m[1]+p*p) / index / p );
      restrict = "theta_reco>";
      restrict += expect - window;
      restrict.Append(" && theta_reco<");
      restrict += expect + window;

      // Count entries, number of hits in window divided by number of tracks //
      momentum[(p-1)/2] = p;
      num = tree->Draw("theta_reco",restrict,"");
      denom = tree2->Draw("otrackid","otrackid>-1 && otrackid<1","");
      count[(p-1)/2] = num / denom;
      cout << "Momentum: " << momentum[(p-1)/2] << " Counts: " << count[(p-1)/2] << " Expected Value: " << expect << endl;

      file->Close();
    }

  TCanvas * c1 = new TCanvas();
  c1->cd();

  TGraph* gr = new TGraph(35,momentum,count);
  gr->SetTitle("; Momentum [GeV]; Counts (K-)");
  gr->Draw("AP");

  TF1* fe = new TF1("fe","[0]*x+[1]",eventstart[0],70);
  TF1* fp = new TF1("fp","[2]*acos( sqrt([0]+x*x)/[1]/x )",eventstart[1],70);
  TF1* fk = new TF1("fk","[2]*acos( sqrt([0]+x*x)/[1]/x )",eventstart[2],70);
  TF1* fpr = new TF1("fpr","[2]*acos( sqrt([0]+x*x)/[1]/x )",eventstart[3],70);

  fe->SetParameters(0.2,30);
  fp->SetParameters(175,3,25);
  fk->SetParameters(175,3,25);
  fpr->SetParameters(175,3,25);

  for (int fitit = 0; fitit < 10; fitit++)
    gr->Fit(fk,"R");

  /*
    The function fits are not well defined for reconstructed momentum.

    The function fits give the approximate values (emission momentum):
    fe = 29.5
    fp = 25.8 * acos( sqrt(178.4 +x*x) / (2.62*x) )
    fk = 22.08 * acos( sqrt(982600 + x*x) / (52.93*x) )
    fpr = TBD
  */
  
  c1->Print("pidf.pdf","pdf");

  return 0;
}
  
