# CommandHandler
An Arduino library that turns the serial monitor, or the Arduino Cloud chat widget, into a CLI.

Write functions in your code, then register them as commands.
Example usage (for the serial monitor):
```cpp
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
```

For documentation, see the examples or the Markdown files in /extras.