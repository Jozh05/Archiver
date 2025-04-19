#include "header/App.hpp"


int main(int argc, char** argv) {
    
    
    
    App::compressFile(argv[1], argv[2]);
    App::decompressFile(argv[2], "./data");


    return 0;
}