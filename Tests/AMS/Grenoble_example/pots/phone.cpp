// (C) 2009-2010 Fraunhofer Institut Integrierte Schaltungen - EAS Dresden
// Author            : Karsten Einwich
// Email             : karsten.einwich@eas.iis.fraunhofer.de
// Created on        : Jan 16, 2010
//
// comb_filter_tb.h
//
// SVN Version       :  $Revision: 904 $
// SVN last checkin  :  $Date: 2010-01-17 23:28:15 +0100 (So, 17 Jan 2010) $
// SVN checkin by    :  $Author: karsten $
// SVN Id            :  $Id: phone.cpp 904 2010-01-17 22:28:15Z karsten $
//

#include "phone.h"


void phone::architecture()
{

  // toggle switch for hook

  hook_sw1 = new sca_eln::sca_de_rswitch("hook_sw1");
     hook_sw1->p(tip);
     hook_sw1->n(w_onhook);
     hook_sw1->ctrl(hook);
     hook_sw1->off_state=true;

  hook_sw2 = new sca_eln::sca_de_rswitch("hook_sw2");
     hook_sw2->p(tip);
     hook_sw2->n(w_offhook);
     hook_sw2->ctrl(hook);



  // ringing load (onhook)

  cring = new sca_eln::sca_c("cring");
    cring -> p(wring);
    cring -> n(w_onhook);
    cring -> value = p.cring;

  rring = new sca_eln::sca_r("rring");
    rring -> p(wring);
    rring -> n(ring);
    rring -> value = p.rring;

  // active (subscriber) load (offhook)

  rs = new sca_eln::sca_r("rs");
    rs -> p(w1);
    rs -> n(w2);
    rs -> value = p.rs;

  cp = new sca_eln::sca_c("cp");
    cp -> p(w1);
    cp -> n(w_offhook);
    cp -> value = p.cp;

  rp = new sca_eln::sca_r("rp");
    rp -> p(w_offhook);
    rp -> n(w1);
    rp -> value = p.rp;

  // microphone
  vin = new sca_eln::sca_tdf_vsource("vin");
    vin -> p(w2);
    vin -> n(ring);
    vin -> inp(v_voice_in);

}
