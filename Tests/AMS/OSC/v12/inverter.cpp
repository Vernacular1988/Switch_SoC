// file inverter.c

// copied from:
// "Analog Circuit Modeling in SystemC"
// M. Conti, M. Caldari, S. Orcioni, G. Biagetti, D.E.I.T. 
// Universita Politecnica delle Marche, via Brecce Bianche, 
// I-60131, Ancona, Italy

// solution of linear first order systems 
// x' = f(x,u)  y = g(x,u)
// solved with simple Euler integration
// x(t+dt) = x(t) + dt * f(x(t),u(t)) 
// y(t) = g(x(t),u(t))
//

#include "inverter.h"

#if 1

int sc_main(int argc, char* argv[]) {

  double start, finish, duration;

  sc_set_time_resolution(1, SC_FS);
  sc_set_default_time_unit(1, SC_SEC);
  // Signals
  sc_signal<float> in, in2, I1, I2, I3;
  sc_signal<float> V1, V2, V3, dt1, dt2, dt3;

  // three inverters in a ring oscilator
  inverter INV1("INV1"); 
  inverter INV2("INV2"); 
  inverter INV3("INV3"); 

  // parameters of inverters
  INV1.WLP=1;		// transistor widths in microns
  INV1.WLN=1;
  INV1.st0=0;		// state (region of operation) variable
  INV1.i1_in(I3);	// input output currents/voltages
  INV1.i2_out(I1);
  INV1.v1_out(V3);
  INV1.v2_in(V1);
  INV1.dt2(dt2);	// needed to syncrhonize time evaluations
  INV1.dt1(dt3);
  INV1.dt(dt1);
  INV1.DTMAX = 3.0e-12f; // f forces a float vs double

  INV2.WLP=1;		
  INV2.WLN=1;
  INV2.st0=0;		
  INV2.i1_in(I1);	
  INV2.i2_out(I2);
  INV2.v1_out(V1);
  INV2.v2_in(V2);
  INV2.dt2(dt3);	
  INV2.dt1(dt1);
  INV2.dt(dt2);
  INV2.DTMAX=3.0e-12f; 

  INV3.WLP=1;		
  INV3.WLN=1;
  INV3.st0=0;		
  INV3.i1_in(I2);	
  INV3.i2_out(I3);
  INV3.v1_out(V2);
  INV3.v2_in(V3);
  INV3.dt2(dt1);	
  INV3.dt1(dt2);
  INV3.dt(dt3);
  INV3.DTMAX=3.0e-12f; 


  // start the simulation 
  start = clock();
  printf("Starting Simulation\n");
  sc_start(1,SC_NS); //40 
  finish = clock();
  duration = double(finish-start);
  return 0;
}

//
// Inverter macro model
// I/O is both voltage and current as four separate scalars
// We compute our currents and voltages with our neigbors
// 
// i1_in is current into the common gate node
//   computed by by our driving neighbor
//
// i2_out is current out of the common drain node to our driven neighbor
//   as a differnce between the P and N channel currents
//
// v1_out is Vcg voltage back to the driving neighbor
//   how much their current charged our capacitors
//
// v2_in  is Vdsn voltage of output node set by our driven neighbor
//   how much our voltage charged our driven neighbors gate capacitor
//

void inverter::calculus() { // SC_THREAD
  float i_in;
  float vout; 
  float cl = CGS;	// should be result of function Cgd
  float cgd = CGD; 	// should be result of function Cgd
  float icgd = 0;      	// should be a function of caps as well
  float state, state2, next_state, next_state2;
  float delta, delta1, delta2;
  // ...
  float V1 = st0;	// our copy of Vin
  float V2 = 0;         // V across Cg
  float dt_aux = DTMIN;	// temp for tuning dt, the local time step
  float next_V1, next_V2; // temp for computing values and time step

  //  sc_time T1, T2;
  //const sc_time& t1 = T1;
  //const sc_time& t2 = T2;

  double t1, t2;
  float i_drain;
  
  // initialize values declared in inverter.h
  i2_out = 0;		// output port values
  v1_out = st0;		// 
  flag = 0;		// shared flag for activations and calculus

  while(true) {
    // input read
    vout = v2_in.read();
    i_in = i1_in.read();

    ///...
    // convergence loop: adjust time step to ensure delta V is within
    // toleraces
    do {
      // use new timestep
      dt = dt_aux;
      printf("delta %g  dt %g \n", delta, dt_aux);

      // re-calculate new voltages from equations using this timestep
      // from paper integrate input current on gate
      icgd = (V1 - (V2 + vout))/Rf;
      next_V1 = V1 + ((i_in/cl) - (icgd/cl))*dt;
      next_V2 = V2 + (icgd/cgd)*dt;
      
      // calculate max delta V to see if we need to change step size 
      delta1 = fabs(next_V1 - V1);
      delta2 = fabs(next_V2 - V2);
      delta = MAX(delta1,delta2);

      // adapt step size 
      if((delta > VTOL2) && (dt > DTMIN)) dt_aux /= F; 
      if((delta < VTOL1) && (dt < DTMAX)) dt_aux *= F; 

    } while ((delta > VTOL2) && (dt > DTMIN));

    // at this point we either have a "good convergence" or we have 
    // decreased the time step as small as we can

    t1 = sc_simulation_time();
    wait(dt,SC_SEC,activation_event);
    t2 = sc_simulation_time();
 
   if(t2 < t1 + dt) {
      next_V1 = V1 + (next_V1 - V1) * (t2 - t1)/dt;
      next_V2 = V2 + (next_V2 - V2) * (t2 - t1)/dt;
      if (flag == 1) dt_aux = DTMIN;
    }

    flag = 0;
    V1 = next_V1;
    V2 = next_V2;

    //...
    i_drain = Ipmos(V1, vout) - Inmos(V1,vout) + icgd; // output current
    i2_out = i_drain;
    v1_out = V1;                   // input voltage

    printf("Iout: %g \t Vout: %g\n", i_drain, V1);

  }
}

// activiation threads, one for each input module
// looks at input value changes and/or notices that the 
// input module has decreased its own step size 
// flag = 1 means that it was a time change

// watches input 1 for changes in value or dt
void inverter::activation1() {
  float in1 = 0;
  float in2 = 0;
  float dt_ext1, dt_ext2, dt_ext;

  while(true) {
    dt_ext1 = dt1.read();
    dt_ext2 = dt2.read();
    dt_ext = MIN(dt_ext1,dt_ext2);

    in2 = i1_in.read();	// new value of input 1 

    if (abs(in2 - in1) > CHANGE_THRESHOLD) {

      activation_event.notify(SC_ZERO_TIME);

    } else if ((dt_ext != 0) && (dt_ext < dt/K)) {

      flag = 1; 
      activation_event.notify(SC_ZERO_TIME);

    }
    in1 = in2;
    wait();
  }
}

// watches input 2 for changes in value or dt
void inverter::activation2() {
  float in1 = 0;
  float in2 = 0;
  float dt_ext1, dt_ext2, dt_ext;

  while(true) {
    dt_ext1 = dt1.read();
    dt_ext2 = dt2.read();
    dt_ext = MIN(dt_ext1,dt_ext2);

    in2 = v2_in.read();	// new value of input 2

    if (abs(in2 - in1) > CHANGE_THRESHOLD) {

      activation_event.notify(SC_ZERO_TIME);

    } else if ((dt_ext != 0) && (dt_ext < dt/K)) {

      flag = 1; 
      activation_event.notify(SC_ZERO_TIME);

    }
    in1 = in2;
    wait();
  }
}

#else 

float Inmos(float, float);
float Ipmos(float, float);

int main(int argc, char **agrv){

  printf("Vin \t Vout \t Ids\n");
  for (float vin = 0; vin<=VDD; vin+=0.1) {
    for(float vout = 0; vout<=VDD; vout+=0.1) {
      printf("%g \t %g \t %g\n", vin, vout, Ipmos(vin,vout)-Inmos(vin,vout));
    }
  }
}

#endif

// returns Ids in terms of Vin (=vgs) Vout (=vds)
float inverter::Inmos(float vin, float vout) {
  float ids = 0;
  float vth = 0.4;
  float vGT = vin-vth; // from -.4 to 1.4
  float beta = 345;

  if(vGT <= 0) {
    // cut off
    ids = 0;
  } else if (vout <= vGT) {
    // linear 
    ids = beta*(vGT-vout/2.0)*vout;
  } else {
    // saturation
    ids = beta*vGT*vGT/2.0;
  }
  printf("Nchannel ids = %g \n",ids);
  return ids;
}


// NOTE: returns Isd (-Ids) in terms of Vin (=vgs+Vdd) Vout (=vds+VDD)
float inverter::Ipmos(float vin, float vout) {
  float ids = 0;
  float vth = -0.4;
  float beta = 345;
  float vgs = vin - VDD;
  float vds = vout - VDD;
  float vGT = (vgs-vth);

  if(vgs-vth >= 0) {
    // cut off
    ids = 0;
  } else if (vds >= (vgs-vth)) {
    // linear 
    ids = beta*(vGT-vds/2.0)*vds;
  } else {
    // saturation
    ids = beta*vGT*vGT/2.0;
  }
  printf("pchannel ids = %g \n",ids);
  return ids;
}
