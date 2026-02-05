#include "alu.h"


void alu::operation(){
  sc_uint<32> alu_tmp;
  sc_uint<5>  shift_value = op_b.read().range(4,0);

  if(alu_ctrl.read() == 1){
  // add 
    alu_tmp = op_a.read() + op_b.read(); 
  }
  else if(alu_ctrl.read() ==  2){
  //sub
    alu_tmp = op_a.read() - op_b.read();
  } 
  else if(alu_ctrl.read() ==  3){
  // or
    alu_tmp = op_a.read() | op_b.read();
  } 
  else if(alu_ctrl.read() ==  4){
  // and
    alu_tmp = op_a.read() & op_b.read();
  } 
  else if(alu_ctrl.read() ==  5){
  // xor
    alu_tmp = op_a.read() ^ op_b.read();
  } 
  else if(alu_ctrl.read() ==  6){
  // sll
    alu_tmp = op_a.read() << shift_value;
  } 
  else if(alu_ctrl.read() ==  7){
  // srl
    alu_tmp = op_a.read() >> shift_value;
  } 
  else if(alu_ctrl.read() ==  8){
  // sra
    alu_tmp = ((sc_int<32>)op_a.read()) >> shift_value;
  } 
  else if(alu_ctrl.read() ==  9){
  // slt
    if(op_a.read()[31] == 1 && op_b.read()[31] == 0)
      alu_tmp  = 1;
    else if(op_a.read()[31] == 0 && op_b.read()[31] == 1)
      alu_tmp  = 0;
    else if((sc_int<32>)op_a.read() < (sc_int<32>)op_b.read())
      alu_tmp  = 1;
    else
      alu_tmp  = 0;

  } 
  else if(alu_ctrl.read() ==  10){
  // sltu
    if((sc_uint<32>)op_a.read() < (sc_uint<32>)op_b.read())
      alu_tmp  = 1;
    else
      alu_tmp  = 0;
  } 

  else{
    alu_tmp = 0;
  }

  alu_result.write(alu_tmp);

}

void alu::trace(){
  sc_trace(tf,  alu_tmp, "alu_tmp");
  sc_trace(tf,  shift_value, "shift_value");
  sc_trace(tf,  alu_ctrl, "alu_ctrl");
  sc_trace(tf,  op_a, "op_a");
  sc_trace(tf,  op_b, "op_b");

}

