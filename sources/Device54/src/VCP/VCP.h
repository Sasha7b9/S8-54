#pragma once
#include "defines.h"
#include "usbd_cdc_interface.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @defgroup VCP
 *  @brief Virtual Com Port
 *  @{
 */
 
#define VCP_FLUSH()                       VCP::Flush()
#define VCP_SEND_DATA_SYNCH(buffer, size) VCP::SendDataSynch(buffer, size)

class VCP
{
public:
    /// �������������
    static void Init();

    static void SendDataAsynch(uint8 *data, int size);

    static void SendDataSynch(const uint8 *data, int size);
    /// ��������� ������ ��� ������������ ����
    static void SendStringAsynch(char *data);
    /// ��������� ������ ��� ������������ ����
    static void SendStringSynch(char *data);
    /// ��� ������ ��������� � ������������ ��������� \\r\\n
    static void SendFormatStringAsynch(char *format, ...);
    /// ��� ������ ��������� � ������������ ��������� \\r\\n
    static void SendFormatStringSynch(char *format, ...);

    static void SendByte(uint8 data);

    static void Flush();

    static USBD_HandleTypeDef handleUSBD;
    static PCD_HandleTypeDef handlePCD;

private:
    static bool PrevSendingComplete();
};

#define CONNECTED_TO_USB        gConnectedToUSB
#define CABLE_USB_IS_CONNECTED  gCableUSBisConnected

extern bool gConnectedToUSB;
extern bool gCableUSBisConnected;


/** @}
 */
