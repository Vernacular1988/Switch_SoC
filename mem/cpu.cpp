//mem.cpp
//implement functoins of CPU

#include <cpu.h>


    /**
     * Generates a random 
     * memory request.
     */
    int CPU::getrndfunc() {
        int rndnum=(rand() % 10);

        if(rndnum < 5)
            return 1;
        else
            return 2;
    }

    /**
     * Generates a random 
     * memory address.
     */
    int CPU::getRndAddress() {
#ifndef GEN_INVALID_ADDRESSES
        return (rand() % MEM_SIZE);
#else
        return (rand() % (MEM_SIZE + 100));
#endif
    }

    /**
     * Generates some random 
     * data to store to or load from memory.
     */
    int CPU::getRndData() {
       return rand();
    }

    // since we can not have two drivers for a signal
    // (even inside one module)
    // we need a control multiplexer to set/reset the 
    // memory operation from write-or-read to none
    void CPU::MemFunMux() {
      if (MemRun.event()) Port_MemFunc.write(MemRun);
      if (MemPause.event()) Port_MemFunc.write(MemPause);
      return;
    }

    void CPU::execCycle() {
        if(m_waitMem) {
            return;
        }

        /**
         * Select random a function, 
         * address and data
         */

        int addr = getRndAddress();
        int f = getrndfunc();


        MemRun.write(f);
        Port_MemAddr.write(addr);

        /**
         * Send some data if 
         * function is WRITE 
         */
        if(f == 2)
            Port_MemData_out.write(getRndData());

        m_waitMem = true;

#if defined(PRINT_WHILE_RUN)
        cout << "CPU : Sent " 
             << (f == 2 ? "write" : "read")
             << " request. Addr = " <<  addr
             << "  Time = " << sc_time_stamp() << endl;
#endif
    }

    /**
     * Process that is executed
     * when the memory finishes handling a 
     * request.
     */
    void CPU::memDone() {
      MemRetSig = Port_MemDone.read();
        if(MemRetSig == 0) {
            return;
        }                

#if defined(PRINT_WHILE_RUN)
        cout << "CPU : Received FIN_SIG from memory."
             << "  Time = " << sc_time_stamp() << endl;
	if (MemRetSig == 3)
	  cout << "ERROR: transaction had an invalid address " << endl;

#endif

        m_waitMem = false;

        // Reset memory's function
        MemPause.write(0);
    }
