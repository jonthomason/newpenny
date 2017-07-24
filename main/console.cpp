// console.cpp
//  Simple debug console
//  June 2017

#include <Arduino.h>
#include "main.h"
#include "console.h"
#include "display.h"

// startConsole
//  Decide whether to start the console based on user input
boolean startConsole(void) {
    Serial.print(F("Hit 'd' to start debug mode: "));
    Serial.flush();
    display.setCursor(0, 24);
    display.display();
    unsigned long timeStart = millis();
    int countdown = 5;
    const int timeEnd = timeStart + countdown * 1000; // milliseconds
    int timeCurrent = millis();
    boolean startConsole = false;
    while (timeCurrent < timeEnd && !startConsole) {
        // print 5 4 3 2 1
        if ((timeEnd - timeCurrent) / 1000 < countdown) {
            Serial.print(countdown, DEC);
            Serial.print(" ");
            Serial.flush();
            display.print(countdown, DEC);
            display.print(" ");
            display.display();
            --countdown;
        }

        // See if they pressed 'd'
        while (Serial.available() > 0) {
            int incoming = Serial.read() | 32; // or with 32 forces value to lowercase
            if (incoming == 'd') {
                startConsole = true;
            }
        }

        // get the new time
        timeCurrent = millis();
    }

    // Dump any extra input before heading to debug console
    while (Serial.available() > 0) {
        Serial.read();
    }
    Serial.println();
    return startConsole;
}

// debugConsole
//  If requested, starts a debug console to read the log or clear it
void debugConsole(void) {
    if (!startConsole()) {
        return;
    }

    // indicate on the display that we're in the console
    display.clearDisplay();
    display.print("Debug console running");
    display.display();

    boolean finished = false;
    int command;
    while (!finished) {
        Serial.println(F("'d' dump log, 'c' clear log, 'x' exit and boot"));
        Serial.print(F("> "));
        do {
          command = Serial.read() | 32;
        } while (command == -1);

        switch (command) {
        case 'd':
            Serial.println();
            theLog.dump();
            break;
        case 'c':
            Serial.println(F("Clearing log. Please wait up to a minute..."));
            theLog.clear();
            Serial.println(F("Log cleared."));
            break;
        case 'x':
            finished = true;
            break;
        }
    }

    // Dump any extra input before exiting debug console
    while (Serial.available() > 0) {
        Serial.read();
    }
    Serial.println();
}
