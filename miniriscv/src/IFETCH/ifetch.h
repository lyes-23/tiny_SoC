#include <systemc.h>

SC_MODULE(ifetch) {


//================================================================================
//    IMEM Interface
//===============================================================================

    sc_out<sc_bv<32>> imem_req_addr;
    sc_out<bool>      imem_req_valid;
    sc_in<bool>       imem_req_ready;
    
    sc_out<sc_bv<32>> imem_resp_instr;
    sc_out<bool>      imem_resp_valid;
    sc_in<bool>       imem_resp_ready;
    sc_out<bool>      imem_resp_error;
    
//=================================================================================
//          
}