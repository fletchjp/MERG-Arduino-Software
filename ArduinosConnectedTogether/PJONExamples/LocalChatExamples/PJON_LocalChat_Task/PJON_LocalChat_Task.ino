// PJON_LocalChat_Task.ino
// I am going to take out the radio stuff so that it works between two Arduinos using Serial Monitor on the IDE.
// The first change is to use SoftwareBitBang and not ThroughSerial.
// I now have this working so that a copy can run in each UNO and I can send messages between them.
// At the moment these come from Serial input.
// This version is going to follow other examples and put the serial processing into a task.

#include <TaskManagerIO.h>


/* LocalChat is a chat application example using the Arduino IDE serial
   monitor implemented in 170 lines of code. PJON in local mode is used to
   support up to 254 participants, the Arduino IDE serial monitor is used as
   the user interface. Using HC-12 (HCMODU0054) wireless serial module a
   decentralized wireless network of computers can be made with USB enabled
   Arduino compatible devices used as USB to HC-12 radio 433MHz gateways.

   User's 1 laptop                                        User's 2 laptop
  __________________                                     __________________
 ||1:Ciao 2!       ||                                   ||1:Ciao 2!       ||
 ||2:Ciao 1 :)     ||                                   ||2:Ciao 1 :)     ||
 ||1:Awesome chat! ||     LOS or line of sight range    ||1:Awesome chat! ||
 ||________________||             150-1800m             ||________________||
 |__________________| USB  _______         _______  USB |__________________|
 |[][][][][][][][][]|=====|ARDUINO|       |ARDUINO|=====|[][][][][][][][][]|
 |[][][][][][][][][]|     | HC-12 |       | HC-12 |     |[][][][][][][][][]|
 |[][][]|____|[][][]|     |_______|       |_______|     |[][][]|____|[][][]|
 |__________________|                                   |__________________|

 Open the Arduino IDE serial monitor at 115200 baud to initiate the chat

 Giovanni Blu Mitolo 2018 */

/* Set synchronous response timeout to 200 milliseconds.
   If operating at less than 9600Bd TS_RESPONSE_TIME_OUT should be longer */
#define TS_RESPONSE_TIME_OUT 200000

/* Include the packet id feature to support packet duplication avoidance i.e.
   avoid cases where a missed acknowledgement because of range or interference
   can lead to message duplications:
   user 1: Ciao!
   user 1: Ciao!
   user 1: Ciao! */
#define PJON_INCLUDE_PACKET_ID

/* Use 63 characters maximum packet length.
   HC-12 has two 64-byte Rx and Tx FIFO memories built into the chip,
   supporting up to 64 bytes maximum packet length.
   This limit is no longer applicable and could be extended. */
#define PJON_PACKET_MAX_LENGTH 63

#include <PJONSoftwareBitBang.h>

PJONSoftwareBitBang bus(0);


uint8_t packet[100];
String string_number;
int req_index = 0;
bool parse_id = true;
bool initialized = false;
uint8_t recipient = 0;

void receiver_function(uint8_t *payload, uint16_t length, const PJON_Packet_Info &packet_info) {
  if(payload[0] == 'X') {
    Serial.println("BLINK");
    Serial.flush();
    digitalWrite(LED_BUILTIN, HIGH);
    delay(30);
    digitalWrite(LED_BUILTIN, LOW);
    bus.reply("X", 1);
  }
  // Received messages sender id and content are printed here
  if(packet_info.tx.id == recipient || packet_info.tx.id == PJON_BROADCAST) {
    Serial.print("user ");
    Serial.print(packet_info.tx.id);
    Serial.print((packet_info.rx.id == PJON_BROADCAST) ? " " : ": ");
    for(uint16_t i = 0; i < length; i++)
      Serial.print((char)payload[i]);
    Serial.println();
  }
};

void setup() {
  // Initialize LED 13 to be off
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  // Set HC12 baudrate (you must use the one configured in HC12, default 9600)
  // HC12.begin(9600);

  // Pass the HC12 Serial instance you want to use for PJON communication
  // bus.strategy.set_serial(&HC12);
  // Avoid messages duplication
  bus.set_packet_id(true);
  bus.strategy.set_pin(12);
 
  bus.set_receiver(receiver_function);
  bus.set_error(error_handler);
  bus.begin();

  //taskManager.scheduleFixedRate(5000,task_measure_environment); // old style
  taskManager.schedule(repeatMillis(250),task_handle_serial_input);

  Serial.begin(115200);
  Serial.println("LocalChat - Giovanni Blu Mitolo 2018");
  Serial.println("adapted for SoftwareBitBang by John Fletcher 2024");
  Serial.println("Version using TaskManagerIO");
  Serial.println("User id configuration guide:");
  Serial.println("1) Set serial monitor 'Newline' ending");
  Serial.println("2) Enter your user id (1-254) in the text field");
  Serial.println("3) Press 'Send'");
  Serial.println();
  Serial.flush();
};

void error_handler(uint8_t code, uint16_t data, void *custom_pointer) {
  if(code == PJON_CONNECTION_LOST) {
    Serial.print("Connection with device ID ");
    Serial.print(bus.packets[data].content[0], DEC);
    Serial.println(" is lost.");
  }
  if(code == PJON_PACKETS_BUFFER_FULL) {
    Serial.print("Packet buffer is full, has now a length of ");
    Serial.println(data, DEC);
    Serial.println("Possible wrong bus configuration!");
    Serial.println("higher PJON_MAX_PACKETS if necessary.");
  }
  if(code == PJON_CONTENT_TOO_LONG) {
    Serial.print("Content is too long, length: ");
    Serial.println(data);
  }
};

void task_handle_serial_input() {
  // Handle PJON connectivity if no data has been sent from serial monitor
  // I need to change this to read the whole line, not one char at a time.
if(Serial.available() > 0)
  { // Handle data from serial monitor
    char req_byte = Serial.read();
    string_number += req_byte;
    if(initialized && recipient) {
      if(!req_index) {
        Serial.print("user ");
        Serial.print((uint8_t)bus.device_id(), DEC);
        Serial.print(": ");
      }
      //Serial.print(req_byte);
      packet[req_index] = req_byte;
    }
    if(req_byte == '\n') {
      if(initialized) {
        if(!recipient) {
          recipient = (uint8_t)string_number.toInt();
          Serial.print("Recipient's user id ");
          Serial.print(recipient, DEC);
          Serial.println(" set.");
          Serial.println();
          Serial.println("LocalChat is now online!");
          uint8_t hello[12] = "X is online.";
          Serial.flush();
          bus.send(recipient, hello, 12);
        } else {
          // I want to print in one go.
          for (int i = 0; i < req_index; i++ )
             Serial.print((char)packet[i]);
          Serial.println();
          Serial.flush();
          bus.send(recipient, packet, req_index);
        }
      } else {
        bus.set_id((uint8_t)string_number.toInt());
        Serial.print("User id ");
        Serial.print(bus.device_id(), DEC);
        Serial.println(" set.");
        Serial.println("Enter the recipient's user id (1-254) and press 'Send'");
        Serial.flush();
        initialized = true;
      }
      parse_id = true;
      req_index = 0;
      string_number = "";
      return;
    }
    req_index++;
  }
};

void loop() {
  taskManager.runLoop();
  bus.update();
  bus.receive(1000);
}

