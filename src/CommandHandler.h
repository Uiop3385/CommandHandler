#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H

#include <Arduino.h>

class CommandHandler {
public:
    using CommandCallback = String (*)(const String*, size_t);

    enum ArgType {
        STRING,
        INT,
        FLOAT
    };

    struct Command {
        String name;
        CommandCallback callback;
        String description;
        size_t minArgs;
        size_t maxArgs;
        ArgType* argTypes;
        size_t argTypesCount;
    };

    CommandHandler(size_t maxCommands, size_t maxArgs, size_t maxMessageLength);
    ~CommandHandler();

    void loop();
    void registerCommand(const String& command, CommandCallback callback, const String& description, 
                        size_t minArgs, size_t maxArgs, const ArgType* argTypes, size_t argTypesCount);

private:
    static size_t commandCount;
    static Command* commands;

    size_t maxCommands;
    size_t maxArgs;
    size_t maxMessageLength;

    char* inputBuffer;
    size_t bufferPosition;

    String parseCommand(String& command, String args[], size_t& argCount);
    void executeCommand(const String& command, const String args[], size_t argCount);
    bool validateArgs(const String args[], size_t argCount, const Command& cmd);
    String readSerial();

    static String helpCommand(const String args[], size_t argCount);
};

#endif