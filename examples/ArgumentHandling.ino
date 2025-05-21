/*
ArgumentHandling.ino
This example demonstrates more advanced usage of the library with arguments.
The multiply command multiplies the two inputted integers together.
*/

#include <CommandHandler.h>

CommandHandler handler(5, 3, 64);  // max 5 commands, 3 args, 64-char messages

// The multiply command
String multiplyCommand(const String* args, size_t argCount) {
    int a = args[0].toInt();
    int b = args[1].toInt();
    return "Result: " + String(a * b);
}

void setup() {
    Serial.begin(9600);

    // Define the argument types: INT, INT
    CommandHandler::ArgType multiplyArgs[] = {
        CommandHandler::INT,
        CommandHandler::INT
    };

    handler.registerCommand(
        "multiply",
        multiplyCommand,
        "Multiplies two integers.",
        2, 2,  // Requires exactly 2 arguments
        multiplyArgs, 2
    );
}

void loop() {
    handler.loop();  // Check for new command inputs on each loop
}

/*
Example usage:
- Input: "multiply 6 7"
- Output: "Result: 42"

- Input: "multiply 6 hello"
- Output: "Error: Argument 2 must be an integer."

This command would be displayed in the help command as such:
"multiply <INT> <INT> - Multiplies two integers."
*/