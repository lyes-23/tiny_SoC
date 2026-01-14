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

	sc_in<sc_bv<32>>	out_if2id_valid;
	sc_out<bool>		out_if2id_ready;

//==========================================
//	Execute stage interface
//==========================================

        sc_out<sc_uint<32>>	id2exe_op1_data;
       	sc_out<sc_uint<32>>	id2exe_op2_data;

        sc_out<bool>		id2exe_reg_wr_en;
        sc_out<sc_uint<2>>	id2exe_result_src;
        sc_out<bool>		id2exe_mem_wr_en;
        sc_out<bool>		id2exe_jump;
        sc_out<bool>		id2exe_branch;
        sc_out<bool>		id2exe_alu_src;
        sc_out<sc_uint<<3>>	id2exe_alu_ctrl;

   	sc_out<sc_uint<32>> 	id2exe_pc_curr;
    	sc_out<sc_uint<32>> 	id2exe_pc_next;

    	sc_out<sc_uint<5>>	id2exe_reg_src1;
    	sc_out<sc_uint<5>>	id2exe_reg_src2;
    	sc_out<sc_uint<5>>	id2exe_reg_dst;

    	sc_out<sc_uint<32>>	id2exe_imm_ext;

	sc_out<bool>		out_id2exe_valid;
	sc_in<bool>		out_id2exe_ready;

	sc_in<bool>		hazard_unit_flush_exe;



    
	fifo<ID2EXE_SIZE,1>	id2exe;

	// Decalaring signals
	
	sc_signal<sc_uint<5>>	reg_src1;
	sc_signal<sc_uint<5>>	reg_src2;
	sc_signal<sc_uint<5>>	reg_dest;

	sc_signal<sc_uint<2>>	imm_src_d;
	
	sc_signal<sc_uint<7>>	op_type;
	sc_signal<sc_uint<3>>	funct3;
	sc_signal<sc_uint<1>>   funct7;	

	sc_signal<sc_uint<32>> 	imm_ext;


        sc_signal<bool>		reg_wr_en;
        sc_signal<sc_uint<2>>	result_src;
        sc_signal<bool>		mem_wr_en;
        sc_signal<bool>		jump;
        sc_signal<bool>		branch;
        sc_signal<bool>		alu_src;
        sc_signal<sc_uint<<3>>	alu_ctrl;


	// pipeline signals
	sc_signal<sc_bv<ID2EXE>>	in_id2exe_data;
	sc_signal<bool>			in_id2exe_valid;
	sc_signal<bool>			in_id2exe_ready;

	sc_signal<sc_bv<ID2EXE_SIZE>>	out_id2exe_data;
	sc_signal<bool>			id2exe_full;
	sc_signal<bool>			id2exe_empty;


    // Instruction format type :

   	 sc_signal<bool> r_type_inst;     // R type format
   	 sc_signal<bool> i_type_inst;     // I type format
   	 sc_signal<bool> s_type_inst;     // S type format
   	 sc_signal<bool> b_type_inst;     // B type format
   	 sc_signal<bool> u_type_inst;     // U type format
   	 sc_signal<bool> j_type_inst;     // J type format
   	 sc_signal<bool> jalr_type_inst;  

    // R-type Instructions :

   	 sc_signal<bool> add_inst;
   	 sc_signal<bool> slt_inst;
   	 sc_signal<bool> sltu_inst;
   	 sc_signal<bool> and_inst;
   	 sc_signal<bool> or_inst;
   	 sc_signal<bool> xor_inst;
   	 sc_signal<bool> sll_inst;
   	 sc_signal<bool> srl_inst;
   	 sc_signal<bool> sub_inst;
   	 sc_signal<bool> sra_inst;

    // I-type Instructions :

    	 sc_signal<bool> addi_inst;
    	 sc_signal<bool> slti_inst;
    	 sc_signal<bool> sltiu_inst;
    	 sc_signal<bool> andi_inst;
    	 sc_signal<bool> ori_inst;
    	 sc_signal<bool> xori_inst;
    	 sc_signal<bool> jalr_inst;
    	 sc_signal<bool> fence_inst;

    // I-type shift instructions :

    	 sc_signal<bool> slli_inst;
    	 sc_signal<bool> srli_inst;
    	 sc_signal<bool> srai_inst;

    // I-type load instructions :

    	 sc_signal<bool> lw_inst;
    	 sc_signal<bool> lh_inst;
    	 sc_signal<bool> lhu_inst;
    	 sc_signal<bool> lb_inst;
    	 sc_signal<bool> lbu_inst;

    // B-type Instruction :

    	 sc_signal<bool> beq_inst;
    	 sc_signal<bool> bne_inst;
    	 sc_signal<bool> blt_inst;
    	 sc_signal<bool> bge_inst;
    	 sc_signal<bool> bltu_inst;
    	 sc_signal<bool> bgeu_inst;

    // U-type Instruction :

    	 sc_signal<bool> lui_inst;
    	 sc_signal<bool> auipc_inst;

    // J-type Instruction :

    	 sc_signal<bool> jal_inst;

    // S-type Instructions :

    	 sc_signal<bool> sw_inst;
    	 sc_signal<bool> sh_inst;
    	 sc_signal<bool> sb_inst;

	 void concat_id2exe();
	 void unconcat_id2exe();
	 void decoding_inst_type();
	 void decoding_inst();
	 void regfile_gestion();
	 void stall_gestion();
	 void trace(sc_trace_file *tf);



    SC_CTOR(decode) : id2exe("id2exe"){
    	
	    id2exe.clk(clk);
	    id2exe.arstn(arstn);

	    id2exe.in_ready_o	(in_id2exe_ready);
	    id2exe.in_valid_i	(in_id2exe_valid);
    	    id2exe.in_data_i	(in_id2exe_data);

	    id2exe.out_ready_i	(out_id2exe_ready);
    	    id2exe.out_valid_o	(out_id2exe_valid);
	    id2exe.out_data_o	(id2exe_out);
	    id2exe.full_o	(id2exe_full);
   	    id2exe.empty_o	(id2exe_empty);


	    SC_METHOD(concat_id2exe);
	    sensitive <<  

	    SC_METHOD(unconcat_id2exe);
	    sensitive

	    SC_METHOD(decoding_inst_type);
	    sensitive

	    SC_METHOD(decoding_inst);
	    sensitive

	    SC_METHOD(regfile_gestion);
	    sensitive

	    SC_METHOD(stall_gestion);
	    sensitive

	
	reset_signal_is(arstn,false);
 
    } 
    
    
    
    
    
    
    
    }


