#include "regfile.h"

void regfile::read(){
  rdata1.write(reg[raddr1.read()]);
  rdata2.write(reg[raddr2.read()]);
}


void regfile::write(){

  for(int i=0; i < 32; i++){
    reg[i] = 0;
  }
  while(true){
    if(waddr3.read() !=0 && write_enable.read() )
      reg[waddr3.read()].write(wdata.read());
    }
    wait(1);
  }
}


void regfile::trace(sc_trace_file *tf){
  sc_trace(tf, raddr1, GET_NAME(raddr1));
  sc_trace(tf, raddr2, GET_NAME(raddr2));
  sc_trace(tf, rdata1, GET_NAME(rdata1));
  sc_trace(tf, rdata2, GET_NAME(rdata2));

  sc_trace(tf, waddr3, GET_NAME(waddr3));
  sc_trace(tf, wdata,  GET_NAME(wdata ));
  sc_trace(tf, write_en,GET_NAME(write_en));

  for(int i = 0; i < 31 ; i++){
    std::string reg_name= "reg_";
    reg_name = reg_name + std::to_string(i);
    sc_trace(tf, reg[i],  signal_get_name(reg[i].name(), reg_name.c_str() ));
  }
}

