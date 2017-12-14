#include "generator.h"
#include "LCDC_registermap.h"
#include "constants.h"
#include "ROM.h"



using namespace std;
using namespace sc_core;
void generator::thread(void){
    initiator.write(START_ADD_LCDC + LCDC_ADDR_REG, START_ADD_MEM_VID);
    initiator.write(START_ADD_LCDC + LCDC_START_REG, 0x00000001);
    initiator.write(START_ADD_LCDC + LCDC_INT_REG, 0x00000000);

    uint32_t mask;
    uint32_t mask_1;
    uint32_t mask_2;
    uint32_t data;
    uint32_t data_1;
    uint32_t data_2;
    uint32_t loop=0;
    while(true){
        loop = (loop + 1)%240;
        sc_time period(1.0/25, SC_SEC);
        wait(period);

        for(int i= 0; i< ROM_SIZE ; i=i+4){
            data_1=0x00000000;
            data_2=0x00000000;
            for(int j = 0; j< 4 ; j++){
                mask = 0xF0000000;
                mask_1 = mask>>4*j;
                mask_2 = mask>>(4*j+16);
                initiator.read(START_ADD_ROM + i, data);
                data_1 = data_1|((data & mask_1)>>j*4) ;
                data_2 = data_2|((data & mask_2)<<((4-j)*4));
            }
            initiator.write(START_ADD_MEM_VID + (i*2 + 320*loop)%76800,data_1);
            initiator.write(START_ADD_MEM_VID + (i*2+4 + 320*loop)%76800,data_2);
        }
        wait(start_int);
    }
}
void generator::method(void){
    start_int.notify();
    initiator.write(START_ADD_LCDC + LCDC_INT_REG, 0x00000000); 
    cout<<"Generator: interrupted"<<endl;
}

generator::generator(sc_core::sc_module_name name) : sc_core::sc_module(name){
    SC_THREAD(thread);
    SC_METHOD(method);
    sensitive<<gen_int;
}
