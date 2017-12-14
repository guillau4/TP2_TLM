#include "ensitlm.h"
#include "bus.h"
#include <systemc.h>


struct generator : sc_core::sc_module {
    ensitlm::initiator_socket<generator> initiator;
    sc_core::sc_in <bool> gen_int;
    void thread(void);
    void method(void);

    sc_core::sc_event start_int;

    SC_CTOR(generator);
};
