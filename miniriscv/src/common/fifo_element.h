#pragma once
#include <systemc.h>



template <int WIDTH, int DEPTH = 8>
SC_MODULE(fifo) {

    static_assert(WIDTH > 0, "WIDTH must be > 0");
    static_assert(DEPTH > 0, "DEPTH must be > 0");

    sc_in_clk   clk;
    sc_in<bool> arstn; // active-low ascync reset

    // fifo input
    sc_out<bool>            in_ready_o;    
    sc_in<bool>             in_valid_i;
    sc_in<sc_bv<WIDTH>>     in_data_i;

    // fifo output
    sc_in<bool>         out_ready_i;
    sc_out<bool>        out_valid_o;
    sc_out<sc_bv<WIDTH>>    out_data_o;

    sc_out<bool>        empty_o;
    sc_out<bool>        full_o;
     
    // internal storage 
    sc_bv<WIDTH> data_s[DEPTH];
    unsigned int  read_ptr  = 0;
    unsigned int  write_ptr = 0;
    unsigned int  count   = 0;


    void clock_set();
    void trace(sc_trace_file *tf) const;

    SC_CTOR(fifo){
        SC_CTHREAD(clock_set, fifo::clk.pos());

        sensitive << in_valid_i << out_ready_i;

        reset_signal_is(arstn, false);

        in_ready_o.initialize(true);
        out_valid_o.initialize(false);
        out_data_o.initialize(sc_bv<WIDTH>());
        empty_o.initialize(true);
        full_o.initialize(false);
    }

};

template<int WIDTH,int DEPTH>
void fifo<WIDTH, DEPTH>::clock_set(){

    in_ready_o.initialize(true);
    out_valid_o.initialize(false);
    out_data_o.initialize(sc_bv<WIDTH>());
    empty_o.initialize(true);
    full_o.initialize(false);
    
    write_ptr=0;
    read_ptr=0;
    count=0;


    wait();
    
    while(true){
        bool in_valid = in_valid_i.read();
        bool out_ready = out_ready_i.read();

        bool can_push = (count < (unsigned)DEPTH);
        bool can_pop  = (count > 0);

        bool do_push =  in_valid && can_push;
        bool do_pop  =  out_ready && can_pop;

        // Handling push & pop
        if(do_push && !do_pop){
            data_s[write_ptr] = in_data_i.read();
            write_ptr = (write_ptr + 1) % DEPTH;
            ++count;
        } else if(!do_push && do_pop){
            read_ptr = (read_ptr + 1) % DEPTH;
            --count;
        } else if( do_push && do_pop){
            data_s[read_ptr] = in_data_i.read();
            write_ptr = (write_ptr + 1) % DEPTH;
            read_ptr  = (read_ptr  + 1) % DEPTH; 

        }

        if(count > 0){
            out_data_o.write(data_s[read_ptr]);
        } else {
            out_data_o.write(sc_bv<WIDTH>());
        }

        empty_o.write(count == 0);
        full_o.write(count == DEPTH);
        in_ready_o.write(count < DEPTH);
        out_valid_o.write(count > 0);

        wait();
    }
}




template<int WIDTH, int DEPTH>
void fifo<WIDTH, DEPTH>::trace(sc_trace_file *tf) const {
    sc_trace(tf, clk, std::string(name()) + ".clk");
    sc_trace(tf, arstn, std::string(name()) + ".arstn");
    sc_trace(tf, in_ready_o, std::string(name()) + ".in_ready");
    sc_trace(tf, in_valid_i, std::string(name()) + ".in_valid");
    sc_trace(tf, in_data_i,  std::string(name()) + ".in_data");
    sc_trace(tf, out_ready_i, std::string(name()) + ".out_ready");
    sc_trace(tf, out_valid_o, std::string(name()) + ".out_valid");
    sc_trace(tf, empty_o, std::string(name()) + ".empty");
    sc_trace(tf, full_o, std::string(name()) + ".full");
    // data tracing: trace head element and a few entries
    sc_trace(tf, out_data_o, std::string(name()) + ".out_data");
}

