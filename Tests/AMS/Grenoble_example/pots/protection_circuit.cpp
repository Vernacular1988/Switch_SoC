// (C) 2009-2010 Fraunhofer Institut Integrierte Schaltungen - EAS Dresden
// Author            : Karsten Einwich
// Email             : karsten.einwich@eas.iis.fraunhofer.de
// Created on        : Jan 16, 2010
//
// protection_circuit.cpp
//
// SVN Version       :  $Revision: 904 $
// SVN last checkin  :  $Date: 2010-01-17 23:28:15 +0100 (So, 17 Jan 2010) $
// SVN checkin by    :  $Author: karsten $
// SVN Id            :  $Id: protection_circuit.cpp 904 2010-01-17 22:28:15Z karsten $
//


#include "protection_circuit.h"

void protection_circuit::architecture()
{
  rprot_tip_slic = new sca_eln::sca_r("rprot_tip_slic");
    rprot_tip_slic->p(tip_slic);
    rprot_tip_slic->n(n_tip);
    rprot_tip_slic->value=p.rprot_tip_slic;

   cprot_tip = new sca_eln::sca_c("cprot_tip");
     cprot_tip->p(n_tip);
     cprot_tip->n(gnd);
     cprot_tip->value=p.cprot_tip;

   rprot_tip_line = new sca_eln::sca_r("rprot_tip_line");
     rprot_tip_line->p(tip);
     rprot_tip_line->n(n_tip);
     rprot_tip_line->value=p.rprot_tip_line;

   /////////////////////////

    rprot_ring_slic = new sca_eln::sca_r("rprot_ring_slic");
      rprot_ring_slic->p(ring_slic);
      rprot_ring_slic->n(n_ring);
      rprot_ring_slic->value=p.rprot_ring_slic;

    cprot_ring = new sca_eln::sca_c("cprot_ring");
      cprot_ring->p(n_ring);
      cprot_ring->n(gnd);
      cprot_ring->value=p.cprot_ring;

    rprot_ring_line = new sca_eln::sca_r("rprot_ring_line");
      rprot_ring_line->p(ring);
      rprot_ring_line->n(n_ring);
      rprot_ring_line->value=p.rprot_ring_line;

}
