# IoT Cloud Integration Documentation

Welcome! This guide will explain everything you need to know about integrating with IoT Cloud.

The information common to both versions of the library is present in [the other document](https://github.com/Uiop3385/CommandHandler/blob/main/extras/SerialMonitorDocs.md).

## Usage examples

See the [IoTCloud_Example.ino example](https://github.com/Uiop3385/CommandHandler/blob/main/examples/IoTCloud_Example/IoTCloud_Example.ino).

## Implementing
This guide expects you to know the basics of Arduino Cloud.
You should have a Thing with a sketch and a dashboard set up before starting.

### Creating the Cloud Variable:

In your Thing, create a new Cloud Variable. Name it however you'd like.
The variable type **must** be Character String.
Make sure the permission is set as "Read/Write", and the update policy to "On change".

---

### Adding the Widget:

Head to your associated dashboard.
Start editing, and add a "Messenger" widget.
Link the variable you've just created to it.
You can modify the other settings and position it as you'd like.

---

### Sketch:

Include the correct version of the library (see the other document).
Since we're no longer limited by the serial monitor, there's no limit on the length of a command. As such, we can simply add the class using the first two arguments:
```cpp
CommandHandler handler(10, 5); // Max 10 commands with up to 5 arguments each
```

Writing and registering commands is unchanged. Follow the other document.

The `loop()` function from the serial monitor version is not included here. Arduino Cloud will run a function when the value of our cloud variable is updated, so we don't need to repeatedly check if there's something new anymore.

This function should have been generated automatically in your sketch. It should be named `onCloudVarNameChange()`, with `CloudVarName` being the name you gave to the cloud variable.

In this function, you can add this simple code:
```cpp
void onCloudVarNameChange() {
   if (cloudVarName.length() == 0) {
     return;
   }
   String response = handler.processInput(cloudVarName);
   cloudVarName = response;
}
```
You can modify this to your liking, but the bottom two lines are essential.

Our cloud variable will now contain the output of the command. Since it's been modified, Arduino Cloud will automatically post it in the Messenger.

## Reference
Anything mentioned here belongs exclusively to this version of the library.

### Defining the class:

The class is defined with:
```cpp

CommandHandler handler(size_t  maxCommands, size_t  maxArgs);
```
You can set `handler` to be anything you would like to refer to the class as within your sketch.
-  `maxCommands`: The maximum number of commands that can be registered.
-  `maxArgs`: The maximum number of arguments that can be passed to a command.

---

### Processing the inputted command:
```cpp
String processInput(const String& input) {}
```
This will evaluate the command passed into the function, `input`.
The output of this function is the output of the command.