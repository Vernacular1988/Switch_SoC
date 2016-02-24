// main.cpp

//#include "top.h"

#include <systemc-ams.h>
//#include "tuv_ams_library/tuv_ams_library.h"
#include "osc.h"
#include "con.h"

#define NN 3

int sc_main(int argc, char* argv[])
{
  sc_set_time_resolution(1, SC_NS);     // for solvers

  sca_tdf::sca_signal<double> vco[NN];
  sca_tdf::sca_signal<double> lpf[NN];
  sca_tdf::sca_signal<double> sw[NN][NN];


  double phc_gain = 100.0;	// phase comparitor gain
  double lp_fc = 10.0;		// low pass filter cut off freq
  double vco_freq = 1000.0; 	// natural frequency of vco
  double kvco = 100.0;		// control sensitivity for vco
  double vco_gain = 1.0;		// amplitude gain of vco

  osc *oscp[NN];
  constant *con[NN][NN];
  char  mn[NN];
 
  /********** instantiating library-modules**********/

  // generate oscilators and weights with loops

  for(int i=0; i<NN; i++) 
    {

      sprintf(mn,"%s%i","osc",i);
      oscp[i] = new osc(mn, phc_gain, lp_fc, vco_freq+(i*10), kvco, vco_gain);
      oscp[i]->vco_ref1(vco[(i+1)%NN]);
      oscp[i]->vco_ref2(vco[(i+2)%NN]);
      oscp[i]->vco_out(vco[i]);
      oscp[i]->lpf_out(lpf[i]);
      oscp[i]->w1(sw[(i+1)%NN][i]);
      oscp[i]->w2(sw[(i+2)%NN][i]);

      cout << "node "<<i<<" to/from "<<(i+1)%NN <<(i+2)%NN << endl; 
      cout << flush;
      
      for(int j=0; j<NN; j++) {
	if(i!= j) {
	  sprintf(mn,"%s%i%i","con",i,j);
	  con[i][j] = new constant(mn, .00);
	  con[i][j]->out(sw[i][j]);
	  con[i][j]->set_timestep(10.0, SC_US);
	}
      }
    }

  /********* tracing signals *************************/

  sca_util::sca_trace_file* atf = sca_util::sca_create_vcd_trace_file( "pll" );

   sca_util::sca_trace( atf, vco[0], "vco1" );
   sca_util::sca_trace( atf, vco[1] ,"vco2" );
   sca_util::sca_trace( atf, vco[2] ,"vco3" );
   sca_util::sca_trace( atf, lpf[0] ,"lpf1" );
   sca_util::sca_trace( atf, lpf[1] ,"lpf2" );
   sca_util::sca_trace( atf, lpf[2] ,"lpf3" );

  sc_start(100.0, SC_MS);

  sca_util::sca_close_vcd_trace_file( atf );

  return 0;
}
