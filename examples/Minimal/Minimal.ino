/*
Minimal.ino
The most basic implementation of the library.
*/

#include <CommandHandler.h>

CommandHandler handler(5, 5, 64);  // max 5 commands, 5 args, 64-char messages

// Define an echo command
String echoCommand(const String* args, size_t argCount) {
    String result;
    for (size_t i = 0; i < argCount; i++) {
        result += args[i] + " ";
    }
    return result;
}

void setup() {
    Serial.begin(9600);

    // Register the "echo" command
    handler.registerCommand("echo", echoCommand, "Echoes back the arguments.", 1, 5, nullptr, 0);
}

void loop() {
    handler.loop();  // Check for new command inputs on each loop
}