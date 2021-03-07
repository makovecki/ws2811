#pragma once
#include <string>
using namespace std;
class Storage
{
    uint32_t handle;
    public:
        Storage();
        void SaveString(string key, string value);
        string GetString(string key, string def);
        string GetString(string key);
        ~Storage();
};