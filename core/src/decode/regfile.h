#pragma   once
#include  <systemc.h>

SC_MODULE(regfile){

//===========================================
//	global Interface
//============================================
  
  sc_in_clk     clk;
  sc_in<bool>   arstn;

//===========================================
//	Regfile Interface
//============================================
  // reading interface
  sc_in<sc_bv<6>>     raddr1;
  sc_in<sc_bv<6>>     raddr2;
  sc_out<sc_uint<32>> rdata1;
  sc_out<sc_uint<32>> rdata2;
  //  writing interface
  sc_in<sc_bv<6>>     waddr3;
  sc_in<sc_uint<32>>  wdata;
  sc_in<bool>         write_en; 
  

  //internal signals 
  sc_signal<sc_uint<32>> reg[31];


  void read_reg();
  void write_reg();
  void trace(sc_trace_file *tf);

  SC_CTOR(regfile){
    SC_METHOD(read_reg);
    sensitive << raddr1 << raddr2 << arstn;
    

    SC_CTHREAD(write_reg , reg::clk.pos());
    async_reset_signal_is(arstn, false);

  } 
};



