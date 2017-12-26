 #pragma once
#include "defines.h"


#define ADDR_BANK           0x60000000

#define ADDR_CDISPLAY               ((uint8 *)(ADDR_BANK + 0x080000))    // ����� 8-������ ��������������� �������������� �����
#define ADDR_FPGA                   ((uint8 *)(ADDR_BANK + 0x300000))    // ����� 16-������ ���������������� ��������������� �����
#define ADDR_RAM                    ((uint8 *)(ADDR_BANK + 0x200000))    // ����� 16-������ ���������������� ��������������� �����


typedef enum
{
    ModeFSMC_None,
    ModeFSMC_Display,
    ModeFSMC_FPGA,
    ModeFSMC_RAM
} ModeFSMC;


class FSMC
{
public:
    static void Init();

    static void SetMode(ModeFSMC mode);
};
