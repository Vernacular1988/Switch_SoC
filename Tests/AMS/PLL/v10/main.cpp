// main.cpp

#include <time.h>
#include "size.h"
#include "pattern.h"
#include <systemc-ams.h>
#include "osc.h"
#include "con.h"
#include "a_delay.h"

int sc_main(int argc, char* argv[])
{
  sc_set_time_resolution(1, SC_NS);     // for solvers

  sca_tdf::sca_signal<double> vco[NN];
  sca_tdf::sca_signal<double> vcod[NN];
  sca_tdf::sca_signal<double> lpf[NN];
  sca_tdf::sca_signal<double> sw[NN][NN];
  sca_tdf::sca_signal<double> phco[NN];
  sca_tdf::sca_signal<double> phcof[NN];


  double phc_gain = 1.0/NN;//100	// phase comparitor gain
  double lp_fc = 100.0;	//10	// low pass filter cut off freq
  double vco_freq = 1000.0; 	// natural frequency of vco
  double kvco = 10.0;//1000 10		// control sensitivity for vco
  double vco_gain = 1.0;		// amplitude gain of vco
  double phase = 0.0;		// inital phase of vco in pll
  double fphase, ophase, dphase = 0.0;
  char  mn[64];			// used to create unique module name strings

  int ninety = (int) floor(0.25/(vco_freq*1e-5)); // # samples for 90 degree phase delay at vco_freq and sample rate

  osc *oscp[NN];		// oscilators with weighted sum input and pll generator 
  a_delay *del[NN];		// delay for 90 degree phase delay in feedback loops
  constant *con[NN][NN];	// constant sources for Sij weights

  phc *phcomp[NN];                // for pairwise phase comparison 
  lp *plpf[NN];                    // low pass filter
  int temp;

  // pattern 1
  // int pattern[NN][NN] = {{ 1, -1, -1,  1,  1},
  // 			 {-1,  1,  1, -1, -1},
  // 			 {-1,  1,  1, -1, -1},
  // 			 { 1, -1, -1,  1,  1},
  // 			 { 1, -1, -1,  1,  1}};

  // pattern 2
  // int pattern[NN][NN] = {{ 1,  1, -1, -1,  1},
  // 			 { 1,  1, -1, -1,  1},
  // 			 {-1, -1,  1,  1, -1},
  // 			 {-1, -1,  1,  1, -1},
  // 			 { 1,  1, -1, -1,  1}};

  // (pattern1 + pattern2) / 2
  // int pattern[NN][NN] =    {{ 1,  0,  -1,  0,  1},
  // 			    { 0,  1,  0, -1,  0},
  // 			    {-1,  0,  1,  0, -1},
  // 			    { 0, -1,  0,  1,  0},
  // 			    { 1,  0, -1,  0,  1} };

  // 90 degree phase delay mapped into sample points
  // cout << "ninety degrees at this sample rate is:" << ninety << " time-steps" << endl;

  srand(time(NULL));

  /********** instantiating library-modules**********/

  // generate oscilators and connections with loop
  for(int i=0; i<NN; i++) {

    sprintf(mn,"%s%i","osc",i);
    phase = (rand() % (int)floor(2 * M_PI * 100))/100.0; // random phase 0-2*PI
    //phase = ((rand() % 2) == 0) ? 0 : M_PI;

    // cout << "phase: " << phase << " which is: "<< phase*360/(2*M_PI)
    // 	 << " degrees" << endl;

    if (i == 0) fphase = ophase = phase;
    if(i>0) {
      dphase = abs(phase-ophase);
      // cout << "phase difference: " << dphase 
      // 	   << " which is: "<< dphase*360/(2*M_PI) 
      // 	   << " degrees" << endl;

      if      (dphase < M_PI/4.0)   cout << " +1 ";
      else if (dphase < 3*M_PI/4.0) cout << "  0 ";
      else if (dphase < 5*M_PI/4.0) cout << " -1 ";
      else if (dphase < 7*M_PI/4.0) cout << "  0 ";
      else                          cout << " +1 ";
      cout << endl;
    }

    if(i == NN-1) {
      ophase = fphase;
      dphase = abs(phase-ophase);
      // cout << " wrapped phase difference: " << dphase  
      // 	   << " which is: "<< dphase*360/(2*M_PI) 
      // 	   << " degrees" << endl;
      
      if      (dphase < M_PI/4.0)   cout << " +1 ";
      else if (dphase < 3*M_PI/4.0) cout << "  0 ";
      else if (dphase < 5*M_PI/4.0) cout << " -1 ";
      else if (dphase < 7*M_PI/4.0) cout << "  0 ";
      else                          cout << " +1 ";
      cout << endl;
    }
    ophase = phase;

    oscp[i] = new osc(mn, phc_gain, lp_fc, vco_freq, kvco, vco_gain, phase);
    oscp[i]->VCO_OUT(vco[i]);
    oscp[i]->LPF_OUT(lpf[i]);
    
    sprintf(mn,"%s%i","del",i);
    del[i] = new a_delay(mn, 0, ninety);
    del[i]->inp(vco[i]);
    del[i]->outp(vcod[i]);
    
    sprintf(mn, "%s%i","ph",i);
    phcomp[i] = new phc(mn, 1, M_PI); // phase compare in terms of pi
    phcomp[i]->in_ref(vco[i]);
    phcomp[i]->in_vco(vco[(i+1)%NN]);
    phcomp[i]->out(phco[i]); 
    
    sprintf(mn, "%s%i","plpf",i);
    plpf[i] = new lp(mn, 10.0);
    plpf[i]->in(phco[i]);
    plpf[i]->out(phcof[i]); 
    
    // connect up the NN inputs
    for(int j=0; j<NN; j++) {
      oscp[i]->VCO_IN[j](vcod[j]);
      oscp[i]->W[j](sw[i][j]);
    }
  }

  // Now we set the weights

  srand(7);  // reset the psudo random number generator 

  for(int i=0; i<NN; i++) {
        for(int j=0; j<=i; j++) {
	  if(i!= j) {	  
	    // W i,j  == W j, i for all nodes
	    //temp = ((rand() % 2) == 0) ? -1 : 1; //temp = pow(-1.0,i); 
	    //temp = ((rand() % 2) == 0) ?  0 : 1; //temp = pow(-1.0,i); 
	    temp = pattern[i][j];
	    cout << temp << " ";

	    sprintf(mn,"%s_%i_%i","con",i,j);
	    //cout << "Node: " << mn << " Temp: " << temp << endl;
	    con[i][j] = new constant(mn, temp);
	    con[i][j]->out(sw[i][j]);
	    con[i][j]->set_timestep(10.0, SC_US);
	    //cout << "node "<<i<<" to/from "<<i << " " << j << endl; 

	    sprintf(mn,"%s_%i_%i","con",j,i);
	    //cout << "Node: " << mn << " Temp: " << temp << endl;
	    con[j][i] = new constant(mn, temp);
	    con[j][i]->out(sw[j][i]);
	    con[j][i]->set_timestep(10.0, SC_US);
	    //cout << "node "<<i<<" to/from "<<j << " " << i << endl; 

	  } else { // W i,i == 0
	    temp = 0;
	    cout << temp << endl;

	    sprintf(mn,"%s_%i_%i","con",i,j);
	    //cout << "Node: " << mn << " Temp: " << temp << endl;
	    con[i][j] = new constant(mn, temp);
	    con[i][j]->out(sw[i][j]);
	    con[i][j]->set_timestep(10.0, SC_US);
	    //cout << "node "<<i<<" to/from "<<i << " " << j << endl; 
      }
    }
  }
  /********* tracing signals *************************/

  sca_util::sca_trace_file* atf = sca_util::sca_create_vcd_trace_file( "pll" );

  for(int i = 0; i<NN; i++) { 
    // sprintf(mn,"%s%i","vco",i);
    // sca_util::sca_trace( atf, vco[i], mn);
    // sprintf(mn,"%s%i","lpf",i);
    // sca_util::sca_trace( atf, lpf[i] ,mn);
    sprintf(mn,"%s%i","phcf",i);
    sca_util::sca_trace( atf, phcof[i] ,mn);
  }

  sc_start(1000.0, SC_MS);

  sca_util::sca_close_vcd_trace_file( atf );

  return 0;
}
