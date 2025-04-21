#include "../header/App.hpp"


int App::execute(int argc, char** argv) {

    if (!validateLine(argc)) {
        std::cerr << "Incorrect number of arguments." << std::endl;
        printUsing(argv[0]);
        return 1;
    }

    OperationType operationType;
    try {
        operationType = validateCommand(argv[1]);
    } 
    catch (const std::invalid_argument& err) {
        std::cerr << err.what() << std::endl;
        printUsing(argv[0]);
        return 2;
    }

    std::pair<std::string, std::string> args;
    try {
        args = validateArgs(argv[2], argv[3], operationType);
    } 
    catch (const std::invalid_argument& err) {
        std::cerr << err.what() << std::endl;
        printUsing(argv[0]);
        return 3;
    }

    switch (operationType)
    {
    case OperationType::COMPRESS:
        try {
            compressFile(args.first, args.second);
        } catch (const std::runtime_error& err) {
            std::cerr << err.what() << std::endl;
            printUsing(argv[0]);
            return 4;
        }
        break;
    
    case OperationType::DECOMPRESS:
        try {
            decompressFile(args.first, args.second);
        } catch (const std::runtime_error& err) {
            std::cerr << err.what() << std::endl;
            printUsing(argv[0]);
            return 4;
        }
    default:
        break;
    }

    return 0;
}