#include <systemc.h>



  SC_MODULE(alu){
  
    sc_in<sc_uint<32>>    op_a;
    sc_in<sc_uint<32>>    op_b;
    sc_in<sc_uint<4>>     alu_ctrl;


    sc_out<sc_uint<32>>   alu_result;

    void  operation()
    void  trace(sc_trace_file *tf);


    SC_CTOR(alu) {
      SC_METHOD(operation);
      sensitive << op_a << op_b << alu_ctrl;
    } 

  };
