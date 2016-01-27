
#include <string>
#include "Constants.h"

std::string Constants::MULTICASTIP4ADRESS = "226.0.0.1";           //Sender and listeners multicastadress
std::string Constants::MULTICASTSENDERMESSAGE = "HELLO!";          //Senders Multicastmessage
std::string Constants::MULTICASTSENDERSTOPMESSAGE = "FINISHED!";   //Senders Multicastmessage after closing
unsigned short Constants::MULTICASTPORT = 4096;                    //Port for Multicastcommunication
int Constants::INITIALSENDS = 3;                                   //How often should the Sender multicast at start
char Constants::LOOPBACK = 0;                                      //Says, if sender will also send back to own device
int Constants::SENDSLEEPTIME = 5000;                               //Defines the wait time between multicast sends for the Multicastsender
int Constants::RECVBUFFERLENGTH = 128;                             //Defines the bufferlength for Multicastlistener

