SC_MODULE(Adder) {
    sc_in< sc_uint<8> > a, b;
    sc_out< sc_uint<8> > c;
    sc_in_clk     clock;        // Clock input of the design
    sc_in<bool>   reset;        // active high, synchronous Reset input
    sc_in<bool>   enable;       // Active high enable signal for counter

    void do_add() {
        if(enable.read()){
            c.write(a.read() + b.read());
        }
    }

    SC_CTOR(Adder) {
        SC_METHOD(do_add);
        sensitive << reset;
        sensitive << clock.pos();
    }
};