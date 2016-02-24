//top.cpp
#include "top.h"


// constructor
top::top(sc_core::sc_module_name n,
	 double phc_gain,
	 double lp_fc,
	 double vco_freq,
	 double kvco,
	 double vco_gain
	 )
{

//instantiate models

  sine sin0("sin0",0.0,1.0,0.0,1.0);
  sin0.out(sw12);

  sine sin1("sin1",0.0,1.0,0.0,1.0);
  sin1.out(sw13);

  sine sin2("sin2",0.0,1.0,0.0,1.0);
  sin2.out(sw21);

  sine sin3("sin3",0.0,1.0,0.0,1.0);
  sin3.out(sw23);

  sine sin4("sin4",0.0,1.0,0.0,1.0);
  sin4.out(sw31);

  sine sin5("sin5",0.0,1.0,0.0,1.0);
  sin5.out(sw32);

  sine sin6("sin6",0.0,1.0,0.0,1.0);
  sin6.out(sint1);

  sine sin7("sin7",0.0,1.0,0.0,1.0);
  sin6.out(sint2);


  osc1 = new osc("osc1", phc_gain, lp_fc, vco_freq, kvco, vco_gain);
  osc1->vco_ref1(vco1_in1);
  osc1->vco_ref2(vco1_in2);
  osc1->vco_out(vco1_out);
  osc1->lpf_out(lpf_out1);
  osc1->w1(sw21);
  osc1->w2(sw31);

  sp1 = new split("sp1");
  sp1->in(vco1_out);
  sp1->out1(vco2_in1);
  sp1->out2(vco3_in1);

  osc3 = new osc("osc3", phc_gain, lp_fc, vco_freq-100, kvco, vco_gain);
  osc3->vco_ref1(vco3_in1); //
  osc3->vco_ref2(vco3_in2);
  osc3->vco_out(vco3_out);
  osc3->lpf_out(lpf_out3);
  osc3->w1(sw13);
  osc3->w2(sw23);

  sp3 = new split("sp3");
  sp3->in(vco3_out);
  sp3->out1(vco1_in2);
  sp3->out2(vco2_in2);

 
  osc2 = new osc("osc2", phc_gain, lp_fc, vco_freq+100, kvco, vco_gain);
  osc2->vco_ref1(vco2_in1);
  osc2->vco_ref2(vco2_in2);
  osc2->vco_out(vco2_out);
  osc2->lpf_out(lpf_out2);
  osc2->w1(sw12);
  osc2->w2(sw32);

  sp2 = new split("sp2");
  sp2->in(vco2_out);
  sp2->out1(vco1_in1);
  sp2->out2(vco3_in2);

 
}
