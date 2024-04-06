#include <mcp2515.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F, 16, 2);

struct can_frame canMsg1;
MCP2515 mcp2515(10);

const int trigPin = 9;
const int echoPin = 8;
// defines variables
long duration;
int distance;


long p = 13;
long q = 11;
long n = 143;
long n_square = 20449;
long phi_n = 120;
long g = 144;
long lamb = 120;
long mu = 87;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input


  lcd.init();
  lcd.clear();
  lcd.backlight();
  
  mcp2515.reset();
  mcp2515.setBitrate(CAN_125KBPS);
  mcp2515.setNormalMode();

}

void loop() {
  // put your main code here, to run repeatedly:
  lcd.clear();
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;
  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);
  lcd.setCursor(0, 0);
  lcd.print("Distance:");
  lcd.setCursor(0, 1);
  lcd.print(distance);

  long cipherText = encrypt(distance);

  canMsg1.can_id = 0x046;
  canMsg1.can_dlc = 2;

  canMsg1.data[0] = lowByte(cipherText);
  canMsg1.data[1] = highByte(cipherText);
  mcp2515.sendMessage(&canMsg1);
  delay(2000);
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
