#include "CommandHandler.h"

size_t CommandHandler::commandCount = 0;
CommandHandler::Command* CommandHandler::commands = nullptr;

CommandHandler::CommandHandler(size_t maxCommands, size_t maxArgs, size_t maxMessageLength)
    : maxCommands(maxCommands), maxArgs(maxArgs), maxMessageLength(maxMessageLength),
      bufferPosition(0) {
    commands = new Command[maxCommands];
    memset(commands, 0, sizeof(Command) * maxCommands);
    inputBuffer = new char[maxMessageLength];
    memset(inputBuffer, 0, maxMessageLength);

    registerCommand("help", &CommandHandler::helpCommand, "Lists all available commands.", 0, 0, nullptr, 0);
}

void CommandHandler::registerCommand(const String& command, CommandCallback callback, const String& description,
                                     size_t minArgs, size_t maxArgs, const ArgType* argTypes, size_t argTypesCount) {
    if (commandCount < maxCommands) {
        commands[commandCount].name = command;
        commands[commandCount].callback = callback;
        commands[commandCount].description = description;
        commands[commandCount].minArgs = minArgs;
        commands[commandCount].maxArgs = maxArgs;

        if (argTypes != nullptr && argTypesCount > 0) {
            commands[commandCount].argTypes = new ArgType[argTypesCount];
            memcpy(commands[commandCount].argTypes, argTypes, sizeof(ArgType) * argTypesCount);
            commands[commandCount].argTypesCount = argTypesCount;
        } else {
            commands[commandCount].argTypes = nullptr;
            commands[commandCount].argTypesCount = 0;
        }

        commandCount++;
    } else {
        Serial.println("Error: Maximum number of commands reached");
    }
}

String CommandHandler::parseCommand(String& command, String args[], size_t& argCount) {
    int firstSpace = command.indexOf(' ');
    if (firstSpace != -1) {
        String arguments = command.substring(firstSpace + 1);
        command = command.substring(0, firstSpace);

        size_t start = 0, end;
        while ((end = arguments.indexOf(' ', start)) != -1 && argCount < maxArgs) {
            args[argCount++] = arguments.substring(start, end);
            start = end + 1;
        }
        if (start < arguments.length() && argCount < maxArgs) {
            args[argCount++] = arguments.substring(start);
        }
    }
    return command;
}

bool CommandHandler::validateArgs(const String args[], size_t argCount, const Command& cmd) {
    if (argCount < cmd.minArgs || argCount > cmd.maxArgs) {
        Serial.println("Error: Incorrect number of arguments, expected "+String(cmd.minArgs)+" at minimum and "+ String(cmd.maxArgs)+" at maximum, received "+String(argCount)+".");
        return false;
    }

    if (cmd.argTypes != nullptr) {
        for (size_t i = 0; i < argCount; i++) {
            switch (cmd.argTypes[i]) {
            case INT:
                if (args[i].toInt() == 0 && args[i] != "0") {
                    Serial.println("Error: Argument " + String(i + 1) + " must be an integer.");
                    return false;
                }
                break;
            case FLOAT:
                if (args[i].toFloat() == 0.0f && args[i] != "0" && args[i] != "0.0") {
                    Serial.println("Error: Argument " + String(i + 1) + " must be a float.");
                    return false;
                }
                break;
            case STRING:
                // All inputs are valid as strings
                break;
            }
        }
    }
    return true;
}

void CommandHandler::executeCommand(const String& command, const String args[], size_t argCount) {
    for (size_t i = 0; i < commandCount; i++) {
        if (commands[i].name == command) {
            if (!validateArgs(args, argCount, commands[i])) {
                return;
            }
            String response = commands[i].callback(args, argCount);
            Serial.println(response);
            return;
        }
    }
    Serial.println("Unknown command: " + command + ". Use help for a list of available commands.");
}

String CommandHandler::readSerial() {
    while (Serial.available() > 0) {
        char inChar = Serial.read();
        if (inChar == '\n' || bufferPosition >= maxMessageLength - 1) {
            inputBuffer[bufferPosition] = '\0';
            String temp = String(inputBuffer);
            memset(inputBuffer, 0, maxMessageLength);
            bufferPosition = 0;
            return temp;
        } else if (inChar != '\r') {
            inputBuffer[bufferPosition++] = inChar;
        }
    }
    return "";
}

String CommandHandler::helpCommand(const String args[], size_t argCount) {
    String result = "Available commands:\n";
    for (size_t i = 0; i < commandCount; i++) {
        result += commands[i].name;

        for (size_t j = 0; j < commands[i].argTypesCount; j++) {
            result += " <";
            switch (commands[i].argTypes[j]) {
                case ArgType::STRING:
                    result += "STRING";
                    break;
                case ArgType::INT:
                    result += "INT";
                    break;
                case ArgType::FLOAT:
                    result += "FLOAT";
                    break;
            }
            result += ">";
        }

        if (commands[i].description.length() > 0) {
            result += " - ";
            result += commands[i].description;
        }

        result += "\n";
    }
    return result;
}

void CommandHandler::loop() {
    String input = readSerial();
    if (input.length() > 0) {
        String* args = new String[maxArgs];
        size_t argCount = 0;
        String command = parseCommand(input, args, argCount);
        executeCommand(command, args, argCount);
        delete[] args;
    }
}

CommandHandler::~CommandHandler() {
    delete[] commands;
    delete[] inputBuffer;
}