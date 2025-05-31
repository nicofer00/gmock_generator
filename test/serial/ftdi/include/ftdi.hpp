#ifndef __FTDI_H__
#define __FTDI_H__

#include "ftd2xx.h"
#include "const.h"
#include <iostream>
#include <vector>

#define FTDI_SERIAL_NUM_LEN (9U)
#define FTDI_CHAR_BUFFER_LEN (64U)

class FTDI
{

public:
    typedef std::string FTDIDeviceDescription_t;
    typedef DWORD FTDIID_t;
    typedef std::vector<FTDIID_t> FTDIIDs_t;
    typedef enum
    {
        PROGRAM,
        READ,
        GET,
        RESET
    } FTDICommandType_t;

    typedef enum
    {
        NONE = FT_FLOW_NONE,
        DTR_DSR = FT_FLOW_DTR_DSR,
        RTS_CTS = FT_FLOW_RTS_CTS,
        XON_XOFF = FT_FLOW_XON_XOFF
    } FTDIFlowType_t;

    void SetDescription(FTDIID_t id, FTDIDeviceDescription_t desc);
    FT_STATUS GetDescription(FTDIID_t id, FTDIDeviceDescription_t *desc);
    FTDIIDs_t GetIDs(void);
    FT_STATUS ResetByDescription(FTDIDeviceDescription_t desc);
    FTDI(void);
    ~FTDI();
    Status_t Open(FTDIID_t id, DWORD baudRate);
    Status_t Close(void);
    bool IsOpen(void);
    std::string Read(size_t bytes);
    size_t Read(char* buffer, size_t numBytes);
    std::string Readline(size_t maxBytes, std::string eol);
    Status_t PurgeInputBuffer(void);
    Status_t PurgeOutputBuffer(void);
    void Write(std::string line);
    void Write(char* bytes, size_t size);
    Status_t SetTimeout(DWORD readTimeout, DWORD writeTimeout);
    Status_t SetDtr(bool high);
    Status_t SetRts(bool high);
    Status_t SetBaudRate(DWORD baudrate);
    Status_t SetFlowControl(FTDIFlowType_t flowType);

private:
    typedef struct
    {
        FTDIID_t id;
        FTDIDeviceDescription_t description;
    } description_t;
    struct
    {
        DWORD ftFlags;
        DWORD ftId;
        DWORD ftType;
        DWORD ftLocId;
        char ftManufacturer[FTDI_CHAR_BUFFER_LEN];
        char ftManufacturerId[FTDI_CHAR_BUFFER_LEN];
        char ftDescription[FTDI_CHAR_BUFFER_LEN];
        char ftSerialNumber[FTDI_CHAR_BUFFER_LEN];
        FT_HANDLE ftHandle;
        FT_EEPROM_X_SERIES ft_eeprom_x_series;
        LONG ftCOMNumber;
    } ftdi = {0};

    FT_HANDLE ftdiOpenHandle;
    DWORD ftdiReadTimeout;
    DWORD ftdiWriteTimeout;

    FT_STATUS loopFTDI(FTDICommandType_t command,
                       void *arg);
    FT_STATUS getFTDIID(DWORD i,
                        FTDIIDs_t *id);
    FT_STATUS getFTDIDescription(DWORD i,
                                 description_t *desc);
    FT_STATUS progFTDIChip(DWORD i,
                           description_t *desc);
    FT_STATUS resetFTDIChip(DWORD i,
                            description_t *desc);
};

#endif // __FTDI_H__
