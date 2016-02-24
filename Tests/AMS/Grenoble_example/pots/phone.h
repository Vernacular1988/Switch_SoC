// (C) 2009-2010 Fraunhofer Institut Integrierte Schaltungen - EAS Dresden
// Author            : Karsten Einwich
// Email             : karsten.einwich@eas.iis.fraunhofer.de
// Created on        : Jan 16, 2010
//
// phone.h
//
// SVN Version       :  $Revision: 904 $
// SVN last checkin  :  $Date: 2010-01-17 23:28:15 +0100 (So, 17 Jan 2010) $
// SVN checkin by    :  $Author: karsten $
// SVN Id            :  $Id: phone.h 904 2010-01-17 22:28:15Z karsten $
//

#ifndef PHONE_H_
#define PHONE_H_

#include <systemc-ams>

SC_MODULE(phone)
{
  sca_eln::sca_terminal tip;
  sca_eln::sca_terminal ring;

  sca_tdf::sca_in<double> v_voice_in;

  sc_core::sc_in<bool> hook;


  struct params
  {
    double rs;
    double rp;
    double cp;
    double rring;
    double cring;

    params()
      {
        rs = 220.0;
        rp = 820.0;
        cp = 115.0e-9;
        rring = 1.0e3;
        cring = 1.0e-6;
      }
  };

  //Instance
  sca_eln::sca_de_rswitch *hook_sw1;
  sca_eln::sca_de_rswitch *hook_sw2;

  sca_eln::sca_c *cring;
  sca_eln::sca_r *rring;
  sca_eln::sca_r *rs;
  sca_eln::sca_c *cp;
  sca_eln::sca_r *rp;
  sca_eln::sca_tdf_vsource *vin;


  //Nodes
  sca_eln::sca_node w_offhook;
  sca_eln::sca_node w_onhook;
  sca_eln::sca_node w1;
  sca_eln::sca_node w2;
  sca_eln::sca_node wring;


   void architecture();


   phone(sc_core::sc_module_name nm, params pa=params()) :  p(pa)
   {
     architecture();

   }

private:

  params p;

};


#endif /* PHONE_H_ */
