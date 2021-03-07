#include "ota.h"
#include <string>
#include <iostream>

extern "C" {    
        #include "otac.h"
    };

    
Ota::Ota()
{
}

list<char> Ota::ProcessMessage(char *buffer,struct in_addr ip)
{
    list<char> ret;
    char* url = substr(buffer,3, (buffer[1]<<8 ) + buffer[2]  );
    bool updated =ota_update(url);//(substr(buffer,3, (buffer[1]<<8 ) + buffer[2]  ));
    
    if (!updated) {
        string err = "OTA failed !";
        for (char c: err) ret.push_back(c);
    }
    //std::cout << "OTA update";
    //DoOTAUpdate(substr(buffer,3, (buffer[1]<<8 ) + buffer[2]  ));
    return ret;
}
char* Ota::substr(char* arr, int begin, int len)
{
    char* res = new char[len];
    for (int i = 0; i < len; i++)
        res[i] = *(arr + begin + i);
    res[len] = 0;
    return res;
}

bool Ota::DoOTAUpdate(char* url)
{
    std::cout << url<<"\n";
    
    return ota_update(url);
    
}
Ota::~Ota()
{
}