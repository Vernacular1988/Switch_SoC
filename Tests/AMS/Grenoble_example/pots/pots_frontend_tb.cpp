// (C) 2009-2010 Fraunhofer Institut Integrierte Schaltungen - EAS Dresden
// Author            : Karsten Einwich
// Email             : karsten.einwich@eas.iis.fraunhofer.de
// Created on        : Jan 16, 2010
//
// pots_frontend_tb.cpp
//
// SVN Version       :  $Revision: 904 $
// SVN last checkin  :  $Date: 2010-01-17 23:28:15 +0100 (So, 17 Jan 2010) $
// SVN checkin by    :  $Author: karsten $
// SVN Id            :  $Id: pots_frontend_tb.cpp 904 2010-01-17 22:28:15Z karsten $
//
// Testbench for Plain old telephone service frontend
//

#include <systemc-ams.h>
#include "phone.h"
#include "protection_circuit.h"
#include "slic.h"

/////////////////////////////////

SCA_TDF_MODULE(sin_src)
{
    sca_tdf::sca_out<double> outp;

    struct params
    {
        double  freq;
        double  ampl;
        double  offset;
        sca_core::sca_time sample_time;

        params()
        {
            ampl = 1.0;
            offset=0.0;
            freq = 1e3;
            sample_time = sca_core::sca_time(10.0,sc_core::SC_US);
        }
    };

    void set_attributes()
    {
        outp.set_timestep(p.sample_time);
    }

    void ac_processing();
    void processing();

    sin_src(sc_core::sc_module_name nm,params pa=params()) : p(pa) {}

    params p;

};


//frequency domain implementation
void sin_src::ac_processing()
{
    //we use for ac-domain for all frequencies the same amplitude and a phase=0
    sca_ac_analysis::sca_ac(outp) = p.ampl;
}


//time domain implementation
void sin_src::processing()
{
    outp = p.ampl * sin(2.0*M_PI*p.freq*get_time().to_seconds()) + p.offset;
}


////////////////////////////////////////////////////////



int sc_main(int argc,char* argv[])
{
  sca_eln::sca_node n_slic_tip, n_slic_ring;
  sca_eln::sca_node n_tip, n_ring, n_tip_ph, n_ring_ph;

  sca_tdf::sca_signal<double> s_v_tip_ring;
  sca_tdf::sca_signal<double> s_i_transversal;

  sca_tdf::sca_signal<double> s_voice;
  sc_core::sc_signal<bool>    s_hook;

  sin_src::params p_src_vtr;
  p_src_vtr.ampl=0.0;
  p_src_vtr.offset=10.0;
  p_src_vtr.freq=1e3;
  sin_src* src_vtr = new sin_src("src_vtr",p_src_vtr);
    src_vtr->outp(s_v_tip_ring);


  slic* i_slic = new slic("i_slic");
    i_slic->tip(n_slic_tip);
    i_slic->ring(n_slic_ring);
    i_slic->v_tip_ring(s_v_tip_ring);
    i_slic->i_transversal(s_i_transversal);

  protection_circuit* i_protection_circuit = new protection_circuit("i_protection_circuit");
    i_protection_circuit->tip_slic(n_slic_tip);
    i_protection_circuit->ring_slic(n_slic_ring);
    i_protection_circuit->tip(n_tip);
    i_protection_circuit->ring(n_ring);


  sca_eln::sca_transmission_line* line=new sca_eln::sca_transmission_line("line");
      line->a1(n_tip);
      line->a2(n_ring);
      line->b1(n_tip_ph);
      line->b2(n_ring_ph);
      line->delay=sca_time(1.0,SC_US);


  phone* i_phone = new phone("i_phone");
    i_phone->tip(n_tip_ph);
    i_phone->ring(n_ring_ph);
    i_phone->v_voice_in(s_voice);
    i_phone->hook(s_hook);

  sin_src::params p_src_voice;
  p_src_voice.ampl=1.0;
  p_src_voice.offset=0.0;
  p_src_voice.freq=1e3;
  sin_src* src_voice = new sin_src("src_voice",p_src_voice);
    src_voice->outp(s_voice);


  sca_util::sca_trace_file* tf=sca_util::sca_create_tabular_trace_file("pots_fe.dat");
  sca_util::sca_trace(tf,n_tip,"n_tip");
  sca_util::sca_trace(tf,n_ring,"n_ring");
  sca_util::sca_trace(tf,n_slic_tip,"n_slic_tip");
  sca_util::sca_trace(tf,n_slic_ring,"n_slic_ring");
  sca_util::sca_trace(tf,s_i_transversal,"s_i_transversal");
  sca_util::sca_trace(tf,s_voice,"s_voice");
  sca_util::sca_trace(tf,s_v_tip_ring,"s_v_tip_ring");
  sca_util::sca_trace(tf,i_phone->rring,"i_phone.i_rring");
  sca_util::sca_trace(tf,i_phone->rs,"i_phone.i_rs");

  s_hook.write(false);  //subscriber onhook

  sc_core::sc_start(5.0,SC_MS);

  s_hook.write(true);  //subscriber offhook

  sc_core::sc_start(5.0,SC_MS);

  src_voice->p.ampl=0.0;
  src_vtr->p.ampl=1.0;

  sc_core::sc_start(5.0,SC_MS);

  s_hook.write(false);  //subscriber onhook

  sc_core::sc_start(5.0,SC_MS);

  src_voice->p.ampl=1.0;

  sc_core::sc_start(5.0,SC_MS);

  s_hook.write(true);  //subscriber offhook

  src_voice->p.ampl=0.0;
  src_vtr->p.ampl=10.0;
  src_vtr->p.freq=50.0;

  sc_core::sc_start(100.0,SC_MS);

  s_hook.write(false);  //subscriber onhook

  sc_core::sc_start(100.0,SC_MS);

  tf->reopen("pots_fe_ac_on_vtr.dat");
  tf->set_mode(sca_util::sca_ac_format(sca_util::SCA_AC_DB_DEG));

  src_voice->p.ampl=0.0;
  src_vtr->p.ampl=1.0;

  sca_ac_analysis::sca_ac_start(10.0,64e3,1000,sca_ac_analysis::SCA_LOG);

  tf->reopen("pots_fe_ac_on_voice.dat");
  src_voice->p.ampl=1.0;
  src_vtr->p.ampl=0.0;

  sca_ac_analysis::sca_ac_start(10.0,64e3,1000,sca_ac_analysis::SCA_LOG);

  tf->disable();

  s_hook.write(true);  //subscriber offhook
  sc_core::sc_start(1.0,sc_core::SC_MS);  // signal must become valid

  src_voice->p.ampl=0.0;
  src_vtr->p.ampl=1.0;

  tf->reopen("pots_fe_ac_off_vtr.dat");
  tf->enable();

  sca_ac_analysis::sca_ac_start(10.0,64e3,1000,sca_ac_analysis::SCA_LOG);

  tf->reopen("pots_fe_ac_off_voice.dat");
  src_voice->p.ampl=1.0;
  src_vtr->p.ampl=0.0;

  sca_ac_analysis::sca_ac_start(10.0,64e3,1000,sca_ac_analysis::SCA_LOG);

  std::cout << sc_core::sc_time_stamp() << " simulation finished" << std::endl;

  return 0;
}

