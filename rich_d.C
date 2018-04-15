// Author: Rourke Sekelsky
// Date: 6/15/17
// This code draws a simple diagram for the RICH detector geometry.
// The reflections stop at an arbitrary parameter z_stop (max/min). This should be changed if you change the geometry and want the reflections.

#include <stdio.h>
#include <math.h>

#define PI 3.14159265


// Function declaration

double etatophi(double eta);
double minphi(double Rshift, double Rbeam, double Rfeature);

double maxphi(double zshift, double Rshift, double Rfeature, double phi);
double longf(double z, double R, double Rf, double p, double y);
double dlongf(double z, double R, double Rf, double p, double y);

double mirrorlimitz(double zshift, double Rfeature, double angle);
double mirrorlimitR(double Rshift, double Rfeature, double angle);
double maxreflection(double maxphi, double maxphimirror, double maxz, double maxR, double maxstop);
double minreflection(double minphi, double minphimirror, double minz, double minR, double minstop);

int rich_d()
{
  // Set environment //


  TCanvas c1("c1","",400,400);
  TH1F* hframe = new TH1F("hframe","",10,0,300);
  gStyle->SetOptStat(0);
  hframe->SetTitle("RICH Detector Geometry");
  hframe->GetXaxis()->SetTitle("z [cm]");
  hframe->GetYaxis()->SetTitle("R [cm]");
  hframe->GetXaxis()->SetRangeUser(0,300);
  hframe->GetYaxis()->SetRangeUser(0,300);
  hframe->SetLineColor(kWhite);
  hframe->Draw();


  // Parameters from G4_RICH //


  double min_eta = 1.45; // Original: 1.45
  double R_mirror = 195; // Original: 195
  double z_shift = 75; // Original: 75
  double R_shift = 18.5; // Original: 18.5


  // Parameters from ePHENIXRICHConstruction //


  double Dratio = 0.85;
  double dR_frontwindow_shrink = 2;
  double R_beam = 3;
  double C_mirror = sqrt(z_shift*z_shift + R_shift*R_shift); // distance to center of mirror
  double C_window = 0.85*C_mirror; // distance to center of window
  double R_frontwindow = R_mirror/2 + C_mirror * (1 - Dratio) - dR_frontwindow_shrink; // not sure how this is calculated


  // Parameters calculated //


  // For minimum phi (in degrees): //
  double min_phi_mirror = minphi(R_shift, R_beam, R_mirror);
  double min_phi_window = minphi(R_shift*Dratio, R_beam, R_frontwindow);


  // For maximum phi (in degrees): //
  double max_phi = etatophi(min_eta);
  double max_phi_mirror = maxphi(z_shift, R_shift, R_mirror, max_phi)*180/PI;
  double max_phi_window = maxphi(z_shift*Dratio, R_shift*Dratio, R_frontwindow, max_phi)*180/PI;


  // Draw features //
  // (Largest feature should be first to avoid overlap issues) //


  // Curved surfaces //


  TArc mirror(z_shift, R_shift, R_mirror, min_phi_mirror, max_phi_mirror);
  mirror.Draw("only");
  mirror.SetLineColor(2);

  TArc window(z_shift*Dratio, R_shift*Dratio, R_frontwindow, min_phi_window, max_phi_window);
  window.Draw("only");
  window.SetLineColor(4);

  TArc mirror_ref(z_shift, R_shift, 1);
  mirror_ref.Draw();
  mirror_ref.SetLineColor(2);
  mirror_ref.SetFillColor(2);

  TArc window_ref(z_shift*Dratio, R_shift*Dratio, 1);
  window_ref.Draw();
  window_ref.SetLineColor(4);
  window_ref.SetFillColor(4);


  // Lines //


  double mirror_min_z = mirrorlimitz(z_shift, R_mirror, min_phi_mirror);
  double mirror_max_z = mirrorlimitz(z_shift, R_mirror, max_phi_mirror);
  double mirror_min_R = mirrorlimitR(R_shift, R_mirror, min_phi_mirror);
  double mirror_max_R = mirrorlimitR(R_shift, R_mirror, max_phi_mirror);

  TBox beam(0,0,300,3);
  beam.Draw();
  beam.SetFillColor(11);

  TLine min(0,0,mirror_min_z,mirror_min_R);
  min.Draw();
  min.SetLineStyle(2);

  TLine max(0,0,mirror_max_z,mirror_max_R);
  max.Draw();
  max.SetLineStyle(2);


  // Reflections //


  // Drawing the max reflection, stopping at arbitrary point (z_stop, R_stop) //
  double z_stop_max = 160; // Original: 160
  double R_stop_max = maxreflection(max_phi, max_phi_mirror, mirror_max_z, mirror_max_R, z_stop_max);

  TLine max_refl(mirror_max_z,mirror_max_R,z_stop_max,R_stop_max);
  max_refl.Draw();
  max_refl.SetLineStyle(2);


  // Drawing the min reflection, same deal //
  double min_phi = atan(mirror_min_R/mirror_min_z) * 180/PI;
  double z_stop_min = 170; // Original: 170
  double R_stop_min = minreflection(min_phi, min_phi_mirror, mirror_min_z, mirror_min_R, z_stop_min);

  TLine min_refl(mirror_min_z,mirror_min_R,z_stop_min,R_stop_min);
  min_refl.Draw();
  min_refl.SetLineStyle(2);


  // Construction features //


  // TLine con1(z_shift,R_shift,mirror_max_z,mirror_max_R);
  // con1.Draw();
  // TLine con2(z_shift,R_shift,mirror_min_z,mirror_min_R);
  // con2.Draw();


  // Construct diagram //


  gPad->RedrawAxis();
  c1.Print("diagram.pdf","pdf");


  return 0;
}


/*--------------------------------------*/
/*------------- Functions --------------*/
/*--------------------------------------*/


double etatophi(double eta)
{
  double phi = 2*atan(exp(-eta)) * 180/PI;

  return phi; // in degrees
}


double minphi(double Rshift, double Rbeam, double Rfeature)
{
  double a = Rshift - Rbeam;
  double result = -asin(a/Rfeature) * 180/PI;

  return result;
}


double maxphi(double zshift, double Rshift, double Rfeature, double phi)
{
  double x = 0.5;
  
  while( longf(zshift, Rshift, Rfeature, phi, x) > pow(10,-6) )
    {
      x = x - longf(zshift, Rshift, Rfeature, phi, x) / dlongf(zshift, Rshift, Rfeature, phi, x);
    }
  
  return x;
}


double longf(double z, double R, double Rf, double p, double y)
{
  double hypot = sqrt( pow(z,2) + pow(R,2) );
  double alpha = atan( R/z );
  
  double result = Rf * cos(y) - cos(p*PI/180) * sqrt( pow(hypot,2) + pow(Rf,2) - 2*hypot*Rf*cos( PI + alpha - y ) ) + z;

  return result;
}


double dlongf(double z, double R, double Rf, double p, double y)
{
  double hypot = sqrt( pow(z,2) + pow(R,2) );
  double alpha = atan( R/z );
  
  double result = -Rf * sin(y) + cos(p*PI/180) * pow( (pow(hypot,2) + pow(R,2) - 2*hypot*Rf*cos( PI + alpha - y )), -0.5) * hypot*Rf*sin( PI + alpha - y);

  return result;
}


double mirrorlimitz(double zshift, double Rmirror, double angle)
{
  double result = zshift + Rmirror*cos(angle*PI/180);

  return result;
}


double mirrorlimitR(double Rshift, double Rmirror, double angle)
{
  double result = Rshift + Rmirror*sin(angle*PI/180);

  return result;
}


double maxreflection(double maxphi, double maxphimirror, double maxz, double maxR, double maxstop)
{
  double slope = tan( (2*maxphimirror - maxphi) * PI/180 );
  double result = slope * (maxstop - maxz) + maxR;

  return result;
}


double minreflection(double minphi, double minphimirror, double minz, double minR, double minstop)
{
  double slope = tan( (2*minphimirror - minphi) * PI/180 );
  double result = slope * (minstop - minz) + minR;

  return result;
}
