/*
    This is an example of sending and receiving on its own, connect two channels to each other using jumpers.
*/

#include <SPI.h>
#include "mcp2518fd_can.h"

/*SAMD core*/
#ifdef ARDUINO_SAMD_VARIANT_COMPLIANCE
    #define SERIAL SerialUSB
#else
    #define SERIAL Serial
#endif

// Set SPI CS Pin according to your hardware
// For Wio Terminal w/ MCP2518FD RPi Hat：
// Channel 0 SPI_CS Pin: BCM 8
// Channel 1 SPI_CS Pin: BCM 7
// Interupt Pin: BCM25
// *****************************************
// For Arduino MCP2515 Hat:
// SPI_CS Pin: D9

const int SPI_CS_PIN_SEND = 9;
const int SPI_CS_PIN_RECEIVE = 10;

mcp2518fd CAN_SEND(SPI_CS_PIN_SEND);
mcp2518fd CAN_RECEIVE(SPI_CS_PIN_RECEIVE);

unsigned char len = 0;
unsigned char buf[8];

void setup() {
    SERIAL.begin(115200);
    while(!Serial); // wait for Serial

    if (CAN_SEND.begin((byte)CAN_500K_1M) != 0 || CAN_RECEIVE.begin((byte)CAN_500K_1M) != 0) {
      Serial.println("CAN-BUS initiliased error!");
      while(1);
    }
    
    SERIAL.println("CAN BUS Shield init ok!");
}

unsigned char stmp[8] = {0, 0, 0, 0, 0, 0, 0, 0};
void loop() {
    // send data:  id = 0x00, standrad frame, data len = 8, stmp: data buf
    stmp[7] = stmp[7] + 1;
    if (stmp[7] == 100) {
        stmp[7] = 0;
        stmp[6] = stmp[6] + 1;

        if (stmp[6] == 100) {
            stmp[6] = 0;
            stmp[5] = stmp[6] + 1;
        }
    }

    CAN_SEND.sendMsgBuf(0x00, 0, 8, stmp);
    delay(100);                       // send data per 100ms
    SERIAL.println("CAN BUS sendMsgBuf ok!");

    // ---------------------
        
    if (CAN_MSGAVAIL == CAN_RECEIVE.checkReceive()) {
    // read data,  len: data length, buf: data buf
      SERIAL.println("checkReceive");
      CAN_RECEIVE.readMsgBuf(&len, buf);
    // print the data
    for (int i = 0; i < len; i++) {
        SERIAL.print(buf[i]); SERIAL.print(" ");
    }
    SERIAL.println();
    }
    SERIAL.println("---LOOP END---");
}

// END FILE
