
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <map>
#include "Netzwerkinterfacenamen.h"
#include "Instancefinder.h"
#include "InstancefinderwithIF.h"




using namespace std;

enum Selectoptions {EINS, ZWEI, DREI, VIER, EXIT};

map <string, Selectoptions> StringtoOption;

void InitializeOptionmap ()
{
    StringtoOption["1"] = EINS;
    StringtoOption["2"] = ZWEI;
    StringtoOption["3"] = DREI;
    StringtoOption["4"] = VIER;
    StringtoOption["EXIT"] = EXIT;
}

int main()
{
    string Usereingabe;
    int SelectedOption;
    InitializeOptionmap();


    while(1)
    {
        cout << "Bitte eingeben, welches Unterprogramm gewuenscht wird.\n\n"
                "1: Namen der vorhandenen Interfaces\n"
                "2: Namen der vorhandenen Interfaces (mit new[],delete[])\n"
                "3: Finden anderer Computer im Netzwerk, die dieses Subprogramm aufgerufen haben\n"
                "4: Finden anderer Computer im Netzwerk, die dieses Subprogramm aufgerufen haben\n   Mit Angabe des Interfaces ueber das gesucht wird\n"
                "exit: Programm beenden\n\n";

        cin >> Usereingabe;
        cin.sync();

        for (string::size_type i = 0; i < Usereingabe.length(); i++)
        {
            Usereingabe[i] = toupper(Usereingabe[i]);
        }

        try
        {
            SelectedOption = StringtoOption.at(Usereingabe);
        }
        catch(out_of_range)
        {
            continue;
        }

        switch(SelectedOption)
        {
            case EINS: getInterfacenames(); break;
            case ZWEI: getInterfacenames2(); break;
            case DREI: findInstances(); break;
            case VIER: findInstanceswithIF(); break;
            case EXIT: exit(1);
            default: break;
        }

        cout << "\n\n\n";
    }
}
