#include <mcp2515.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F, 16, 2);

struct can_frame canMsg;
MCP2515 mcp2515(10);

long n = 143;
long n_square = 20449;
long lamb = 120;
long mu = 87;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  lcd.init();
  lcd.clear();
  lcd.backlight();
  
  mcp2515.reset();
  mcp2515.setBitrate(CAN_125KBPS);
  mcp2515.setNormalMode();
}

void loop() {
  // put your main code here, to run repeatedly:
  lcd.setCursor(0, 0);
  Serial.println();
  long cipherText1 = 0;
  long cipherText2 = 0;
  long distance = 0;
  while (!(mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK)) {
    if(canMsg.can_id == 0x046 ) {
      distance = (canMsg.data[1] << 8) | canMsg.data[0];
    }
    if(canMsg.can_id == 0x047 ) {
      cipherText1 = (canMsg.data[1] << 8) | canMsg.data[0];
    }
    if(canMsg.can_id == 0x048 ) {
      cipherText2 = (canMsg.data[1] << 8) | canMsg.data[0];
    }
  }
  long decrypted1 = decrypt(cipherText1);
  Serial.print("decrypted message from sender1 : ");
  Serial.println(decrypted1);
  lcd.print(decrypted1);
  lcd.setCursor(0, 1);
  long decrypted2 = decrypt(cipherText2);
  Serial.print("decrypted message from sender2 : ");
  Serial.println(decrypted2);
  lcd.print(decrypted2);
  // delay(2000);
  lcd.setCursor(5, 0);
  long decryptedD = decrypt(distance);
  Serial.print("decrypted distance from sender3 : ");
  Serial.println(decryptedD);
  lcd.print(decryptedD);
}

long decrypt(long c){
  long d1 = power_mod(c, lamb, n_square) - 1;
  long d2 = d1 / n * mu;
  long d3 = d2 % n;
  return d3;
}

long power_mod(long base, long exp, long mod) {
  long result = 1;
  base = base % mod;
  while (exp > 0) {
      if (exp % 2 == 1)
          result = (result * base) % mod;
      exp = exp >> 1;
      base = (base * base) % mod;
  }
  return result;
}