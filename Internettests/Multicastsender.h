#ifndef MULTICASTSENDER_H_INCLUDED
#define MULTICASTSENDER_H_INCLUDED


//Multicasts HELLO! in the network so that other instances could recognize this one
class Multicastsender
{
public:
    
    //needed static, so that the Object "multicastsender" in findInstances can be deleted before thread ends and no need for join();
    static bool stop;
    static short instancecount; //will cause errors when >1

    Multicastsender() = default;

    ~Multicastsender() = default;
    
    int operator()(u_long interfaceindex);

    void stopsender();
    
};


#endif // MULTICASTSENDER_H_INCLUDED
