#include <mcp2515.h>


struct can_frame canMsg1;

MCP2515 mcp2515(10);
int data[2];
void setup() {
  Serial.begin(9600); // Initialize serial communication

  mcp2515.reset();
  mcp2515.setBitrate(CAN_125KBPS);
  mcp2515.setNormalMode();
  
  Serial.println("Example: Write to CAN");
}

void loop() {
 
  data[0] = 0x6; // least significant 8 bit 
  data[1] = 0x3; // Most significan 8 bit

  canMsg1.can_id  = 0x048;
  canMsg1.can_dlc = 2;
  canMsg1.data[0] = data[0];
  canMsg1.data[1] = data[1];
  mcp2515.sendMessage(&canMsg1);
  Serial.println("Messages sent");
}

