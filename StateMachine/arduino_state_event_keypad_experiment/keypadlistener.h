// keypadlistener.h

#ifndef KEYPAD_LISTENER_H
#define KEYPAD_LISTENER_H

class MyKeyboardListener : public KeyboardListener {
public:
    void keyPressed(char key, bool held) override {
        Serial.print("Key ");
        Serial.print(key);
        Serial.print(" is pressed, held = ");
        Serial.println(held);
        key_pressed = key;
    }

    void keyReleased(char key) override {
        Serial.print("Released ");
        Serial.println(key);
        key_pressed = 'X';
    }
};

#endif
