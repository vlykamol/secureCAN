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
  // encryption till 142 with these parameters
  long plainText = 142;
  long cipherText = encrypt(plainText);
  long decrypted = decrypt(cipherText);
}

void loop() {
  // put your main code here, to run repeatedly:
  // Serial.print("hello");
}

long encrypt(long m){
  Serial.println();
  long r = 3;
  long cipherText = power_mod(g, m, n_square);
  long cipherText2 = power_mod(r, n, n_square);
  long cipherText3 = (cipherText * cipherText2) % n_square;
  return cipherText3;
}

long decrypt(long c){
  long d1 = power_mod(c, lamb, n_square) - 1;
  long d2 = d1 / n * mu;
  long d3 = d2 % n;
  return d3;
}

// Function to compute (base^exp) % mod
long power_mod(long base, long exp, long mod) {
  long result = 1;
  base = base % mod; // Update base if it is >= mod
  while (exp > 0) {
      // If exp is odd, multiply base with result
      if (exp % 2 == 1)
          result = (result * base) % mod;
      // Now exp is even, divide it by 2
      exp = exp >> 1; // Same as exp /= 2
      base = (base * base) % mod;
  }
  return result;
}