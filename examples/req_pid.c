// This example shows the PID request functionality. The KWP fast init is used 
// because this is the only handshake for which I have a car to test with (Opel Corsa C).
// The example is based on the "readerKWP" and "reader_esp32" examples in the original library's repository.
// Copy this example's contents to main.c to use it.

#include "obd9141.h"

#define INBETWEEN_DELAY_MS 1 // No delay required inbetween requests, can be used to reduce spam

void app_main(void)
{
    OBD9141_begin();
    printf("Initialisation...\n");
    OBD9141_delay(1000);
    while(1){
        bool init_success = OBD9141_init_kwp();
        printf("init_success: %d\n", init_success);
        OBD9141_delay(50);
        if(init_success){
            bool res;
            while(1){
                // Load [%]
                res = OBD9141_get_current_pid(0x04, 1);
                if(res){
                    printf("Load: %d [%%]\n", OBD9141_read_uint8() * 100 / 255);
                }
                OBD9141_delay(INBETWEEN_DELAY_MS);

                // Engine Coolant Temperature [°C]
                res = OBD9141_get_current_pid(0x05, 1);
                if (res){
                    printf("Coolant T: %d [°C]\n", OBD9141_read_uint8() - 40);
                }
                OBD9141_delay(INBETWEEN_DELAY_MS);

                // RPM
                res = OBD9141_get_current_pid(0x0C, 2);
                if (res){
                    printf("%d [RPM]\n", OBD9141_read_uint16() / 4);
                }
                OBD9141_delay(INBETWEEN_DELAY_MS);

                // Vehicle Speed [km/h]
                res = OBD9141_get_current_pid(0x0D, 1);
                if (res){
                    printf("Speed: %d [km/h]\n", OBD9141_read_uint8());
                }
                OBD9141_delay(INBETWEEN_DELAY_MS);

                // Intake Air Temperature [°C]
                res = OBD9141_get_current_pid(0x0F, 1);
                if (res){
                    printf("Intake air T: %d [°C]\n", OBD9141_read_uint8() - 40);
                }
                OBD9141_delay(INBETWEEN_DELAY_MS);

                // Mass Air Flow [g/s]
                res = OBD9141_get_current_pid(0x10, 2);
                if (res){
                    printf("MAF: %.2f [g/s]\n", OBD9141_read_uint16() / 100.0f);
                }
                OBD9141_delay(INBETWEEN_DELAY_MS);

                // Throttle [%]
                res = OBD9141_get_current_pid(0x11, 1);
                if (res){
                    printf("Throttle: %d [%%]\n", OBD9141_read_uint8() * 100 / 255);
                }
                OBD9141_delay(500);
            }
        }
        OBD9141_delay(3000); // Wait before retrying connection
    }
}