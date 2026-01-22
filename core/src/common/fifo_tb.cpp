#include <systemc.h>
#include <iostream>
#include <cmath>
#include <cstdint>
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

    sc_signal<bool> consumer_done;
    sc_signal<bool> producer_done;
    sc_signal<bool> done_checking;
    // Variables to able to checkout the producer and the consumer 
    	static constexpr int test_size  = pow(2,depth);
    	static constexpr int array_size = test_size ;

	int testing_sequence[array_size] = {0};
	int received_sequence[array_size] = {0};
	int scoreboard_score[array_size] = {0};

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


	producer_done.write(false);
	consumer_done.write(false);
	done_checking.write(false);

        // register the producer process as a thread
        SC_THREAD(producer);
        sensitive << clk.posedge_event();

        // register the consumer process as a thread
        SC_THREAD(consumer);
        sensitive << clk.posedge_event();

	SC_METHOD(scoreboard);
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
        while(!arstn.read() ) wait();
	if( !producer_done.read()){
        for(uint64_t i{0}; i < 1ULL << DEPTH; i++ ){
            do{wait();}
            while(!in_ready_o.read());
            in_valid_i.write(true);
	    testing_sequence[i] = rand() % int(pow(2,WIDTH));
            in_data_i.write(testing_sequence[i]);
            wait();
            in_valid_i.write(false);
        }
	producer_done.write(true);
	}

    }

    void consumer(){

    wait();  
    while (!arstn.read() ) wait();
    if(  !consumer_done.read()){
    for(uint64_t i{0}; i < 1ULL << DEPTH; i++) {
        out_ready_i.write(true);
	do {  wait(); } while (!out_valid_o.read());
	received_sequence [i] = out_data_o.read().to_int(); 
	out_ready_i.write(false);
        }
    consumer_done.write(true);
    }
    }
    
    void scoreboard(){

    if (producer_done.read() && consumer_done.read() && !done_checking.read()){
    bool all_match = true;
    for (uint64_t i = 0; i < 1ULL<<DEPTH ; i++) {
        if (received_sequence[i] != testing_sequence[i]) {
            std::cerr << "Mismatch at index " << i << ": Expected " 
                      << testing_sequence[i] << " but received " 
                      << received_sequence[i] << std::endl;
            all_match = false;
        }
    }

    if (all_match) {
        std::cout << "Scoreboard check passed" << std::endl;
    } else {
        std::cerr << "Scoreboard check failed" << std::endl;
    }
    done_checking.write(true);
    sc_stop();
}

	}
};

	


int sc_main(int argc, char* argv[])
{
    fifo_tb<32,8> tb("my_tb");

    sc_trace_file *tf= sc_create_vcd_trace_file("fifo_wave");

    tb.fifo_model.trace(tf);

    //sc_start(10, SC_US);
	sc_start();
    sc_close_vcd_trace_file(tf);
    return 0;
}
