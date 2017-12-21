#pragma once 
#include "defines.h"
#include "Controls.h"


void    Panel_Init();
void    Panel_DeInit();
void    Panel_Update();
void    Panel_TransmitData(uint16 data);            // �������� ������� � �� ������ ����������.
void    Panel_Disable();                        // � ���������� ������ ������ ���� ��������� ��������� ���������� pressedButton, �� �������� ������ ������� ��������.
void    Panel_Enable();
PanelButton   Panel_WaitPressingButton();       // ������� ������� �������.
bool    Panel_ProcessingCommandFromPIC(uint16 command);
uint16  Panel_NextData();
PanelButton  Panel_PressedButton();