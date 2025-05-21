# Serial Monitor Documentation

Welcome! This guide will explain everything you need to know about the library for usage in the serial monitor.


## Arduino Cloud Integration
See [this guide](https://github.com/Uiop3385/CommandHandler/blob/main/extras/IoTCloudDocs.md) for the specifics. The rest of the documentation is found here.


## Installation

This is most easily done from the Library Manager. Search `CommandHandler`, then install.

Alternatively, download the latest release of the source code [here](https://github.com/Uiop3385/CommandHandler/releases/latest), then extract to the "libraries/" folder of the IDE.


## Importing the library

Add `#include <CommandHandler.h>` to the beginning of your sketch for the serial monitor version.
If you're using the Arduino Cloud integration, add this instead:
`#include <CommandHandler_IoTCloud.h>`.


## Usage examples

See the [Minimal.ino example](https://github.com/Uiop3385/CommandHandler/blob/main/examples/Minimal/Minimal.ino).

```cpp
#include <CommandHandler.h>

CommandHandler  handler(5, 5, 64); // max 5 commands, 5 args, 64-character buffer

String echoCommand(const String* args, size_t argCount) {
  String response;

  for (size_t i = 0; i < argCount; i++) {
    response += args[i] +  " ";
  }
  
  return response;
} 

void setup() {
  Serial.begin(9600);
  handler.registerCommand("echo", echoCommand, "Echoes the given arguments.", 1, 5, nullptr, 0);
}

void loop() {
  handler.loop();
}
```

For an example of usage with arguments, refer to [ArgumentHandling.ino](https://github.com/Uiop3385/CommandHandler/blob/main/examples/ArgumentHandling/ArgumentHandling.ino).
  


## Reference

### Functions:
The functions you write to be used as commands should be formatted as such:

```cpp
String functionName(const String* args, size_t argCount) {}
```
Your function should output a String. This output is then automatically transmitted to the Serial Monitor.
The arguments of the function are provided by the library.

---

### Arguments:
To access the arguments passed to the command, use `args[i]`.
i is the index of the argument you're trying to get. For example, the first argument would be obtained using `args[0]`, the second one using `args[1]`, and so on.

Even though type validation is done automatically, it's recommended to make sure they are right. Use `toInt()` or `toFloat()` as such:
`args[i].toXXX()`.

---

### Defining the class:
The class is defined with:
```cpp
CommandHandler handler(size_t maxCommands, size_t maxArgs, size_t maxMessageLength);
```
You can set `handler` to be anything you would like to refer to the class as within your sketch.

- `maxCommands`: The maximum number of commands that can be registered.
- `maxArgs`: The maximum number of arguments that can be passed to a command.
- `maxMessageLength`: 
  - The maximum length, in bytes, that an incoming command can have. Inputs longer than this value will be cut.
  - *Having this above 64 is pointless, as inputs would be larger than the capacity of the input buffer of the Arduino*.

---

### Using the `loop()` function:
```cpp
void CommandHandler::loop() {}
```
Use it as `handler.loop()`, with handler being the name of the class.
Call this function inside of your program's `loop()` function, or anytime you would like inputted commands to be processed.

---

### Registering commands:
```cpp
void CommandHandler::registerCommand(
  const String& name,
  CommandCallback callback,
  const String& description,
  size_t minArgs,
  size_t maxArgs,
  const ArgType* argTypes,
  size_t argTypesCount
) {}
```
This function is used to register new commands.
- `command`: The name of the command.
- `callback`: The name of the function called by the command, without the parenthesis.
- `description`: The description of the command, which will be displayed when running `help`. *Set this to `""` if you don't want a description to be displayed*.
- `minArgs`: The minimum number of arguments to be passed to the function. *Set this to `0` if your command doesn't take any arguments*.
- `maxArgs`: The maximum number of arguments to be passed to the function. *Set this to `0` if your command doesn't take any arguments*.
- `argTypes`: The array of arguments for the command. See below for more information. *Set this to `nullptr` if your command doesn't take any arguments*.
- `argTypesCount`:
	- The number of different argument **types** your command takes.
	- For example, if argTypes is equal to:
		- `{ CommandHandler::INT, CommandHandler::INT, CommandHandler::FLOAT };`
	- Then this value should be equal to 2, since there are two types being used, `CommandHandler::INT` and `CommandHandler::FLOAT`.
	- *Set this to `0` if your command doesn't take any arguments*.

---

### Creating argument types:
```cpp
enum ArgType {
  STRING,
  INT,
  FLOAT
};
```
The library supports the argument types listed above.
You need to create an array using `ArgType`, and pass it to `registerCommand()`, like so:
```cpp
CommandHandler::ArgType arrayName[] = {
  CommandHandler::XXX,
  CommandHandler::XXX
};
```
`XXX` would be replaced by one of the types defined in the above enum.
`arrayName` is what you would call your argument array. This is the name you pass to `registerCommand()`.
