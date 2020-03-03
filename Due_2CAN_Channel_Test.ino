#include "DueCANLayer.h"

// CAN Layer functions
extern byte canInit(byte cPort, long lBaudRate);
extern byte canTx(byte cPort, long lMsgID, bool bExtendedFormat, byte* cData, byte cDataLen);
extern byte canRx(byte cPort, long* lMsgID, bool* bExtendedFormat, byte* cData, byte* cDataLen);

void setup()
{
  // Set the serial interface baud rate
  SerialUSB.begin(115200);
}// end setup

void loop()
{
  // Declarations
  byte cTxData0[] = {};
  byte cTxData1[] = {0x0F, 0x0E, 0x0D, 0x0C, 0x0C, 0x0B, 0x0A, 0x08};
  int nTimer0 = 0;
  int nTimer1 = 0;
  bool initialized = false;
  
  while(1)  // Endless loop
  {
    delay(1);

    if (!initialized) {
      if(canInit(0, CAN_BPS_125K) == CAN_OK) {
        SerialUSB.print("CAN0: Initialized Successfully.\n\r"); 
        initialized = true;
      }
      else {
        SerialUSB.print("CAN0: Initialization Failed.\n\r"); 
      }
      continue;
    }
    
    // Send the messages
    if(++nTimer0 == 1000)
    {
      if(canTx(0, 1823, false, cTxData0, 0) != CAN_OK)
        SerialUSB.print("CAN0: Error during data transmission.\n\r");
      else
        SerialUSB.println("Sent successfully");
  
      nTimer0 = 0;

    }// end if
    // Check for received message
    long lMsgID;
    bool bExtendedFormat;
    byte cRxData[8];
    byte cDataLen;
    if(canRx(0, &lMsgID, &bExtendedFormat, &cRxData[0], &cDataLen) == CAN_OK)
    {
      SerialUSB.print("CAN0: Rx - MsgID:");
      SerialUSB.print(lMsgID, HEX);
      SerialUSB.print(" Ext:");
      SerialUSB.print(bExtendedFormat);
      SerialUSB.print(" Len:");
      SerialUSB.print(cDataLen);
      SerialUSB.print(" Data:");

      for(byte cIndex = 0; cIndex < cDataLen; cIndex++)
      {
        SerialUSB.print(cRxData[cIndex], HEX);
        SerialUSB.print(" ");
      }// end for

      SerialUSB.print("\n\r");
      
    }// end if

    if(canRx(1, &lMsgID, &bExtendedFormat, &cRxData[0], &cDataLen) == CAN_OK)
    {
      SerialUSB.print("CAN1: Rx - MsgID:");
      SerialUSB.print(lMsgID, HEX);
      SerialUSB.print(" Ext:");
      SerialUSB.print(bExtendedFormat);
      SerialUSB.print(" Len:");
      SerialUSB.print(cDataLen);
      SerialUSB.print(" Data:");

      for(byte cIndex = 0; cIndex < cDataLen; cIndex++)
      {
        SerialUSB.print(cRxData[cIndex], HEX);
        SerialUSB.print(" ");
      }// end for

      SerialUSB.print("\n\r");
      
    }// end if

  }// end while

}// end loop
