/*******************************************************************************
 * File     : Test_Adc_SetupResultBuffer.c
 * Module   : ADC driver (AUTOSAR CP R22-11 / R25-11, Infineon AURIX TC397)
 * Function : Adc_SetupResultBuffer() - SWS_ADCDriver (Service ID 0x0C)
 * Author   : Samarth Shirahatti (Validation team)
 *
 * Result convention (Test_Result):
 *     1 -> PASS (TEST_PASS)
 *     0 -> FAIL (TEST_FAIL)
 ******************************************************************************/


#include "Adc.h"
#include "Adc_Cfg.h"
#include "Det.h"           
#include <stddef.h>


#define ADC_SID_SETUP_RESULT_BUFFER  (0x0Cu)


#define TEST_PASS (1u)
#define TEST_FAIL (0u)


static uint8 Test_Result;


extern const Adc_ConfigType adcConfig;
extern Adc_ValueGroupType* Adc_GroupResultBufferPtr[];


static Adc_ValueGroupType Test_DataBuffer[10];
static Adc_ValueGroupType Test_DataBuffer2[10];
static Adc_ValueGroupType *Null_BufferPtr = NULL;


extern void DetStub_Reset(void);
extern uint32 DetStub_GetCount(void);
extern uint8 DetStub_GetLastApiId(void);
extern uint8 DetStub_GetLastErrorId(void);
extern void Test_Reset(void);


uint8 Test_Adc_SetupResultBuffer_GetResult(void)
{
    return Test_Result;
}


void Test_Adc_SetupResultBuffer_Tc_001(void)
{
    Std_ReturnType status;

   
    Test_Reset();
    DetStub_Reset();

   
    Adc_Init(&adcConfig);

    
    status = Adc_SetupResultBuffer(0, Test_DataBuffer);

    if ((status == E_OK) && (DetStub_GetCount() == 0u))
    {
        Test_Result = TEST_PASS;
    }
    else
    {
        Test_Result = TEST_FAIL;
    }
}


void Test_Adc_SetupResultBuffer_Tc_002(void)
{
    Std_ReturnType status;

    Test_Reset();
    DetStub_Reset();

    
    Adc_DeInit();

    
    status = Adc_SetupResultBuffer(0, Test_DataBuffer);

    
    if ((status == E_NOT_OK) &&
        (DetStub_GetCount() == 1u) &&
        (DetStub_GetLastApiId() == ADC_SID_SETUP_RESULT_BUFFER) &&
        (DetStub_GetLastErrorId() == ADC_E_UNINIT))
    {
        Test_Result = TEST_PASS;
    }
    else
    {
        Test_Result = TEST_FAIL;
    }
}


void Test_Adc_SetupResultBuffer_Tc_003(void)
{
    Std_ReturnType status;

    Test_Reset();
    DetStub_Reset();

    Adc_Init(&adcConfig);

    
    Adc_SetupResultBuffer(0, Test_DataBuffer);

   
    status = Adc_SetupResultBuffer(99, Test_DataBuffer2);

   
    if ((status == E_NOT_OK) &&
        (DetStub_GetCount() == 1u) &&
        (DetStub_GetLastErrorId() == ADC_E_PARAM_GROUP) &&
        (Adc_GroupResultBufferPtr[0] == Test_DataBuffer))
    {
        Test_Result = TEST_PASS;
    }
    else
    {
        Test_Result = TEST_FAIL;
    }
}

void Test_Adc_SetupResultBuffer_Tc_004(void)
{
    Std_ReturnType status;

    Test_Reset();
    DetStub_Reset();

    Adc_Init(&adcConfig);

   
    status = Adc_SetupResultBuffer(0, Null_BufferPtr);

    
    if ((status == E_NOT_OK) &&
        (DetStub_GetCount() == 1u) &&
        (DetStub_GetLastErrorId() == ADC_E_PARAM_POINTER))
    {
        Test_Result = TEST_PASS;
    }
    else
    {
        Test_Result = TEST_FAIL;
    }
}


void Test_Adc_SetupResultBuffer_Tc_005(void)
{
    Std_ReturnType status;

    Test_Reset();
    DetStub_Reset();

    Adc_Init(&adcConfig);

    
    (void)Adc_SetupResultBuffer(0, Test_DataBuffer);
    
    
    Adc_StartGroupConversion(0);

   
    status = Adc_SetupResultBuffer(0, Test_DataBuffer2);

    if ((status == E_NOT_OK) &&
        (DetStub_GetLastErrorId() == ADC_E_BUSY))
    {
        Test_Result = TEST_PASS;
    }
    else
    {
        Test_Result = TEST_FAIL;
    }
    
   
    Adc_StopGroupConversion(0);
}


void Test_Adc_SetupResultBuffer_Tc_006(void)
{
    Std_ReturnType status;

    Test_Reset();
    DetStub_Reset();

    Adc_Init(&adcConfig);

    status = Adc_SetupResultBuffer(0, Test_DataBuffer);

    
    if ((status == E_OK) && 
        (Adc_GroupResultBufferPtr[0] == Test_DataBuffer))
    {
        Test_Result = TEST_PASS;
    }
    else
    {
        Test_Result = TEST_FAIL;
    }
}


void Test_Adc_SetupResultBuffer_Tc_007(void)
{
    Std_ReturnType status1, status2;

    Test_Reset();
    DetStub_Reset();

    Adc_Init(&adcConfig);

   
    status1 = Adc_SetupResultBuffer(0, Test_DataBuffer);

    
    status2 = Adc_SetupResultBuffer(0, Test_DataBuffer2);

    if ((status1 == E_OK) && 
        (status2 == E_OK) && 
        (Adc_GroupResultBufferPtr[0] == Test_DataBuffer2))
    {
        Test_Result = TEST_PASS;
    }
    else
    {
        Test_Result = TEST_FAIL;
    }
}