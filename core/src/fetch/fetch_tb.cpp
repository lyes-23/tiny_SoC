#include <systemc.h>
#include "ifetch.h"


int sc_main(int argc, char* argv[]) {

    sc_clock clk("clk", 10, SC_NS);
    sc_signal<bool> arstn;


        // IMEM interface signals
        sc_signal<sc_bv<32>> imem_req_addr;
        sc_signal<bool>      imem_req_valid;
        sc_signal<sc_bv<32>> imem_resp_instr;
        sc_signal<bool>      imem_resp_ready;
        sc_signal<bool>      imem_resp_error;

        // IF->ID interface signals
        sc_signal<sc_bv<32>> out_if2id_pc_curr;
        sc_signal<bool>      out_if2id_valid;
        sc_signal<bool>      out_if2id_ready;
        sc_signal<sc_bv<32>> out_if2id_instr;
        sc_signal<sc_bv<32>> out_if2id_pc_next;

        // hazard/control signals
        sc_signal<bool> hazard_unit_flush_d;
        sc_signal<bool> pc_branching;
        sc_signal<sc_bv<32>> pc_branching_target;
        sc_signal<bool> hazard_unit_stall_f;
        sc_signal<bool> hazard_unit_stall_d;

        fetch dut("DUT");

        dut.clk(clk);
        dut.arstn(arstn);

        // IMEM bindings
        dut.imem_req_addr(imem_req_addr);
        dut.imem_req_valid(imem_req_valid);
        dut.imem_resp_instr(imem_resp_instr);
        dut.imem_resp_ready(imem_resp_ready);
        dut.imem_resp_error(imem_resp_error);

        // IF->ID bindings
        dut.out_if2id_pc_curr(out_if2id_pc_curr);
        dut.out_if2id_valid(out_if2id_valid);
        dut.out_if2id_ready(out_if2id_ready);
        dut.out_if2id_instr(out_if2id_instr);
        dut.out_if2id_pc_next(out_if2id_pc_next);

        // hazard/control bindings
        dut.hazard_unit_flush_d(hazard_unit_flush_d);
        dut.pc_branching(pc_branching);
        dut.pc_branching_target(pc_branching_target);
        dut.hazard_unit_stall_f(hazard_unit_stall_f);
        dut.hazard_unit_stall_d(hazard_unit_stall_d);

        // simple reset pulse to complete elaboration
        arstn = false;
        sc_start(1, SC_NS);
        arstn = true;

        cout << "Elaboration done correctly" << endl;

        return 0;
}

