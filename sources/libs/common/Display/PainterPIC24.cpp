#include "Painter.h"
#include "Hardware/FSMC.h"
#include "Hardware/Timer.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Painter::SendToDisplay(uint8 *bytes, int numBytes)
{
    for (int i = 0; i < numBytes; i += 4)
    {
        while (HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_11) == GPIO_PIN_RESET)
        {
        };

        Timer::PauseOnTicks(75);    /// \todo ����� ����� �������� ��������� �� ��������� � �8-53 (��� ������� 120���, ����� - 180���)

        *ADDR_CDISPLAY = *bytes++;
        *ADDR_CDISPLAY = *bytes++;
        *ADDR_CDISPLAY = *bytes++;
        *ADDR_CDISPLAY = *bytes++;
    }
}
