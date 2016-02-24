// (C) 2009-2010 Fraunhofer Institut Integrierte Schaltungen - EAS Dresden
// Author            : Karsten Einwich
// Email             : karsten.einwich@eas.iis.fraunhofer.de
// Created on        : Jan 16, 2010
//
// slic.h
//
// SVN Version       :  $Revision: 904 $
// SVN last checkin  :  $Date: 2010-01-17 23:28:15 +0100 (So, 17 Jan 2010) $
// SVN checkin by    :  $Author: karsten $
// SVN Id            :  $Id: slic.h 904 2010-01-17 22:28:15Z karsten $
//
// Subscriber Line Interface Circuit

#ifndef SLIC_H_
#define SLIC_H_

#include <systemc-ams>

SC_MODULE(slic)
{
  sca_eln::sca_terminal tip;
  sca_eln::sca_terminal ring;

  sca_tdf::sca_in<double> v_tip_ring;

  sca_tdf::sca_out<double> i_transversal;

  struct params
  {
    double scale_v_tr;
    double scale_i_tr;

    params()
    {
      scale_v_tr=1.0;
      scale_i_tr=1.0;
    }
  };

  sca_eln::sca_tdf_vsource* tip_driver;
  sca_eln::sca_tdf_vsource* ring_driver;

  sca_eln::sca_cccs*  i_tip_mirror;
  sca_eln::sca_cccs*  i_ring_mirror;

  sca_eln::sca_r*  r_tr_meas;
  sca_eln::sca_tdf_vsink* i_tr_measure;

  sca_eln::sca_node n_tr_i;
  sca_eln::sca_node n_tip_gnd, n_ring_gnd;
  sca_eln::sca_node_ref gnd;

  void architecture();

  slic(sc_core::sc_module_name, params pa=params()) : p(pa)
  {
    architecture();
  }

private:

  params p;

};

#endif /* SLIC_H_ */
