/*
  This macro will create files suitable to be used in the bnl_rich_analyzer_pid read() function.
  To check how will the results match up, one should use the run() function on the original evaluator and then read() on the text file outputted,
  then compare that with using this macro on the original evaluator and then read() on the text file outputted here.
  
  You have to set particle yourself! This is truth information.
*/

#include <fstream>

int setup_read_file(){

  // Input file //
  // From RICH evaluator //
  TString file_name = "/sphenix/user/sekelsky/particle_simulations/1000_events/evals/1000_electron.root";
  TFile* root_file = new TFile(file_name,"OPEN");
  TNtuple* tree = (TNtuple*)root_file->Get("eval_rich");
  TNtuple* tree_small = (TNtuple*)root_file->Get("eval_rich_small");

  int nevents;
  int particle = 0; // 0 = electron, 1 = pion, 2 = kaon, 3 = proton
  TString base("mptot>=");
  TH1F *h1 = new TH1F("h1","",10000,0,0.3);

  for (int imom=0; imom<60; imom++){
    
    TString restrict = base;
    restrict += imom;
    restrict.Append(" && mptot<");
    restrict += imom+1;

    TString restrict2 = restrict;
    restrict2.Append(" && theta_reco<(theta_true+0.008) && theta_reco>(theta_true-0.008) && acos(mpz/mptot)*180/3.14159<16 && acos(mpz/mptot)*180/3.14159>15");

    nevents = tree_small->Draw("event",restrict,"");

    tree->Draw("theta_reco>>h1",restrict2,"");

    /* Output */
    ofstream out("pid_mine.txt", ios::app);
    streambuf *coutbuf = cout.rdbuf();
    cout.rdbuf(out.rdbuf());

    if (nevents != 0)
      cout << h1->GetMean() << " " << h1->GetRMS() << " " << (int)h1->GetEntries()/nevents << " " << imom << " " << particle << endl;
    else
      cout << h1->GetMean() << " " << h1->GetRMS() << " " << 0 << " " << imom << " " << particle << endl;

    h1->Reset();
  }

  return 0;
}
