//mem.cpp
//implement functoins of memory module

#include mem.h

void Memory::checkAddress() {

// if address exceed memory size, set error code to 1
         if(m_curAddr >= MEM_SIZE)
             m_errorCode = MEM_ERR_INV_ADDRESS;
         else
             m_errorCode = 0;
    }

int Memory::read() {
        checkAddress();
	//if no error, read data from current address, return read-finish signal
        if(m_errorCode == 0) {
            Port_Data_out.write(m_data[m_curAddr]);

            m_readsCnt++;
            return 1;
        } else {
	//if there is error, return error signal
            m_errorsCnt++;
            return 3;
        }
    }


int Memory::write() {
        checkAddress();
	//if no error, write data into current address, return write-finish signal
        if(m_errorCode == 0) {
            m_data[m_curAddr] = m_curData;
            
            m_writesCnt++;
            return 2;
        } else {
	//if there is error, return error signal
            m_errorsCnt++;
            return 3;
        }
    }

void Memory::execute() {
        if(m_curFunc != 0) {
            m_clkCnt++;

            if(m_clkCnt == 100) {
                int retSig = 3;

                switch(m_curFunc) {
                   case 1  : { retSig = read();  break; }
                   case 2 : { retSig = write(); break; }
                }

                Port_DoneSig.write(retSig);

#if defined(PRINT_WHILE_RUN)
                cout << "Memory : Finished " 
                     << (m_curFunc == 2 ? "write" : "read")
                     << " request. Addr = " <<  m_curAddr
                     << "  Time = " << sc_time_stamp() << endl;
#endif

                m_clkCnt  = 0;
                m_curFunc = 0;
            }

            return;
        }

        if(Port_Func.read() == 0) {
            return;
        }

        // Read request's data
        m_curFunc = Port_Func.read();
        m_curAddr = Port_Addr.read();
        m_curData = Port_Data_in.read();

        // Reset return signal
        Port_DoneSig.write(0);

#if defined(PRINT_WHILE_RUN)
        cout << "Memory : Received " 
             << (m_curFunc == 2 ? "write" : "read")
             << " request. Addr = " <<  m_curAddr
             << "  Time = " << sc_time_stamp() << endl;
#endif
    }
