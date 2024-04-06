
#include <mcp2515.h>

struct can_frame canMsg;
MCP2515 mcp2515(10);

int16_t sensor_value;
int16_t sensor_value_2;

void setup() {
  Serial.begin(9600);
  
  mcp2515.reset();
  mcp2515.setBitrate(CAN_125KBPS);
  mcp2515.setNormalMode();
  
  Serial.println("------- CAN Read ----------");
}

void loop() {
  while (!(mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK)) {
    float distance;
    if(canMsg.can_id == 0x048 ) {
      sensor_value  = word(canMsg.data[1], canMsg.data[0]);
      Serial.print(" Distance: ");
      Serial.println(sensor_value);
    }
  }
}

