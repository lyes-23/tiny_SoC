#include "decode.h"



void decode::concat_id2exe(){

	sc_bv<ID2EXE_SIZE>	id2exe_concat;

	id2exe_concat.range(218,217)		=	result_src.read();
	id2exe_concat.range(216,214)		=	alu_ctrl.read();
	id2exe_concat.range(213,212)		=	mem_size.read();
	id2exe_concat[211]		        	=	reg_wr_en.read();
	id2exe_concat[210]		        	=	mem_wr_en.read();
	id2exe_concat.range(178,209)		=	mem_data.read();
	id2exe_concat[177]		        	=	jump.read();
	id2exe_concat[176]		        	=	branch.read();
	id2exe_concat[175]		        	=	alu_src.read();

	id2exe_concat.range(174,143)		=	imm_ext_data.read();

	id2exe_concat.range(142,138)		= 	reg_dest.read();
	id2exe_concat.range(137,133)		=	reg_src1.read();
	id2exe_concat.range(132,128)		=	reg_src2.read();

	id2exe_concat.range(127,96)		=	rdata1.read();
	id2exe_concat.range(95,	64)		=	rdata2.read();

	id2exe_concat.range(63, 32)		=	branch_offset.read();
	id2exe_concat.range(31,  0)		=	if2id_pc_next.read();

	in_id2exe_data.write(id2exe_concat);
}


void decode::unconcat_id2exe(){
	sc_bv<ID2EXE_SIZE>	id2exe_unconcat	=	out_id2exe_data.read();

	id2exe_alu_ctrl.write	( (sc_bv_base) id2exe_unconcat.range(218,217));
	id2exe_result_src.write	( (sc_bv_base) id2exe_unconcat.range(216,214));

	id2exe_mem_size.write	( (sc_bv_base) id2exe_unconcat[213,212]);

	id2exe_reg_wr_en.write	( (bool) id2exe_unconcat[211]	);
	id2exe_mem_wr_en.write	( (bool) id2exe_unconcat[210]	);
  id2exe_mem_data.write   ( (sc_bv_base) id2exe_unconcat.range(178,209) );

	id2exe_jump.write	( (bool) id2exe_unconcat[177]	);
	id2exe_branch.write	( (bool) id2exe_unconcat[176]	);
	id2exe_alu_src.write	( (bool) id2exe_unconcat[175]	);
	

	id2exe_imm_ext.write(	(sc_bv_base) id2exe_unconcat.range(174,143) );

	id2exe_reg_dst.write(	(sc_bv_base) id2exe_unconcat.range(142,138) );
	id2exe_reg_src1.write(	(sc_bv_base) id2exe_unconcat.range(137,133) );
	id2exe_reg_src2.write(	(sc_bv_base) id2exe_unconcat.range(132,128) );

	id2exe_op1_data.write(	(sc_bv_base) id2exe_unconcat.range(127,96) );
	id2exe_op2_data.write(	(sc_bv_base) id2exe_unconcat.range(95,64) );

  id2exe_branch_offset.write( (sc_bv_base) id2exe_unconcat.range(32,63));
	id2exe_pc_next.write(	(sc_bv_base) id2exe_unconcat.range(31,0) );

}

void decode::decoding_inst_type(){

	sc_uint<32> if_inst = if2id_inst.read(); 
	op_type     = if_inst.range(6,0);

	r_type_inst = op_type == 0b0110011  ? 1 : 0;
    	i_type_inst = (op_type == 0b0010011 || op_type == 0b0000011) ? 1 : 0;
     	s_type_inst = op_type == 0b0100011 ? 1 : 0;
     	b_type_inst = op_type == 0b1100011 ? 1 : 0;
     	u_type_inst = (op_type == 0b0110111 || op_type == 0b0010111) ? 1 : 0;
     	j_type_inst = op_type == 0b1101111 ? 1 : 0;
     	jalr_type_inst = op_type == 0b1100111 ? 1 : 0;
}

void decode::decoding_inst(){

	sc_uint<32> if_inst = if2id_inst.read();

	funct3	= if_inst.range(14,12);
	funct7  = if_inst.range(31,25);
	
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

	sc_uint<32> if_inst = if2id_inst.read();  

	sc_uint<6>  raddr1_var;
	sc_uint<6>  raddr2_var;
	sc_uint<6>  addr_src_var;

	if(r_type_inst ){
	raddr1_var	= if_inst.range(19,15);
	raddr2_var	= if_inst.range(24,20);
	addr_src_var	= if_inst.range(11,7);
	}
	else if(i_type_inst ){
	raddr1_var	= if_inst.range(19,15);
	raddr2_var	= 0;
	addr_src_var	= if_inst.range(11,7);
	} 
	else if(s_type_inst ){
	raddr1_var	= if_inst.range(19,15);
	raddr2_var	= if_inst.range(24,20);
	addr_src_var	= 0;
		
	}
	else if(b_type_inst ){
	raddr1_var	= if_inst.range(19,15);
	raddr2_var	= if_inst.range(24,20);
	addr_src_var	= 0;
	}
	else if(u_type_inst ){
	raddr1_var	= 0;
	raddr2_var	= 0;
	addr_src_var	= if_inst.range(11,7);
	}
	else if(j_type_inst ){
	raddr1_var	= 0;
	raddr2_var	= 0;
	addr_src_var	= if_inst.range(11,7);
	}
	else if(jalr_type_inst){
	raddr1_var	= if_inst.range(19,15);
	raddr2_var	= 0;
	addr_src_var	= if_inst.range(11,7);
	}
	else {
	raddr1_var	= 0;
	raddr2_var	= 0;
	addr_src_var	= 0;
	}

	raddr1.write(raddr1_var);
	raddr2.write(raddr2_var);
	reg_dest.write(addr_src_var);
}

void decode::control_unit(){


	sc_uint<32> id2exe_op1_var;
  sc_uint<32> id2exe_op2_var;
  sc_uint<32> if_inst = if2id_inst.read();
  sc_uint<32> mem_data_var;
  sc_uint<32> imm_ext_var;
	
  bool  	    branch_var   = false;
  bool        jump_var     = false;
  bool        illegal_inst = false;


	if(r_type_inst || i_type_inst || u_type_inst ){
		if(i_type_inst){
			id2exe_op1_var = rdata1;
			id2exe_op2_var.range(11,0) = if_inst.range(31,20);
			if(if_inst[1] ){
			id2exe_op2_var.range(31,12) = 0xFFFFF; 
			} else{
			id2exe_op2_var.range(31,12) = 0;
			}
		}
		else if(u_type_inst){
			id2exe_op1_var.range(31,12) = if_inst.range(31,12);
			id2exe_op1_var.range(11,0 ) = 0;
			if(auipc_inst){
			id2exe_op2_var = if2id_pc_curr;
			}
			else {
			id2exe_op2_var = rdata2;
			}
		}
		else {
			id2exe_op1_var = rdata1;
			id2exe_op2_var = rdata2;
		}

		// controlling memory loading instructions
		if(lw_inst | lh_inst | lhu_inst | lb_inst | lbu_inst) {
			if(lw_inst)	
				mem_size = 0;
			else if(lh_inst | lhu_inst)	
				mem_size = 1;
			else if(lb_inst | lbu_inst)
				mem_size = 2;
			mem_wr_en = true;
		}
		else {
		mem_wr_en = false;
		mem_size  = 0;
		}
	}
	else if (s_type_inst){
 
    branch_var   = false;
    jump_var     = false;
    illegal_inst = false;

    // The data to be stored comes from rs2 
		mem_data_var = rdata2.read();
    mem_wr_en    = true;
    
    if(sw_inst){
      mem_size = 0;
    }
    else if(sh_inst){
      mem_size = 1; 
    }
    else if(sb_inst){
      mem_size = 2; 
    }

    // the data of the rs1 that will be added to the imm
    id2exe_op1_var = rdata1.read();

    // Handling the Imm from the instruction and sign extension
		id2exe_op2_var.range(11,5) = if_inst.range(31,25);
		id2exe_op2_var.range(5,0 ) = if_inst.range(11,7 );
		if(if_inst[31]){
		  id2exe_op2_var.range(31,12) = 0xFFFFF;
		}
		else{
      id2exe_op2_var.range(31,12) = 0;
		}
	}
	else if (b_type_inst){
	  branch_var   = true;
	  jump_var     = false;
	  illegal_inst = false;
    mem_data_var = 0;
    mem_wr_en    = false;
    mem_size     = 0;
    alu_ctrl     = ;

    id2exe_op1_var = rdara1.read();
    id2exe_op2_var = rdata2.read();

    
    if(if_inst[31]){
        branch_offset_var.range(31,13) = 0xFFFFF; 
    }
    else{
        branch_offset_var.range(31,13) = 0;
    }

    branch_offset_var[12] = if_inst[31]; 
    branch_offset_var[11] = if_inst[7]
    branch_offset_var.range(10,5) = if_inst.range(30,25);
    branch_offset_var.range(4,1 ) = if_inst.range(11,8 );
    branch_offset_var[0]  = 0;

    if(beq_inst){}
    else if(bne_inst){}
    else if(blt_inst){}
    else if(bge_inst){}
    else if(bltu_inst){}
    else if(bgeu_inst){}



  
  }
  
  
  else if( j_type_inst || jalr_type_inst ){
	
	}
	else if( fence_type_inst) {

	}
	else{
	
	}
}


void decode::stall_gestion(){

}


void decode::trace(sc_trace_file *tf){

}



