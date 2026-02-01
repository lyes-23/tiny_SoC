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

  sc_in<bool>		    id2exe_reg_wr_en;
  sc_in<sc_uint<2>>	id2exe_result_src;
  sc_in<bool>		    id2exe_mem_wr_en;
  sc_in<sc_uint<2>>	id2exe_mem_size;
  sc_in<sc_uint<32>>	id2exe_mem_data;

  sc_in<bool>		    id2exe_jump;
  sc_in<bool>		    id2exe_branch;
  sc_in<sc_uint<32>> id2exe_branch_offset;

  sc_in<sc_uint<4>>	id2exe_alu_ctrl;

  sc_in<sc_uint<32>> id2exe_pc_next;

  sc_in<sc_uint<5>>	id2exe_reg_src1;
  sc_in<sc_uint<5>>	id2exe_reg_src2;
  sc_in<sc_uint<5>>	id2exe_reg_dst;

  sc_in<sc_uint<32>>	id2exe_imm_ext;

  sc_in<bool>		    out_id2exe_valid;
  sc_out<bool>		    out_id2exe_ready;

//===========================================
//  memory interface	
//============================================

  sc_out<sc_uint<32>>   alu_result;  // exe result

  sc_out<sc_uint<6>> DEST_RE;
  sc_out<bool>       WB_RE;

  sc_out<sc_uint<32>> MEM_DATA_RE;
  sc_out<sc_uint<2>>  MEM_SIZE_RE;
  sc_out<bool>        MEM_SIGN_EXTEND_RE;
  sc_out<bool>        MEM_LOAD_RE;
  sc_out<bool>        MEM_STORE_RE;

  sc_out<bool>        exe2mem_reg_wr_en;
  sc_out<sc_uint<2>>  exe2mem_result_src;
  sc_out<mem_wr_en>   exe2mem_mem_wr_en;
  sc_out<sc_uint<2>>  exe2mem_mem_size;
  sc_out<sc_uint<32>> exe2mem_mem_data;

  sc_out<sc_uint<5>>  exe2mem_reg_dst;

  sc_out<sc_uint<32>> exe2mem_pc_next;
  


}
