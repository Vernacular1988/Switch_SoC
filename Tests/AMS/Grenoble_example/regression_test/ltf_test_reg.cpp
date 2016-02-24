/*****************************************************************************
   (C) 2010 Fraunhofer Institut Integrierte Schaltungen - EAS Dresden
   
  ltf_proxy_test.cpp - description

  Original Author: Karsten Einwich Fraunhofer IIS/EAS Dresden

  Created on: 24.04.2009

   SVN Version       :  $Revision: 884 $
   SVN last checkin  :  $Date: 2010-01-10 00:31:22 +0100 (So, 10 Jan 2010) $
   SVN checkin by    :  $Author: karsten $
   SVN Id            :  $Id: ltf_test_reg.cpp 884 2010-01-09 23:31:22Z karsten $

 *****************************************************************************/

#include <systemc-ams>

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

//lowpass implementation as non-conservative behavioral model with sca_ltf_nd
SCA_TDF_MODULE (lowpass_nd)
{
    // ports
    sca_tdf::sca_in<double  > inp;
    sca_tdf::sca_out<double  > outp;


    struct params //parameter
    {
        double  fc;

        params() //default for parameter
        {
            fc = 1.0e3;
        }
    };

    void ac_processing();  //ac-implementation
    void initialize();      //initialization
    void processing();     //time domain processing method

    params p;


    // constructor
    lowpass_nd(sc_core::sc_module_name nm, params pa=params()) : p(pa)
    {}

    // definition of local variables
private:
    sca_tdf::sca_ltf_nd  ltf1;
    sca_util::sca_vector<double> A, B, S;


};

//implementation of methods

void lowpass_nd::initialize()
{
    B(0) = 1.0;

    A(0) = 1.0;
    A(1) = 1.0/(2.0*M_PI*p.fc);
}


//frequency domain implementation
void lowpass_nd::ac_processing()
{
    sca_ac_analysis::sca_ac(outp) =  sca_ac_analysis::sca_ac_ltf_nd(B,A,sca_ac_analysis::sca_ac(inp));
}

//time domain implementation
void lowpass_nd::processing()
{
    outp = ltf1(B,A,S,inp);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////


//lowpass implementation as non-conservative behavioral model with sca_ss
SCA_TDF_MODULE (lowpass_ss)
{
    // ports
    sca_tdf::sca_in<double  > inp;
    sca_tdf::sca_out<double  > outp;


    struct params //parameter
    {
        double  fc;

        params() //default for parameter
        {
            fc = 1.0e3;
        }
    };

    void ac_processing();  //ac-implementation
    void initialize();     //initialization
    void processing();     //time domain processing method

    params p;


    // constructor
    lowpass_ss(sc_core::sc_module_name nm, params pa=params()) : p(pa)
    {}

    // definition of local variables
private:
    sca_tdf::sca_ss  ss1;
    sca_util::sca_vector<double> S;
    sca_util::sca_matrix<double> A, B, C, D;


};

//implementation of methods

void lowpass_ss::initialize()
{
    double r_val = 1.0e3;
    double c_val = 1.0/(2.0*M_PI*p.fc*r_val);

    // dQ=i=1/r * (vin-vout)  ; vout = 1/c *Q
    // dQ   = -1/r/c * Q    + 1/r  * vin
    // vout =   1/c  * Q    +  0.0 * vin
    A(0,0) = -1.0/c_val/r_val;
    B(0,0) = 1.0/r_val;

    C(0,0) = 1.0/c_val;
    D(0,0) = 0.0;
}


//frequency domain implementation
void lowpass_ss::ac_processing()
{
	sca_util::sca_vector<sca_util::sca_complex> tmp_in(1),tmp_out;
	tmp_in(0)=sca_ac_analysis::sca_ac(inp);
    tmp_out =  sca_ac_analysis::sca_ac_ss(A,B,C,D,tmp_in);
    sca_ac_analysis::sca_ac(outp)=tmp_out(0);
}


//time domain implementation
void lowpass_ss::processing()
{
	sca_util::sca_vector<double> tmp_in, tmp_out;
	tmp_in(0)=inp.read();
    tmp_out = ss1(A,B,C,D,S,tmp_in);

    outp=tmp_out(0);
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////



//lowpass implementation as non-conservative behavioral model with sca_ltf_zp
SCA_TDF_MODULE (lowpass_zp)
{
    // ports
    sca_tdf::sca_in<double  >    inp;
    sca_tdf::sca_out<double  >   outp;


    struct params //parameter
    {
        double  fc;

        params() //default for parameter
        {
            fc = 1.0e3;
        }
    };

    void ac_processing();  //ac-implementation
    void initialize();     //initialization
    void processing();     //time domain processing method

    params p;


    // constructor
    lowpass_zp(sc_core::sc_module_name nm, params pa=params()) : p(pa)
    {}

    // definition of local variables
private:
    sca_tdf::sca_ltf_zp  ltf1;
    sca_util::sca_vector<sca_util::sca_complex>  Z,P;
    sca_util::sca_vector<double>       S;
    double  K;


};

//implementation of methods

//vector initialization
void lowpass_zp::initialize()
{
    // H(s) = K / (s-P(0))

    P(0) = -2.0*M_PI*p.fc;

    K    =  2.0*M_PI*p.fc;
}


//frequency domain
void lowpass_zp::ac_processing()
{
    sca_ac_analysis::sca_ac(outp) =  sca_ac_analysis::sca_ac_ltf_zp(Z,P,sca_ac_analysis::sca_ac(inp),K);
}


//time domain
void lowpass_zp::processing()
{
    outp = ltf1(Z,P,S,inp,K);
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

//lowpass implementation as hierarchical module with a non-conservative inport
//and a lsf network

SC_MODULE(lowpass_lsf)
{
    sca_tdf::sca_in<double> inp;
    sca_lsf::sca_out      outp;

    struct params //parameter
    {
        double  fc;

        params() //default for parameter
        {
            fc=1.0e3; //default for 1kHz cut off freq.
        }
    };

    void architecture(); //method for netlist

    //component declaration
    sca_lsf::sca_tdf_source* value_in;
    sca_lsf::sca_dot*        dt;
    sca_lsf::sca_sub*        sub;
    sca_lsf::sca_gain*       gain;


    //signal declaration
    sca_lsf::sca_signal s_in, s_1, s_2;

    lowpass_lsf(sc_core::sc_module_name nm,params pa=params()) : p(pa)
    {
        architecture(); //generate netlist
    }

private:

    params p;
};


//netlist implementation
void lowpass_lsf::architecture()
{
	value_in = new sca_lsf::sca_tdf_source("value_in");
	  value_in->inp(inp);
	  value_in->y(s_in);

	sub = new sca_lsf::sca_sub("sub");
	  sub->x1(s_in);
	  sub->x2(s_1);
	  sub->y(outp);

	gain = new sca_lsf::sca_gain("gain");
	  gain->x(s_2);
	  gain->y(s_1);
	  gain->k=1.0/2.0/M_PI/p.fc;

    dt = new sca_lsf::sca_dot("dt");
      dt->x(outp);
      dt->y(s_2);
}



///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

//lowpass implementation as hierarchical module with a non-conservative inport
//and a lsf network with sca_lsf::sca_ss module

SC_MODULE(lowpass_lsf_ss)
{
    sca_tdf::sca_in<double> inp;
    sca_lsf::sca_out      outp;

    struct params //parameter
    {
        double  fc;

        params() //default for parameter
        {
            fc=1.0e3; //default for 1kHz cut off freq.
        }
    };

    void architecture(); //method for netlist

    //component declaration
    sca_lsf::sca_tdf_source* value_in;
    sca_lsf::sca_ss*         i_ss;



    //signal declaration
    sca_lsf::sca_signal s_in;

    lowpass_lsf_ss(sc_core::sc_module_name nm,params pa=params()) : p(pa)
    {
        architecture(); //generate netlist
    }

private:

    params p;
};


//netlist implementation
void lowpass_lsf_ss::architecture()
{
    double r_val = 1.0e3;
    double c_val = 1.0/(2.0*M_PI*p.fc*r_val);
    sca_util::sca_matrix<double> A, B, C, D;

    A(0,0) = -1.0/c_val/r_val;
    B(0,0) = 1.0/r_val;

    C(0,0) = 1.0/c_val;
    D(0,0) = 0.0;


	value_in = new sca_lsf::sca_tdf_source("value_in");
	  value_in->inp(inp);
	  value_in->y(s_in);


	i_ss = new sca_lsf::sca_ss("i_ss",A,B,C,D);
	  i_ss->x(s_in);
	  i_ss->y(outp);
	  /*i_ss->a=A;
	  i_ss->b=B;
	  i_ss->c=C;
	  i_ss->d=D;*/
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

//lowpass implementation as hierarchical module with a non-conservative inport
//and a lsf network with sca_lsf::sca_ss module

SC_MODULE(lowpass_lsf_ltf_nd)
{
    sca_tdf::sca_in<double> inp;
    sca_lsf::sca_out      outp;

    struct params //parameter
    {
        double  fc;

        params() //default for parameter
        {
            fc=1.0e3; //default for 1kHz cut off freq.
        }
    };

    void architecture(); //method for netlist

    //component declaration
    sca_lsf::sca_tdf_source* value_in;
    sca_lsf::sca_ltf_nd*     i_ltf_nd;



    //signal declaration
    sca_lsf::sca_signal s_in;

    lowpass_lsf_ltf_nd(sc_core::sc_module_name nm,params pa=params()) : p(pa)
    {
        architecture(); //generate netlist
    }

private:

    params p;
};


//netlist implementation
void lowpass_lsf_ltf_nd::architecture()
{
	value_in = new sca_lsf::sca_tdf_source("value_in");
	  value_in->inp(inp);
	  value_in->y(s_in);


	i_ltf_nd = new sca_lsf::sca_ltf_nd("i_ltf_nd");
	  i_ltf_nd->x(s_in);
	  i_ltf_nd->y(outp);
	  i_ltf_nd->num=sca_util::sca_create_vector(1.0);
	  i_ltf_nd->den=sca_util::sca_create_vector(1.0, 1./(2.0*M_PI*p.fc));


}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

//lowpass implementation as hierarchical module with a non-conservative inport
//and a lsf network with sca_lsf::sca_ltf_zp module

SC_MODULE(lowpass_lsf_ltf_zp)
{
    sca_tdf::sca_in<double> inp;
    sca_lsf::sca_out      outp;

    struct params //parameter
    {
        double  fc;

        params() //default for parameter
        {
            fc=1.0e3; //default for 1kHz cut off freq.
        }
    };

    void architecture(); //method for netlist

    //component declaration
    sca_lsf::sca_tdf_source* value_in;
    sca_lsf::sca_ltf_zp*     i_ltf_zp;



    //signal declaration
    sca_lsf::sca_signal s_in;

    lowpass_lsf_ltf_zp(sc_core::sc_module_name nm,params pa=params()) : p(pa)
    {
        architecture(); //generate netlist
    }

private:

    params p;
};



//netlist implementation
void lowpass_lsf_ltf_zp::architecture()
{
	value_in = new sca_lsf::sca_tdf_source("value_in");
	  value_in->inp(inp);
	  value_in->y(s_in);


	i_ltf_zp = new sca_lsf::sca_ltf_zp("i_ltf_zp");
	  i_ltf_zp->x(s_in);
	  i_ltf_zp->y(outp);
	  i_ltf_zp->k=2.0*M_PI*p.fc;
	  i_ltf_zp->poles=sca_util::sca_create_vector(sca_util::sca_complex(-2.0*M_PI*p.fc,0.0));


}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

//lowpass implementation as hierarchical module with a non-conservative inport
//and a conservative (electrical) outport and implemented as conservative RC-network

SC_MODULE(lowpass_rc)
{
    sca_tdf::sca_in<double> inp;
    sca_eln::sca_terminal      outp;

    struct params //parameter
    {
        double  r_val;
        double  c_val;

        params() //default for parameter
        {
            double fc=1.0e3; //default for 1kHz cut off freq.

            r_val = 1.0e3;
            c_val = 1.0/(2.0*M_PI*fc*r_val);
        }
    };

    void architecture(); //method for netlist

    //component declaration
    sca_eln::sca_tdf_vsource* vin;
    sca_eln::sca_r*     r1;
    sca_eln::sca_c*     c1;

    //node declaration
    sca_eln::sca_node n_in;
    sca_eln::sca_node_ref  gnd;   //reference node

    lowpass_rc(sc_core::sc_module_name nm,params pa=params()) : p(pa)
    {
        architecture(); //generate netlist
    }

private:

    params p;
};


//netlist implementation
void lowpass_rc::architecture()
{
    vin = new sca_eln::sca_tdf_vsource("vin");
      vin->inp(inp);
      vin->p(n_in);
      vin->n(gnd);

    r1 = new sca_eln::sca_r("r1");
      r1->value=p.r_val;           //the value can be assigned via the member...
      r1->n(n_in);
      r1->p(outp);

    c1 = new sca_eln::sca_c("c1",p.c_val); //or via the constructor
      c1->p(outp);
      c1->n(gnd);
}




///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

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


//frequency domain implementation
void sin_src::ac_processing()
{
    //we use for ac-domain for all frequencies the same amplitude and a phase=0
    sca_ac_analysis::sca_ac(outp) = p.ampl;
}


//time domain implementation
void sin_src::processing()
{
    outp = p.ampl * sin(2.0*M_PI*p.freq*get_time().to_seconds());
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// tb implementation
void check_td_results(std::string fname);
void check_ac_results(std::string fname);

int sc_main(int argn,char* argc[])
{
    //define non-conservative signals
    sca_tdf::sca_signal<double> s_in, s_out_nd, s_out_ss, s_out_zp;

    //define electrical node
    sca_eln::sca_node n_out_rc;

    //define lsf signal
    sca_lsf::sca_signal s_out_lsf, s_out_lsf_ss, s_out_lsf_ltf_nd, s_out_lsf_ltf_zp;

    //instantiate and connect components

    sin_src* i_src = new sin_src("i_src");
      i_src->outp(s_in);

    ////////////////////////////////////////////////

    lowpass_nd* i_lp_nd=new lowpass_nd("i_lp_nd");
      i_lp_nd->inp(s_in);
      i_lp_nd->outp(s_out_nd);

    ////////////////////////////////////////////////

      lowpass_ss* i_lp_ss=new lowpass_ss("i_lp_ss");
        i_lp_ss->inp(s_in);
        i_lp_ss->outp(s_out_ss);


    ////////////////////////////////////////////////

    lowpass_zp* i_lp_zp=new lowpass_zp("i_lp_zp");
      i_lp_zp->inp(s_in);
      i_lp_zp->outp(s_out_zp);

    ////////////////////////////////////////////////

       lowpass_lsf* i_lp_lsf=new lowpass_lsf("i_lp_lsf");
         i_lp_lsf->inp(s_in);
         i_lp_lsf->outp(s_out_lsf);


    ////////////////////////////////////////////////

       lowpass_lsf_ss* i_lp_lsf_ss=new lowpass_lsf_ss("i_lp_lsf_ss");
          i_lp_lsf_ss->inp(s_in);
          i_lp_lsf_ss->outp(s_out_lsf_ss);

    ////////////////////////////////////////////////

       lowpass_lsf_ltf_nd* i_lp_lsf_ltf_nd=new lowpass_lsf_ltf_nd("i_lp_lsf_ltf_nd");
		   i_lp_lsf_ltf_nd->inp(s_in);
		   i_lp_lsf_ltf_nd->outp(s_out_lsf_ltf_nd);

	////////////////////////////////////////////////

	   lowpass_lsf_ltf_zp* i_lp_lsf_ltf_zp=new lowpass_lsf_ltf_zp("i_lp_lsf_ltf_zp");
			i_lp_lsf_ltf_zp->inp(s_in);
			i_lp_lsf_ltf_zp->outp(s_out_lsf_ltf_zp);

    ////////////////////////////////////////////////

    lowpass_rc* i_lp_rc=new lowpass_rc("i_lp_rc");
      i_lp_rc->inp(s_in);
      i_lp_rc->outp(n_out_rc);

    ////////////////////////////////////////////////

    sca_util::sca_trace_file* tf = sca_util::sca_create_tabular_trace_file("low_pass_td.dat");
      sca_util::sca_trace(tf,s_in,"s_in");
      sca_util::sca_trace(tf,s_out_nd,"s_out_nd");
      sca_util::sca_trace(tf,s_out_ss,"s_out_ss");
      sca_util::sca_trace(tf,s_out_zp,"s_out_zp");
      sca_util::sca_trace(tf,s_out_lsf,"s_out_lsf");
      sca_util::sca_trace(tf,s_out_lsf_ss,"s_out_lsf_ss");
      sca_util::sca_trace(tf,s_out_lsf_ltf_nd,"s_out_lsf_ltf_nd");
      sca_util::sca_trace(tf,s_out_lsf_ltf_zp,"s_out_lsf_ltf_zp");

      sca_util::sca_trace(tf,n_out_rc,"n_out_rc");          //voltage at node
      sca_util::sca_trace(tf,i_lp_rc->n_in,"i_lp_rc.n_in"); //voltage of internal node
      sca_util::sca_trace(tf,i_lp_rc->r1,"i_lp_rc.r1");     //current through r1
      sca_util::sca_trace(tf,i_lp_rc->c1,"i_lp_rc.c1");     //current through c1

    ////////////////////////////////////////////////

    sc_core::sc_start(5.0,sc_core::SC_MS); //start time domain simulation for 5ms

    std::cout << sc_core::sc_time_stamp() << " time domain simulation finished" << std::endl << std::endl;


    ////////////////////////////////////////////////


    //we store the ac result in another file
    tf->reopen("low_pass_ac.dat");
    //we store the ac result as magnitude in dB and phase in degree
    //instead the default complex numbers
    tf->set_mode(sca_util::sca_ac_format(sca_util::SCA_AC_DB_DEG));

    //start ac-simulation from 100Hz to 10kHz with 200 points logarithmically spaced
    sca_ac_analysis::sca_ac_start(10.0,100.0e3,400,sca_ac_analysis::SCA_LOG);

    std::cout << "AC-simulation finished" << std::endl;

    /////////////////////////////////////////////////////////////////////////
    //// Result check
    ////////////////////////////////////////////////////////////////////////

    std::cout << std::endl << "---------------------------------------------------------";
    std::cout << std::endl << "Check results of time domain simulation ..." << std::endl;
    check_td_results("low_pass_td.dat");


    std::cout << std::endl << "---------------------------------------------------------";
    std::cout << std::endl << "Check results of ac domain simulation ..." << std::endl;
    check_ac_results("low_pass_ac.dat");


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
        double s_in;
        str >> s_in;  //ignore  s_in

        double out_nd,   out_ss, out_zp, out_lsf, out_lsf_ss, out_lsf_ltf_nd,  out_rc;
        double out_lsf_ltf_zp;

        str >> out_nd   >> out_ss >> out_zp  >> out_lsf >> out_lsf_ss
            >> out_lsf_ltf_nd >> out_lsf_ltf_zp >> out_rc;

        if(!check_val(out_nd,out_ss))
        {
            std::cerr << "Error TD-check failed for out_ss at: " << time << std::endl;
            exit(-1);
        }

        if(!check_val(out_nd,out_zp))
        {
            std::cerr << "Error TD-check failed for out_zp at: " << time << std::endl;
            exit(-1);
        }

        if(!check_val(out_nd,out_lsf))
        {
            std::cerr << "Error TD-check failed for out_lsf at: " << time << std::endl;
            exit(-1);
        }

        if(!check_val(out_nd,out_lsf_ss))
        {
            std::cerr << "Error TD-check failed for out_lsf_ss at: " << time << std::endl;
            exit(-1);
        }

        if(!check_val(out_nd,out_lsf_ltf_nd))
        {
            std::cerr << "Error TD-check failed for out_lsf_ltf_nd at: " << time << std::endl;
            exit(-1);
        }

        if(!check_val(out_nd,out_lsf_ltf_zp))
        {
            std::cerr << "Error TD-check failed for out_lsf_ltf_zp at: " << time << std::endl;
            exit(-1);
        }

        if(!check_val(out_nd,out_rc))
        {
            std::cerr << "Error TD-check failed for out_rc at: " << time << std::endl;
            exit(-1);
        }
    };

    std::cout  << "                                             results ok." << std::endl;
}

//////////////////////////////////////////////

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
        double s_in;
        str >> s_in >> s_in; //ignore s_in (dB/phase)

        double s_out_nd_db, s_out_ss_db,  s_out_zp_db,  s_out_lsf_db;
        double s_out_lsf_ss_db,  s_out_lsf_ltf_nd_db,  s_out_lsf_ltf_zp_db,   s_out_rc_db;

        double s_out_nd_ph, s_out_ss_ph,  s_out_zp_ph,  s_out_lsf_ph;
        double s_out_lsf_ss_ph,  s_out_lsf_ltf_nd_ph,  s_out_lsf_ltf_zp_ph,   s_out_rc_ph;


        str >> s_out_nd_db>>s_out_nd_ph;
        str >> s_out_ss_db>>s_out_ss_ph;
        str >> s_out_zp_db>>s_out_zp_ph;
        str >> s_out_lsf_db>>s_out_lsf_ph;
        str >> s_out_lsf_ss_db>>s_out_lsf_ss_ph;
        str >> s_out_lsf_ltf_nd_db>>s_out_lsf_ltf_nd_ph;
        str >> s_out_lsf_ltf_zp_db>>s_out_lsf_ltf_zp_ph;
        str >> s_out_rc_db>>s_out_rc_ph;

        //check whether ac has done anything
        if(s_out_nd_db < -300.0)
        {
        	SC_REPORT_ERROR("SystemC-AMS","AC-check failed");
        }

        if(!check_val(s_out_nd_db,s_out_ss_db))
        {
            std::cerr << "Error AC-check db failed for s_out_ss at: " << freq << std::endl;
            exit(-1);
        }

        if(!check_val(s_out_nd_ph,s_out_ss_ph))
         {
             std::cerr << "Error AC-check phase failed for s_out_ss_ph at: " << freq << std::endl;
             exit(-1);
         }


        if(!check_val(s_out_nd_db,s_out_lsf_ltf_nd_db))
        {
            std::cerr << "Error AC-check db failed for s_out_lsf_ltf_nd_db at: " << freq << std::endl;
            exit(-1);
        }

        if(!check_val(s_out_nd_ph,s_out_lsf_ltf_nd_ph))
         {
             std::cerr << "Error AC-check phase failed for s_out_lsf_ltf_nd_db at: " << freq << std::endl;
             exit(-1);
         }


        if(!check_val(s_out_nd_db,s_out_lsf_ltf_zp_db))
        {
            std::cerr << "Error AC-check db failed for s_out_lsf_ltf_zp_db at: " << freq << std::endl;
            exit(-1);
        }

        if(!check_val(s_out_nd_ph,s_out_lsf_ltf_zp_ph))
         {
             std::cerr << "Error AC-check phase failed for s_out_lsf_ltf_zp_db at: " << freq << std::endl;
             exit(-1);
         }




        if(!check_val(s_out_nd_db,s_out_zp_db))
        {
            std::cerr << "Error AC-check db failed for s_out_zp at: " << freq << std::endl;
            exit(-1);
        }

        if(!check_val(s_out_nd_ph,s_out_zp_ph))
        {
            std::cerr << "Error AC-check phase failed for s_out_zp_ph at: " << freq << std::endl;
            exit(-1);
        }

        //////////////////////////////////////////////////////////////

        if(!check_val(s_out_nd_db,s_out_lsf_db))
        {
            std::cerr << "Error AC-check db failed for s_out_lsf at: " << freq << std::endl;
            exit(-1);
        }

        if(!check_val(s_out_nd_ph,s_out_lsf_ph))
        {
            std::cerr << "Error AC-check phase failed for s_out_lsf_ph at: " << freq << std::endl;
            exit(-1);
        }

        //////////////////////////////////////////////////////////////

         if(!check_val(s_out_nd_db,s_out_lsf_ss_db))
         {
             std::cerr << "Error AC-check db failed for s_out_lsf at: " << freq << std::endl;
             exit(-1);
         }

         if(!check_val(s_out_nd_ph,s_out_lsf_ss_ph))
         {
             std::cerr << "Error AC-check phase failed for s_out_lsf_ph at: " << freq << std::endl;
             exit(-1);
         }


        //////////////////////////////////////////////////////////////

        if(!check_val(s_out_nd_db,s_out_rc_db))
        {
            std::cerr << "Error AC-check db failed for s_out_rc at: " << freq << std::endl;
            exit(-1);
        }

        if(!check_val(s_out_nd_ph,s_out_rc_ph))
        {
            std::cerr << "Error AC-check phase failed for s_out_rc_ph at: " << freq << std::endl;
            exit(-1);
        }
    };

    std::cout  << "                                             results ok." << std::endl;
}
