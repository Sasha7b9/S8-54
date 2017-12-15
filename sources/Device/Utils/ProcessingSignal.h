#pragma once
#include "Utils/Measures.h"
#include "defines.h"
#include "Data/DataStorage.h"
#include "FPGA/FPGAtypes.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @addtogroup Utils
 *  @{
 *  @defgroup ProcessingSignal
 *  @{
 */

#define MARKER_VOLTAGE(ch, num)  (markerVoltage[ch][num] - MIN_VALUE)
#define MARKER_TIME(ch, num)    (markerTime[ch][num])

extern int markerVoltage[NumChannels][2];
extern int markerTime[NumChannels][2];

class Processing
{
public:
    /// \brief ���������� ������ ��� ���������. ������ ������� �� DS, inA, inB.
    /// �������� ������, ��������������� ������� ����������� set, �������������� ����� � ������������ � outA, outB.
    static void SetData(bool needSmoothing);
    /// �������� ������� ������� ����������, �������������� �������� ������� ������� posCurT
    static float GetCursU(Channel ch, float posCurT);
    /// �������� ������� ������� �������, ��������������� �������� ������� ������� ���������� posCurU
    static float GetCursT(Channel ch, float posCurU, int numCur);
    /// ���������������� ��������� ��������� ������ ������������� �������� sinX/X
    static void InterpolationSinX_X(uint8 *data, int numPoints, TBase tBase);
    /// ���������� ������ ��������������� ���������
    static char* GetStringMeasure(Meas measure, Channel ch, char *buffer, int lenBuf);
    /// ��������� ��� ���������
    static void CalculateMeasures();
private:
    /// ���������� ������� � ������������� � ������� ����������
    static void CountedToCurrentSettings();
    /// ���������� ������� � �������������� � ������� Range
    static void CountedRange(Channel ch);
    /// ���������� ������� � ������ � �������������� � ������� TBase
    static void CountedTBase();
    /// ���������� ���������� ����� � ������� � ������������� ����������
    static void CountedEnumPoints();
};


/** @}  @}
 */
