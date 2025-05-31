#include "serialInterface.hpp"
#include <vector>
#include <stdint.h>
#include <cstdio>


SerialInterface::SerialInterface(void)
{
#ifndef TEST
    SerialInterface::devices = serial::list_ports();
#endif
}

SerialInterface::~SerialInterface()
{
}

std::string SerialInterface::read(size_t size)
{
    return SerialInterface::serial.read(size);
};

size_t SerialInterface::readByte(uint8_t* buffer, size_t bytesToRead)
{
    return SerialInterface::serial.read(buffer, bytesToRead);
}

std::string SerialInterface::readline(size_t max, std::string eol)
{
    return SerialInterface::serial.readline(max, eol);
}

void SerialInterface::write(std::string string)
{
    SerialInterface::serial.write(string);
}

void SerialInterface::writeByte(uint8_t* bytes, size_t size)
{
    SerialInterface::serial.write(bytes, size);
}

bool SerialInterface::isOpen(void)
{
    return serial.isOpen();
}

SerialPortNums_t SerialInterface::GetPortNums(void)
{
    SerialPortNums_t nums;
    for (serial::PortInfo i : SerialInterface::devices)
    {
        nums.push_back(i.port);
    }
    return nums;
}

Status_t SerialInterface::OpenPort(SerialPortNum_t port, uint32_t baud)
{
    Status_t status = SYS_STATUS_OK;
    std::cout << "Attempting to open " << port << std::endl;
    try
    {
        SerialInterface::serial.setPort(port);
        SerialInterface::serial.setBaudrate(baud);
        SerialInterface::serial.open();
        SerialInterface::serial.setDTR(false);
        SerialInterface::serial.setRTS(false);
    }
    catch (const serial::SerialException &e)
    {
        std::cerr << e.what() << '\n';
        status = SYS_STATUS_FAIL;
    }
    catch (const std::invalid_argument &e)
    {
        std::cerr << e.what() << '\n';
        status = SYS_STATUS_ENODEV;
    }
    return status;
}

void SerialInterface::ClosePort(void)
{
    SerialInterface::serial.close();
}

bool SerialInterface::GetPortOpen(void)
{
    return this->isOpen();
}

std::string SerialInterface::SerialRead(size_t bytesToRead)
{
    std::string ret;
    ret = this->read(bytesToRead);

    return ret;
}

size_t SerialInterface::SerialRead(uint8_t* buffer, size_t bytesToRead)
{
    size_t ret;
    ret = this->readByte(buffer,bytesToRead);

    return ret;
}

std::string SerialInterface::SerialReadUntil(std::string end)
{
    std::string ret;
    ret = this->readline(MAX_BYTES_READ, end);

    return ret;
}

ReadStatus_t SerialInterface::SerialReadUntil(std::string end,
        std::string &buffer)
{
    buffer = this->readline(MAX_BYTES_READ, end);
    ReadStatus_t ret;

    if (buffer.find(end) == std::string::npos)
    {
        ret = READ_FAIL;
    }
    else if (buffer.length() >= MAX_BYTES_READ)
    {
        ret = READ_MAX_SIZE;
    }
    else
    {
        ret = READ_OK;
    }
    return ret;
}

void SerialInterface::SerialWrite(std::string command)
{
    this->write(command);
}

void SerialInterface::SerialWrite(uint8_t *data, size_t size)
{
    this->writeByte(data, size);
}

void SerialInterface::SerialSetTimeout(uint32_t readTimeout,
                                       uint32_t writeTimeout)
{
    SerialInterface::serial.setTimeout(serial::Timeout::max(), readTimeout, 1,
                                       writeTimeout, 1);
}

void SerialInterface::EnumeratePorts(void)
{
    SerialPortInfo_t::iterator iter = devices.begin();
    std::cout << "Port Options:\n";
    while (iter != devices.end())
    {
        serial::PortInfo device = *iter++;
        printf("    %s, %s, %s\n", device.port.c_str(), device.description.c_str(),
               device.hardware_id.c_str());
    }
}

void SerialInterface::HardReset(SerialPortNum_t port)
{
    this->SetRTS(true);
    sleep(100);
    this->SetRTS(false);
    this->ClosePort();
}

void SerialInterface::ResetOutputBuffer(void)
{
    SerialInterface::serial.flushOutput();
}

void SerialInterface::ResetInputBuffer(void)
{
    SerialInterface::serial.flushInput();
}

void SerialInterface::SetDTR(bool level)
{
    SerialInterface::serial.setDTR(level);
}

void SerialInterface::SetRTS(bool level)
{
    SerialInterface::serial.setRTS(level);
}

void SerialInterface::SetBaudrate(uint32_t baud)
{
    SerialInterface::serial.setBaudrate(baud);
}
