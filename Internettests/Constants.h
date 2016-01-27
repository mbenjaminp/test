#ifndef CONSTANTS_H_INCLUDED
#define CONSTANTS_H_INCLUDED

#include <string>

class Constants
{
public:
    static std::string MULTICASTIP4ADRESS;                   //Sender and listeners multicastadress
    static std::string MULTICASTSENDERMESSAGE;               //Senders Multicastmessage
    static std::string MULTICASTSENDERSTOPMESSAGE;           //Senders Multicastmessage after closing
    static unsigned short MULTICASTPORT;                     //Port for Multicastcommunication
    static int INITIALSENDS;                                 //How often should the Sender multicast at start
    static char LOOPBACK;                                    //Says, if sender will also send back to own device
    static int SENDSLEEPTIME;                                //Defines the wait time between multicast sends for the Multicastsender
    static int RECVBUFFERLENGTH;                             //Defines the bufferlength for Multicastlistener
    static unsigned long RECVFROMTIMEOUTMILLISEC;                      //Defines the timeout (Seconds) for the Multicastlisteners recvfrom
 
};

#endif // CONSTANTS_H_INCLUDED
