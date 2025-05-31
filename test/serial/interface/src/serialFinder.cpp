#include "serialFinder.hpp"
#include "ftdiInterface.hpp"

SerialFinder::SerialFinder(void) :
    find(
{
    FTDIInterface::FTDIExists,
})
{
    serial = NULL;
}

SerialFinder::~SerialFinder()
{
    if (serial)
    {
        delete serial;
    }
}

SerialInterface *SerialFinder::GetSerialInterface(SerialPortNum_t port)
{
    if (port.empty())
    {
        for (size_t i = 0; i < find.size() && serial == NULL; i++)
        {
            serial = find[i]();
        }
    }
    if (serial == NULL)
    {
        serial = new SerialInterface();
    }
    return serial;
}
