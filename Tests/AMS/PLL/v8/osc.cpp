// osc.cpp
#include "osc.h"

// array version

//constructor
osc::osc(sc_core::sc_module_name n,
	 double phc_gain,
	 double lp_fc,
	 double vco_freq,
	 double kvco,
	 double vco_gain,
	 double phase
	 ) 
{
  char mn[NN];

  //instantiate models
  for(int i=0; i<NN; i++) {

    sprintf(mn,"%s%i","mula",i);
    mula[i] = new mul(mn);	
    mula[i]->in1(VCO_IN[i]);
    mula[i]->in2(W[i]);
    mula[i]->out(MULA[i]);  // special case the NN=1 case (no adders)

  }

  // build an unbalanced tree (same number of adders as balanced)

  if(NN >= 2) {
    sprintf(mn,"%s%i","adder",0);
    adder[0] = new add(mn);
    adder[0]->in1(MULA[0]);
    adder[0]->in2(MULA[1]);
    adder[0]->out(ADDER[0]);
    cout << "ADDER "<<0<< " gets mul " << 0 << " and mul" << 1 << endl;
 
    for(int i=1; i<NN-1; i++) {
      sprintf(mn,"%s%i","adder",i);
      adder[i] = new add(mn);
      adder[i]->in1(ADDER[i-1]);
      adder[i]->in2(MULA[i+1]);
      adder[i]->out(ADDER[i]);
      cout << "ADDER "<<i<< " gets Adder " << i-1 << " and mul" << i+1 << endl; 
    }  
  }

  pll1 = new my_pll("pll1", phc_gain, lp_fc, vco_freq, kvco, vco_gain, phase);
  if(NN == 1) {
    pll1->ref(MULA[0]);
  } 
  else {
    pll1->ref(ADDER[NN-2]);
    cout << "pll gets adder "<< NN-2 << endl;
  }
  pll1->vcod(VCO_OUT);
  pll1->lpo(LPF_OUT);

}
