#include <systemc.h>
#include <iostream>
#include <cmath>
#include "fifo_element.h"

template <int WIDTH, int DEPTH >
SC_MODULE(fifo_tb){

	
   static constexpr int width = WIDTH ; 
   static constexpr int depth = DEPTH;
   fifo<width,depth> fifo_model;

    sc_signal<bool> clk;
    sc_signal<bool> arstn;

    sc_signal<bool> in_ready_o;
    sc_signal<bool> in_valid_i;
    sc_signal<sc_bv<width>> in_data_i;

    sc_signal<bool> out_ready_i;
    sc_signal<bool> out_valid_o;
    sc_signal<sc_bv<width>> out_data_o;

    sc_signal<bool> empty_o;
    sc_signal<bool> full_o;



    SC_CTOR(fifo_tb) : fifo_model("DUT") {
	    
	fifo_model.clk(clk);
        fifo_model.arstn(arstn);
        fifo_model.in_ready_o(in_ready_o);
        fifo_model.in_valid_i(in_valid_i);
        fifo_model.in_data_i(in_data_i);
        fifo_model.out_ready_i(out_ready_i);
        fifo_model.out_valid_o(out_valid_o);
        fifo_model.out_data_o(out_data_o);
        fifo_model.empty_o(empty_o);
        fifo_model.full_o(full_o);


        // register the producer process as a thread
        SC_THREAD(producer);
        sensitive << clk.posedge_event();

        // register the consumer process as a thread
        SC_THREAD(consumer);
        sensitive << clk.posedge_event();

        SC_THREAD(clock_gen);
        SC_THREAD(reset_dut);


         
    }

    void clock_gen(){
        while(true){
            clk.write(false);
            wait(5,SC_NS);
            clk.write(true);
            wait(5,SC_NS);
        }
    }

    void reset_dut(){
        arstn.write(false);
        wait(5, SC_NS); 
        arstn.write(true);
    }
    
    void producer(){
	srand(time(nullptr));
        wait();
        while(!arstn.read()) wait();
        while(true){
            do{wait();}
            while(!in_ready_o.read());
            in_valid_i.write(true);
            in_data_i.write(rand() % int(pow(32,2)));
            wait();
            in_valid_i.write(false);
        }

    }

    void consumer(){
    
    wait();  
    
    while (!arstn.read()) wait();
    while (true) {
        out_ready_i.write(true);
        if (out_valid_o.read()) {
            auto data = out_data_o.read();
            std::cout << "Read data: " << data << std::endl;
        }
        wait();
        }
    }
};


int sc_main(int argc, char* argv[])
{
    fifo_tb<32,8> tb("my_tb");

    sc_trace_file *tf= sc_create_vcd_trace_file("fifo_wave");

    tb.fifo_model.trace(tf);

    sc_start(1, SC_MS);

    sc_close_vcd_trace_file(tf);
    return 0;
}
