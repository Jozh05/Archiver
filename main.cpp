#include "header/App.hpp"

std::atomic<bool> App::stopFlag = false;

int main(int argc, char** argv) {

    App::setupSignalHandler();

    return App::execute(argc, argv);
}