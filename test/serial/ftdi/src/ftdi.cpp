#include "ftdi.hpp"
#include "serialInterface.hpp"
#include "const.h"

#ifdef __unix__
#include <unistd.h>
#define SETUP()                                   \
    if (system("/usr/local/bin/bind_ftdi.sh -u")) \
        std::cout << "Failed to run script bind_ftdi.sh -u" << std::endl;
#define TEARDOWN()                                \
    if (system("/usr/local/bin/bind_ftdi.sh -b")) \
        std::cout << "Failed to run script bind_ftdi.sh -b" << std::endl;
#define RESET_FTDI(handle) \
    FT_ResetDevice(handle)
#else
#define SETUP()
#define TEARDOWN()
#define RESET_FTDI(handle) \
    FT_ResetPort(handle)
#include <windows.h>
#endif
#include <stdio.h>
#include <string.h>
#define READLINE_BYTE_NUM 1
#define FTDI_DEVICE_DESCRIPTION "FT231X USB UART"
#define FTDI_VENDOR_ID (0x0403)
#define FTDI_PRODUCT_ID (0x6015)

FTDI::FTDI(void)
{
    static const WORD VendorIdBuf = FTDI_VENDOR_ID;
    static const WORD ProductIdBuf = FTDI_PRODUCT_ID;
    SETUP();
    FTDI::ftdi.ft_eeprom_x_series.common.deviceType = FT_DEVICE_X_SERIES;
    FTDI::ftdi.ft_eeprom_x_series.common.VendorId = VendorIdBuf;
    FTDI::ftdi.ft_eeprom_x_series.common.ProductId = ProductIdBuf;
    ftdiOpenHandle = nullptr;
}

FT_STATUS FTDI::loopFTDI(FTDICommandType_t command, void *arg)
{
    FT_STATUS ftStatus = FT_OK;
    DWORD numDevs = 0U;

    ftStatus = FT_CreateDeviceInfoList(&numDevs);
    if (ftStatus != FT_OK)
    {
        std::cout << "ERROR: FT_CreateDeviceInfoList failed." << std::endl;
    }
    else if (numDevs == 0)
    {
        std::cout << "ERROR: No devices connected." << std::endl;
        ftStatus = FT_DEVICE_NOT_FOUND;
    }
    for (DWORD i = 0; i < numDevs && ftStatus == FT_OK; i++)
    {
        switch (command)
        {
            case GET:
                ftStatus = getFTDIID(i,
                                     static_cast<FTDI::FTDIIDs_t *>(arg));
                break;
            case READ:
                ftStatus = getFTDIDescription(i,
                                              static_cast<FTDI::description_t *>(arg));
                break;
            case PROGRAM:
                ftStatus = progFTDIChip(i,
                                        static_cast<FTDI::description_t *>(arg));
                break;
            case RESET:
                ftStatus = resetFTDIChip(i,
                                         static_cast<FTDI::description_t *>(arg));
                break;
            default:
                break;
        }
    }

    return ftStatus;
}

FT_STATUS FTDI::getFTDIID(DWORD i, FTDI::FTDIIDs_t *id)
{
    FT_STATUS ftStatus = FT_OK;
    ftStatus = FT_GetDeviceInfoDetail(i, &FTDI::ftdi.ftFlags, &FTDI::ftdi.ftType,
                                      &FTDI::ftdi.ftId, &FTDI::ftdi.ftLocId,
                                      FTDI::ftdi.ftSerialNumber,
                                      FTDI::ftdi.ftDescription,
                                      &FTDI::ftdi.ftHandle);
    {
        if (ftStatus != FT_OK)
        {
            std::cout << "ERROR: FT_GetDeviceInfoDetail failed" << std::endl;
        }
        else
        {
            ftStatus = FT_OpenEx(FTDI::ftdi.ftDescription, FT_OPEN_BY_DESCRIPTION,
                                 &FTDI::ftdi.ftHandle);
            if (ftStatus != FT_OK)
            {
                std::cout << "ERROR: FT_OpenEx failed." << std::endl;
                ftStatus = FT_Close(FTDI::ftdi.ftHandle);
            }
            else
            {
                id->push_back(FTDI::ftdi.ftId);
                ftStatus = FT_Close(FTDI::ftdi.ftHandle);
            }
        }
    }

    return ftStatus;
}

FT_STATUS FTDI::getFTDIDescription(DWORD i, FTDI::description_t *desc)
{
    FT_STATUS ftStatus = FT_OK;
    ftStatus = FT_GetDeviceInfoDetail(i, &FTDI::ftdi.ftFlags, &FTDI::ftdi.ftType,
                                      &FTDI::ftdi.ftId, &FTDI::ftdi.ftLocId,
                                      FTDI::ftdi.ftSerialNumber,
                                      FTDI::ftdi.ftDescription,
                                      &FTDI::ftdi.ftHandle);
    if (desc->id == ftdi.ftId)
    {
        desc->description = ftdi.ftDescription;
    }
    return ftStatus;
}

FT_STATUS FTDI::progFTDIChip(DWORD i,
                             FTDI::description_t *desc)
{
    FT_STATUS ftStatus = FT_OK;
    ftStatus = FT_GetDeviceInfoDetail(i, &FTDI::ftdi.ftFlags, &FTDI::ftdi.ftType,
                                      &FTDI::ftdi.ftId, &FTDI::ftdi.ftLocId,
                                      FTDI::ftdi.ftSerialNumber,
                                      FTDI::ftdi.ftDescription,
                                      &FTDI::ftdi.ftHandle);
    if (desc->id == ftdi.ftId)
    {
        ftStatus = FT_OpenEx(FTDI::ftdi.ftDescription, FT_OPEN_BY_DESCRIPTION,
                             &FTDI::ftdi.ftHandle);
        ftStatus = FT_EEPROM_Program(FTDI::ftdi.ftHandle,
                                     &FTDI::ftdi.ft_eeprom_x_series,
                                     sizeof(FTDI::ftdi.ft_eeprom_x_series),
                                     FTDI::ftdi.ftManufacturer,
                                     FTDI::ftdi.ftManufacturerId,
                                     (char *)desc->description.c_str(),
                                     FTDI::ftdi.ftSerialNumber);
        if (ftStatus != FT_OK)
        {
            std::cout << "ERROR: FT_EEPROM_Program status not ok: "
                      << ftStatus << std::endl;
        }
        else
        {
            std::cout << "SUCCESS: EEPROM programmed." << std::endl;
        }
        ftStatus = FT_Close(FTDI::ftdi.ftHandle);
    }

    return ftStatus;
}

FT_STATUS FTDI::resetFTDIChip(DWORD i,
                              description_t *desc)
{
    FT_STATUS ftStatus = FT_OK;
    FT_HANDLE handle;
    ftStatus = FT_GetDeviceInfoDetail(i, &FTDI::ftdi.ftFlags, &FTDI::ftdi.ftType,
                                      &FTDI::ftdi.ftId, &FTDI::ftdi.ftLocId,
                                      FTDI::ftdi.ftSerialNumber,
                                      FTDI::ftdi.ftDescription,
                                      &FTDI::ftdi.ftHandle);
    if (desc->description == ftdi.ftDescription)
    {
        FT_Open(i, &handle);
        ftStatus = RESET_FTDI(handle);
        FT_Close(handle);
    }
    return ftStatus;
}

void FTDI::SetDescription(FTDI::FTDIID_t id,
                          FTDI::FTDIDeviceDescription_t desc)
{
    FTDI::description_t s =
    {
        id,
        desc,
    };
    loopFTDI(PROGRAM, &s);
}

FT_STATUS FTDI::GetDescription(FTDI::FTDIID_t id,
                               FTDI::FTDIDeviceDescription_t *desc)
{
    FTDI::description_t s =
    {
        id,
        "",
    };
    FT_STATUS status = this->loopFTDI(READ, &s);
    *desc = s.description;
    if (status != FT_OK)
    {
        *desc = "";
    }
    return status;
}

FTDI::FTDIIDs_t FTDI::GetIDs(void)
{
    FTDI::FTDIIDs_t ids;
    FT_STATUS status = this->loopFTDI(GET, &ids);
    if (status != FT_OK)
    {
        ids.clear();
    }
    return ids;
}

FT_STATUS FTDI::ResetByDescription(FTDIDeviceDescription_t desc)
{
    FTDI::description_t s =
    {
        0,
        desc,
    };
    FT_STATUS status = this->loopFTDI(RESET, &s);
    return status;
}

FTDI::~FTDI()
{
    TEARDOWN();
}

Status_t FTDI::Open(FTDI::FTDIID_t id, DWORD baudRate)
{
    Status_t ret = SYS_STATUS_OK;
    FT_STATUS ftStatus;
    FTDI::FTDIDeviceDescription_t description;
    ftStatus = GetDescription(id, &description);
    ftStatus = FT_OpenEx((PVOID)description.c_str(), FT_OPEN_BY_DESCRIPTION,
                         &ftdiOpenHandle);
    if (ftStatus != FT_OK)
    {
        std::cout << "ERROR: FT_OpenEx status not ok: "
                  << ftStatus << std::endl;
        ret = SYS_STATUS_OK;
    }

    ftStatus = RESET_FTDI(ftdiOpenHandle);
    if (ftStatus != FT_OK)
    {
        std::cout << "ERROR: FT_ResetPort status not ok, closing handle: "
                  << ftStatus << std::endl;
        ret = SYS_STATUS_OK;
    }

    ftStatus = FT_SetBaudRate(ftdiOpenHandle, baudRate);
    if (ftStatus != FT_OK)
    {
        std::cout << "ERROR: FT_SetBaudRate status not ok, closing handle: "
                  << ftStatus << std::endl;
        Close();
        ret = SYS_STATUS_OK;
    }

    ftStatus = FT_SetTimeouts(ftdiOpenHandle, ftdiReadTimeout, ftdiWriteTimeout);
    if (ftStatus != FT_OK)
    {
        std::cout << "ERROR: FT_SetTimeouts status not ok: "
                  << ftStatus << std::endl;
        ret = SYS_STATUS_OK;
    }

    ftStatus = FT_Purge(ftdiOpenHandle, FT_PURGE_RX);
    if (ftStatus != FT_OK)
    {
        std::cout << "ERROR: FT_Purge status not ok: "
                  << ftStatus << std::endl;
        ret = SYS_STATUS_OK;
    }
    return ret;
}

Status_t FTDI::Close(void)
{
    Status_t ret = SYS_STATUS_FAIL;

    if (ftdiOpenHandle)
    {
        FT_STATUS ftStatus = FT_Close(ftdiOpenHandle);
        if (ftStatus != FT_OK)
        {
            std::cout << "ERROR: FT_Close status not ok: "
                      << ftStatus << std::endl;
        }
        ftdiOpenHandle = nullptr;
        ret = SYS_STATUS_OK;
    }
    return ret;
}

bool FTDI::IsOpen(void)
{
    bool ret = (ftdiOpenHandle != nullptr);
    return ret;
}

std::string FTDI::Read(size_t numBytes)
{
    std::string ret;
    char buffer[1024] = {0};
    DWORD bytesReturned = 0;
    DWORD bytesToRead = numBytes;
    FT_STATUS ftStatus;

    ftStatus = FT_Read(ftdiOpenHandle, buffer, bytesToRead, &bytesReturned);
    if (ftStatus != FT_OK)
    {
        std::cout << "ERROR: FT_Read status not ok: "
                  << ftStatus << std::endl;
    }
    ret = buffer;
    return ret;
}

size_t FTDI::Read(char* buffer, size_t numBytes)
{

    DWORD bytesReturned = 0;
    DWORD bytesToRead = numBytes;
    FT_STATUS ftStatus;

    ftStatus = FT_Read(ftdiOpenHandle, buffer, bytesToRead, &bytesReturned);
    if (ftStatus != FT_OK)
    {
        std::cout << "ERROR: FT_Read status not ok: "
                  << ftStatus << std::endl;
    }
    return bytesReturned;
}

void FTDI::Write(std::string line)
{
    FT_STATUS ftStatus;
    DWORD bytesWritten = 0;
    char *buffer = (char *)line.c_str();
    DWORD bytesToWrite = line.length();
    // clear the current read and write buffer before writing - makes any further reads
    // occur at the start of the next write
    // This mimicks the behavior of serial library

    ftStatus = FT_Purge(ftdiOpenHandle, FT_PURGE_RX | FT_PURGE_TX);

    if (ftStatus != FT_OK)
    {
        std::cout << "ERROR: FT_Purge status not ok: "
                  << ftStatus << std::endl;
    }

    ftStatus = FT_Write(ftdiOpenHandle, buffer, bytesToWrite, &bytesWritten);
    if (ftStatus != FT_OK)
    {
        std::cout << "ERROR: FT_Write status not ok: "
                  << ftStatus << std::endl;
    }
}


void FTDI::Write(char* bytes, size_t size)
{
    FT_STATUS ftStatus;
    DWORD bytesWritten = 0;

    ftStatus = FT_Write(ftdiOpenHandle, bytes, size, &bytesWritten);
    if (ftStatus != FT_OK)
    {
        std::cout << "ERROR: FT_Write status not ok: "
                  << ftStatus << std::endl;
    }

}

std::string FTDI::Readline(size_t maxBytes, std::string eol)
{
    FT_STATUS ftStatus;

    std::string ret;
    std::string::iterator stringIt;
    DWORD bytesReturned = 0;
    DWORD bytesToRead = READLINE_BYTE_NUM;
    DWORD totalBytes = 0;
    char buffer = 0;
    while (true)
    {
        ftStatus = FT_Read(ftdiOpenHandle, &buffer, sizeof(buffer), &bytesReturned);
        totalBytes += bytesReturned;
        if (ftStatus != FT_OK)
        {
            std::cout << "ERROR: FT_Read status not ok: " << ftStatus << std::endl;
            break;
        }
        else
        {
            ret.push_back(buffer);
            if (ret.find(eol, ret.length() - eol.length()) != std::string::npos)
            {
                break;
            }
            else if ((bytesReturned != bytesToRead) || (totalBytes >= maxBytes))
            {
                break;
            }
        }
    }
    return ret;
}

Status_t FTDI::PurgeInputBuffer(void)
{
    FT_STATUS ftStatus;
    Status_t ret = SYS_STATUS_FAIL;
    if (ftdiOpenHandle)
    {
        ftStatus = FT_Purge(ftdiOpenHandle, FT_PURGE_RX);
        if (ftStatus == FT_OK)
        {
            ret = SYS_STATUS_OK;
        }
    }
    return ret;
}

Status_t FTDI::PurgeOutputBuffer(void)
{
    FT_STATUS ftStatus;
    Status_t ret = SYS_STATUS_FAIL;
    if (ftdiOpenHandle)
    {
        ftStatus = FT_Purge(ftdiOpenHandle, FT_PURGE_TX);
        if (ftStatus == FT_OK)
        {
            ret = SYS_STATUS_OK;
        }
    }
    return ret;
}

Status_t FTDI::SetTimeout(DWORD readTimeout, DWORD writeTimeout)
{
    Status_t ret = SYS_STATUS_FAIL;

    ftdiReadTimeout = readTimeout;
    ftdiWriteTimeout = writeTimeout;
    FT_STATUS ftStatus;
    if (ftdiOpenHandle)
    {
        ftStatus = FT_SetTimeouts(ftdiOpenHandle, readTimeout, writeTimeout);
        if (ftStatus != FT_OK)
        {
            std::cout << "ERROR: FT_SetTimeouts status not ok: "
                      << ftStatus << std::endl;
        }
        ret = SYS_STATUS_OK;
    }
    return ret;
}

Status_t FTDI::SetDtr(bool high)
{
    Status_t ret = SYS_STATUS_FAIL;
    FT_STATUS ftStatus;
    if (ftdiOpenHandle)
    {
        if(high)
        {
            ftStatus = FT_SetDtr(ftdiOpenHandle);
        }
        else
        {
            ftStatus = FT_ClrDtr(ftdiOpenHandle);
        }
        if (ftStatus != FT_OK)
        {
            std::cout << "ERROR: FT_SetDtr status not ok: "
                      << ftStatus << std::endl;
        }
        ret = SYS_STATUS_OK;
    }
    return ret;
}


Status_t FTDI::SetRts(bool high)
{
    Status_t ret = SYS_STATUS_FAIL;
    FT_STATUS ftStatus;
    if (ftdiOpenHandle)
    {
        if(high)
        {
            ftStatus = FT_SetRts(ftdiOpenHandle);
        }
        else
        {
            ftStatus = FT_ClrRts(ftdiOpenHandle);
        }
        if (ftStatus != FT_OK)
        {
            std::cout << "ERROR: FT_SetRts status not ok: "
                      << ftStatus << std::endl;
        }
        ret = SYS_STATUS_OK;
    }
    return ret;
}

Status_t FTDI::SetBaudRate(DWORD baudrate)
{
    Status_t ret = SYS_STATUS_FAIL;
    FT_STATUS ftStatus;
    if (ftdiOpenHandle)
    {
        ftStatus = FT_SetBaudRate(ftdiOpenHandle, (ULONG)baudrate);

        if (ftStatus != FT_OK)
        {
            std::cout << "ERROR: FT_SetBaudRate status not ok, closing handle: "
                      << ftStatus << std::endl;
            Close();
        }
        ret = SYS_STATUS_OK;
    }

    return ret;
}

Status_t FTDI::SetFlowControl(FTDIFlowType_t flowType)
{
    Status_t ret = SYS_STATUS_FAIL;
    FT_STATUS ftStatus;
    if (ftdiOpenHandle)
    {
        ftStatus = FT_SetFlowControl(ftdiOpenHandle, flowType, 0x11, 0x13);

        if (ftStatus != FT_OK)
        {
            std::cout << "ERROR: FT_SetRts status not ok: "
                      << ftStatus << std::endl;
        }
        ret = SYS_STATUS_OK;
    }
    return ret;
}
