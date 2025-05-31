#include "ftdiInterface.hpp"
#define FTDI_CHECK(f) if (ftdi == nullptr) { ftdi = new FTDI(); } f;
#define FTDI_CHECK_RETURN(f) if (ftdi == nullptr) { ftdi = new FTDI(); } return f;

FTDIInterface::FTDIInterface(void)
{
    ftdi = nullptr;
}

FTDIInterface::~FTDIInterface()
{
    if (ftdi != NULL)
    {
        delete ftdi;
    }
}

std::string FTDIInterface::read(size_t size)
{
    FTDI_CHECK_RETURN(ftdi->Read(size));
};

size_t FTDIInterface::readByte(uint8_t* buffer, size_t bytesToRead)
{
    FTDI_CHECK_RETURN(ftdi->Read((char*) buffer, bytesToRead));
}

std::string FTDIInterface::readline(size_t max, std::string eol)
{
    FTDI_CHECK_RETURN(ftdi->Readline(max, eol));
}

void FTDIInterface::write(std::string string)
{
    FTDI_CHECK_RETURN(ftdi->Write(string));
}

void FTDIInterface::writeByte(uint8_t* bytes, size_t size)
{
    FTDI_CHECK(ftdi->Write((char *)bytes, size));
}

bool FTDIInterface::isOpen(void)
{
    FTDI_CHECK_RETURN(ftdi->IsOpen());
}

SerialInterface *FTDIInterface::FTDIExists(void)
{
    SerialInterface *ftdi = NULL;
    FTDI find;
    if (find.GetIDs().size() > 0)
    {
        ftdi = new FTDIInterface();
    }
    return ftdi;
}

FTDI::FTDIID_t FTDIInterface::FindFTDI(void)
{
    FTDI::FTDIID_t id = 0;
    FTDI::FTDIIDs_t nums = ftdi->GetIDs();
    FTDI::FTDIIDs_t smashNums;

    for (FTDI::FTDIID_t i : nums)
    {
        FTDI::FTDIDeviceDescription_t desc;
        ftdi->GetDescription(i, &desc);
        if (desc == "SMASH")
        {
            smashNums.push_back(i);
        }
    }
    if (smashNums.size() == 1)
    {
        std::cout <<
                  "Only one FTDI chip detected with description 'SMASH', using its ID.\n";
        id = smashNums[0];
    }
    else if (nums.size() == 1)
    {
        FTDI::FTDIDeviceDescription_t desc;
        if (ftdi->GetDescription(nums[0], &desc) != FT_OK)
        {
            std::cout <<
                      "Invalid COM port specified. No devices connected to specified port.\n";
        }
        else
        {
            std::cout << "FTDI description is not 'SMASH'."
                      " Description Read: "
                      << desc
                      << std::endl
                      << "Setting description to that"
                      " and cycling COM port for device."
                      " (Will take a few seconds)\n";
            char description[FTDI_SERIAL_NUM_LEN] = "SMASH";
            ftdi->SetDescription(nums[0], description);
            id = nums[0];
        }
    }
    else if (nums.empty())
    {
        std::cout << "No COM port attached,"
                  " please plug one in and try again"
                  << std::endl;
    }
    else
    {
        std::cout << "Too many COM ports attached,"
                  " please plug only one in and try again"
                  << std::endl;
    }
    return id;
}

Status_t FTDIInterface::OpenPort(SerialPortNum_t port, uint32_t baud)
{
    Status_t status = SYS_STATUS_OK;
    std::cout << "No port specified, searching..." << std::endl;
    FTDI::FTDIID_t id = 0;
    FTDI_CHECK(id = FindFTDI());
    if (id)
    {
        ftdi->Open(id, baud);
    }
    else
    {
        std::cout << "No FTDI port found, specify one of the following with -S <port>"
                  << std::endl;
        EnumeratePorts();
        status = SYS_STATUS_ENODEV;
    }
    return status;
}

void FTDIInterface::ClosePort(void)
{
    FTDI_CHECK(ftdi->Close());
}

void FTDIInterface::SerialSetTimeout(uint32_t readTimeout,
                                     uint32_t writeTimeout)
{
    FTDI_CHECK(FTDI_CHECK(ftdi->SetTimeout(readTimeout, writeTimeout)));
}

void FTDIInterface::ResetOutputBuffer(void)
{
    FTDI_CHECK(ftdi->PurgeOutputBuffer());
}

void FTDIInterface::ResetInputBuffer(void)
{
    FTDI_CHECK(ftdi->PurgeInputBuffer());
}

void FTDIInterface::SetDTR(bool level)
{
    FTDI_CHECK(ftdi->SetDtr(level));
}

void FTDIInterface::SetRTS(bool level)
{
    FTDI_CHECK(ftdi->SetRts(level));
}

void FTDIInterface::SetBaudrate(uint32_t baud)
{
    FTDI_CHECK(ftdi->SetBaudRate(baud));
}
