#ifndef MULTICASTLISTENER_H_INCLUDED
#define MULTICASTLISTENER_H_INCLUDED

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string.h>
#include <thread>
#include <chrono>
#include <vector>
#include "Constants.h"


//Listens to multicasts in the network
class Multicastlistener
{
public:
    
    //needed static, so that the Object "multicastsender" in findInstances can be deleted before thread ends and no need for join();
    static bool stop;
    static short instancecount; //will cause errors when >1

    Multicastlistener() = default;

    ~Multicastlistener() = default;

    int operator()(u_long interfaceindex);
    
    void stoplistener();
    
};

#endif // MULTICASTLISTENER_H_INCLUDED
