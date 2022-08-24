/// @file  MulticoreTalking.ino
/// @brief Multitalking combined with the TalkingToMyself example.
///
/// Using both onboard I2C interfaces, have one master and one slave
/// and send data both ways between them
///
/// To run, connect GPIO0 to GPIO2, GPIO1 to GPIO3 on a single Pico
/// I have added 4 to the pin numbers to get a set of pin values.
///
/// Demonstrates a simple use of the setup1()/loop1() functions
/// for a multiprocessor run.

// Will output something like, where C0 is running on core 0 and
// C1 is on core 1, in parallel.

// 11:23:07.507 -> C0: Blue leader standing by...
// 11:23:07.507 -> C1: Red leader standing by...
// 11:23:07.507 -> C1: Stay on target...
// 11:23:08.008 -> C1: Stay on target...
// 11:23:08.505 -> C0: Blue leader standing by...
// 11:23:08.505 -> C1: Stay on target...
// 11:23:09.007 -> C1: Stay on target...
// 11:23:09.511 -> C0: Blue leader standing by...
// 11:23:09.511 -> C1: Stay on target...
// 11:23:10.015 -> C1: Stay on target...

// Released to the public domain

#include <Wire.h>

// The normal, core0 setup
void setup() {
  Serial.begin(115200);
  delay(10000);
  Serial.printf("Send and receive I2C between two cores.\n");
  Wire.setSDA(4);
  Wire.setSCL(5);
  Wire.begin();
  Serial.printf("C0: Blue leader standing by with Wire...\n");
}

static char buff[100];

void loop() {
  Serial.printf("C0: Blue leader sending...\n");
  static int p;
  char b[90];
  Wire.beginTransmission(0x30);
  sprintf(b, "pass %d", p++);
  Wire.write(b, strlen(b));
  Wire.endTransmission();
  delay(1000);
  Serial.printf("buff: '%s'\r\n", buff);
  // Read from the slave and print out
  Wire.requestFrom(0x30, 6);
  Serial.print("\nrecv: '");
  while (Wire.available()) {
    Serial.print((char)Wire.read());
  }
  Serial.println("'");
  delay(1000);
}

// Running on core1
void setup1() {
  delay(10000);
  Serial.printf("C1: Red leader standing by...\n");
  Wire1.setSDA(6);
  Wire1.setSCL(7);
  Wire1.begin(0x30);
  Wire1.onReceive(recv);
  Wire1.onRequest(req);
}

// These are called in an **INTERRUPT CONTEXT** which means NO serial port
// access (i.e. Serial.print is illegal) and no memory allocations, etc.

// Called when the I2C slave gets written to
void recv(int len) {
  int i;
  // Just stuff the sent bytes into a global the main routine can pick up and use
  for (i = 0; i < len; i++) {
    buff[i] = Wire1.read();
  }
  buff[i] = 0;
}

// Called when the I2C slave is read from
void req() {
  static int ctr = 765;
  char buff[7];
  // Return a simple incrementing hex value
  sprintf(buff, "%06X", (ctr++) % 65535);
  Wire1.write(buff, 6);
}
void loop1() {
  Serial.printf("C1: Stay on target...\n");
  delay(500);
}
