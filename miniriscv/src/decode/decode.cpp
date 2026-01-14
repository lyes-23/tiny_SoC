#include "decode.h"



void decode::concat_id2exe(){

	sc_bv<ID2EXE_SIZE>	id2exe_concat;

	id2exe_concat.range(184,183)		=	result_src.read();
	id2exe_concat.range(182,180)		=	alu_ctrl.read();
	
	id2exe_concat[179]			=	reg_wr_en.read();
	id2exe_concat[178]			=	mem_wr_en.read();
	id2exe_concat[177]			=	jump.read();
	id2exe_concat[176]			=	branch.read();
	id2exe_concat[175]			=	alu_src.read();

	id2exe_concat.range(174,143)		=	imm_ext_data.read();

	id2exe_concat.range(142,138)		= 	reg_dest.read();
	id2exe_concat.range(137,133)		=	reg_src1.read();
	id2exe_concat.range(132,128)		=	reg_src2.read();

	id2exe_concat.range(127,96)		=	rdata1.read();
	id2exe_concat.range(95,	64)		=	rdata2.read();

	id2exe_concat.range(63, 32)		=	if2id_pc_curr.read();
	id2exe_concat.range(31,  0)		=	if2id_pc_next.read();

	in_id2exe_data.write(id2exe_concat);
}


void decode::unconcat_id2exe(){
	sc_bv<ID2EXE_SIZE>	id2exe_unconcat	=	out_id2exe_data.read();

	id2exe_alu_ctrl.write	( (sc_bv_base) id2exe_unconcat.range(184,183));
	id2exe_result_src.write	( (sc_bv_base) id2exe_unconcat.range(182,180));

	id2exe_reg_wr_en.write	( (bool) id2exe_unconcat[179]	);
	id2exe_mem_wr_en.write	( (bool) id2exe_unconcat[178]	);
	id2exe_jump.write	( (bool) id2exe_unconcat[177]	);
	id2exe_branch.write	( (bool) id2exe_unconcat[176]	);
	id2exe_alu_src.write	( (bool) id2exe_unconcat[175]	);
	

	id2exe_imm_ext.write(	(sc_bv_base) id2exe_unconcat.range(174,143) );

	id2exe_reg_dst.write(	(sc_bv_base) id2exe_unconcat.range(142,138) );
	id2exe_reg_src1.write(	(sc_bv_base) id2exe_unconcat.range(137,133) );
	id2exe_reg_src2.write(	(sc_bv_base) id2exe_unconcat.range(132,128) );

	id2exe_op1_data.write(	(sc_bv_base) id2exe_unconcat.range(127,96) );
	id2exe_op2_data.write(	(sc_bv_base) id2exe_unconcat.range(95,64) );

	id2exe_pc_curr.write(	(sc_bv_base) id2exe_unconcat.range(63,32) );
	id2exe_pc_next.write(	(sc_bv_base) id2exe_unconcat.range(31,0) );

}

void decode::decoding_inst_type(){

	sc_uint<32> if_instr = if2id_instr.read(); 
	op_type     = if_instr.range(6,0);

	r_type_inst = op_type == 0b0110011  ? 1 : 0;
    	i_type_inst = (op_type == 0b0010011 || op_type == 0b0000011) ? 1 : 0;
     	s_type_inst = op_type == 0b0100011 ? 1 : 0;
     	b_type_inst = op_type == 0b1100011 ? 1 : 0;
     	u_type_inst = (op_type == 0b0110111 || op_type == 0b0010111) ? 1 : 0;
     	j_type_inst = op_type == 0b1101111 ? 1 : 0;
     	jalr_type_inst = op_type == 0b1100111 ? 1 : 0;
}

void decode::decoding_inst(){

	sc_uint<32> if_instr = if2id_instr.read();

	funct3	= if_instr.range(14,12);
	funct7  = if_instr.range(31,25);
	
	// R type instructions

    	if (op_type == 0b0110011 && funct7 == 0b0000000 &&
         	funct3 == 0b000)
         	add_inst.write(1);
     	else
     	    add_inst.write(0);
     	if (op_type == 0b0110011 && funct7 == 0b0000000 &&
     	    funct3 == 0b010)
     	    slt_inst.write(1);
     	else
     	    slt_inst.write(0);
     	if (op_type == 0b0110011 && funct7 == 0b0000000 &&
     	    funct3 == 0b011)
     	    sltu_inst.write(1);
     	else
     	    sltu_inst.write(0);
     	if (op_type == 0b0110011 && funct7 == 0b0000000 &&
     	    funct3 == 0b111)
     	    and_inst.write(1);
     	else
     	    and_inst.write(0);
     	if (op_type == 0b0110011 && funct7 == 0b0000000 &&
     	    funct3 == 0b110)
     	    or_inst.write(1);
     	else
     	    or_inst.write(0);
     	if (op_type == 0b0110011 && funct7 == 0b0000000 &&
     	    funct3 == 0b100)
     	    xor_inst.write(1);
     	else
     	    xor_inst.write(0);
     	if (op_type == 0b0110011 && funct7 == 0b0000000 &&
     	    funct3 == 0b001)
     	    sll_inst.write(1);
     	else
     	    sll_inst.write(0);
     	if (op_type == 0b0110011 && funct7 == 0b0000000 &&
     	    funct3 == 0b101)
     	    srl_inst.write(1);
     	else
     	    srl_inst.write(0);
     	if (op_type == 0b0110011 && funct7 == 0b0100000 &&
     	    funct3 == 0b000)
     	    sub_inst.write(1);
     	else
     	    sub_inst.write(0);
     	if (op_type == 0b0110011 && funct7 == 0b0100000 &&
     	    funct3 == 0b101)
     	    sra_inst.write(1);
     	else
     	    sra_inst.write(0);

     	// I type instructions 

     	if (op_type == 0b0010011 && funct3 == 0b000)
     	    addi_inst.write(1);
     	else
     	    addi_inst.write(0);
     	if (op_type == 0b0010011 && funct3 == 0b010)
     	    slti_inst.write(1);
     	else
     	    slti_inst.write(0);
     	if (op_type == 0b0010011 && funct3 == 0b011)
     	    sltiu_inst.write(1);
     	else
     	    sltiu_inst.write(0);
     	if (op_type == 0b0010011 && funct3 == 0b111)
     	    andi_inst.write(1);
     	else
     	    andi_inst.write(0);
     	if (op_type == 0b0010011 && funct3 == 0b110)
     	    ori_inst.write(1);
     	else
     	    ori_inst.write(0);
     	if (op_type == 0b0010011 && funct3 == 0b100)
     	    xori_inst.write(1);
     	else
     	    xori_inst.write(0);
     	if (op_type == 0b0001111 && funct3 == 0b000)
     	    fence_inst.write(1);
     	else
     	    fence_inst.write(0);

     	// I-type shift instructions :

     	if (op_type == 0b0010011 && funct7 == 0b0000000 &&
     	    funct3 == 0b001)
     	    slli_inst.write(1);
     	else
     	    slli_inst.write(0);
     	if (op_type == 0b0010011 && funct7 == 0b0000000 &&
     	    funct3 == 0b101)
     	    srli_inst.write(1);
     	else
     	    srli_inst.write(0);
     	if (op_type == 0b0010011 && funct7 == 0b0100000 &&
     	    funct3 == 0b101)
     	    srai_inst.write(1);
     	else
     	    srai_inst.write(0);

     	// I-type load instructions :

     	if (op_type == 0b0000011 && funct3 == 0b010)
     	    lw_inst.write(1);
     	else
     	    lw_inst.write(0);
     	if (op_type == 0b0000011 && funct3 == 0b001)
     	    lh_inst.write(1);
     	else
     	    lh_inst.write(0);
     	if (op_type == 0b0000011 && funct3 == 0b101)
     	    lhu_inst.write(1);
     	else
     	    lhu_inst.write(0);
     	if (op_type == 0b0000011 && funct3 == 0b000)
     	    lb_inst.write(1);
     	else
     	    lb_inst.write(0);
     	if (op_type == 0b0000011 && funct3 == 0b100)
     	    lbu_inst.write(1);
     	else
     	    lbu_inst.write(0);

     	// B-type Instructions :

     	if (op_type == 0b1100011 && funct3 == 0b000)
     	    beq_inst.write(1);
     	else
     	    beq_inst.write(0);
     	if (op_type == 0b1100011 && funct3 == 0b001)
     	    bne_inst.write(1);
     	else
     	    bne_inst.write(0);
     	if (op_type == 0b1100011 && funct3 == 0b100)
     	    blt_inst.write(1);
     	else
     	    blt_inst.write(0);
     	if (op_type == 0b1100011 && funct3 == 0b101)
     	    bge_inst.write(1);
     	else
     	    bge_inst.write(0);
     	if (op_type == 0b1100011 && funct3 == 0b110)
     	    bltu_inst.write(1);
     	else
     	    bltu_inst.write(0);
     	if (op_type == 0b1100011 && funct3 == 0b111)
     	    bgeu_inst.write(1);
     	else
     	    bgeu_inst.write(0);

     	// U-type Instructions :

     	if (op_type == 0b0110111)
     	    lui_inst.write(1);
     	else
     	    lui_inst.write(0);
     	if (op_type == 0b0010111)
     	    auipc_inst.write(1);
     	else
     	    auipc_inst.write(0);

     	// J-type Instructions :

     	if (op_type == 0b1101111)
     	    jal_inst.write(1);
     	else
     	    jal_inst.write(0);
     	if (op_type == 0b1100111)
     	    jalr_inst.write(1);
     	else
     	    jalr_inst.write(0);

     	// S-type Instructions :

     	if (op_type == 0b0100011 && funct3 == 0b010)
     	    sw_inst.write(1);
     	else
     	    sw_inst.write(0);
     	if (op_type == 0b0100011 && funct3 == 0b001)
     	    sh_inst.write(1);
     	else
     	    sh_inst.write(0);
     	if (op_type == 0b0100011 && funct3 == 0b000)
     	    sb_inst.write(1);
     	else
     	    sb_inst.write(0);
}

void decode::regfile_gestion(){

}
