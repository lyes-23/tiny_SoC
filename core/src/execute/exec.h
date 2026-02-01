#include <systemc.h>
#include <iostream>
#include "miniriscv_def.h"
#include "fifo_element.h"


SC_MODULE(exec){

  sc_in_clk           clk;
  sc_in<bool>         arstn;

//===========================================
//  Decode stage interface	
//============================================

  sc_in<sc_uint<32>>	id2exe_op1_data;
  sc_in<sc_uint<32>>	id2exe_op2_data;

  sc_in<bool>		      id2exe_reg_wr_en;
  sc_in<sc_uint<2>>	  id2exe_result_src;
  sc_in<bool>		      id2exe_mem_wr_en;
  sc_in<sc_uint<2>>	  id2exe_mem_size;
  sc_in<sc_uint<32>>	id2exe_mem_data;

  sc_in<sc_uint<4>>	  id2exe_alu_ctrl;
  sc_in<sc_uint<32>>  id2exe_pc_next;
  sc_in<sc_uint<5>>	  id2exe_reg_dst;
  sc_in<sc_uint<32>>	id2exe_imm_ext;

// fifo ID2EXE interface
  sc_in<bool>		      out_id2exe_valid;
  sc_out<bool>		    out_id2exe_ready;

//===========================================
//  memory interface	
//============================================

  sc_out<sc_uint<32>>   alu_result; 

  sc_out<sc_uint<2>>  exe2mem_result_src;
  sc_out<mem_wr_en>   exe2mem_mem_wr_en;
  sc_out<sc_uint<2>>  exe2mem_mem_size;
  sc_out<sc_uint<32>> exe2mem_mem_data;

  sc_out<bool>        exe2mem_reg_wr_en;
  sc_out<sc_uint<5>>  exe2mem_reg_dst;

  sc_out<sc_uint<32>> exe2mem_pc_next;
  
//===========================================
//   signals exe2if
//============================================
  
  sc_out<sc_uint<32>> exe2if_pc_target;
  sc_out<bool>        exe2if_pc_src;

  
// fifo interface EXE2MEM
  sc_out<bool>        out_exe2mem_valid;
  sc_in<bool>         out_exe2mem_ready;

  fifo<EXE2MEM_SIZE,1>     exe2mem;

// declaring signals





// pipeline signals
  sc_signal<bool>     in_exe2mem_valid;
  sc_signal<bool>     in_exe2mem_ready;
  sc_signal<bool>     exe2mem_full;
  sc_signal<bool>     exe2mem_empty;
  sc_signal<sc_bv<EXE2MEM_SIZE>>  in_exe2mem_data;
  sc_signal<sc_bv<EXE2MEM_SIZE>>  out_exe2mem_data;

  
  SC_CTOR(exec) : exe2mem("exe2mem"){
        exe2mem.clk(clk);
        exe2mem.arstn(arstn);

        exe2mem.in_ready_o	(in_exe2mem_ready);
        exe2mem.in_valid_i	(in_exe2mem_valid);
        exe2mem.in_data_i	  (in_exe2mem_data);

        exe2mem.out_ready_i	(out_exe2mem_ready);
        exe2mem.out_valid_o	(out_exe2mem_valid);
        exe2mem.out_data_o	(out_exe2mem_data);

        exe2mem.full_o	    (exe2mem_full);
        exe2mem.empty_o	    (exe2mem_empty);


  
  }


}
