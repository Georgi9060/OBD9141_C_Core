// This example shows the supported PIDs functionality. The KWP fast init is used 
// because this is the only handshake for which I have a car to test with (Opel Corsa C).
// The example is based on the "supported_pids" in the original library's repository.
// Copy this example's contents to main.c to use it.

#include "obd9141.h"

// For more information: https://en.wikipedia.org/wiki/OBD-II_PIDs#Service_01_PID_00
bool OBD9141_print_supported_pid(uint32_t value, uint8_t offset) {
    const uint8_t* v = (const uint8_t*)&value;
    bool is_more = false;
    for (uint8_t i = 0; i < 4; i++)
    {
    const uint8_t b = v[3 - i];
    // printf("Now handling b: 0x%02X\n", b);
    for (int8_t j = 7; j >= 0; j--)
    {
        uint8_t pid = offset + (i + 1) * 8 - j;
        bool supported = b & (1 << j);
        printf("0x%02X: %d\n", pid, supported);
        is_more = supported;  // last read bit is always whether the next block is supported.
    }
    }
    return is_more;
}

void app_main(void)
{
    OBD9141_begin();
    printf("Initialisation...\n");
    OBD9141_delay(1000);
    while(1){
        bool init_success = OBD9141_init_kwp();
        printf("init_success: %d\n", init_success);
        OBD9141_delay(50);
        uint8_t offset = 0;
        if(init_success){
            bool is_more = true;
            while(is_more){
                bool res = OBD9141_get_current_pid(offset, 4);
                if(res){
                    uint32_t sup_block = OBD9141_read_uint32();
                    printf("PID 0x%02X  -> 0x%02X\n", offset, sup_block);
                    is_more = OBD9141_print_supported_pid(sup_block, offset);
                    if(is_more){offset += 0x20;}
                    else{offset = 0;}
                }
                OBD9141_delay(200);
            }
        }
        OBD9141_delay(3000); // Wait before retrying connection
    }
}
