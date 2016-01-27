

#include <winsock2.h>
#include <ws2ipdef.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <string>
#include <thread>
#include <iostream>
#include "InstancefinderwithIF.h"
#include "Netzwerkinterfacenamen.h"
#include "Multicastsender.h"
#include "Multicastlistener.h"


int findInstanceswithIF()
{

    IP_ADAPTER_INFO* pAdapterInfo;
    IP_ADAPTER_INFO* pAdapter = NULL;
    //needed to calculate the amount of Adapters to store
    ULONG memoryneeded = 0;
    DWORD dwRetVal = 0;
    //used to read User input when selecting the Interface
    std::string userselectinput;
    int selectedInterfaceindex;
    //says, if the User selected Interface exists
    bool interfacefound = false;
    //used to exit Programm
    std::string interrupter;


    if (GetAdaptersInfo(pAdapterInfo, &memoryneeded) == ERROR_BUFFER_OVERFLOW)
    {
        //creates an array of IP_ADAPTER_INFO with hopefully as many entries as needed (replaced the MALLOC function)
        pAdapterInfo = new IP_ADAPTER_INFO[memoryneeded / sizeof (IP_ADAPTER_INFO)];
        if (pAdapterInfo == NULL)
        {
            printf("Error allocating memory needed to call GetAdaptersinfo\n");
            return -1;
        }
    }

    if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &memoryneeded)) != NO_ERROR)
    {
        printf("GetAdaptersInfo failed with error: %d\n", dwRetVal);
        return -1;
    }


    for(pAdapter = pAdapterInfo; pAdapter != NULL; pAdapter = pAdapter->Next)
    {
        printf("\n\nDescription: %s\n", pAdapter->Description);
        printf("Type:        ");
        switch (pAdapter->Type)
        {
            case MIB_IF_TYPE_OTHER:
                printf("Other\n");
                break;
            case MIB_IF_TYPE_ETHERNET:
                printf("Ethernet\n");
                break;
            case MIB_IF_TYPE_TOKENRING:
                printf("Token Ring\n");
                break;
            case MIB_IF_TYPE_FDDI:
                printf("FDDI\n");
                break;
            case MIB_IF_TYPE_PPP:
                printf("PPP\n");
                break;
            case MIB_IF_TYPE_LOOPBACK:
                printf("Lookback\n");
                break;
            case MIB_IF_TYPE_SLIP:
                printf("Slip\n");
                break;
            default:
                printf("Unknown type %ld\n", pAdapter->Type);
                break;
        }
        printf("Index:       %d\n", pAdapter->Index);
    }

    while(!interfacefound)
    {
        printf("\n\nBitte geben sie den Index des gewuenschten Interfaces an,\n alternativ 0 um das Subprogramm zu beenden.\n\n");
        std::cin.sync();
        std::getline(std::cin, userselectinput);
        std::cin.sync();
        
        try
        {
            selectedInterfaceindex = std::stoi(userselectinput);
        }
        catch(...)
        {
            continue;
        }
        
        if(selectedInterfaceindex == 0)
        {
            return 0;
        }
        for(pAdapter = pAdapterInfo; pAdapter != NULL; pAdapter = pAdapter->Next)
        {
            if(selectedInterfaceindex == pAdapter->Index)
            {
                interfacefound = true;
                break;
            }
        }

    }

    printf("\n\nPress RETURN to exit.\n");
    //care, this Object gets deleted after returning from findInstances !!!!
    Multicastsender multicastsender;
    Multicastlistener multicastlistener;
    //starting thread with selected Networkinterface
    std::thread sender(std::ref(multicastsender), selectedInterfaceindex);
    std::thread listener(std::ref(multicastlistener), selectedInterfaceindex);
    std::getline(std::cin, interrupter);
    multicastsender.stopsender();
    multicastlistener.stoplistener();
    sender.detach();
    listener.detach();

    if (pAdapterInfo)
    {
        //replacement for the FREE function
        delete[] pAdapterInfo;
    }

    return 0;
}
