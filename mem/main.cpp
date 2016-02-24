#include <systemc.h>
#include <signal.h>

#include <cpu.h>

void sig_handler(int sigNum)
{
    sc_stop();
}

/*-----------------------------------*/
/* Program's Entry Point */

int sc_main(int argc, char* argv[])
{
    /**
     * Register SIGINT and SIGTERM
     * signal handlers 
     */

    signal(SIGINT,  sig_handler);
    signal(SIGTERM, sig_handler);

    /*-----------------------------------*/

    cout << "\n\nCreating Modules............";

    /* Instantiate Modules */
    Memory mem("main_memory");
    CPU    cpu("cpu");

    /* Signals */
    sc_signal<int>  		sigMemFunc;
    sc_signal<int> 		sigMemDone;
    sc_signal<int>               sigMemAddr;
    sc_signal<int>               sigMemData_write, sigMemData_read;

    /* The clock that will drive the CPU and Memory*/
    sc_clock clk("clk",1,SC_NS);

    /*-----------------------------------*/

    cout << "DONE\nConnecting Modules' Ports...";

    /* Connecting module ports with signals */
    mem.Port_Func(sigMemFunc);
    mem.Port_Addr(sigMemAddr);
    mem.Port_Data_in(sigMemData_write);
    mem.Port_Data_out(sigMemData_read);
    mem.Port_DoneSig(sigMemDone);

    cpu.Port_MemFunc(sigMemFunc);
    cpu.Port_MemAddr(sigMemAddr);
    cpu.Port_MemData_out(sigMemData_write);
    cpu.Port_MemData_in(sigMemData_read);
    cpu.Port_MemDone(sigMemDone);

    mem.Port_CLK(clk);
    cpu.Port_CLK(clk);

    cout << "DONE\n" << endl;

    /*-----------------------------------*/

#if defined(PRINT_WHILE_RUN)
    cout << "\nWhile running press CTRL+C to exit.\n" 
         << "Press Enter to start...";

    getchar();
    cout << endl << endl;
#else
    cout << "\nPress Enter to start...";
    getchar();
    cout << "\n\nRunning (press CTRL+C to exit)... ";
#endif

    fflush(stdout);


    /* Start Simulation */
    sc_start(1200,SC_NC);

    /* Show Results */

    cout << "\n   Simulation Time       : " << sc_time_stamp()  << endl
         << "   Delta Cycles Executed : "   << sc_delta_count() << endl << endl;

    cout << "   Total Memory Requests : " << mem.getRequestsCount() << endl
         << "   Memory READs          : " << mem.getReadsCount()    << endl
         << "   Memory WRITEs         : " << mem.getWritesCount()   << endl
         << "   Invalid Accesses      : " << mem.getErrorsCount()   << endl;

    cout << "\nPress Enter to exit...";
    getchar();
    cout << endl << endl;

    return 0;
}
