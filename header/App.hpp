#pragma once
#include <fstream>
#include <iostream>
#include <filesystem>
#include <string>
#include <stdexcept>
#include <atomic>

#include <archive.h>
#include <archive_entry.h>

enum class OperationType {
    COMPRESS,
    DECOMPRESS
};

class App {

private:
    static std::atomic<bool> stopFlag;

    inline static archive* arch = nullptr;
    inline static archive_entry* entry = nullptr;

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

    static void checkSignal(OperationType, const std::filesystem::path& outputPath);

    static void signalHandler(int);

public:
    static void setupSignalHandler();
    static int execute(int argc, char** argv);
};