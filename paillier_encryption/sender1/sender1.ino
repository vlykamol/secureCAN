// sender1
// sends encrypted data to cloud


#include <mcp2515.h>

struct can_frame canMsg1;
MCP2515 mcp2515(10);


long p = 13;
long q = 11;
long n = 143;
long n_square = 20449;
long phi_n = 120;
long g = 144;
long lamb = 120;
long mu = 87;

void setup() {
  Serial.begin(9600);

  mcp2515.reset();
  mcp2515.setBitrate(CAN_125KBPS);
  mcp2515.setNormalMode();
}

void loop() {
  // encryption till 142 with these parameters
  long plainText = 10;
  long cipherText = encrypt(plainText);
  Serial.print("plain text : ");
  Serial.println(plainText);
  canMsg1.can_id = 0x047;
  canMsg1.can_dlc = 2;

  canMsg1.data[0] = lowByte(cipherText);
  canMsg1.data[1] = highByte(cipherText);
  mcp2515.sendMessage(&canMsg1);
  delay(1000);
}

long encrypt(long m){
  Serial.println();
  long r = 3;
  long cipherText = power_mod(g, m, n_square);
  long cipherText2 = power_mod(r, n, n_square);
  long cipherText3 = (cipherText * cipherText2) % n_square;
  return cipherText3;
}

// Function to compute (base^exp) % mod
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