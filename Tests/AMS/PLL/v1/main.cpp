// main.cpp

#include "pll.h"

int sc_main(int argc, char* argv[])
{
  sc_set_time_resolution(1, SC_NS);     //

 /********** defining signals and parameters *********** */

  sca_tdf::sca_signal<double> modulation;
  sca_tdf::sca_signal<double> source;
  sca_tdf::sca_signal<double> output1;
  sca_tdf::sca_signal<double> output2;
  
  double s_freq = 1000.0, m_freq=10.0;
  int rate = 1;

 /********** setting parameters for simulation *********** */

  cout <<"\n" << "source frequency = ";  
  cin >> s_freq;
  std::cout <<"\n s_freq is " << s_freq <<"\n" ;

  cout <<"\n" << "modulation frequency =";  
  cin >> m_freq;
  std::cout <<"\n m_freq is " << m_freq <<"\n" ;

  cout <<"\n" << "sample rate ="; 
  cin >> rate;
  std::cout <<"\n rate is " << rate << "\n";

/********** instantiating library-modules**********/

  sine	sinin("sinin",
	      s_freq,	// freq_def
	      1.0,	// amp_def
	      0.0,	// phase
	      0.0,	// offset
	      false,	// amp_control
	      false,	// freq_control
	      1	// datarate
	      );

  //sinin.freq_in(modulation);

  sinin.set_timestep(1.0, SC_US);

  sinin.out(source);


  my_pll pll1("pll1", 
	   1.0, 	// phc_gain
	   100.0, 	// lp_fc
	   1000.0, 	// vco_freq
	   1.0,		// kvco
	   1.0,		// vco_gain
	   1,		// vco_out_rate 
	   1		// phc_out_rate
	   );

  pll1.ref(source);
  pll1.vcoo(output1);
  pll1.lpo(output2);

  drain drn1("drn1");
  drn1.in(output1);

  drain drn2("drn2");
  drn2.in(output2);


/********* tracing signals */
  sca_util::sca_trace_file* atf = sca_util::sca_create_vcd_trace_file( "pll" );

  sca_util::sca_trace( atf, source, "source" );
  sca_util::sca_trace( atf, modulation ,"modulation" );

  sca_util::sca_trace( atf, output1 ,"output1" );
  sca_util::sca_trace( atf, output2 ,"output2" );

  sc_start(20.0, SC_MS);

  sca_util::sca_close_vcd_trace_file( atf );

  return 0;
}
