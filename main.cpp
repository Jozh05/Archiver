#include "header/App.hpp"

std::atomic<bool> App::stopFlag = false;

int main(int argc, char** argv) {

    std::signal(SIGINT, App::signalHandler);

    return App::execute(argc, argv);
}