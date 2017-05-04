#include <systemc.h>
#include "adder.h"
#include "control.h"

int sc_main(int argc, char *argv[]) {
    if(argc < 2) {
        cout << "usage: ./main [N]" << endl;
        exit(0);
    }

    sc_set_time_resolution(1, SC_PS);
    sc_time             t1(1, SC_PS);
    //------------------------ADDER1--------------------------------
    Adder adder1("adder1");
    sc_signal< sc_uint<8> > a, b, c, i, N;
    sc_signal<bool>   clock;
    sc_signal<bool>   reset;
    sc_signal<bool>   enable, outready, start;

    adder1.a(c);
    adder1.b(i);
    adder1.c(c);
    adder1.clock(clock);
    adder1.reset(reset);
    adder1.enable(enable);

    sc_trace_file *wt = sc_create_vcd_trace_file("sumwave");
    sc_trace(wt, adder1.c, "c");
    sc_trace(wt, adder1.clock, "clock");
    sc_trace(wt, adder1.reset, "reset");
    sc_trace(wt, adder1.enable, "enable");

    a.write(1);
    c.write(0);
    i.write(0);

    //------------------------ADDER2--------------------------------
    Adder adder2("adder2");

    adder2.a(a);
    adder2.b(i);
    adder2.c(i);
    adder2.clock(clock);
    adder2.reset(reset);
    adder2.enable(enable);

    //---------------------Control------------------------------------
    Control control("control");

    sc_trace(wt, control.N, "N");
    sc_trace(wt, control.i, "i");
    sc_trace(wt, control.outready, "outready");
    sc_trace(wt, control.start, "start");

    control.N(N);
    control.i(i);
    control.outready(outready);
    control.clock(clock);
    control.enable(enable);
    control.start(start);

    //--------------------Clock---------------------------------------
    enable.write(1);
    start.write(1);
    N.write(argv[1]);
    for (;!outready.read();) {
        clock = 0;
        sc_start(t1);
        clock = 1;
        sc_start(t1);
    }
    cout << c.read() << endl;

    sc_start(t1);
    sc_stop();

    sc_close_vcd_trace_file(wt);
    return(0);
}