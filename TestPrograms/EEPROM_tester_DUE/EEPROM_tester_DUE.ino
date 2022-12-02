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
  //WIRE.setSDA(6);
  //WIRE.setSCL(7);
  WIRE.begin();
  //while (!Serial)
   //  delay(10);
  Serial.println("\nI2C Scanner about to start");
  //Wire.setClock(50000);
}

byte write_eeprom(uint16_t writeAddress,uint8_t* data, uint8_t len)
{
  WIRE.beginTransmission(EEPROM_ADDRESS);
  WIRE.write((byte)(writeAddress & 0xFF00) >> 8);
  WIRE.write((byte)(writeAddress & 0xFF));
  uint8_t i;
  byte r;
  for(i = 0; i < len; i++){
    WIRE.write(data[i]);
  }
  r = WIRE.endTransmission();
  delay(20);
  return r;
}

uint8_t read_eeprom(uint16_t read_Address,uint8_t* data, uint8_t len)
{
  byte r;
  WIRE.beginTransmission(EEPROM_ADDRESS);
  WIRE.write((byte)(read_Address & 0xFF00) >> 8);
  WIRE.write((byte)(read_Address & 0xFF));
  r = WIRE.endTransmission();
  if (r != 0) {
    Serial.print("read_eeprom failed with ");
    Serial.println(r);
    return 0;
  }
  WIRE.requestFrom(EEPROM_ADDRESS, len);
  uint8_t i;
  for(i = 0; i < len; i++){
    if(WIRE.available()) data[i] = WIRE.read();
    else break;
  }
  return i;
}

char message[30];

void loop() {
  byte error, address;
  uint8_t bytes_read;
  uint16_t start_point = 500;
  int nDevices;
  //char message[30];
  char writemessage[] = "1234567890";

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
  Serial.print("Writing to eeprom address 0x");
  Serial.print(EEPROM_ADDRESS,HEX);
  Serial.print(" with ");
  Serial.println(writemessage);
  error = write_eeprom(start_point, (uint8_t*)writemessage, sizeof(writemessage));
  if (error == 0 )
    {  Serial.println("Write successful"); } else
    {  Serial.print("Write failed with "); Serial.println(error); }
    Serial.print("reading eeprom ");
    bytes_read = read_eeprom(start_point, (uint8_t*) message, sizeof(message));
    Serial.print(bytes_read); Serial.println(" bytes read");
    if (bytes_read > 0) Serial.println(message);
  }
  delay(5000);           // wait 5 seconds for next scan
}
