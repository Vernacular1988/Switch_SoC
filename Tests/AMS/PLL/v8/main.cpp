// main.cpp

#include "size.h"

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


  double phc_gain = 100.0;//100	// phase comparitor gain
  double lp_fc = 10.0;	//10	// low pass filter cut off freq
  double vco_freq = 1000.0; 	// natural frequency of vco
  double kvco = 100.0;//100		// control sensitivity for vco
  double vco_gain = 1.0;		// amplitude gain of vco
  double phase = 0.0;		// iniital phase of vco in pll
  char  mn[64];			// used to create unique module name strings

  int ninety = (int) floor(0.25/(vco_freq*1e-5)); // # samples for 90 degree phase delay at vco_freq and sample rate

  osc *oscp[NN];		// oscilators with weighted sum input and pll generator 
  a_delay *del[NN];		// delay for 90 degree phase delay in feedback loops
  constant *con[NN][NN];	// constant sources for Sij weights


  
  cout << "ninety " << ninety << endl;

  
  /********** instantiating library-modules**********/



  // generate oscilators and weights with loops

  for(int i=0; i<NN; i++) 
    {

      sprintf(mn,"%s%i","osc",i);
      phase = (rand() % (int)floor(2 * M_PI * 100))/100.0; // random phase 0-2*PI
      cout << "phase: " << phase << endl;
      oscp[i] = new osc(mn, phc_gain, lp_fc, vco_freq, kvco, vco_gain, phase);
      oscp[i]->VCO_OUT(vco[i]);
      oscp[i]->LPF_OUT(lpf[i]);

      sprintf(mn,"%s%i","del",i);
      del[i] = new a_delay(mn, 0, ninety);
      del[i]->inp(vco[i]);
      del[i]->outp(vcod[i]);

      for(int j=0; j<NN; j++) {
	//	if(i!= j) {     // do we special case the Cii weights and inputs to the pll nodes ?
	
	  oscp[i]->VCO_IN[j](vcod[j]);
	  oscp[i]->W[j](sw[i][j]);
  
	  sprintf(mn,"%s%i%i","con",i,j);
	  con[i][j] = new constant(mn, 0.05);
	  con[i][j]->out(sw[i][j]);
	  con[i][j]->set_timestep(10.0, SC_US);
	  //	}
	  cout << "node "<<i<<" to/from "<<i << " " << j << endl; 
      }
    }

  /********* tracing signals *************************/

  sca_util::sca_trace_file* atf = sca_util::sca_create_vcd_trace_file( "pll" );

  for(int i = 0; i<NN; i++) { 
    sprintf(mn,"%s%i","vco",i);
    sca_util::sca_trace( atf, vco[i], mn);
    sprintf(mn,"%s%i","lpf",i);
    sca_util::sca_trace( atf, lpf[i] ,mn);
  }

  sc_start(100.0, SC_MS);

  sca_util::sca_close_vcd_trace_file( atf );

  return 0;
}
