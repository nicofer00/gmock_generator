#ifndef __SERIAL_INTERFACE_H__
#define __SERIAL_INTERFACE_H__

#include <iostream>
#include <functional>
#include "serial/serial.h"
#include "const.h"


#define MAX_BYTES_READ 65536
#define BAUDRATE 115200

typedef std::vector<serial::PortInfo> SerialPortInfo_t;
typedef serial::Serial SerialPort_t;
typedef std::string SerialPortNum_t;
typedef std::vector<std::string> SerialPortNums_t;

typedef enum
{
    READ_FAIL = 0U,
    READ_OK,
    READ_MAX_SIZE,
} ReadStatus_t;

class SerialInterface
{
private:
    SerialPortInfo_t devices;
    SerialPort_t serial;

    virtual std::string read(size_t);
    virtual size_t readByte(uint8_t*,size_t);
    virtual std::string readline(size_t, std::string);
    virtual void write(std::string);
    virtual void writeByte(uint8_t*, size_t);
    virtual bool isOpen(void);

public:
    SerialInterface(void);
    virtual ~SerialInterface();
    virtual SerialPortNums_t GetPortNums(void);
    virtual bool GetPortOpen(void);
    virtual std::string SerialRead(size_t bytesToRead);
    virtual size_t SerialRead(uint8_t* buffer, size_t bytesToRead);
    virtual std::string SerialReadUntil(std::string end);
    virtual ReadStatus_t SerialReadUntil(std::string end, std::string &buffer);
    virtual void SerialWrite(std::string command);
    virtual void SerialWrite(uint8_t *data, size_t size);
    virtual void EnumeratePorts(void);
    virtual void HardReset(SerialPortNum_t port);

    virtual Status_t OpenPort(SerialPortNum_t port, uint32_t baud = BAUDRATE);
    virtual void ClosePort(void);
    virtual void SerialSetTimeout(uint32_t readTimeout, uint32_t writeTimeout);
    virtual void ResetOutputBuffer(void);
    virtual void ResetInputBuffer(void);
    virtual void SetDTR(bool);
    virtual void SetRTS(bool);
    virtual void SetBaudrate(uint32_t);
};

#endif // __SERIAL_INTERFACE_H__
