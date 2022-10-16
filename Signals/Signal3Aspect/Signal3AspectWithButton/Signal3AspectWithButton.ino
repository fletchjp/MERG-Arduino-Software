// Example from Merg Journal June 2020 page49
// Sven Rosvall

// Arduino Nano as in the article.

/* This case  uses the  trackPin and cycles through the colours.
    when a train is detected.
*/

int trackPin = 2;
int greenPin = 3;
int redPin = 5;
int yellowPin = 6;

void setup()
{
  pinMode(trackPin, INPUT_PULLUP);
  pinMode(greenPin, OUTPUT);
  pinMode(redPin, OUTPUT);
  pinMode(yellowPin, OUTPUT);
  digitalWrite(greenPin, HIGH);
}

void loop()
{

  while (digitalRead(trackPin) == HIGH )
 {
 }

  digitalWrite(greenPin, LOW);
  digitalWrite(redPin, HIGH);

  delay(5000);
  digitalWrite(redPin, LOW);
  digitalWrite(yellowPin, HIGH);

  delay(5000);
  digitalWrite(yellowPin, LOW);
  digitalWrite(greenPin, HIGH);

  //delay(5000);
}