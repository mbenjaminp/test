
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string.h>
#include <thread>
#include <chrono>
#include "Instancefinder.h"
#include "Constants.h"
#include "Multicastsender.h"
#include "Multicastlistener.h"


//called function from main
int findInstances()
{
    std::string interrupter;

    printf("\nPress RETURN to exit.\n");
    //care, this Object gets deleted after returning from findInstances !!!!
    Multicastsender multicastsender;
    Multicastlistener multicastlistener;
    //starting thread with DEFAULT Netwarkinterface
    std::thread sender(std::ref(multicastsender), INADDR_ANY);
    std::thread listener(std::ref(multicastlistener), INADDR_ANY);
    std::getline(std::cin, interrupter);
    multicastsender.stopsender();
    multicastlistener.stoplistener();
    sender.detach();
    listener.detach();
    return 0;
}


