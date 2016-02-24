//-------------------------------------------------------------------------------------------
// Copyright (c) 2008-2010, Institute of Computer Technology, University of Technology Vienna
// Authors:
//   Christoph Grimm, Jan Haase, Markus Damm, Jiong Ou,  
//   Joseph Wenninger, Peter Brunmayr, Muhammad Farooq
// Contact:
//   tuv-ams-lib@ict.tuwien.ac.at
// For the license please read LICENSE.txt
//-------------------------------------------------------------------------------------------

#include "systemc-ams.h"

#include "tuv_ams_library/tuv_ams_library.h"

using namespace TUV_ams_lib::bb;


SCA_TDF_MODULE(drain) 
{
  sca_tdf::sca_in<bool> in;

  SCA_CTOR(drain) {}
};

/*****************************************************/


int sc_main(int argc, char* argv[])
{
  sc_set_time_resolution(1, SC_PS);	//

 /********** defining signals and parameters *********** */

  sca_tdf::sca_signal<bool> binary_data_tx;
  sca_tdf::sca_signal<double> modulated_data_tx;
  sca_tdf::sca_signal<double> modulated_data_noisy_tx;
  sca_tdf::sca_signal<bool> binary_data_rx;


 double freq;
 int    rate;
  /********** setting parameters for simulation *********** */


  cout <<"\n" << "frequency= ";  cin >> freq;
  cout <<"\n";

  cout << "sample rate ="; cin >> rate;
  cout <<"\n";

/*******************************************************************/


  /********** instantiating library-modules**********/

  rand_bool binary_source("stimuli",rate); 
  binary_source.out(binary_data_tx);
  binary_source.out.set_timestep(1,SC_MS);

  qpsk qpsk_tr("qpsk_tr",freq,rate);
  qpsk_tr.in(binary_data_tx);
  qpsk_tr.out(modulated_data_tx);

  air channel("air",0.4,"gauss_white",1,0,rate);
  channel.in(modulated_data_tx);
  channel.out(modulated_data_noisy_tx);

  qpsk_de qpsk_rx("qpsk_rx",freq,rate);
  qpsk_rx.in(modulated_data_noisy_tx);
  qpsk_rx.out(binary_data_rx);

  drain drn("drn");
  drn.in(binary_data_rx);


  /********* tracing signals */
  sca_util::sca_trace_file* atf = sca_util::sca_create_vcd_trace_file( "tr" );

  sca_util::sca_trace( atf, binary_data_tx ,"binary_data_tx" );
  sca_util::sca_trace( atf, modulated_data_tx ,"modulated_data_tx" );
  sca_util::sca_trace( atf, modulated_data_noisy_tx ,"modulated_data_noisy_tx" );
  sca_util::sca_trace( atf, binary_data_rx,"binary_data_rx" );

  sc_start(20, SC_MS);

  sca_util::sca_close_vcd_trace_file( atf );

  return 0;
}


