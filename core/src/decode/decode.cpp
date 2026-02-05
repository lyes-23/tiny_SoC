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

	id2exe_concat.range(174,143)		=	branch_offset.read();

	id2exe_concat.range(142,138)		= reg_dest.read();
	id2exe_concat.range(137,133)		=	reg_src1.read();
	id2exe_concat.range(132,128)		=	reg_src2.read();

	id2exe_concat.range(127,96)		=	rdata1.read();
	id2exe_concat.range(95,	64)		=	rdata2.read();

	id2exe_concat.range(63, 32)		=	branch_offset.read();
	id2exe_concat.range(31,  0)		=	if2id_pc_next.read();

	in_id2exe_data.write(id2exe_concat);
  in_id2exe_valid.write( !id2exe_full.read() && in_id2exe_ready.read() );
}


void decode::unconcat_id2exe(){



	sc_bv<ID2EXE_SIZE>	id2exe_unconcat	=	out_id2exe_data.read();

	id2exe_alu_ctrl.write	( (sc_bv_base) id2exe_unconcat.range(218,217));
	id2exe_result_src.write	( (sc_bv_base) id2exe_unconcat.range(216,214));

	id2exe_mem_size.write	( (sc_bv_base) id2exe_unconcat.range(213,212));

	id2exe_reg_wr_en.write	( (bool) id2exe_unconcat[211]	);
	id2exe_mem_wr_en.write	( (bool) id2exe_unconcat[210]	);
  id2exe_mem_data.write   ( (sc_bv_base) id2exe_unconcat.range(178,209) );
	id2exe_jump.write	( (bool) id2exe_unconcat[177]	);
	id2exe_branch.write	( (bool) id2exe_unconcat[176]	);
	

	id2exe_branch_offset.write(	(sc_bv_base) id2exe_unconcat.range(174,143) );

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

	r_type_inst = (op_type.read() == 0b0110011)  ? 1 : 0;
    	i_type_inst = (op_type.read() == 0b0010011 || op_type.read() == 0b0000011) ? 1 : 0;
     	s_type_inst = op_type.read() == 0b0100011 ? 1 : 0;
     	b_type_inst = op_type.read() == 0b1100011 ? 1 : 0;
     	u_type_inst = (op_type.read() == 0b0110111 || op_type.read() == 0b0010111) ? 1 : 0;
     	j_type_inst = op_type.read() == 0b1101111 ? 1 : 0;
     	jalr_type_inst = op_type.read() == 0b1100111 ? 1 : 0;
}

void decode::decoding_inst(){

	sc_uint<32> if_inst = if2id_inst.read();

	funct3	= if_inst.range(14,12);
	funct7  = if_inst.range(31,25);
	
	// R type instructions

    	if (op_type.read() == 0b0110011 && funct7.read() == 0b0000000 &&
         	funct3.read() == 0b000)
         	add_inst.write(1);
     	else
     	    add_inst.write(0);
     	if (op_type.read() == 0b0110011 && funct7.read() == 0b0000000 &&
     	    funct3.read() == 0b010)
     	    slt_inst.write(1);
     	else
     	    slt_inst.write(0);
     	if (op_type.read() == 0b0110011 && funct7.read() == 0b0000000 &&
     	    funct3.read() == 0b011)
     	    sltu_inst.write(1);
     	else
     	    sltu_inst.write(0);
     	if (op_type.read() == 0b0110011 && funct7.read() == 0b0000000 &&
     	    funct3.read() == 0b111)
     	    and_inst.write(1);
     	else
     	    and_inst.write(0);
     	if (op_type.read() == 0b0110011 && funct7.read() == 0b0000000 &&
     	    funct3.read() == 0b110)
     	    or_inst.write(1);
     	else
     	    or_inst.write(0);
     	if (op_type.read() == 0b0110011 && funct7.read() == 0b0000000 &&
     	    funct3.read() == 0b100)
     	    xor_inst.write(1);
     	else
     	    xor_inst.write(0);
     	if (op_type.read() == 0b0110011 && funct7.read() == 0b0000000 &&
     	    funct3.read() == 0b001)
     	    sll_inst.write(1);
     	else
     	    sll_inst.write(0);
     	if (op_type.read() == 0b0110011 && funct7.read() == 0b0000000 &&
     	    funct3.read() == 0b101)
     	    srl_inst.write(1);
     	else
     	    srl_inst.write(0);
     	if (op_type.read() == 0b0110011 && funct7.read() == 0b0100000 &&
     	    funct3.read() == 0b000)
     	    sub_inst.write(1);
     	else
     	    sub_inst.write(0);
     	if (op_type.read() == 0b0110011 && funct7.read() == 0b0100000 &&
     	    funct3.read() == 0b101)
     	    sra_inst.write(1);
     	else
     	    sra_inst.write(0);

     	// I type instructions 

     	if (op_type.read() == 0b0010011 && funct3.read() == 0b000)
     	    addi_inst.write(1);
     	else
     	    addi_inst.write(0);
     	if (op_type.read() == 0b0010011 && funct3.read() == 0b010)
     	    slti_inst.write(1);
     	else
     	    slti_inst.write(0);
     	if (op_type.read() == 0b0010011 && funct3.read() == 0b011)
     	    sltiu_inst.write(1);
     	else
     	    sltiu_inst.write(0);
     	if (op_type.read() == 0b0010011 && funct3.read() == 0b111)
     	    andi_inst.write(1);
     	else
     	    andi_inst.write(0);
     	if (op_type.read() == 0b0010011 && funct3.read() == 0b110)
     	    ori_inst.write(1);
     	else
     	    ori_inst.write(0);
     	if (op_type.read() == 0b0010011 && funct3.read() == 0b100)
     	    xori_inst.write(1);
     	else
     	    xori_inst.write(0);
     	if (op_type.read() == 0b0001111 && funct3.read() == 0b000)
     	    fence_inst.write(1);
     	else
     	    fence_inst.write(0);

     	// I-type shift instructions :

     	if (op_type.read() == 0b0010011 && funct7.read() == 0b0000000 &&
     	    funct3.read() == 0b001)
     	    slli_inst.write(1);
     	else
     	    slli_inst.write(0);
     	if (op_type.read() == 0b0010011 && funct7.read() == 0b0000000 &&
     	    funct3.read() == 0b101)
     	    srli_inst.write(1);
     	else
     	    srli_inst.write(0);
     	if (op_type.read() == 0b0010011 && funct7.read() == 0b0100000 &&
     	    funct3.read() == 0b101)
     	    srai_inst.write(1);
     	else
     	    srai_inst.write(0);

     	// I-type load instructions :

     	if (op_type.read() == 0b0000011 && funct3.read() == 0b010)
     	    lw_inst.write(1);
     	else
     	    lw_inst.write(0);
     	if (op_type.read() == 0b0000011 && funct3.read() == 0b001)
     	    lh_inst.write(1);
     	else
     	    lh_inst.write(0);
     	if (op_type.read() == 0b0000011 && funct3.read() == 0b101)
     	    lhu_inst.write(1);
     	else
     	    lhu_inst.write(0);
     	if (op_type.read() == 0b0000011 && funct3.read() == 0b000)
     	    lb_inst.write(1);
     	else
     	    lb_inst.write(0);
     	if (op_type.read() == 0b0000011 && funct3.read() == 0b100)
     	    lbu_inst.write(1);
     	else
     	    lbu_inst.write(0);

     	// B-type Instructions :

     	if (op_type.read() == 0b1100011 && funct3.read() == 0b000)
     	    beq_inst.write(1);
     	else
     	    beq_inst.write(0);
     	if (op_type.read() == 0b1100011 && funct3.read() == 0b001)
     	    bne_inst.write(1);
     	else
     	    bne_inst.write(0);
     	if (op_type.read() == 0b1100011 && funct3.read() == 0b100)
     	    blt_inst.write(1);
     	else
     	    blt_inst.write(0);
     	if (op_type.read() == 0b1100011 && funct3.read() == 0b101)
     	    bge_inst.write(1);
     	else
     	    bge_inst.write(0);
     	if (op_type.read() == 0b1100011 && funct3.read() == 0b110)
     	    bltu_inst.write(1);
     	else
     	    bltu_inst.write(0);
     	if (op_type.read() == 0b1100011 && funct3.read() == 0b111)
     	    bgeu_inst.write(1);
     	else
     	    bgeu_inst.write(0);

     	// U-type Instructions :

     	if (op_type.read() == 0b0110111)
     	    lui_inst.write(1);
     	else
     	    lui_inst.write(0);
     	if (op_type.read() == 0b0010111)
     	    auipc_inst.write(1);
     	else
     	    auipc_inst.write(0);

     	// J-type Instructions :

     	if (op_type.read() == 0b1101111)
     	    jal_inst.write(1);
     	else
     	    jal_inst.write(0);
     	if (op_type.read() == 0b1100111)
     	    jalr_inst.write(1);
     	else
     	    jalr_inst.write(0);

     	// S-type Instructions :

     	if (op_type.read() == 0b0100011 && funct3.read() == 0b010)
     	    sw_inst.write(1);
     	else
     	    sw_inst.write(0);
     	if (op_type.read() == 0b0100011 && funct3.read() == 0b001)
     	    sh_inst.write(1);
     	else
     	    sh_inst.write(0);
     	if (op_type.read() == 0b0100011 && funct3.read() == 0b000)
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
  sc_uint<32> branch_offset_var;
  sc_uint<4>  alu_ctrl_var;
  sc_uint<2>  result_src_var;
  bool        signed_op_var;
  
	
  bool  	    branch_var   = false;
  bool        jump_var     = false;
  bool        illegal_inst = false;

  bool        reg_wr_en_var= false;


	if(r_type_inst || i_type_inst || u_type_inst ){

    branch_var = false;
    jump_var   = false; 
    illegal_inst= false;
    branch_offset_var = 0;
    reg_wr_en_var = true;


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
      result_src_var = 1;
			id2exe_op1_var.range(31,12) = if_inst.range(31,12);
			id2exe_op1_var.range(11,0 ) = 0;
			if(auipc_inst){
			id2exe_op2_var = if2id_pc_curr.read();
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
      result_src_var = 1;
			mem_wr_en = true;
			if(lw_inst)	
				mem_size = 0;
			else if(lh_inst | lhu_inst)	
				mem_size = 1;
			else if(lb_inst | lbu_inst)
				mem_size = 2;
		  }
		  else {
        mem_wr_en = false;
		    mem_size  = 0;
        result_src_var = 0;
      }

    if ( add_inst || addi_inst ){
        alu_ctrl_var = 1;
        signed_op_var = true;
    }else if ( sub_inst ){
        alu_ctrl_var = 2;
        signed_op_var = true; 
    } else if (or_inst || ori_inst){
        alu_ctrl_var = 3;
        signed_op_var = true;
    } else if ( and_inst || andi_inst ){
        alu_ctrl_var = 4;
        signed_op_var = true;
    } else if ( xor_inst || xori_inst ){
        alu_ctrl_var = 5;
        signed_op_var = true;
    } else if( sll_inst || slli_inst ){
        alu_ctrl_var = 6;
        signed_op_var = true;
    } else if ( srl_inst || srli_inst){
        alu_ctrl_var = 7;
        signed_op_var = true;
    } else if ( sra_inst || srai_inst ){
        alu_ctrl_var = 8;
        signed_op_var = false;
    } else if ( slt_inst || slti_inst){
        alu_ctrl_var = 9;
        signed_op_var = true;
    } else if ( sltu_inst || sltiu_inst){
        alu_ctrl_var = 10;
        signed_op_var = true;
    } else
        alu_ctrl_var = 0;


}
else if (s_type_inst){

  branch_var   = false;
  jump_var     = false;
  illegal_inst = false;
  reg_wr_en_var= false;
  result_src_var= 0;

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
  illegal_inst = false;
  mem_data_var = 0;
  mem_wr_en    = false;
  mem_size     = 0;
  alu_ctrl_var     = 0;
  reg_wr_en    = false;
  result_src_var = 0;
  

    id2exe_op1_var = rdata1.read();
    id2exe_op2_var = rdata2.read();

    
    if(if_inst[31]){
        branch_offset_var.range(31,13) = 0xFFFFF; 
    }
    else{
        branch_offset_var.range(31,13) = 0;
    }

    branch_offset_var[12] = if_inst[31]; 
    branch_offset_var[11] = if_inst[7];
    branch_offset_var.range(10,5) = if_inst.range(30,25);
    branch_offset_var.range(4,1 ) = if_inst.range(11,8 );
    branch_offset_var[0]  = 0;

    // BEQ
    if (beq_inst.read()) {
        jump_var = (id2exe_op1_var == id2exe_op2_var);
    }
    
    // BNE
    else if (bne_inst.read()) {
        jump_var = (id2exe_op1_var != id2exe_op2_var);
    }
    
    // BLT signed
    else if (blt_inst.read()) {
        if (id2exe_op1_var[31] ^ id2exe_op2_var[31]) {
            jump_var = id2exe_op1_var[31];
        } else {
            jump_var = (id2exe_op1_var < id2exe_op2_var);
        }
    }
    
    // BGE signed
    else if (bge_inst.read()) {
        if (id2exe_op1_var[31] ^ id2exe_op2_var[31]) {
            jump_var = !id2exe_op1_var[31];
        } else {
            jump_var = (id2exe_op1_var >= id2exe_op2_var);
        }
    }
    
    // BLTU unsigned
    else if (bltu_inst.read()) {
        jump_var = (id2exe_op1_var < id2exe_op2_var);
    }
    
    // BGEU unsigned
    else if (bgeu_inst.read()) {
        jump_var = (id2exe_op1_var >= id2exe_op2_var);
    }
    else {
        jump_var = false;
    }


  
  }
  
  
  else if( j_type_inst || jalr_type_inst ){
    branch_var   = false;
	  illegal_inst = false;
    mem_data_var = 0;
    mem_wr_en    = false;
    mem_size     = 0;
    alu_ctrl_var  = 0;
    signed_op_var = true;
    jump_var     = true;
    reg_wr_en     = true;
    result_src_var= 2;


    if(j_type_inst){
        id2exe_op1_var = if2id_pc_curr.read();
        id2exe_op2_var = 0;
        // sign extension of branch
        if (if_inst.range(31, 31) == 1) {
            branch_offset_var.range(31, 21) = 0b11111111111;
        }
        else {
            branch_offset_var.range(31, 21) = 0;
        }

        branch_offset_var.range(20, 20) = if_inst.range(31, 31);
        branch_offset_var.range(19, 12) = if_inst.range(19, 12);
        branch_offset_var.range(11, 11) = if_inst.range(20, 20);
        branch_offset_var.range(10, 1)  = if_inst.range(30, 21);
        branch_offset_var.range(0, 0)   = 0;

    }
    else if(jalr_type_inst){
        id2exe_op1_var = if2id_pc_curr.read();
        id2exe_op2_var = 0;
        // sign extension of branch
        if (if_inst.range(31, 31) == 1) {
            branch_offset_var.range(31, 12) = 0xFFFFF;
        }
        else {
            branch_offset_var.range(31, 12) = 0;
        }

        branch_offset_var.range(11,0) = if_inst.range(31, 20);
        branch_offset_var = branch_offset_var + id2exe_op1_var + rdata1.read();
    }
	
	}
	else if( fence_type_inst) {
    branch_var   = false;
	  illegal_inst = false;
    mem_data_var = 0;
    mem_wr_en    = false;
    mem_size     = 0;
    alu_ctrl_var = 0;
    signed_op_var= true;
    jump_var     = false;
    reg_wr_en    = false;
    result_src_var= 0;
    branch_offset_var = 0;
    id2exe_op1_var = 0;
    id2exe_op2_var = 0;


	}
	else{
	  branch_var   = false;
	  illegal_inst = false;
    mem_data_var = 0;
    mem_wr_en    = false;
    mem_size     = 0;
    alu_ctrl_var = 0;
    signed_op_var= true;
    jump_var     = false;
    reg_wr_en    = false; 
    result_src_var= 0;
    branch_offset_var = 0;
    id2exe_op1_var = 0;
    id2exe_op2_var = 0;
	}

    if (r_type_inst){ 
        if (if_inst.range(31, 25) != 0 && if_inst.range(31, 25) != 0b0100000)  
          illegal_inst = true;} 
    else if (b_type_inst){ 
        if (if_inst.range(14, 12) == 2 || if_inst.range(14, 12) == 3) 
          illegal_inst = true;}
    else if (s_type_inst){ 
        if (if_inst.range(14, 12) > 2) 
          illegal_inst = true;}
    

    branch.write(branch_var);
    mem_data.write(mem_data_var); 
    alu_ctrl.write(alu_ctrl_var); 
    signed_op.write(signed_op_var);
    jump.write(jump_var);
    reg_wr_en.write(reg_wr_en_var);
    result_src.write(result_src_var);
    branch_offset.write(branch_offset_var);
    id2exe_op1_data.write(id2exe_op1_var); 
    id2exe_op2_data.write(id2exe_op2_var); 
	
    

}



void decode::trace(sc_trace_file *tf){

    // Clock and reset
    sc_trace(tf, clk, "clk");
    sc_trace(tf, arstn, "arstn");

    // Register file interface
    sc_trace(tf, raddr1, "raddr1");
    sc_trace(tf, raddr2, "raddr2");
    sc_trace(tf, rdata1, "rdata1");
    sc_trace(tf, rdata2, "rdata2");

    // Fetch stage interface
    sc_trace(tf, if2id_pc_curr, "if2id_pc_curr");
    sc_trace(tf, if2id_pc_next, "if2id_pc_next");
    sc_trace(tf, if2id_inst, "if2id_inst");
    sc_trace(tf, out_if2id_valid, "out_if2id_valid");
    sc_trace(tf, out_if2id_ready, "out_if2id_ready");

    // Execute stage interface outputs
    sc_trace(tf, id2exe_op1_data, "id2exe_op1_data");
    sc_trace(tf, id2exe_op2_data, "id2exe_op2_data");
    sc_trace(tf, id2exe_reg_wr_en, "id2exe_reg_wr_en");
    sc_trace(tf, id2exe_result_src, "id2exe_result_src");
    sc_trace(tf, id2exe_mem_wr_en, "id2exe_mem_wr_en");
    sc_trace(tf, id2exe_mem_size, "id2exe_mem_size");
    sc_trace(tf, id2exe_mem_data, "id2exe_mem_data");
    sc_trace(tf, id2exe_jump, "id2exe_jump");
    sc_trace(tf, id2exe_branch, "id2exe_branch");
    sc_trace(tf, id2exe_branch_offset, "id2exe_branch_offset");
    sc_trace(tf, id2exe_alu_ctrl, "id2exe_alu_ctrl");
    sc_trace(tf, id2exe_pc_next, "id2exe_pc_next");
    sc_trace(tf, id2exe_reg_src1, "id2exe_reg_src1");
    sc_trace(tf, id2exe_reg_src2, "id2exe_reg_src2");
    sc_trace(tf, id2exe_reg_dst, "id2exe_reg_dst");
    sc_trace(tf, out_id2exe_valid, "out_id2exe_valid");
    sc_trace(tf, out_id2exe_ready, "out_id2exe_ready");
    sc_trace(tf, hazard_unit_flush_exe, "hazard_unit_flush_exe");

    // Internal control signals
    sc_trace(tf, reg_src1, "reg_src1");
    sc_trace(tf, reg_src2, "reg_src2");
    sc_trace(tf, reg_dest, "reg_dest");
    sc_trace(tf, op_type, "op_type");
    sc_trace(tf, funct3, "funct3");
    sc_trace(tf, funct7, "funct7");
    sc_trace(tf, reg_wr_en, "reg_wr_en");
    sc_trace(tf, result_src, "result_src");
    sc_trace(tf, mem_wr_en, "mem_wr_en");
    sc_trace(tf, mem_size, "mem_size");
    sc_trace(tf, mem_data, "mem_data");
    sc_trace(tf, jump, "jump");
    sc_trace(tf, branch, "branch");
    sc_trace(tf, branch_offset, "branch_offset");
    sc_trace(tf, alu_ctrl, "alu_ctrl");
    sc_trace(tf, signed_op, "signed_op");

    // Pipeline signals
    sc_trace(tf, in_id2exe_data, "in_id2exe_data");
    sc_trace(tf, in_id2exe_valid, "in_id2exe_valid");
    sc_trace(tf, in_id2exe_ready, "in_id2exe_ready");
    sc_trace(tf, out_id2exe_data, "out_id2exe_data");
    sc_trace(tf, id2exe_full, "id2exe_full");
    sc_trace(tf, id2exe_empty, "id2exe_empty");

    // Instruction format type signals
    sc_trace(tf, r_type_inst, "r_type_inst");
    sc_trace(tf, i_type_inst, "i_type_inst");
    sc_trace(tf, s_type_inst, "s_type_inst");
    sc_trace(tf, b_type_inst, "b_type_inst");
    sc_trace(tf, u_type_inst, "u_type_inst");
    sc_trace(tf, j_type_inst, "j_type_inst");
    sc_trace(tf, jalr_type_inst, "jalr_type_inst");
    sc_trace(tf, fence_type_inst, "fence_type_inst");

    // R-type instruction signals
    sc_trace(tf, add_inst, "add_inst");
    sc_trace(tf, slt_inst, "slt_inst");
    sc_trace(tf, sltu_inst, "sltu_inst");
    sc_trace(tf, and_inst, "and_inst");
    sc_trace(tf, or_inst, "or_inst");
    sc_trace(tf, xor_inst, "xor_inst");
    sc_trace(tf, sll_inst, "sll_inst");
    sc_trace(tf, srl_inst, "srl_inst");
    sc_trace(tf, sub_inst, "sub_inst");
    sc_trace(tf, sra_inst, "sra_inst");

    // I-type instruction signals
    sc_trace(tf, addi_inst, "addi_inst");
    sc_trace(tf, andi_inst, "andi_inst");
    sc_trace(tf, ori_inst, "ori_inst");
    sc_trace(tf, xori_inst, "xori_inst");
    sc_trace(tf, jalr_inst, "jalr_inst");
    sc_trace(tf, fence_inst, "fence_inst");

    // I-type shift instruction signals
    sc_trace(tf, sltiu_inst, "sltiu_inst");
    sc_trace(tf, slti_inst, "slti_inst");
    sc_trace(tf, slli_inst, "slli_inst");
    sc_trace(tf, srli_inst, "srli_inst");
    sc_trace(tf, srai_inst, "srai_inst");

    // I-type load instruction signals
    sc_trace(tf, lw_inst, "lw_inst");
    sc_trace(tf, lh_inst, "lh_inst");
    sc_trace(tf, lhu_inst, "lhu_inst");
    sc_trace(tf, lb_inst, "lb_inst");
    sc_trace(tf, lbu_inst, "lbu_inst");

    // B-type instruction signals
    sc_trace(tf, beq_inst, "beq_inst");
    sc_trace(tf, bne_inst, "bne_inst");
    sc_trace(tf, blt_inst, "blt_inst");
    sc_trace(tf, bge_inst, "bge_inst");
    sc_trace(tf, bltu_inst, "bltu_inst");
    sc_trace(tf, bgeu_inst, "bgeu_inst");

    // U-type instruction signals
    sc_trace(tf, lui_inst, "lui_inst");
    sc_trace(tf, auipc_inst, "auipc_inst");

    // J-type instruction signals
    sc_trace(tf, jal_inst, "jal_inst");

    // S-type instruction signals
    sc_trace(tf, sw_inst, "sw_inst");
    sc_trace(tf, sh_inst, "sh_inst");
    sc_trace(tf, sb_inst, "sb_inst");

    // FIFO trace
    id2exe.trace(tf);
}



