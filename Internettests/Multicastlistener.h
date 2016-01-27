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
    //needed static, so that the Object "multicastsender" in findInstances can be deleted before thread ends and no need for join();
    static bool stop;
    static short instancecount; //will cause errors when >1

public:

    Multicastlistener() = default;

    ~Multicastlistener() = default;


    int operator()(u_long interfaceindex)
    {

        instancecount++;

        if(instancecount > 1)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(6000));
        }

        if(instancecount > 1)
        {
            printf("Error, to many Multicastlisteners\n");
            instancecount--;
            return -1;
        }

        WSADATA wsaData;
        struct sockaddr_in saddr;
        struct ip_mreq imreq;
        char recvbuffer[Constants::RECVBUFFERLENGTH];
        //used to get ip-addresses of the executing machine
        char localhostname[128];
        struct addrinfo hints;
        struct addrinfo *hostaddresses, *localaddressiterator;

        //stores all different adresses from which the listener recieved a multicastsender Message
        std::vector <sockaddr_in>  networkinstances;

        //The socket
        int multicastsocket;

        //needed for lengthstoring when sizeof() can�t be used directly
        int getsize;

        //required to check if recvaddress is already stored
        int found;

        //used to free the buffer after reading
        int messagelength;



        //Windows required ...
        if(WSAStartup(MAKEWORD(2,0), &wsaData) != 0)
        {
            fprintf(stderr, "WSAStartup failed.\n");
            instancecount--;
            return -1;
        }


        //setting adressparameters for the multicast adress
        memset(&saddr, 0, sizeof(struct sockaddr_in));
        saddr.sin_family = PF_INET;
        saddr.sin_addr.s_addr = htonl(INADDR_ANY);
        saddr.sin_port = htons(Constants::MULTICASTPORT);


        //seting parameters for multicastgroup (also interface)
        memset(&imreq, 0, sizeof(struct ip_mreq));
        imreq.imr_multiaddr.s_addr = inet_addr(Constants::MULTICASTIP4ADRESS.c_str());
        imreq.imr_interface.s_addr = htonl(interfaceindex);


        //starting UDP Socket
        multicastsocket = socket(AF_INET, SOCK_DGRAM, 0);

        if (multicastsocket < 0)
        {
            perror("Error creating listener socket");
            instancecount--;
            return -1;
        }


        //binding Socket
        if(bind(multicastsocket, (struct sockaddr *)&saddr, sizeof(struct sockaddr_in)) < 0)
        {
            perror("Error binding listener socket");
            instancecount--;
            return -1;
        }


        //adding socket to Multicastgroup
        if(setsockopt(multicastsocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (const char *)&imreq, sizeof(struct ip_mreq)) < 0)
        {
            perror("Error from listener joining Multicastgroup");
            instancecount--;
            return -1;
        }


        //can�t use directly
        getsize = sizeof(struct sockaddr_in);
        std::cout << "\nThe following Instances joined the network:\n";


        //get ip-addresses of the executing machine
        memset(&hints, 0 , sizeof hints);
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_DGRAM;

        gethostname(localhostname, sizeof localhostname);
        if(getaddrinfo(localhostname, NULL, &hints, &hostaddresses) != 0)
        {
            perror("Error getting own hostadresses");
        }


        //starting to recv multicasts
        stop = false;

        while(!stop)
        {
            found = -1;
            recvfrom(multicastsocket, recvbuffer, Constants::RECVBUFFERLENGTH, 0, (struct sockaddr*) &saddr, &getsize);

            //checking for standart Multicastsendermessages
            if(strncmp(recvbuffer, Constants::MULTICASTSENDERMESSAGE.c_str(), Constants::MULTICASTSENDERMESSAGE.length()) == 0)
            {
                for(int n = 0; n < networkinstances.size(); n++)
                {
                    if(networkinstances.at(n).sin_addr.s_addr == saddr.sin_addr.s_addr)
                    {
                        found = n;
                        break;
                    }
                }

                if(found > -1)
                {
                    continue;
                }

                networkinstances.push_back(saddr);
                std::cout << "\n" << inet_ntoa(saddr.sin_addr) << "  " << recvbuffer + Constants::MULTICASTSENDERMESSAGE.length();


                //checking if recieved Message is from same machine
                for(localaddressiterator = hostaddresses; localaddressiterator != NULL; localaddressiterator = localaddressiterator->ai_next)
                {
                    if(localaddressiterator->ai_family == AF_INET)
                    {
                        struct sockaddr_in *ip4localaddr = (struct sockaddr_in *) localaddressiterator->ai_addr;
                        if(ip4localaddr->sin_addr.s_addr == saddr.sin_addr.s_addr)
                        {
                            std::cout << " (locale)";
                            break;
                        }
                    }
                }
            }

            //checking for closing Multicastsendermessages
            if(strncmp(recvbuffer, Constants::MULTICASTSENDERSTOPMESSAGE.c_str(), Constants::MULTICASTSENDERSTOPMESSAGE.length()) == 0)
            {
                for(int n = 0; n < networkinstances.size(); n++)
                {
                    if(networkinstances.at(n).sin_addr.s_addr == saddr.sin_addr.s_addr)
                    {
                        found = n;
                        break;
                    }
                }
                if(found > -1)
                {
                    std::cout << "\n" << inet_ntoa(saddr.sin_addr) <<  " left the network";
                    networkinstances.erase(networkinstances.begin() + found);
                }
            }

            //free buffer
            messagelength = 0;
            while(recvbuffer[messagelength] != 0 && messagelength < Constants::RECVBUFFERLENGTH)
            {
                recvbuffer[messagelength] = 0;
                messagelength++;
            }
        }

        stop = false;

        instancecount--;

        closesocket(multicastsocket);

        freeaddrinfo(hostaddresses);

        WSACleanup();

        return 0;
    }

    void stoplistener()
    {
        stop = true;
    }
};

//bad solution, but ...
bool Multicastlistener::stop = false;
short Multicastlistener::instancecount = 0;

#endif // MULTICASTLISTENER_H_INCLUDED
