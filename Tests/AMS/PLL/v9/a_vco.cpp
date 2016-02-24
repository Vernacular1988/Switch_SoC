// a_vco.cpp

#include "a_vco.h"

// minor rewrite of TUV a_vco to set initial phase -- spl 
/******************************Analog VCO*************************************************/
void my_vco::initialize()
{
  //sample_time = out.get_timestep().to_seconds();	// read sample time
  sample_time = out.get_timestep().to_seconds();
  cout << "Sample-Time: " << sample_time << endl;
}

void my_vco::set_attributes()
{
  out.set_rate(rate);
}

void my_vco::processing()
{

  double fr=freq+kvco*in.read();
  stepsize = sample_time*fr*2.*M_PI;

  for(int i=0; i<rate; i++)  		// write <rate> data token
    {
      out.write(gain*sin(actval),i);
      actval+=stepsize;
    }
}

my_vco::my_vco(sc_core::sc_module_name n, double freq_data, int datarate, double _kvco, double _gain, double _phase)
{
  freq=freq_data;
  rate=datarate;
  actval = _phase;
  kvco=_kvco;
  gain=_gain;
}
