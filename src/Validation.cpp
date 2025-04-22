#include "../header/App.hpp"

void App::printUsing(char* arg0) {
    std::cout << "Using:" << std::endl;
    std::cout << arg0 << " cmp <File path> <Archive name> to compress file" << std::endl;
    std::cout << arg0 << " dcmp <Archive path> <Output directory> to decompress file" << std::endl;
}


bool App::validateLine(int argc) {
    return argc == 4;
}

OperationType App::validateCommand(char* command) {

    OperationType operationType;
    std::string com(command);
    if (com == "cmp") {
        operationType = OperationType::COMPRESS;
    }
    else if (com == "dcmp") {
        operationType = OperationType::DECOMPRESS;
    }
    else {
        throw std::invalid_argument("Incorrect command");
    }
    return operationType;
}



std::string App::validateStringAsFilePath(char* pathStr) {
    std::error_code ec;
    std::filesystem::path path(pathStr);
    if (std::filesystem::exists(path, ec) && !ec && std::filesystem::is_regular_file(path, ec) && !ec) {
        return path.generic_string();
    }
    else
        throw std::invalid_argument("Incorrect path to file");
}

std::string App::validateStringAsFileName(char* fileName) {
    
    std::string result(fileName);
    const std::string invalid = R"(/\:*?"<>|)";

    for (char c : result) {
        if (invalid.find(c) != std::string::npos) {
            throw std::invalid_argument("Invalid file name");
        }
    }
    return result;
}

std::string App::validateStringAsDirName(char* dirName) {
    std::string result(dirName);
    const std::string invalid = R"(:*?"<>|)";

    for (char c : result) {
        if (invalid.find(c) != std::string::npos) {
            throw std::invalid_argument("Invalid directory name");
        }
    }
    return std::filesystem::path(result).generic_string();
}

std::pair<std::string, std::string> App::validateArgs(char* arg1, char* arg2, OperationType operationType) {
    switch (operationType)
    {
    case OperationType::COMPRESS: {
        std::string inputFile = validateStringAsFilePath(arg1);
        std::string archiveName = validateStringAsFileName(arg2);
        return {inputFile, archiveName};
    }
    case OperationType::DECOMPRESS: {
        std::string archivePath = validateStringAsFilePath(arg1);
        std::string outputDir = validateStringAsDirName(arg2);
        return {archivePath, outputDir};
    }
    default:
        return {};
    }
}