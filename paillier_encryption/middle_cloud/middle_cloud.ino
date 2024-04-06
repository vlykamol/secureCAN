// reciver - sender
// recives two encrypted message from sender1(some value) and sender3(sensor value : distance)
// multipy two encrypted message and send to reciver1



#include <mcp2515.h>

struct can_frame canMsg;
struct can_frame canMsg1;
MCP2515 mcp2515(10);

void setup() {
  Serial.begin(9600);

  mcp2515.reset();
  mcp2515.setBitrate(CAN_125KBPS);
  mcp2515.setNormalMode();
}

long cipherText1;
long distance;

void loop() {
  
  while (!(mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK)) {
    if(canMsg.can_id == 0x046 ) {
      distance = (canMsg.data[1] << 8) | canMsg.data[0];
    }
    if(canMsg.can_id == 0x047 ) {
      cipherText1 = (canMsg.data[1] << 8) | canMsg.data[0];
    }
  }
  canMsg1.can_id = 0x048;
  canMsg1.can_dlc = 4;
  Serial.print("cipher text from sendr1 : ");
  Serial.println(cipherText1);
  Serial.print("cipher text from sendr3 (distance) : ");
  Serial.println(distance);

  Serial.print("multiply of cipher text : ");
  long cipherText = cipherText1 * distance;
  Serial.println(cipherText);

  canMsg1.data[0] = (cipherText >> 24) & 0xFF;
  canMsg1.data[1] = (cipherText >> 16) & 0xFF;
  canMsg1.data[2] = (cipherText >> 8) & 0xFF;
  canMsg1.data[3] = cipherText & 0xFF;
  mcp2515.sendMessage(&canMsg1);
}