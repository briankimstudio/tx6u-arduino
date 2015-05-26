// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

#include "DHT.h"

#define DHTPIN  2     // PIN # for DHT22 Temperature Sensor
#define TXPIN   10    // PIN # for 433Mhz Transmitter
#define LEDPIN  13    // PIN # for LED Indicator

#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor for normal 16mhz Arduino
DHT dht(DHTPIN, DHTTYPE);
// NOTE: For working with a faster chip, like an Arduino Due or Teensy, you
// might need to increase the threshold for cycle counts considered a 1 or 0.
// You can do this by passing a 3rd parameter for this threshold.  It's a bit
// of fiddling to find the right value, but in general the faster the CPU the
// higher the value.  The default for a 16mhz AVR is a value of 6.  For an
// Arduino Due that runs at 84mhz a value of 30 works.
// Example to initialize DHT sensor for Arduino Due:
//DHT dht(DHTPIN, DHTTYPE, 30);

unsigned long msg_timer = 60000; // Transmits a signal every 1 minute
unsigned long msg_gap = 32; //gap between packets

char buf[11]; //message to be built

void setup() {
  Serial.begin(9600);
  Serial.println("Wireless Temp");
  pinMode(LEDPIN,OUTPUT);
  pinMode(TXPIN, OUTPUT);
  dht.begin();
}

// Changed int to float
// num is in Celsius. Ex) 26.30
// Brian May 24, 2015
void make_message(float num) {

  // make up a TC6U message from the integer num (-500 to 499)

  // parity table  0 1 2 3 4 5 6 7 8 9
  char   ptable[]={
    0,1,1,2,1,2,2,3,1,2  }; //number of bits set
  int p;  //parity bit
  char h,t,d;  //hundreds,tens,digits

  p=0; //clear parity

  // preamble
  buf[0] = 0;
  buf[1] = 0xa;  //message length and start bits
  buf[2] = 0;    //type = temperature
  buf[3] = 0xf;  //device ID
  buf[4] = 0x0;  //bottom bit is parity

  // Adjusted digit mismatch in LA CROSSE WS-9016TWC
  // Brian May 24, 2015
  num = num * 10;

  // encode the integer
  num = num+500;
  h = num/100;

  buf[5] = h;  //save hundreds
  p += ptable[h];  //update parity calculation
  num = num - h*100;
  t = num/10;
  buf[6] = t; //save tens
  p += ptable[t];  //update parity
  d = num - t*10;
  buf[7] = d; //save digits
  p += ptable[d];
  buf[8] = h; //second copy of hundreds digit
  buf[9] = t; //second copy of tens digit

  //if value parity odd, set message parity bit
  if ( (p&1) == 1) buf[4] |= 1;
  //calculate and store checksum
  buf[10] = (buf[1]+buf[3]+buf[4]+buf[5]+buf[6]+buf[7]+buf[8]+buf[9])&0x0F;
}

void send_one(char s) { //s=1 for short TXPIN off. 4th bit of quartet
  digitalWrite(TXPIN,HIGH);
  delayMicroseconds(588);
  digitalWrite(TXPIN,LOW);
  if (s) delayMicroseconds(1020);
  else delayMicroseconds(1088);
}

void send_zero(char s) { //s=1 for short TX off, 4th bit of quartet
  digitalWrite(TXPIN,HIGH);
  delayMicroseconds(1400);
  digitalWrite(TXPIN,LOW);
  if (s) delayMicroseconds(1020);
  else delayMicroseconds(1088);
}

void send_burst(char *msg) {
  char i,c;

  for(i=0; i<12; i++) {

    c = *msg++;  //values in lower 4 bits

    //unroll inner loop, send a quartet

    if(c&8) send_one(0);
    else send_zero(0);
    if(c&4) send_one(0);
    else send_zero(0);
    if(c&2) send_one(0);
    else send_zero(0);
    if(c&1) send_one(1); //"short tx off" (1020 us) for last bit
    else send_zero(1);
  }
}

// Changed int to float
// Brian May 24, 2015
void send_message(float num) {
  digitalWrite(LEDPIN,HIGH);
  make_message(num);
  send_burst(buf);
  delay(msg_gap);
  send_burst(buf);
  digitalWrite(LEDPIN,LOW);
}

void loop() {
  // Wait a few seconds between measurements.
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float t = dht.readTemperature();

  // Check if any reads failed and exit early (to try again).
  if (isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  Serial.print(t);
  Serial.println(" *C\t");

  send_message(t); // t is in Celisius. Ex) 26.30
  delay(msg_timer);
}
