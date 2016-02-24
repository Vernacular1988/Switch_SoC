// (C) 2009-2010 Fraunhofer Institut Integrierte Schaltungen - EAS Dresden
// Author            : Karsten Einwich
// Email             : karsten.einwich@eas.iis.fraunhofer.de
// Created on        : Jan 16, 2010
//
// slic.cpp
//
// SVN Version       :  $Revision: 904 $
// SVN last checkin  :  $Date: 2010-01-17 23:28:15 +0100 (So, 17 Jan 2010) $
// SVN checkin by    :  $Author: karsten $
// SVN Id            :  $Id: slic.cpp 904 2010-01-17 22:28:15Z karsten $
//
// Subscriber Line Interface Circuit

#include "slic.h"


void slic::architecture()
{
  tip_driver = new sca_eln::sca_tdf_vsource("tip_driver");
    tip_driver->inp(v_tip_ring);
    tip_driver->p(tip);
    tip_driver->n(n_tip_gnd);
    tip_driver->scale=p.scale_v_tr/2.0;

  i_tip_mirror = new sca_eln::sca_cccs("i_tip_mirror");
    i_tip_mirror->ncp(n_tip_gnd);
    i_tip_mirror->ncn(gnd);
    i_tip_mirror->np(n_tr_i);
    i_tip_mirror->nn(gnd);
    i_tip_mirror->value=0.5;

   ring_driver = new sca_eln::sca_tdf_vsource("ring_driver");
     ring_driver->inp(v_tip_ring);
     ring_driver->p(ring);
     ring_driver->n(n_ring_gnd);
     ring_driver->scale=-p.scale_v_tr/2.0;

   i_ring_mirror = new sca_eln::sca_cccs("i_ring_mirror");
     i_ring_mirror->ncp(n_ring_gnd);
     i_ring_mirror->ncn(gnd);
     i_ring_mirror->np(n_tr_i);
     i_ring_mirror->nn(gnd);
     i_ring_mirror->value=-0.5;

/////////////////////////


   r_tr_meas = new sca_eln::sca_r("r_tr_meas");
     r_tr_meas->p(n_tr_i);
     r_tr_meas->n(gnd);
     r_tr_meas->value=1.0;


   i_tr_measure = new sca_eln::sca_tdf_vsink("i_tr_measure");
     i_tr_measure->p(n_tr_i);
     i_tr_measure->n(gnd);
     i_tr_measure->outp(i_transversal);
     i_tr_measure->scale=p.scale_i_tr;



}
