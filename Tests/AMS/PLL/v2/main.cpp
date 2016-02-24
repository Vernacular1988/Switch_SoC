// main.cpp

#include "pll.h"

int sc_main(int argc, char* argv[])
{
  sc_set_time_resolution(1, SC_NS);     //

 /********** defining signals and parameters *********** */

  sca_tdf::sca_signal<double> modulation;
  sca_tdf::sca_signal<double> source;
  sca_tdf::sca_signal<double> vco_out_delay;
  // pll loop signals
  sca_tdf::sca_signal<double> lpf_out;
  sca_tdf::sca_signal<double> pco;
  sca_tdf::sca_signal<double> vco_out;
  //

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

  sine sinin("sinin",
	      s_freq,	// freq_def
	      1.0,	// amp_def
	      0.0,	// phase
	      0.0,	// offset
	      false,	// amp_control
	      false,	// freq_control
	      1	// datarate
	      );

  //sinin.freq_in(modulation);

  sinin.set_timestep(10.0, SC_US);

  sinin.out(source);


// note this is a minor rewite of the pll in the tuv_ams bb library
/*****************************PLL***************************************/

  phc phc_sub("phc_sub",
	      1,	// phc_out_rate
	      100.0	// phc_gain (will be Mpyed by kvco)
	      );

  phc_sub.in_ref(source);
  phc_sub.in_vco(vco_out_delay);
  phc_sub.out(pco);

  lp lp_sub("lp_sub",
	    10.0	// lp_fc  (about 1/100 of vco_freq)
	    );

  lp_sub.in(pco); 
  lp_sub.out(lpf_out); //lpo  output 2

  a_vco vco_sub("vco_sub",
		1000.0,	// vco_freq,
		1,	// vco_out_rate
		100.0,	// kvco units are add/sub from freq (pull in range)
		1.0	//vco_gain
		);

  vco_sub.in(lpf_out);  // lpo
  vco_sub.out(vco_out); // vcoo output 1

  //vco_sub.out.set_delay(1); // does not work

  a_delay vco_delay("vco_delay", 
		    0.0, 	// initial value
		    1		// delay
		    );
  vco_delay.inp(vco_out);
  vco_delay.outp(vco_out_delay);


  drain drn1("drn1");
  drn1.in(vco_out_delay);

  drain drn2("drn2");
  drn2.in(lpf_out);

  //drain drn3("drn3");
  //drn2.in(vco_out);


/********* tracing signals *************************/
  sca_util::sca_trace_file* atf = sca_util::sca_create_vcd_trace_file( "pll" );

  sca_util::sca_trace( atf, source, "source" );
  sca_util::sca_trace( atf, modulation ,"modulation" );
  sca_util::sca_trace( atf, vco_out ,"vco_out" );
  sca_util::sca_trace( atf, vco_out_delay ,"vco_out_delay" );
  sca_util::sca_trace( atf, lpf_out ,"lpf_out" );

  sc_start(1000.0, SC_MS);

  sca_util::sca_close_vcd_trace_file( atf );

  return 0;
}
