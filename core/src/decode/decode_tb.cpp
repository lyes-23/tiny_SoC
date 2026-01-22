
#include <systemc.h>
#include "decode.h"


int sc_main(int argc, char* argv[]) {

        sc_clock clk("clk", 10, SC_NS);
        sc_signal<bool> arstn;
    //===========================================
    //	Regfile Interface
    //============================================
      	sc_signal<sc_bv<6>>	    raddr1;
      	sc_signal<sc_bv<6>>	    raddr2;
      	sc_signal<sc_uint<32>>	rdata1;
      	sc_signal<sc_uint<32>>	rdata2;
      //==========================================
      //	Fetch stage interface
      //=========================================
      	sc_signal<sc_bv<32>>	  if2id_pc_curr;
      	sc_signal<sc_bv<32>>	  if2id_pc_next;
      	sc_signal<sc_bv<32>>	  if2id_inst;
      	sc_signal<sc_bv<32>>	  out_if2id_valid;
      	sc_signal<bool>		      out_if2id_ready;
      //==========================================
      //	Execute stage interface
      //==========================================
        sc_signal<sc_uint<32>>	id2exe_op1_data;
        sc_signal<sc_uint<32>>	id2exe_op2_data;
        sc_signal<bool>		      id2exe_reg_wr_en;
        sc_signal<sc_uint<2>>	  id2exe_result_src;
        sc_signal<bool>		      id2exe_mem_wr_en;
      	sc_signal<sc_uint<2>>	  id2exe_mem_size;
      	sc_signal<sc_uint<32>>	id2exe_mem_data;
        sc_signal<bool>		      id2exe_jump;
        sc_signal<bool>		      id2exe_branch;
        sc_signal<sc_uint<32>>  id2exe_branch_offset;
        sc_signal<sc_uint<4>>	  id2exe_alu_ctrl;
        sc_signal<sc_uint<32>>  id2exe_pc_next;
        sc_signal<sc_uint<5>>	  id2exe_reg_src1;
        sc_signal<sc_uint<5>>	  id2exe_reg_src2;
        sc_signal<sc_uint<5>>	  id2exe_reg_dst;
        sc_signal<sc_uint<32>>	id2exe_imm_ext;
      	sc_signal<bool>		      out_id2exe_valid;
      	sc_signal<bool>		      out_id2exe_ready;
      	sc_signal<bool>		      hazard_unit_flush_exe;


        decode dut("DUT");

        // Clock and Reset bindings
        dut.clk(clk);
        dut.arstn(arstn);

        // Register file interface bindings
        dut.raddr1(raddr1);
        dut.raddr2(raddr2);
        dut.rdata1(rdata1);
        dut.rdata2(rdata2);

        // Fetch stage interface bindings
        dut.if2id_pc_curr(if2id_pc_curr);
        dut.if2id_pc_next(if2id_pc_next);
        dut.if2id_inst(if2id_inst);
        dut.out_if2id_valid(out_if2id_valid);
        dut.out_if2id_ready(out_if2id_ready);

        // Execute stage interface bindings
        dut.id2exe_op1_data(id2exe_op1_data);
        dut.id2exe_op2_data(id2exe_op2_data);
        dut.id2exe_reg_wr_en(id2exe_reg_wr_en);
        dut.id2exe_result_src(id2exe_result_src);
        dut.id2exe_mem_wr_en(id2exe_mem_wr_en);
        dut.id2exe_mem_size(id2exe_mem_size);
        dut.id2exe_mem_data(id2exe_mem_data);
        dut.id2exe_jump(id2exe_jump);
        dut.id2exe_branch(id2exe_branch);
        dut.id2exe_branch_offset(id2exe_branch_offset);
        dut.id2exe_alu_ctrl(id2exe_alu_ctrl);
        dut.id2exe_pc_next(id2exe_pc_next);
        dut.id2exe_reg_src1(id2exe_reg_src1);
        dut.id2exe_reg_src2(id2exe_reg_src2);
        dut.id2exe_reg_dst(id2exe_reg_dst);
        dut.id2exe_imm_ext(id2exe_imm_ext);
        dut.out_id2exe_valid(out_id2exe_valid);
        dut.out_id2exe_ready(out_id2exe_ready);
        dut.hazard_unit_flush_exe(hazard_unit_flush_exe);

        arstn = false;
        sc_start(1, SC_NS);
        arstn = true;

        cout << "Elaboration done correctly" << endl;

        return 0;
}


