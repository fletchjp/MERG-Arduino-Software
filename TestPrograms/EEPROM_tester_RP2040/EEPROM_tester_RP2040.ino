// --------------------------------------
// Extend to test EEPROM
// --------------------------------------
// i2c_scanner for Pico
//
// Modified from https://playground.arduino.cc/Main/I2cScanner/
// --------------------------------------

#include <Wire.h>

// Set I2C bus to use: Wire, Wire1, etc.
#define WIRE Wire1

static byte EEPROM_ADDRESS = 0x50;

void setup() {
  delay(5000);
  Serial.begin(115200);
  delay(5000);
  Serial.println("\nI2C Scanner for Pico");
  WIRE.setSDA(6);
  WIRE.setSCL(7);
  WIRE.begin(0x30);
  //while (!Serial)
   //  delay(10);
  Serial.println("\nI2C Scanner about to start");
  //Wire.setClock(50000);
}

void write_eeprom(uint16_t writeAddress,uint8_t* data, uint8_t len)
{
  WIRE.beginTransmission(EEPROM_ADDRESS);
  WIRE.write((byte)(writeAddress & 0xFF00) >> 8);
  WIRE.write((byte)(writeAddress & 0x00FF));
  uint8_t i;
  for(i = 0; i < len; i++){
    WIRE.write(data[i]);
  }
  WIRE.endTransmission();
}

void read_eeprom(uint16_t read_Address,uint8_t* data, uint8_t len)
{
  WIRE.beginTransmission(EEPROM_ADDRESS);
  WIRE.write((byte)(read_Address & 0xFF00) >> 8);
  WIRE.write((byte)(read_Address & 0x00FF));
  WIRE.endTransmission();
  WIRE.requestFrom(EEPROM_ADDRESS, len);
}

void loop() {
  byte error, address;
  int nDevices;
  char message[30];
  char writemessage[] = "Hello AT24C256 World!";

  Serial.println("Scanning...");

  nDevices = 0;
  
  for(address = 1; address < 128; address++ ) 
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmission to see if
    // a device did acknowledge to the address.
    WIRE.beginTransmission(address);
    error = WIRE.endTransmission();

    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !");

      nDevices++;
    }
    else if (error==4) 
    {
      Serial.print("Unknown error at address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else {
    Serial.print(nDevices);
    Serial.println(" devices found - done\n");
  }
  Serial.print("Writing to eeprom with ");
  Serial.println(writemessage);
  write_eeprom(0, (uint8_t*)writemessage, sizeof(message));
  //  Serial.print(nDevices);
  //  Serial.println(" devices found - done\n");
  Serial.println("reading eeprom");
  read_eeprom(0, (uint8_t*) message, sizeof(message));
  Serial.println(message);

  delay(5000);           // wait 5 seconds for next scan
}
