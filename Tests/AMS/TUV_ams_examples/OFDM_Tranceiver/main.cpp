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
#include "declarations.h"
#include "tuv_ams_library/tuv_ams_library.h"

using namespace TUV_ams_lib::bb;


SCA_TDF_MODULE(drain) {
  sca_tdf::sca_in<bool> in;
  SCA_CTOR(drain) {}
};

int sc_main(int argc, char* argv[])
{
  sc_set_time_resolution(1, SC_PS);	// setting the time resolution


  /* ********* defining signals and parameters *********** */

  sca_tdf::sca_signal<bool> sig_stimuli;
  sca_tdf::sca_signal<double> sig_out;
  sca_tdf::sca_signal<double> noise_out;
  sca_tdf::sca_signal<double> sig_noise;
  sca_tdf::sca_signal<bool> sig_received;

  int constl_dim;
  double ampl_se;
  double ampl_re;
  double n_va;
  double attent;

  /* ********* setting parameters for simulation *********** */
  cout<<data_rate<<endl;
  cout<<"step ="<<1/freq_bit*8*2/data_rate<<endl;
  cout << "constel_dim = "; cin >> constl_dim;
  cout << "ampl_se = "; cin >> ampl_se;
  cout << "ampl_re = "; cin >> ampl_re;
  cout << "n_va = "; cin >> n_va;
  cout << "attent = "; cin >> attent;

  /* ********* instantiating SDF-modules ********* */

  rand_bool i_stimuli("stimuli",16);                                                      // Stimuli generator: As a signal source it generates uniformly
  i_stimuli.out(sig_stimuli);                                                             // distributed random sequence of bits on its output."sig_stimuli"
  i_stimuli.out.set_timestep(1/freq_bit,SC_SEC);                                                 // is the corresponding stimul signal.

  ofdm_se<8> i_tran("transmitter",freq_carrier,constl_dim,freq_bit,data_rate,ampl_se);      // OFDM Transmitter: It takes the sequence of bits from the
  i_tran.in(sig_stimuli);                                                                    // stimuli generator and converts them to radio frequency signal
  i_tran.out(sig_out);                                                                       // which will be sent per an antenna."sig_out"
                                                                                              // is the radio frequency signal which will be sent per antenna.
  air i_air("air",attent,"gauss_white",n_va,0,data_rate);                     // This part models the environment. A noise generator generates
  i_air.in(sig_out);                                                                         // white gaussian noises and adds them to the output signal of
  i_air.out(sig_noise);                                                                      // the Transmitter.Modeling of attentuation of the environment is also allowed.

  ofdm_re<8> i_receiver("receiver",freq_carrier,constl_dim,freq_bit,data_rate,ampl_re);  // OFDM Receiver: It takes the signal from the environment and
  i_receiver.in(sig_noise);                                                                      // translats them back to the original sent digital signal.
  i_receiver.out(sig_received);

  drain drn("drn");                                                                       // This is only a module used to consume tokens as we can not let a
  drn.in(sig_received);	                                                                  // systemc-ams scheduling loop open.

  /* ******** tracing signals */
sca_util::sca_trace_file* atf = sca_util::sca_create_vcd_trace_file( "tr" );
  sca_util::sca_trace( atf, sig_stimuli ,"stimuli" );
  sca_util::sca_trace( atf, sig_out ,"sig_wo_noise" );
  sca_util::sca_trace( atf, sig_received,"sig_received" );

sca_util::sca_trace( atf, sig_noise,"sig_wt_noise" );

  sc_start(0.01, SC_MS);

  sca_util::sca_close_vcd_trace_file( atf );

  return 0;
}


