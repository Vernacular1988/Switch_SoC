/*****************************************************************************
   (C) 2010 Fraunhofer Institut Integrierte Schaltungen - EAS Dresden
   
  state_space_complex_test.cpp - description

  Original Author: Karsten Einwich Fraunhofer IIS/EAS Dresden

  Created on: 30.12.2009

   SVN Version       :  $Revision: 915 $
   SVN last checkin  :  $Date: 2010-01-27 00:03:09 +0100 (Mi, 27 Jan 2010) $
   SVN checkin by    :  $Author: karsten $
   SVN Id            :  $Id: state_space_complex_test.cpp 915 2010-01-26 23:03:09Z karsten $

 *****************************************************************************/

/*****************************************************************************/

#include <systemc-ams>


////////////////////////////////////////////////////////
//
// complex statespace example derived from an electrical network
//
////////////////////////////////////////////////////////

// State space implementation of complex example

SCA_TDF_MODULE (statespace_from_net)
{
    // ports
    sca_tdf::sca_in<double> vin1;
    sca_tdf::sca_in<double> vin2;

    sca_tdf::sca_out<double  > icli;
    sca_tdf::sca_out<double  > irco;
    sca_tdf::sca_out<double  > vrco;

    struct params //parameter
    {
        double  Ktr;
        double  L2tr;
        double  R1tr;
        double  L1tr;
        double  Rintr;
        double  Cintr;
        double  R3tr;

        double  Cli;
        double  RRF1;
        double  CRF1;
        double  RRF2;
        double  CRF2;
        double  RCO;

        params() //default for parameter
        {
            Ktr = 1.0/2.0;
            L2tr= 18.0e-3;
            R1tr= 1.0e3;
            L1tr= 14.2e-3;
            Rintr=6.3;
            Cintr=200.0e-12;
            R3tr =6.2;

            Cli = 0.12e-6;
            RRF1= 100.0;
            CRF1= 1.0e-12;
            RRF2= 100.0;
            CRF2= 1.0e-12;
            RCO = 25.0;
        }
    };

    void ac_processing();  //ac-implementation
    void initialize();     //initialization
    void processing();     //time domain processing method

    params p;


    // constructor
    statespace_from_net(sc_core::sc_module_name nm, params pa=params()) : p(pa)
    {}

    // definition of local variables
private:
    sca_tdf::sca_ss  state_space1;
    sca_util::sca_matrix<double> A, B, C, D;
    sca_util::sca_vector<double> S;

};


//implementation of methods

void statespace_from_net::initialize()
{
    A(0,0) = 0.0;
    A(0,1) = 0.0;
    A(0,2) = 0.0;
    A(0,3) = 0.0;
    A(0,4) = 0.0;
    A(0,5) = -(p.L2tr/(pow(p.Cli,2)*p.Ktr));

    A(1,0) = 0.0;
    A(1,1) = -((p.R1tr*p.RCO + p.RCO*p.Rintr + p.R1tr*p.RRF1 + p.RCO*p.RRF1 + \
                p.Rintr*p.RRF1)/(p.Cintr*p.R1tr*p.RCO*p.RRF1 + p.Cintr*p.RCO*p.Rintr*p.RRF1));
    A(1,2) = p.CRF1/(pow(p.Cintr,2)*p.RRF1);
    A(1,3) = 0.0;
    A(1,4) = -((p.L1tr*p.R1tr)/(pow(p.Cintr,2)*(p.R1tr + p.Rintr)));
    A(1,5) = -((p.L2tr*p.R1tr)/(pow(p.Cintr,2)*(p.R1tr + p.Rintr)));

    A(2,0) = 0.0;
    A(2,1) = p.Cintr/(pow(p.CRF1,2)*p.RRF1);
    A(2,2) = -((p.RRF1 + p.RRF2)/(p.CRF1*p.RRF1*p.RRF2));
    A(2,3) = p.CRF2/(pow(p.CRF1,2)*p.RRF2);
    A(2,4) = 0.0;
    A(2,5) = 0.0;

    A(3,0) = 0.0;
    A(3,1) = 0.0;
    A(3,2) = p.CRF1/(pow(p.CRF2,2)*p.RRF2);
    A(3,3) = -(1.0/(p.CRF2*p.RRF2));
    A(3,4) = 0.0;
    A(3,5) = 0.0;

    A(4,0) = 0.0;
    A(4,1) = (p.Cintr*p.R1tr)/(pow(p.L1tr,2)*(p.R1tr + p.Rintr));
    A(4,2) = 0.0;
    A(4,3) = 0.0;
    A(4,4) = -((p.R1tr*p.Rintr)/(p.L1tr*(p.R1tr + p.Rintr)));
    A(4,5) = -((p.L2tr*p.R1tr*p.Rintr)/(pow(p.L1tr,2)*(p.R1tr + p.Rintr)));

    A(5,0) = p.Cli/(p.Ktr*pow(p.L2tr,2));
    A(5,1) = (p.Cintr*p.R1tr)/(pow(p.L2tr,2)*(p.R1tr + p.Rintr));
    A(5,2) = 0.0;
    A(5,3) = 0.0;
    A(5,4) = -((p.L1tr*p.R1tr*p.Rintr)/(pow(p.L2tr,2)*(p.R1tr + p.Rintr)));
    A(5,5) = -((p.R3tr*p.Rintr + p.R1tr*(p.R3tr + \
                                         pow(p.Ktr,2)*p.Rintr))/(pow(p.Ktr,2)*p.L2tr*(p.R1tr + p.Rintr)));


    /////////////////////////////////////////////////////////////////////////

    B(0,0) = 0.0;
    B(0,1) = 0.0;

    B(1,0) = 0.0;
    B(1,1) = 1.0/(pow(p.Cintr,2)*p.RCO);

    B(2,0) = 0.0;
    B(2,1) = 0.0;

    B(3,0) = 0.0;
    B(3,1) = 0.0;

    B(4,0) = 0.0;
    B(4,1) = 0.0;

    B(5,0) = -1.0/(p.Ktr*pow(p.L2tr,2));
    B(5,1) = 0.0;

    ////////////////////////////////////////////////////////////////////////


    C(0,0) = 0.0;
    C(0,1) = 0.0;
    C(0,2) = 0.0;
    C(0,3) = 0.0;
    C(0,4) = 0.0;
    C(0,5) = -p.L2tr/p.Ktr;

    C(1,0) = 0.0;
    C(1,1) = p.Cintr/p.RCO;
    C(1,2) = 0.0;
    C(1,3) = 0.0;
    C(1,4) = 0.0;
    C(1,5) = 0.0;

    C(2,0) = 0.0;
    C(2,1) = p.Cintr;
    C(2,2) = 0.0;
    C(2,3) = 0.0;
    C(2,4) = 0.0;
    C(2,5) = 0.0;

    ////////////////////////////////////////////////////////////////////////


    D(0,0) = 0.0;
    D(0,1) = 0.0;

    D(1,0) = 0.0;
    D(1,1) = -1.0/p.RCO;

    D(2,0) = 0.0;
    D(2,1) = 0.0;



    //////////////////////////////////////////////////////////////////////////
}


void statespace_from_net::ac_processing()
{
    sca_util::sca_vector<sca_util::sca_complex> inp;
    inp(0) = sca_ac_analysis::sca_ac(vin1);
    inp(1) = sca_ac_analysis::sca_ac(vin2);

    sca_util::sca_vector<sca_util::sca_complex> out;
    out = sca_ac_analysis::sca_ac_ss(A,B,C,D,inp);

    sca_ac_analysis::sca_ac(icli) = out(0);
    sca_ac_analysis::sca_ac(irco) = out(1);
    sca_ac_analysis::sca_ac(vrco) = out(2);
}


void statespace_from_net::processing()
{
    sca_util::sca_vector<double> inp;
    inp(0) = vin1.read();
    inp(1) = vin2.read();

    sca_util::sca_vector<double> outp = state_space1(A,B,C,D,S,inp);


    icli.write(outp(0));
    irco.write(outp(1));
    vrco.write(outp(2));
}


/////////////////////////////////////////////////////////////////////////////////

SC_MODULE(statespace_from_net_lsf)
{
    sca_tdf::sca_in<double> vin1;
    sca_lsf::sca_out        icli;

    struct params //parameter
    {
        double  Ktr;
        double  L2tr;
        double  R1tr;
        double  L1tr;
        double  Rintr;
        double  Cintr;
        double  R3tr;

        double  Cli;
        double  RRF1;
        double  CRF1;
        double  RRF2;
        double  CRF2;
        double  RCO;

        params() //default for parameter
        {
            Ktr = 1.0/2.0;
            L2tr= 18.0e-3;
            R1tr= 1.0e3;
            L1tr= 14.2e-3;
            Rintr=6.3;
            Cintr=200.0e-12;
            R3tr =6.2;

            Cli = 0.12e-6;
            RRF1= 100.0;
            CRF1= 1.0e-12;
            RRF2= 100.0;
            CRF2= 1.0e-12;
            RCO = 25.0;
        }
    };

    void architecture(); //method for netlist

    //component declaration
    sca_lsf::sca_tdf_source* value_in;
    sca_lsf::sca_ss*         i_ss;



    //signal declaration
    sca_lsf::sca_signal s_in;

    statespace_from_net_lsf(sc_core::sc_module_name nm,params pa=params()) : p(pa)
    {
        architecture(); //generate netlist
    }

private:

    params p;
};


//netlist implementation
void statespace_from_net_lsf::architecture()
{
    sca_util::sca_matrix<double> A, B, C, D;

    A(0,0) = 0.0;
    A(0,1) = 0.0;
    A(0,2) = 0.0;
    A(0,3) = 0.0;
    A(0,4) = 0.0;
    A(0,5) = -(p.L2tr/(pow(p.Cli,2)*p.Ktr));

    A(1,0) = 0.0;
    A(1,1) = -((p.R1tr*p.RCO + p.RCO*p.Rintr + p.R1tr*p.RRF1 + p.RCO*p.RRF1 + \
                p.Rintr*p.RRF1)/(p.Cintr*p.R1tr*p.RCO*p.RRF1 + p.Cintr*p.RCO*p.Rintr*p.RRF1));
    A(1,2) = p.CRF1/(pow(p.Cintr,2)*p.RRF1);
    A(1,3) = 0.0;
    A(1,4) = -((p.L1tr*p.R1tr)/(pow(p.Cintr,2)*(p.R1tr + p.Rintr)));
    A(1,5) = -((p.L2tr*p.R1tr)/(pow(p.Cintr,2)*(p.R1tr + p.Rintr)));

    A(2,0) = 0.0;
    A(2,1) = p.Cintr/(pow(p.CRF1,2)*p.RRF1);
    A(2,2) = -((p.RRF1 + p.RRF2)/(p.CRF1*p.RRF1*p.RRF2));
    A(2,3) = p.CRF2/(pow(p.CRF1,2)*p.RRF2);
    A(2,4) = 0.0;
    A(2,5) = 0.0;

    A(3,0) = 0.0;
    A(3,1) = 0.0;
    A(3,2) = p.CRF1/(pow(p.CRF2,2)*p.RRF2);
    A(3,3) = -(1.0/(p.CRF2*p.RRF2));
    A(3,4) = 0.0;
    A(3,5) = 0.0;

    A(4,0) = 0.0;
    A(4,1) = (p.Cintr*p.R1tr)/(pow(p.L1tr,2)*(p.R1tr + p.Rintr));
    A(4,2) = 0.0;
    A(4,3) = 0.0;
    A(4,4) = -((p.R1tr*p.Rintr)/(p.L1tr*(p.R1tr + p.Rintr)));
    A(4,5) = -((p.L2tr*p.R1tr*p.Rintr)/(pow(p.L1tr,2)*(p.R1tr + p.Rintr)));

    A(5,0) = p.Cli/(p.Ktr*pow(p.L2tr,2));
    A(5,1) = (p.Cintr*p.R1tr)/(pow(p.L2tr,2)*(p.R1tr + p.Rintr));
    A(5,2) = 0.0;
    A(5,3) = 0.0;
    A(5,4) = -((p.L1tr*p.R1tr*p.Rintr)/(pow(p.L2tr,2)*(p.R1tr + p.Rintr)));
    A(5,5) = -((p.R3tr*p.Rintr + p.R1tr*(p.R3tr + \
                                         pow(p.Ktr,2)*p.Rintr))/(pow(p.Ktr,2)*p.L2tr*(p.R1tr + p.Rintr)));


    /////////////////////////////////////////////////////////////////////////

    B(0,0) = 0.0;
    //B(1,0) = 0.0;

    B(1,0) = 0.0;
    //B(1,1) = 1.0/(pow(p.Cintr,2)*p.RCO);

    B(2,0) = 0.0;
    //B(1,2) = 0.0;

    B(3,0) = 0.0;
    //B(3,1) = 0.0;

    B(4,0) = 0.0;
    //B(4,1) = 0.0;

    B(5,0) = -1.0/(p.Ktr*pow(p.L2tr,2));
    //B(5,1) = 0.0;

    ////////////////////////////////////////////////////////////////////////


    C(0,0) = 0.0;
    C(0,1) = 0.0;
    C(0,2) = 0.0;
    C(0,3) = 0.0;
    C(0,4) = 0.0;
    C(0,5) = -p.L2tr/p.Ktr;

/*
    C(1,0) = 0.0;
    C(1,1) = p.Cintr/p.RCO;
    C(1,2) = 0.0;
    C(1,3) = 0.0;
    C(1,4) = 0.0;
    C(1,5) = 0.0;

    C(2,0) = 0.0;
    C(2,1) = p.Cintr;
    C(2,2) = 0.0;
    C(2,3) = 0.0;
    C(2,4) = 0.0;
    C(2,5) = 0.0;
    */

    ////////////////////////////////////////////////////////////////////////


    D(0,0) = 0.0;
/*    D(0,1) = 0.0;

    D(1,0) = 0.0;
    D(1,1) = -1.0/p.RCO;

    D(2,0) = 0.0;
    D(2,1) = 0.0;  */



    //////////////////////////////////////////////////////////////////////////


	value_in = new sca_lsf::sca_tdf_source("value_in");
	  value_in->inp(vin1);
	  value_in->y(s_in);


	i_ss = new sca_lsf::sca_ss("i_ss",A,B,C,D);
	  i_ss->x(s_in);
	  i_ss->y(icli);
	  /*i_ss->a=A;
	  i_ss->b=B;
	  i_ss->c=C;
	  i_ss->d=D;*/
}




///////////////////////////////////////////////////////////////////////////////
//
// corresponding electrical net of complex example
//
///////////////////////////////////////////////////////////////////////////////

SC_MODULE(transformer)
{
    sca_eln::sca_terminal de;
    sca_eln::sca_terminal li;

    struct params
    {
        double Cin;
        double Rin;
        double R1;
        double L1;
        double L2;
        double K;
        double R3;

        params()
        {
            Cin = 200e-12;
            Rin = 6.3;
            R1  = 1e3;
            L1  = 14.2e3;
            L2  = 18.0e3;
            K   = 1.0;
            R3  = 6.2;
        }
    };

    sca_eln::sca_c *cin;
    sca_eln::sca_r *rin, *r1, *r3;
    sca_eln::sca_l *l1,  *l2;
    sca_eln::sca_ideal_transformer *tr1;

    sca_eln::sca_node w_t1, w_t2, w_t3;
    sca_eln::sca_node_ref  gnd;

    transformer(sc_core::sc_module_name nm,params pa=params()) : p(pa)
    {
        architecture();
    }

private:

    void architecture();

    params p;
};

void transformer::architecture()
{
    cin = new sca_eln::sca_c("cin");
    cin->p(de);
    cin->n(gnd);
    cin->value=p.Cin;

    rin = new sca_eln::sca_r("rin");
    rin->p(de);
    rin->n(w_t1);
    rin->value=p.Rin;

    r1 = new sca_eln::sca_r("r1");
    r1->p(w_t1);
    r1->n(gnd);
    r1->value=p.R1;

    l1 = new sca_eln::sca_l("l1");
    l1->p(w_t1);
    l1->n(gnd);
    l1->value=p.L1;

    l2 = new sca_eln::sca_l("l2");
    l2->p(w_t1);
    l2->n(w_t2);
    l2->value=p.L2;

    tr1 = new sca_eln::sca_ideal_transformer("tr1");
    tr1->p1(w_t3);
    tr1->n1(gnd);
    tr1->p2(w_t2);
    tr1->n2(gnd);
    tr1->ratio = p.K;

    r3 = new sca_eln::sca_r("r3");
    r3->p(w_t3);
    r3->n(li);
    r3->value = p.R3;


}


//top-level module of complex electrical network
SC_MODULE(elec_net_impl)
{
    // ports
    sca_tdf::sca_in<double  > vin1;
    sca_tdf::sca_in<double  > vin2;

    sca_tdf::sca_out<double  > icli;
    sca_tdf::sca_out<double  > irco;
    sca_eln::sca_terminal      vrco;


    struct params //parameter
    {
        double  Ktr;
        double  L2tr;
        double  R1tr;
        double  L1tr;
        double  Rintr;
        double  Cintr;
        double  R3tr;

        double  Cli;
        double  RRF1;
        double  CRF1;
        double  RRF2;
        double  CRF2;
        double  RCO;

        params() //default for parameter
        {
            Ktr = 1.0/2.0;
            L2tr= 18.0e-3;
            R1tr= 1.0e3;
            L1tr= 14.2e-3;
            Rintr=6.3;
            Cintr=200.0e-12;
            R3tr =6.2;

            Cli = 0.12e-6;
            RRF1= 100.0;
            CRF1= 1.0e-12;
            RRF2= 100.0;
            CRF2= 1.0e-12;
            RCO = 25.0;
        }
    };

    sca_eln::sca_tdf_vsource *v_in1, *v_in2;
    sca_eln::sca_c     *cli, *crf1, *crf2;
    sca_eln::sca_r     *rrf1, *rrf2, *rco;
    transformer *trafo1;
    sca_eln::sca_tdf_isink   *i_cli, *i_rco;

    sca_eln::sca_node w_1, w_3, w_4, w_5, w_ti,w_cli_meas, w_rco_meas;
    sca_eln::sca_node_ref gnd;

    elec_net_impl(sc_core::sc_module_name nm,params pa=params()) : p(pa)
    {
        architecture();
    }

private:

    void architecture();

    params p;


};


void elec_net_impl::architecture()
{
    v_in1 = new sca_eln::sca_tdf_vsource("v_in1");
    v_in1->p(w_1);
    v_in1->n(w_cli_meas);
    v_in1->inp(vin1);


    i_cli = new sca_eln::sca_tdf_isink("i_cli");
    i_cli->n(w_cli_meas);
    i_cli->p(gnd);
    i_cli->outp(icli);

    cli = new sca_eln::sca_c("cli");
    cli->p(w_1);
    cli->n(w_ti);
    cli->value = p.Cli;

    transformer::params p_trafo1;
    p_trafo1.Cin = p.Cintr;
    p_trafo1.Rin = p.Rintr;
    p_trafo1.R1  = p.R1tr;
    p_trafo1.L1  = p.L1tr;
    p_trafo1.L2  = p.L2tr;
    p_trafo1.K   = p.Ktr;
    p_trafo1.R3  = p.R3tr;
    trafo1 = new transformer("trafo1",p_trafo1);
    trafo1->li(w_ti);
    trafo1->de(vrco);

    rrf1 = new sca_eln::sca_r("rrf1");
    rrf1->p(vrco);
    rrf1->n(w_3);
    rrf1->value = p.RRF1;

    crf1 = new sca_eln::sca_c("crf1");
    crf1->p(w_3);
    crf1->n(gnd);
    crf1->value = p.CRF1;

    rrf2 = new sca_eln::sca_r("rrf2");
    rrf2->p(w_3);
    rrf2->n(w_4);
    rrf2->value=p.RRF2;

    crf2 = new sca_eln::sca_c("crf2");
    crf2->p(w_4);
    crf2->n(gnd);
    crf2->value=p.CRF2;

    rco = new sca_eln::sca_r("rco");
    rco->p(vrco);
    rco->n(w_5);
    rco->value = p.RCO;

    v_in2 = new sca_eln::sca_tdf_vsource("v_in2");
    v_in2->p(w_5);
    v_in2->n(w_rco_meas);
    v_in2->inp(vin2);

    i_rco = new sca_eln::sca_tdf_isink("i_cli_sdf");
    i_rco->p(w_rco_meas);
    i_rco->n(gnd);
    i_rco->outp(irco);

}



/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
// non-conservative sine wave source

SCA_TDF_MODULE(sin_src)
{
    sca_tdf::sca_out<double> outp;

    struct params
    {
        double  freq;
        double  ampl;
        sca_core::sca_time sample_time;

        params()
        {
            ampl = 1.0;
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



void sin_src::ac_processing()
{
    //we use for ac-domain for all frequencies the same amplitude and a phase=0
    sca_ac_analysis::sca_ac(outp) = p.ampl;
}


void sin_src::processing()
{
    outp = p.ampl * sin(2.0*M_PI*p.freq*get_time().to_seconds());
}



void check_td_results(std::string fname);
void check_ac_results(std::string fname);


///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////
// tb implementation
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

int sc_main(int argn,char* argc[])
{

    ////////////////////////////////////////////////////////
    /// Complex example
    ////////////////////////////////////////////////////////

    //current parameter of complex state space example
    double Ktr = 1.0/2.0;
    double L2tr= 18.0e-3;
    double R1tr= 1.0e3;
    double L1tr= 14.2e-3;
    double Rintr=6.3;
    double Cintr=200.0e-12;
    double R3tr =6.2;

    double Cli = 0.12e-6;
    double RRF1= 100.0;
    double CRF1= 1.0e-12;
    double RRF2= 100.0;
    double CRF2= 1.0e-12;
    double RCO = 25.0;

    //define non-conservative signals
    sca_tdf::sca_signal<double> s_vin1,s_vin2, s_icli, s_irco, s_vrco;
    sca_tdf::sca_signal<double> s_icli_elec, s_irco_elec;
    sca_eln::sca_node w_vrco;
    sca_lsf::sca_signal s_icli_lsf;

    //instantiate and connect components
    sin_src::params i_src1_p;
    i_src1_p.ampl=1.0;
    sin_src* i_src1 = new sin_src("i_src1",i_src1_p);
    i_src1->outp(s_vin1);

    sin_src::params i_src2_p;
    i_src2_p.ampl=0.0;
    sin_src* i_src2 = new sin_src("i_src2",i_src2_p);
    i_src2->outp(s_vin2);



    statespace_from_net::params p_ss;
    p_ss.Ktr = Ktr;
    p_ss.L2tr= L2tr;
    p_ss.R1tr= R1tr;
    p_ss.L1tr= L1tr;
    p_ss.Rintr=Rintr;
    p_ss.Cintr=Cintr;
    p_ss.R3tr =R3tr;
    p_ss.Cli = Cli;
    p_ss.RRF1= RRF1;
    p_ss.CRF1= CRF1;
    p_ss.RRF2= RRF2;
    p_ss.CRF2= CRF2;
    p_ss.RCO = RCO;
    statespace_from_net* i_ss_impl=new statespace_from_net("i_ss_impl",p_ss);
    i_ss_impl->vin1(s_vin1);
    i_ss_impl->vin2(s_vin2);
    i_ss_impl->icli(s_icli);
    i_ss_impl->irco(s_irco);
    i_ss_impl->vrco(s_vrco);


    statespace_from_net_lsf::params p_lsf_ss;
    p_ss.Ktr = Ktr;
    p_ss.L2tr= L2tr;
    p_ss.R1tr= R1tr;
    p_ss.L1tr= L1tr;
    p_ss.Rintr=Rintr;
    p_ss.Cintr=Cintr;
    p_ss.R3tr =R3tr;
    p_ss.Cli = Cli;
    p_ss.RRF1= RRF1;
    p_ss.CRF1= CRF1;
    p_ss.RRF2= RRF2;
    p_ss.CRF2= CRF2;
    p_ss.RCO = RCO;
    statespace_from_net_lsf* i_lsf_ss_impl=new statespace_from_net_lsf("i_lsf_ss_impl",p_lsf_ss);
    i_lsf_ss_impl->vin1(s_vin1);
    //i_lsf_ss_impl->vin2(s_vin2);
    i_lsf_ss_impl->icli(s_icli_lsf);
    //i_lsf_ss_impl->irco(s_irco);
    //i_lsf_ss_impl->vrco(s_vrco);



    elec_net_impl::params p_elec;
    p_elec.Ktr = Ktr;
    p_elec.L2tr= L2tr;
    p_elec.R1tr= R1tr;
    p_elec.L1tr= L1tr;
    p_elec.Rintr=Rintr;
    p_elec.Cintr=Cintr;
    p_elec.R3tr =R3tr;
    p_elec.Cli = Cli;
    p_elec.RRF1= RRF1;
    p_elec.CRF1= CRF1;
    p_elec.RRF2= RRF2;
    p_elec.CRF2= CRF2;
    p_elec.RCO = RCO;
    elec_net_impl* i_net_impl = new elec_net_impl("i_net_impl",p_elec);
    i_net_impl->vin1(s_vin1);
    i_net_impl->vin2(s_vin2);
    i_net_impl->icli(s_icli_elec);
    i_net_impl->irco(s_irco_elec);
    i_net_impl->vrco(w_vrco);



    ////////////////////////////////////////////////////////////////////
    ///  Tracing
    ////////////////////////////////////////////////////////////////////


    sca_util::sca_trace_file* tf = sca_util::sca_create_tabular_trace_file("ss_complex_td.dat");

    //input signals
    sca_util::sca_trace(tf,s_vin1,"s_vin1");
    sca_util::sca_trace(tf,s_vin2,"s_vin2");

    //statespace signals
    sca_util::sca_trace(tf,s_icli,"s_icli_ss");
    sca_util::sca_trace(tf,s_irco,"s_irco_ss");
    sca_util::sca_trace(tf,s_vrco,"s_vrco_ss");

    //electrical net signals
    sca_util::sca_trace(tf,s_icli_elec,"s_icli_elec");
    sca_util::sca_trace(tf,s_irco_elec,"s_irco_elec");
    sca_util::sca_trace(tf,w_vrco,"w_vrco_elec");

    sca_util::sca_trace(tf,s_icli_lsf,"s_icli_lsf");




    /////////////////////////////////////////////////////////////////////
    //// Simulation Control
    /////////////////////////////////////////////////////////////////////

    sc_core::sc_start(5.0,sc_core::SC_MS); //start time domain simulation for 5ms


    std::cout << sc_core::sc_time_stamp() << " TD1-simulation finished" << std::endl << std::endl;

    //we store the ac result in another file
    tf->reopen("ss_ac1.dat");
    //we store the ac result as magnitude in dB and phase in degree
    //instead the default complex numbers
    tf->set_mode(sca_util::sca_ac_format(sca_util::SCA_AC_DB_DEG));
    //start ac-simulation from 100Hz to 10kHz with 200 points logarithmically spaced
    sca_ac_analysis::sca_ac_start(10.0,100.0e3,400,sca_ac_analysis::SCA_LOG);
    std::cout << "AC1-simulation finished" << std::endl << std::endl;


    tf->reopen("ss_complex_td.dat",std::ios::app); //reopen time domain file

    i_src1->p.ampl=0.0;
    i_src2->p.ampl=1.0;

    sc_core::sc_start(5.0,sc_core::SC_MS); //start time domain simulation for 5ms
    std::cout << sc_core::sc_time_stamp()  << " TD2-simulation finished" << std::endl << std::endl;


    tf->reopen("ss_ac2.dat");
    sca_ac_analysis::sca_ac_start(10.0,100.0e3,400,sca_ac_analysis::SCA_LOG);
    std::cout << "AC2-simulation finished" << std::endl << std::endl;


    tf->reopen("ss_complex_td.dat",std::ios::app); //reopen time domain file
    i_src1->p.ampl=1.0;

    sc_core::sc_start(5.0,sc_core::SC_MS); //start time domain simulation for 5ms
    std::cout << sc_core::sc_time_stamp()  << " TD3-simulation finished" << std::endl << std::endl;

    i_src1->p.freq=6.9e3;

    sc_core::sc_start(5.0,sc_core::SC_MS); //start time domain simulation for 5ms
    std::cout << sc_core::sc_time_stamp()  << " TD4-simulation finished" << std::endl << std::endl;



    /////////////////////////////////////////////////////////////////////////
    //// Result check
    ////////////////////////////////////////////////////////////////////////

    std::cout << std::endl << "---------------------------------------------------------";
    std::cout << std::endl << "Check results of time domain simulation ..." << std::endl;
    check_td_results("ss_complex_td.dat");


    std::cout << std::endl << "---------------------------------------------------------";
    std::cout << std::endl << "Check results of ac1 domain simulation ..." << std::endl;
    check_ac_results("ss_ac1.dat");

    std::cout << std::endl << "---------------------------------------------------------";
    std::cout << std::endl << "Check results of ac2 domain simulation ..." << std::endl;
    check_ac_results("ss_ac2.dat");


    return 0;
}



///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


// check routines

bool check_val(double val1, double val2,double abs_err=1e-30, double rel_err=1e-10)
{
    if( (fabs(val1)<abs_err) || (fabs(val2)<abs_err) )
    {
        if(fabs(val1-val2)>abs_err) return false;
    }
    else
    {
        if(fabs((val1-val2)/(val1+val2)/2.0) > rel_err)
        {
            return false;
        }
    }

    return true;
}


void check_td_results(std::string fname)
{
    std::ifstream tdf(fname.c_str());
    std::string line;
    std::getline(tdf,line);
    while(std::getline(tdf,line))
    {
        std::istringstream str(line);

        double time;

        str >> time;
        double vin;
        str >> vin; str >> vin;  //ignore

        double s_icli_ss,   s_irco_ss,   s_vrco_ss;
        double s_icli_elec, s_irco_elec, w_vrco_elec, s_icli_lsf;

        str >> s_icli_ss   >> s_irco_ss   >> s_vrco_ss;
        str >> s_icli_elec >> s_irco_elec >> w_vrco_elec >> s_icli_lsf;

        if(!check_val(s_icli_ss,s_icli_elec))
        {
            std::cerr << "Error TD-check failed for s_icli_ss at: " << time << std::endl;
            exit(-1);
        }

        if(time<5e-3) //second source not implemented
        {
        	if(!check_val(s_icli_lsf,s_icli_elec))
        	{
        		std::cerr << "Error TD-check failed for s_icli_lsf at: " << time << std::endl;
        		exit(-1);
        	}
        }

        if(!check_val(s_irco_ss,s_irco_elec))
        {
            std::cerr << "Error TD-check failed for s_irco_ss at: " << time << std::endl;
            exit(-1);
        }

        if(!check_val(s_vrco_ss,w_vrco_elec))
        {
            std::cerr << "Error TD-check failed for s_vrco_ss at: " << time << std::endl;
            exit(-1);
        }

    };

    std::cout  << "                                             results ok." << std::endl;
}


void check_ac_results(std::string fname)
{
    std::ifstream tdf(fname.c_str());
    std::string line;
    std::getline(tdf,line);
    while(std::getline(tdf,line))
    {
        std::istringstream str(line);

        double freq;

        str >> freq;
        double vin;
        str >> vin; str >> vin; str >> vin; str >> vin; //ignore

        double s_icli_ss_db,   s_irco_ss_db,   s_vrco_ss_db;
        double s_icli_ss_ph,   s_irco_ss_ph,   s_vrco_ss_ph;
        double s_icli_elec_db, s_irco_elec_db, w_vrco_elec_db;
        double s_icli_elec_ph, s_irco_elec_ph, w_vrco_elec_ph;
        double s_icli_lsf_ph,   s_icli_lsf_db;

        str >> s_icli_ss_db>>s_icli_ss_ph     >> s_irco_ss_db>>s_irco_ss_ph     >> s_vrco_ss_db>>s_vrco_ss_ph;
        str >> s_icli_elec_db>>s_icli_elec_ph >> s_irco_elec_db>>s_irco_elec_ph >> w_vrco_elec_db>>w_vrco_elec_ph;
        str >> s_icli_lsf_db >>   s_icli_lsf_ph;

        if(!check_val(s_icli_ss_db,s_icli_elec_db))
        {
            std::cerr << "Error AC-check db failed for s_icli_ss at: " << freq << std::endl;
            exit(-1);
        }

        if(!check_val(s_icli_ss_ph,s_icli_elec_ph))
        {
            std::cerr << "Error AC-check phase failed for s_icli_ss at: " << freq << std::endl;
            exit(-1);
        }

        if(fname=="ss_ac1.dat")  //second source not implemented
        {

        	if(!check_val(s_icli_lsf_db,s_icli_elec_db))
        	{
        		std::cerr << "Error AC-check db failed for s_icli_lsf at: " << freq << std::endl;
        		exit(-1);
        	}

        	if(!check_val(s_icli_lsf_ph,s_icli_elec_ph))
        	{
        		std::cerr << "Error AC-check phase failed for s_icli_lsf at: " << freq << std::endl;
        		exit(-1);
        	}
        }

        if(!check_val(s_irco_ss_db,s_irco_elec_db))
        {
            std::cerr << "Error AC-check db failed for s_irco_ss at: " << freq << std::endl;
            exit(-1);
        }

        if(!check_val(s_irco_ss_ph,s_irco_elec_ph))
        {
            std::cerr << "Error AC-check phase failed for s_irco_ss at: " << freq << std::endl;
            exit(-1);
        }

        if(!check_val(s_vrco_ss_db,w_vrco_elec_db))
        {
            std::cerr << "Error AC-check dB failed for s_vrco_ss at: " << freq << std::endl;
            exit(-1);
        }

        if(!check_val(s_vrco_ss_ph,w_vrco_elec_ph))
        {
            std::cerr << "Error AC-check phase failed for s_vrco_ss at: " << freq << std::endl;
            exit(-1);
        }


    };

    std::cout  << "                                             results ok." << std::endl;
}

