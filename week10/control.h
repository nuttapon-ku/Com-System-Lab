SC_MODULE(Control) {
    sc_in< sc_uint<8> > N, i;
    sc_in_clk clock;
    sc_out<bool> enable,outready;
    sc_in<bool> start;                  // Active high enable signal for control

    void do_add() {
        if(start.read()){
            if(i.read() < N.read() -1) {
                enable.write(1);
            }
            else {
                enable.write(0);
                outready.write(1);
            }
        }
    }

    SC_CTOR(Control) {
        SC_METHOD(do_add);
        sensitive << start;
        sensitive << clock.pos();
    }
};