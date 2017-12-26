#pragma once 
#include "defines.h"
#include "Controls.h"


class Panel
{
public:
    static void Init();
    static void DeInit();
    static void Update();
    /// �������� ������� � �� ������ ����������.
    static void TransmitData(uint16 data);
    /// � ���������� ������ ������ ���� ��������� ��������� ���������� pressedButton, �� �������� ������ ������� ��������.
    static void Disable();
    static void Enable();
    /// ������� ������� �������.
    static PanelButton WaitPressingButton();
    static bool ProcessingCommandFromPIC(uint16 command);
    static uint16 NextData();
    static PanelButton PressedButton();
};
