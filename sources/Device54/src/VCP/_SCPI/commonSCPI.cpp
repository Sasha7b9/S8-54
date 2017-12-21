#include "defines.h"
#include "commonSCPI.h"
#include "Hardware/Hardware.h"
#include "Settings/Settings.h"
#include "VCP/VCP.h"
#include "VCP/SCPI/SCPI.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Process_IDN(uint8 *)
{
    char buf[100] = {0};

    sprintf(buf, "%s, %s, V%s, %0X", "MNIPI", MODEL_EN, NUM_VER, Hardware::CalculateCRC32());

    SCPI_SEND(buf);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Process_RUN(uint8 *)
{

}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Process_STOP(uint8 *)
{

}

//----------------------------------------------------------------------------------------------------------------------------------------------------
void Process_RESET(uint8 *)
{
    Settings::Load(true);
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Process_AUTOSCALE(uint8 *)
{

}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Process_REQUEST(uint8 *)
{
    SCPI_SEND(MODEL_EN);
}