#include "ensitlm.h"
#include "bus.h"
#include "generator.h"
#include "memory.h"
#include "LCDC.h"
#include "constants.h"
#include "ROM.h"
#include <systemc.h>

int sc_main(int argc, char **argv){
    (void)argc;
    (void)argv;
    sc_time period (1.0/25,SC_SEC);

    generator gen("Generator1");
    Memory mem("Memory", SIZE_MEM);
    Bus router("Bus");
    LCDC c("Controller", period);
    ROM r("Rom");
    sc_core::sc_signal < bool , SC_MANY_WRITERS > irq_signal ("IRQ");

    router.map(mem.target,START_ADD_MEM,SIZE_MEM);
    router.map(c.target_socket,START_ADD_LCDC,SIZE_LCDC);
    router.map(r.socket,START_ADD_ROM,ROM_SIZE);

    gen.initiator.bind(router.target);
    router.initiator.bind(mem.target);
    c.initiator_socket.bind(router.target);
    router.initiator.bind(c.target_socket);
    router.initiator.bind(r.socket);
    c.display_int(irq_signal);
    gen.gen_int(irq_signal);


    sc_core::sc_start();
    return 0;
}
