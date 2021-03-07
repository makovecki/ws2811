#include "echoprocessor.h"
#include <list>
#include "storage.h"

IProcessor::~IProcessor()
{
    
}

EchoProcessor::EchoProcessor()
{
}

list<char> EchoProcessor::ProcessMessage(char *buffer,struct in_addr ip)
{
    Storage s;
    string name = s.GetString("name","ws2811");
    list<char> ret(name.begin(), name.end());
    return ret;
}
EchoProcessor::~EchoProcessor()
{
}