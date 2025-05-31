#ifndef __SERIAL_FINDER_H__
#define __SERIAL_FINDER_H__

#include "serialInterface.hpp"
#include <map>

class SerialFinder
{
private:
    SerialInterface *serial;
    std::vector<SerialInterface *(*)(void)> find;
public:
    SerialFinder(void);
    ~SerialFinder();
    SerialInterface *GetSerialInterface(SerialPortNum_t port);
};

#endif
