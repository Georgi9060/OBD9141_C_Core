// This example shows the reading of diagnostic trouble codes (DTC) functionality. The KWP fast init is used 
// because this is the only handshake for which I have a car to test with (Opel Corsa C).
// The example is based on the "readDTC" example in the original library's repository.
// Copy this example's contents to main.c to use it.

/*
  This example shows how to use the library to read the diagnostic trouble codes
  from the ECU and print these in a human readable format to the serial port.

  Huge thanks goes out to to https://github.com/produmann for helping with
  development of this feature and the extensive testing on a real car.
*/

#include "obd9141.h"

void app_main(void)
{
    OBD9141_begin();
    printf("Initialisation...\n");
    OBD9141_delay(1000);
    while(1){
        // Trouble code consists of a letter and then four digits, we write this
        // human readable ascii string into the dtc_buf which we then write to the
        // serial port.
        uint8_t dtc_buf[5];

        bool init_success = OBD9141_init_kwp();
        printf("init_success: %d\n", init_success);
        OBD9141_delay(50);
        if(init_success){
            uint8_t res;
            while(1){
                // res will hold the number of trouble codes that were received.
                // If no diagnostic trouble codes were retrieved it will be zero.
                // The ECU may return trouble codes which decode to P0000, this is
                // not a real trouble code but instead used to indicate the end of
                // the trouble code list.
                res = OBD9141_read_trouble_codes();
                if (res){
                    printf("Read: %d codes:\n", res);
                    for (uint8_t index = 0; index < res; index++){
                    // retrieve the trouble code in its raw two byte value.
                    uint16_t trouble_code = OBD9141_get_trouble_code(index);

                    // If it is equal to zero, it is not a real trouble code
                    // but the ECU returned it, print an explanation.
                    if (trouble_code == 0){
                        printf("P0000 (reached end of trouble codes)\n");
                    }
                    else{
                        // convert the DTC bytes from the buffer into readable string
                        OBD9141_decode_dtc(trouble_code, dtc_buf);
                        // Print the 5 readable ascii strings to the serial port.
                        printf("%s\n", dtc_buf);
                    }
                    }
                }
                else{printf("No trouble codes retrieved.\n");}

                OBD9141_delay(200);
            }
        }
        OBD9141_delay(3000); // Wait before retrying connection
    }
}
