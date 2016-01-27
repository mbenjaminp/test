
#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string.h>
#include <thread>
#include <chrono>
#include "Constants.h"
#include "Multicastsender.h"

//bad solution, but ...
bool Multicastsender::stop = false;
short Multicastsender::instancecount = 0;

int Multicastsender::operator ()(u_long interfaceindex)
{
    
    instancecount++;
    
    if(instancecount > 1)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(6000));
    }
    
    if(instancecount > 1)
    {
        printf("Error, to many Multicastsenders\n");            instancecount--;
        return -1;
    }
    
    WSADATA wsaData;
    struct sockaddr_in saddr;
    int multicastsocket;
    char localhostname[128];
    std::string localhostnamestr;
    
    //setting adressparameters for the multicast adress
    memset(&saddr, 0, sizeof(struct sockaddr_in));
    saddr.sin_family = PF_INET;
    saddr.sin_addr.s_addr = inet_addr(Constants::MULTICASTIP4ADRESS.c_str());
    saddr.sin_port = htons(Constants::MULTICASTPORT);
    
    //Windows required ...
    if(WSAStartup(MAKEWORD(2,0), &wsaData) != 0)
    {
        fprintf(stderr, "WSAStartup failed.\n");
        instancecount--;
        return -1;
    }
    
    
    //starting UDP Socket
    multicastsocket = socket(AF_INET, SOCK_DGRAM, 0);
    
    if (multicastsocket < 0)
    {
        perror("Error creating socket");
        instancecount--;
        return -1;
    }
    
    //choosing Interface for sending
    interfaceindex = htonl(interfaceindex);
    if(setsockopt(multicastsocket, IPPROTO_IP, IP_MULTICAST_IF, (const char *) &interfaceindex, sizeof(interfaceindex)) < 0)
    {
        perror("Error selecting Interface");
        instancecount--;
        return -1;
    }
    
    //disable loopback
    if(setsockopt(multicastsocket, IPPROTO_IP, IP_MULTICAST_LOOP, (const char *) &Constants::LOOPBACK, sizeof(Constants::LOOPBACK)) < 0)
    {
        perror("Error setting loopback option");
        instancecount--;
        return -1;
    }
    
    //get own hostname
    gethostname(localhostname, sizeof localhostname);
    localhostnamestr = localhostname;
    
    //Initialsends
    for(int n = 0; n < Constants::INITIALSENDS; n++)
    {
        if(sendto(multicastsocket, (Constants::MULTICASTSENDERMESSAGE + localhostnamestr).c_str(), Constants::MULTICASTSENDERMESSAGE.length() + localhostnamestr.length(), 0, (struct sockaddr *)&saddr, sizeof(struct sockaddr_in)) < 0)
        {
            perror("sendto failed");
            instancecount--;
            return -1;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    stop = false;
    
    //sendloop with
    while(!stop)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(Constants::SENDSLEEPTIME));
        if(sendto(multicastsocket, (Constants::MULTICASTSENDERMESSAGE + localhostnamestr).c_str(), Constants::MULTICASTSENDERMESSAGE.length() + localhostnamestr.length(), 0, (struct sockaddr *)&saddr, sizeof(struct sockaddr_in)) < 0)
        {
            perror("sendto failed");
            instancecount--;
            return -1;
        }
    }
    
    stop = false;
    
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    
    if(sendto(multicastsocket, (Constants::MULTICASTSENDERSTOPMESSAGE + localhostname).c_str(), (Constants::MULTICASTSENDERSTOPMESSAGE + localhostname).length(), 0, (struct sockaddr *)&saddr, sizeof(struct sockaddr_in)) < 0)
    {
        perror("sendto failed");
        instancecount--;
        return -1;
    }
    
    closesocket(multicastsocket);
    
    WSACleanup();
    
    instancecount--;
    
    return 0;
}

void Multicastsender::stopsender()
{
    stop = true;
}