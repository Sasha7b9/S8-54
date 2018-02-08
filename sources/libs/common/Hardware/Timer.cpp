#include "defines.h"
#include "Timer.h"
#include "Log.h"
#include "Hardware/it.h"
#include <limits.h>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static TIM_HandleTypeDef handleTIM2 =
{
    TIM2,
    {
        0,
        TIM_COUNTERMODE_UP,
        (uint)-1,
        TIM_CLOCKDIVISION_DIV1
    }
};



#ifdef S8_54
static TIM_HandleTypeDef handleTIM3 =
{
    TIM3,
    {
        54000 - 1,
        TIM_COUNTERMODE_UP,
        1,
        TIM_CLOCKDIVISION_DIV1
    }
};
#endif

#ifdef S8_53
static TIM_HandleTypeDef handleTIM3 =
{
    TIM3,
    {
        119,
        TIM_COUNTERMODE_UP,
        500,
        TIM_CLOCKDIVISION_DIV1
    }
};
#endif



typedef struct
{
    pFuncVV func;       // ������� �������
    uint    dTms;          // ������ ������������, ��
    uint    timeNextMS;    // ����� ���������� ������������. ���� == 0xffffffff, �� ������ ���������
    bool    repeat;        // ���� true, ����� �����������, ���� �� ����� ������� ������� Timer_Disable()
    uint8   notUsed0;
    uint8   notUsed1;
    uint8   notUsed2;
} TimerStruct;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static TimerStruct timers[NumTimers];
static uint timeStartLogging = 0;
static uint timePrevPoint = 0;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#undef TIME_NEXT
#define TIME_NEXT(type) (timers[type].timeNextMS)


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void StartTIM(uint timeStop);    // ������� �����, ������� ����������� � timeStop ��
static void StopTIM();
static uint NearestTime();          // ���������� ����� ������������ ���������� �������, ���� 0, ���� �������� ���
static void TuneTIM(TypeTimer2 type);   // ��������� ������� �� ������


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Timer::IsRun(TypeTimer2 type)
{
    return TIME_NEXT(type) != UINT_MAX;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Timer::Init()
{
    for(uint i = 0; i < NumTimers; i++)
    {
        timers[i].timeNextMS = UINT_MAX;
    }

    __HAL_RCC_TIM2_CLK_ENABLE();    // ��� �����
    __HAL_RCC_TIM3_CLK_ENABLE();    // ��� ��������

    HAL_NVIC_EnableIRQ(TIM3_IRQn);
    HAL_NVIC_SetPriority(TIM3_IRQn, 1, 1);

    HAL_TIM_Base_Init(&handleTIM2);
    HAL_TIM_Base_Start(&handleTIM2);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Timer::DeInit()
{
    HAL_NVIC_DisableIRQ(TIM3_IRQn);
    HAL_TIM_Base_Stop(&handleTIM2);
    HAL_TIM_Base_DeInit(&handleTIM2);

    __HAL_RCC_TIM2_CLK_DISABLE();
    __HAL_RCC_TIM3_CLK_DISABLE();
}

#ifdef __cplusplus
extern "C" {
#endif

#pragma clang diagnostic ignored "-Wmissing-prototypes"
    
//----------------------------------------------------------------------------------------------------------------------------------------------------
void TIM3_IRQHandler()
{
    HAL_TIM_IRQHandler(&handleTIM3);
}

#ifdef __cplusplus
}
#endif

//----------------------------------------------------------------------------------------------------------------------------------------------------
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *)
{
    uint time = gTimeMS;

    if (NearestTime() > time)
    {
        return;
    }

    StopTIM();

    for (uint type = 0; type < NumTimers; type++)
    {
        if (TIME_NEXT(type) <= time)            // ���� ������ ����� ������������
        {
            TimerStruct *timer = &timers[type];
            timer->func();
            if (timer->repeat)
            {
                do      // ���� ����� ������, ��� ��������� ������ SysTick, ������� ����������� ������������, ����� ��������� ���������,
                {       // � ���� ������� ����������� ������, ��� timer->dTm ��, �� ��� ��� ��������
                    timer->timeNextMS += timer->dTms;
                } while (timer->timeNextMS < gTimeMS);

            }
            else
            {
                timer->timeNextMS = UINT_MAX;
            }
        }
    }

    StartTIM(NearestTime());
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Timer::Set(TypeTimer2 type, pFuncVV func, uint dTms)
{
    TimerStruct *timer = &timers[type];
    timer->func = func;
    timer->dTms = dTms;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Timer::SetAndStartOnce(TypeTimer2 type, pFuncVV func, uint dTms)
{
    Timer::Set(type, func, dTms);
    StartOnce(type);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Timer::SetAndEnable(TypeTimer2 type, pFuncVV func, uint dTms)
{
    Set(type, func, dTms);
    Enable(type);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Timer::StartOnce(TypeTimer2 type)
{
    timers[type].repeat = false;
    TuneTIM(type);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Timer::Enable(TypeTimer2 type)
{
    timers[type].repeat = true;
    TuneTIM(type);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void TuneTIM(TypeTimer2 type)
{
    TimerStruct *timer = &timers[type];

    uint timeNearest = NearestTime();

    uint timeNext = gTimeMS + timer->dTms;
    timer->timeNextMS = timeNext;

    if(timeNext < timeNearest)      // ���� ������ ������ ��������� ������ ��������
    {
        StartTIM(timeNext);         // �� ������� ������ �� ���� �����
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Timer::Disable(TypeTimer2 type)
{
    timers[type].timeNextMS = UINT_MAX;
    timers[type].repeat = false;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static uint NearestTime()
{
    uint time = UINT_MAX;

    for(uint type = 0; type < NumTimers; type++)
    {
        if(TIME_NEXT(type) < time)
        {
            time = TIME_NEXT(type);
        }
    }
    
    return time;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void StartTIM(uint timeStopMS)
{
    StopTIM();

    if(timeStopMS == UINT_MAX)
    {
        return;
    }

    uint dT = timeStopMS - gTimeMS;

    handleTIM3.Init.Period = (dT * 2) - 1;      // 10 ������������� 0.1��. �.�. ���� ��� ����� 1��, ����� �������� (100 - 1)

    HAL_TIM_Base_Init(&handleTIM3);
    HAL_TIM_Base_Start_IT(&handleTIM3);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void StopTIM()
{
    HAL_TIM_Base_Stop_IT(&handleTIM3);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Timer::PauseOnTime(uint timeMS)
{
    HAL_Delay(timeMS);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Timer::PauseOnTicks(uint numTicks)
{
    uint startTicks = gTimeTics;
    while (gTimeTics - startTicks < numTicks)
    {
    };
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Timer::StartMultiMeasurement()
{
    TIM2->CR1 &= (uint)~TIM_CR1_CEN;
    TIM2->CNT = 0;
    TIM2->CR1 |= TIM_CR1_CEN;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Timer::StartLogging()
{
    timeStartLogging = gTimeTics;
    timePrevPoint = timeStartLogging;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
uint Timer::LogPointUS(char *name)
{
    uint interval = gTimeTics - timePrevPoint;
    timePrevPoint = gTimeTics;
    LOG_WRITE("%s %.2f us", name, interval / 120.0);
    return interval;
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
uint Timer::LogPointMS(char *name)
{
    uint interval = gTimeTics - timePrevPoint;
    timePrevPoint = gTimeTics;
    LOG_WRITE("%s %.2f ms", name, interval / 120e3);
    return interval;
}

#pragma clang diagnostic warning "-Wmissing-field-initializers"
#pragma clang diagnostic warning "-Wunused-parameter"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#undef TIME_NEXT
