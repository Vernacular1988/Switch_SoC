// main.cpp

#include "drain.h"
#include "pll.h"

int sc_main(int argc, char* argv[])
{
  sc_set_time_resolution(1, SC_NS);     // for solvers

 /********** defining signals *********** */

  sca_tdf::sca_signal<double> modulation;
  sca_tdf::sca_signal<double> source;
  sca_tdf::sca_signal<double> vco_out_delay;
  // pll loop signals
  sca_tdf::sca_signal<double> lpf_out;
  sca_tdf::sca_signal<double> pco;
  sca_tdf::sca_signal<double> vco_out;

 /********** defining parameters *********** */
  // sine wave generator control
  double	s_freq = 1000;	// sine base frequency
  double	m_freq = 10.0;	// sine modulator frequency
  int 		s_rate = 1;	// sine sampling rate
  double 	s_amp = 1.0;	// sine amplitude
  double	s_ph = 0.0;	// sine phase
  double	s_offset = 0.0;	// sine offset
  bool		s_fc = false;	// sine freq control port active
  bool		s_ac = false;	// sine amp control port active

  // pll controls
  // phase comparator control
  int		phc_out_rate = s_rate; // phase comparitor output rate
  double	phc_gain = 100.0;	// phase comparitor gain

  // low pass filter control
  double	lp_fc = 10.0;	// low pass filter cut off freq

  // analog vco control
  double	vco_freq = 1000.0; // natural frequency of vco
  int		vco_out_rate = s_rate; // output sample rate of vco
  double	kvco = 100.0;	// control sensitivity for vco
  double	vco_gain = 1.0;	// amplitude gain of vco
 

 /********** setting runtime parameters for simulation *********** */

  cout <<"\n" << "source frequency = ";  
  cin >> s_freq;
  std::cout <<"\n s_freq is " << s_freq <<"\n" ;

  cout <<"\n" << "modulation frequency =";  
  cin >> m_freq;
  std::cout <<"\n m_freq is " << m_freq <<"\n" ;

  cout <<"\n" << "sine sample rate ="; 
  cin >> s_rate;
  std::cout <<"\n sine sample rate is " << s_rate << "\n";

/********** instantiating library-modules**********/

  sine sinin("sinin", s_freq, s_amp, s_ph, s_offset, s_fc, s_ac, s_rate); 

  //sinin.freq_in(modulation);
  sinin.set_timestep(10.0, SC_US);  // at least one module/port needs this
  sinin.out(source);

  // four modules to make the pll (analog hierarchy not possble)

  // phase comparitor
  phc phc_sub("phc_sub",phc_out_rate,phc_gain);
  phc_sub.in_ref(source);
  phc_sub.in_vco(vco_out_delay);
  phc_sub.out(pco);

  // low pass filter
  lp lp_sub("lp_sub",lp_fc);
  lp_sub.in(pco);
  lp_sub.out(lpf_out);

  // analog vco
  a_vco vco_sub("vco_sub",vco_freq,vco_out_rate,kvco,vco_gain);
  vco_sub.in(lpf_out);
  vco_sub.out(vco_out);
  //vco_sub.out.set_delay(1);   // does not work

  // analog delay block (needed because of above)
  a_delay vco_delay("vco_delay");
  vco_delay.inp(vco_out);
  vco_delay.outp(vco_out_delay);

  // sink for unused output signal
  drain drn2("drn2");
  drn2.in(lpf_out);


/********* tracing signals *************************/
  sca_util::sca_trace_file* atf = sca_util::sca_create_vcd_trace_file( "pll" );

  sca_util::sca_trace( atf, source, "source" );
  sca_util::sca_trace( atf, modulation ,"modulation" );
  sca_util::sca_trace( atf, vco_out_delay ,"vco_out_delay" );
  sca_util::sca_trace( atf, lpf_out ,"lpf_out" );

  sc_start(1000.0, SC_MS);

  sca_util::sca_close_vcd_trace_file( atf );

  return 0;
}
