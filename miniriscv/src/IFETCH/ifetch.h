#include <systemc.h>
#include "fifo_element.h"
#include "miniriscv_def.h"

SC_MODULE(fetch) {

	sc_in_clk 	clk;
	sc_in<bool>	arstn;

//================================================================================
//    IMEM Interface
//===============================================================================

	sc_out<sc_bv<32>> imem_req_addr;
    	sc_out<bool>      imem_req_valid;
	sc_in<bool>       imem_req_ready;
    
    	sc_out<sc_bv<32>> imem_resp_instr;
    	sc_out<bool>      imem_resp_valid;
    	sc_in<bool>       imem_resp_ready;
    	sc_out<bool>      imem_resp_error;
    
//=================================================================================
//     Interface with Decoding stage      
//=================================================================================
	// branch or jump detected 
    	sc_in<sc_bv<32>>	id2if_pc_curr; 
	sc_in<bool>		id2if_pc_valid;
	sc_out<bool>		id2if_pc_ready;
	
	//
	sc_out<sc_bv<32>>	if2id_instr;
    	sc_out<sc_bv<32>>  	if2id_pc_next;
	sc_in<bool>		if2id_ready;
	sc_out<bool>		if2id_valid;
	sc_in<bool>		if2id_flush; // clear the pipeline in case of branching

	void trace(sc_trace_file *tf);
	void fetching();

	fifo<IF2ID,1> fifo_element;

	
	sc_signal<bool>               	 if2id_empty;
	sc_signal<bool>              	 if2id_full;

	sc_signal<sc_bv<IF2ID_SIZE>>	 if2id_in;
	sc_signal<sc_bv<IF2ID_SIZE>>	 if2id_out;  // instruction sent to if2dec

	
	SC_CTOR(fetch) : fifo_element("if2id"){
		
		fifo_element.clk(clk);
		fifo_element.arstn(arstn);

		fifo_element.in_ready_o(if2id_ready);
		fifo_element.in_valid_i(if2id_valid);
		fifo_element.in_data_i( if2id_in);

		fifo_element.out_ready_i(id2if_ready);
		fifo_element.out_valid_o(id2if_valid);
		fifo_element.out_data_o( if2id_out);

		fifo_element.full_o(if2id_full);
		fifo_element.empty_o(if2id_empty);

		SC_METHOD(fetching);
		sensitive << arstn <<  
		
    }

	void fetching(){
		// IF2ID pipeline input
		sc_bv<IF2ID_SIZE> if2id_concat;
		if2id_concat.range(63,32) = 	(sc_bv_base)if2id_instr.read(); 
		if2id_concat.range(31,0)  = 	(sc_bv_vase)if2id_pc_next.read();
		if2dec_in.write(if2id_concat);

		// IF2ID pipline output
		sc_bv<IF2ID_SIZE> if2id_unconcat= if2id_out;
		if2id_instr.write(if2id_unconcat.range(63,32));
		if2id_next_pc.write(if2if_unconcat.range(31,0));

		// Requesting the instruction from IMEM
		if(imem_req_ready.ready()){
			imem_req_valid.write(true);
			imem_req_data.write(if2id_pc_curr);
		}
		else {
			imem_req_valid.write(false);
		}


		if(if2id_flush.read()){
			imem_req_valid.write(false);
			id2if_ready.write(true);
			if2id_valid.write(false);
		} else {
			bool stall =  if2id_full || imem_resp_error || id2if_pc_ready; 
			if2id_valid.	
		}


		
	}


	void trace(){
	
	
	}

	
};
