#include "ifetch.h"

void fetch::imem_request(){
	// requesting the instruction from the IMEM
	if( in_if2id_ready.read() && !stall_f.read() ){
	    imem_req_addr.write(pc_reg);
	    imem_req_valid.write(true);
	    imem_resp_ready.write(true); 
	}
	else {
	    imem_req_valid.write(false);
	    imem_resp_ready.write(false); 
	}
}

void fetch::fifo_handling(){
	if( !if2id_full.read() && !stall_d.read() && !flush_d.read() && in_if2id_ready.read() ) {
    in_if2id_valid.write(true);
  } 
  else {
    in_if2id_valid.write(false);
  }
}

void fetch::fifo_concat(){
  sc_bv<IF2ID_SIZE> if2id_data;
  sc_uint<32>       pc_next = pc_reg.read().to_uint() + 4;
  if2id_data.range(31, 0)   = imem_resp_instr.read();
  if2id_data.range(63, 32)  = pc_reg.read();
  if2id_data.range(95, 64)  = pc_next;
  if2id_in.write(if2id_data);
}

void fetch::fifo_unconcat(){
	sc_bv<IF2ID_SIZE> if2id_unconcat = if2id_out.read();
	out_if2id_instr.write(if2id_unconcat.range(31, 0));
	out_if2id_pc_curr.write(if2id_unconcat.range(63,32));
	out_if2id_pc_next.write(if2id_unconcat.range(95,64));
}

void fetch::fetch_update(){
	if (!arstn.read()) {
    pc_reg.write(START_ADRESS);
  } else if (pc_src.read() == true) {
    pc_reg.write( pc_branching_target.read().to_uint());
  } else if ( if2id_full.read() || stall_f.read() ) {
    pc_reg.write(pc_reg.read().to_uint());
  } else {
    pc_reg.write(pc_reg.read().to_uint()+4) ;
  }
}	
	
void fetch::trace(sc_trace_file *tf){
    	sc_trace(tf, clk,             "clk");
    	sc_trace(tf, arstn,           "arstn");
    	sc_trace(tf, imem_req_addr,   "imem_req_addr");
    	sc_trace(tf, imem_req_valid,  "imem_req_valid");
    	sc_trace(tf, imem_resp_instr, "imem_resp_instr");
    	sc_trace(tf, imem_resp_ready, "imem_resp_ready");
    	sc_trace(tf, flush_d,         "flush_d");
    	sc_trace(tf, out_if2id_instr,     "out_if2id_instr");
    	sc_trace(tf, out_if2id_pc_next,   "out_if2id_pc_next");
    	sc_trace(tf, out_if2id_valid,     "out_if2id_valid");
    	sc_trace(tf, out_if2id_ready,     "out_if2id_ready");
	    sc_trace(tf, flush_d,             "flush_d");
	    sc_trace(tf, pc_src,              "pc_src");
	    sc_trace(tf, pc_branching_target, "pc_branching_target");
	    sc_trace(tf, stall_f,             "stall_f");
    	sc_trace(tf, stall_d,             "stall_d");
	    sc_trace(tf, if2id_in,            "if2id_in");
    	sc_trace(tf, if2id_out,           "if2id_out");
    	sc_trace(tf, if2id_full,          "if2id_full");
	    sc_trace(tf, if2id_empty,         "if2id_empty");
	    fifo_element.trace(tf);
} 
