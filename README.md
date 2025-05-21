# CommandHandler
An Arduino library that turns the serial monitor into a CLI, with an Arduino IoT Cloud integration.
If this library was of any use for you, please consider starring it!

**Features:**
- Register commands with **descriptions**, **argument limits**, and **custom argument types** (INT, FLOAT, STRING).
- A built-in help command automatically lists all available commands, their argument types, and descriptions. **No need to make it yourself!**
- Supports per-argument type validation (ensuring the first argument is an INT, the second a FLOAT, etc).
- **Easy to use**
- Clean and user-friendly error handling

**Arduino Cloud integration:**
- Use the Arduino Cloud chat widget as the command-line interface.
- **Remotely control** and debug your device from the cloud **without needing serial communication**.
- Input commands **anywhere**, from a computer or a smartphone.

**For help, see:**
- [Examples](https://github.com/Uiop3385/CommandHandler/tree/main/examples)
- [Serial Monitor documentation](https://github.com/Uiop3385/CommandHandler/blob/main/extras/SerialMonitorDocs.md)
- [Arduino Cloud integration documentation](https://github.com/Uiop3385/CommandHandler/blob/main/extras/IoTCloudDocs.md)

Write functions in your code, then register them as commands.
Basic example usage (for the serial monitor):
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
