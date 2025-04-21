#pragma once
#include <fstream>
#include <iostream>
#include <filesystem>
#include <string>
#include <stdexcept>

#include <archive.h>
#include <archive_entry.h>

enum class OperationType {
    COMPRESS,
    DECOMPRESS
};

class App {

public:

    App() = delete;
    App(const App&) = delete;

private:
    static void compressFile(const std::string& inputFile, const std::string& archiveName);
    static void decompressFile(const std::string& archiveFile, const std::string& outputDir);

    static void printUsing(char* arg0);

    static bool validateLine(int argc);
    static OperationType validateCommand(char* command);
    static std::pair<std::string, std::string> validateArgs(char* arg1, char* arg2, OperationType);
    static std::string validateStringAsFilePath(char* str);
    static std::string validateStringAsFileName(char* str);
    static std::string validateStringAsDirName(char* str);

public:
    static int execute(int argc, char** argv);
};