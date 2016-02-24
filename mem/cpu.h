//cpu.h 
//This head file defines the model structure of cpu
//It send data and instruction to memory


#include <systemc.h>
#include <signal.h>

#include <mem.h>

SC_MODULE(CPU) {

public:
    /* Ports */

    sc_in<bool>               Port_CLK;
    sc_in<int>  		Port_MemDone;
    sc_out<int>  		Port_MemFunc;
    sc_out<int>               Port_MemAddr;

    sc_out<int>             Port_MemData_out;
    sc_in<int>              Port_MemData_in;

    /* Constructor */

    SC_CTOR(CPU) {
         SC_METHOD(execCycle);
         sensitive << Port_CLK.pos();
         dont_initialize();

         SC_METHOD(memDone);
         sensitive << Port_MemDone;
         dont_initialize();

	 SC_METHOD(MemFunMux);
	 sensitive << MemRun << MemPause;
	 dont_initialize();

         m_waitMem = false;
    }

private:
    bool m_waitMem;
    sc_signal<int>  MemRun, MemPause;
    sc_signal<int> MemRetSig;

    /**
     * Generates a random 
     * memory request.
     */
    int getrndfunc();

    /**
     * Generates a random 
     * memory address.
     */
    int getRndAddress();

    /**
     * Generates some random 
     * data to store to or load from memory.
     */
    int getRndData();

    // since we can not have two drivers for a signal
    // (even inside one module)
    // we need a control multiplexer to set/reset the 
    // memory operation from write-or-read to none
    void MemFunMux();

    void execCycle();


    /**
     * Process that is executed
     * when the memory finishes handling a 
     * request.
     */
    void memDone();       


};


