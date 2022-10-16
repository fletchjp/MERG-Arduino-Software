// Example from Merg Journal June 2020 page49
// Expanded to two linked signals
// Sven Rosvall

// Arduino Nano as in the article.

/* This case  uses the  trackPin and cycles through the colours.
    when a train is detected.
*/

/* The same pin allocation ias in TwoIndependentSignal3Aspect example. */
int trackPin = 2;
int greenPin1 = 3;
int redPin1 = 5;
int yellowPin1 = 6;

int greenPin2 =9;
int redPin2 = 10;
int yellowPin2 =11;

void setup()
{
  pinMode(trackPin, INPUT_PULLUP);
  pinMode(greenPin1, OUTPUT);
  pinMode(redPin1, OUTPUT);
  pinMode(yellowPin1, OUTPUT);
  pinMode(greenPin2, OUTPUT);
  pinMode(redPin2, OUTPUT);
  pinMode(yellowPin2, OUTPUT);
  /* Start with the line clear */
  digitalWrite(greenPin1, HIGH);
  digitalWrite(greenPin2, HIGH);
  Serial.begin(9600);
  Serial.println("Two linked 3 Aspect signals starting");
}

void loop()
{

  while (digitalRead(trackPin) == HIGH )
 {
 }
 /* Train passes the first signal */
  digitalWrite(greenPin2, LOW);
  digitalWrite(redPin2, HIGH);
 
/* Train passes the fsecondsignal */
 delay(5000);
  digitalWrite(greenPin1, LOW);
  digitalWrite(redPin1, HIGH);
  digitalWrite(redPin2, LOW);
  digitalWrite(yellowPin2, HIGH);

  delay(5000);
  digitalWrite(redPin1, LOW);
  digitalWrite(yellowPin1, HIGH);
  digitalWrite(yellowPin2, LOW);
  digitalWrite(greenPin2, HIGH);

  delay(5000);
  digitalWrite(yellowPin1, LOW);
  digitalWrite(greenPin1, HIGH);

  //delay(5000);
}
