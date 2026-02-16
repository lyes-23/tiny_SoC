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
  sc_in<sc_bv<32>>  imem_resp_instr;
	sc_out<bool> 	    imem_resp_ready;
   
//=================================================================================
//     Interface with Decoding stage      
//=================================================================================
  sc_out<sc_bv<32>>	out_if2id_pc_curr; 
	sc_out<bool>		  out_if2id_valid;
	sc_in<bool>		    out_if2id_ready;
	sc_out<sc_bv<32>>	out_if2id_instr;
  sc_out<sc_bv<32>> out_if2id_pc_next;


	sc_in<bool>		    flush_d; 
	sc_in<bool>		    pc_src;
	sc_in<sc_bv<32>> 	pc_branching_target;

	sc_in<bool>		    stall_f;
	sc_in<bool>		    stall_d;

	void fetch_update();
  void fifo_concat();
  void fifo_unconcat();
  void fifo_handling();
  void imem_request();
	void trace(sc_trace_file *tf);

	fifo<IF2ID_SIZE,1> fifo_element;

	sc_signal<bool>			          in_if2id_ready;
	sc_signal<bool>			          in_if2id_valid;
	
	sc_signal<bool>               if2id_empty;
	sc_signal<bool>              	if2id_full;

	sc_signal<sc_bv<IF2ID_SIZE>>	if2id_in;
	sc_signal<sc_bv<IF2ID_SIZE>>	if2id_out; 

	sc_signal<sc_bv<32>>		      pc_reg;

	
	SC_CTOR(fetch) : fifo_element("if2id"){
		
		fifo_element.clk(clk);
		fifo_element.arstn(arstn  );

		fifo_element.in_ready_o	(in_if2id_ready);
		fifo_element.in_valid_i	(in_if2id_valid);
		fifo_element.in_data_i	(if2id_in);

		fifo_element.out_ready_i(out_if2id_ready);
		fifo_element.out_valid_o(out_if2id_valid);
		fifo_element.out_data_o	( if2id_out);

		fifo_element.full_o	(if2id_full);
		fifo_element.empty_o	(if2id_empty);

		SC_METHOD(fetch_update);
		sensitive << clk.pos() << arstn << imem_resp_instr << out_if2id_ready  << stall_f << stall_d << flush_d << pc_src ;
		
		SC_METHOD(fifo_concat);
		sensitive << imem_resp_instr;
		
		SC_METHOD(fifo_unconcat);
		sensitive << if2id_out;
		
		SC_METHOD(fifo_handling);
		sensitive << if2id_full << stall_d << flush_d;
		
		SC_METHOD(imem_request);
		sensitive << in_if2id_ready << stall_f;
    }
};

