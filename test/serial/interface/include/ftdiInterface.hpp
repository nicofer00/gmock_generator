#ifndef __FTDI_INTERFACE_H__
#define __FTDI_INTERFACE_H__

#include "serialInterface.hpp"
#include "ftdi.hpp"
#include <iostream>

class FTDIInterface : public SerialInterface
{
private:
    FTDI *ftdi;
    std::string read(size_t) override;
    size_t readByte(uint8_t*,size_t) override;
    std::string readline(size_t, std::string) override;
    void write(std::string) override;
    void writeByte(uint8_t*, size_t) override;
    bool isOpen(void) override;
public:
    FTDIInterface(void);
    ~FTDIInterface(void) override;
    static SerialInterface *FTDIExists(void);
    FTDI::FTDIID_t FindFTDI(void);
    Status_t OpenPort(SerialPortNum_t port,
                      uint32_t baud = BAUDRATE) override;
    void ClosePort(void) override;
    void SerialSetTimeout(uint32_t readTimeout,
                          uint32_t writeTimeout) override;
    void ResetOutputBuffer(void) override;
    void ResetInputBuffer(void) override;
    void SetDTR(bool) override;
    void SetRTS(bool) override;
    void SetBaudrate(uint32_t) override;
};

#endif
