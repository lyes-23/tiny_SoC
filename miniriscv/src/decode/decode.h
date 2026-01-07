#include <systemc.h>
#include <iostream>
#include "miniriscv_def.h"
#include "fifo_element.h"


SC_MODULE(decode){

	sc_in_clk		clk;
	sc_in<bool>		arstn;

//===========================================
//	Regfile Interface
//============================================

	sc_out<sc_bv<6>>	raddr1;
	sc_out<sc_bv<6>>	raddr2;

	sc_in<sc_uint<32>>	rdata1;
	sc_in<sc_uint<32>>	rdata2;

//==========================================
//	Fetch stage interface
//=========================================
	
	sc_in<sc_bv<32>>	if2id_pc_curr;
	sc_in<sc_bv<32>>	if2id_pc_next;
	sc_in<sc_bv<32>>	if2id_instr;

	sc_in<sc_bv<32>>	if2id_pc_valid;
	sc_out<bool>		if2id_pc_ready;

//==========================================
//	Execute stage interface
//==========================================

    sc_out<sc_uint<32>> op1_data;
    sc_out<sc_uint<32>> op2_data;

    sc_out<bool>	regfile_write_en_exe;
    sc_out<sc_uint<2>>	result_src_exe;
    sc_out<bool>	memory_write_enable_exe;

    sc_out<bool>	jump_exe;
    sc_out<bool>	branch_exe;
    sc_out<bool>	alu_src_exe;
    sc_out<sc_uint<<3>> alu_ctrl;

    sc_out<sc_uint<32>> id2exe_pc_curr;
    sc_out<sc_uint<32>> id2exe_pc_next;

    sc_out<sc_uint<5>>	reg_src1_exe;
    sc_out<sc_uint<5>>	reg_src2_exe;
    sc_out<sc_uint<5>>	reg_dst_exe;

    sc_out<sc_uint<32>>	imm_extd_exe;

    sc_out<


