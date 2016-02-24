// (C) 2009-2010 Fraunhofer Institut Integrierte Schaltungen - EAS Dresden
// Author            : Karsten Einwich
// Email             : karsten.einwich@eas.iis.fraunhofer.de
// Created on        : Jan 16, 2010
//
// protection_circuit.h
//
// SVN Version       :  $Revision: 904 $
// SVN last checkin  :  $Date: 2010-01-17 23:28:15 +0100 (So, 17 Jan 2010) $
// SVN checkin by    :  $Author: karsten $
// SVN Id            :  $Id: protection_circuit.h 904 2010-01-17 22:28:15Z karsten $
//

#ifndef PROTECTION_CIRCUIT_H_
#define PROTECTION_CIRCUIT_H_

#include <systemc-ams>

SC_MODULE(protection_circuit)
{
  //Ports
  sca_eln::sca_terminal  tip_slic;
  sca_eln::sca_terminal  ring_slic;
  sca_eln::sca_terminal  tip;
  sca_eln::sca_terminal  ring;

  struct params
  {
    double rprot_tip_line;
    double rprot_ring_line;
    double rprot_tip_slic;
    double rprot_ring_slic;
    double cprot_tip;
    double cprot_ring;

    params()
    {
      rprot_tip_line  = 20.0;
      rprot_ring_line = 20.0;
      rprot_tip_slic  = 20.0;
      rprot_ring_slic = 20.0;
      cprot_tip  = 18.0e-9;
      cprot_ring = 18.0e-9;
    }
  };


  sca_eln::sca_c     *cprot_tip;
  sca_eln::sca_c     *cprot_ring;

  sca_eln::sca_r     *rprot_tip_line;
  sca_eln::sca_r     *rprot_ring_line;
  sca_eln::sca_r     *rprot_tip_slic;
  sca_eln::sca_r     *rprot_ring_slic;


  //Nodes
  sca_eln::sca_node  n_tip;
  sca_eln::sca_node  n_ring;
  sca_eln::sca_node_ref  gnd;

  void architecture();

  protection_circuit(sc_core::sc_module_name, params pa=params()) : p(pa)
  {
     architecture();

  }


private:
   params p;

};




#endif /* PROTECTION_CIRCUIT_H_ */
