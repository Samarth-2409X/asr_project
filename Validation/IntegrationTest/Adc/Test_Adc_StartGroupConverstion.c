#include <stdio.h>
#include "Adc.h"
#include "Det.h"

/*
 * Include implementation directly for unit testing.
 * This gives the test access to static driver variables.
 */
#include "Adc_StartGroupConversion.c"

/* DET mock variables */

uint8 Det_ErrorId = 0xFFU;
uint8 Det_RuntimeErrorId = 0xFFU;

uint8 Det_ErrorCount = 0U;
uint8 Det_RuntimeErrorCount = 0U;

uint16 Det_ModuleId;
uint8 Det_InstanceId;
uint8 Det_ApiId;

/* DET mock functions */

Std_ReturnType Det_ReportError(
    uint16 ModuleId,
    uint8 InstanceId,
    uint8 ApiId,
    uint8 ErrorId)
{
    Det_ModuleId = ModuleId;
    Det_InstanceId = InstanceId;
    Det_ApiId = ApiId;
    Det_ErrorId = ErrorId;

    Det_ErrorCount++;

    return E_OK;
}

Std_ReturnType Det_ReportRuntimeError(
    uint16 ModuleId,
    uint8 InstanceId,
    uint8 ApiId,
    uint8 ErrorId)
{
    Det_ModuleId = ModuleId;
    Det_InstanceId = InstanceId;
    Det_ApiId = ApiId;
    Det_RuntimeErrorId = ErrorId;

    Det_RuntimeErrorCount++;

    return E_OK;
}

/* ADC configuration and result buffer */

Adc_ConfigType adcConfig;
Adc_ValueGroupType ResultBuffer[2];

/* Reset driver state before every test */

void Reset_Test(void)
{
    Adc_GroupType i;

    adcConfig.AdcHwUnit.AdcGroup[0].AdcGroupTriggSrc =
        ADC_TRIGG_SRC_SW;

    adcConfig.AdcHwUnit.AdcGroup[1].AdcGroupTriggSrc =
        ADC_TRIGG_SRC_SW;

    Adc_DriverState = ADC_STATE_INIT;
    Adc_ConfigPtr = &adcConfig;

    for (i = 0U; i < ADC_MAX_GROUPS_PER_HW_UNIT; i++)
    {
        Adc_GroupRuntime[i].GroupStatus = ADC_IDLE;

        Adc_GroupRuntime[i].ResultBufferPtr = NULL_PTR;

        Adc_GroupRuntime[i].NotificationEnabled = FALSE;
    }

    Det_ErrorId = 0xFFU;
    Det_RuntimeErrorId = 0xFFU;

    Det_ErrorCount = 0U;
    Det_RuntimeErrorCount = 0U;
}

/* -----------------------------------------
   TEST CASE 1:
   Valid group and initialized result buffer
   ----------------------------------------- */

void Test_01_Valid_Group(void)
{
    Reset_Test();

    Adc_GroupRuntime[0U].ResultBufferPtr = ResultBuffer;

    Adc_StartGroupConversion(0U);

    if ((Adc_GroupRuntime[0U].GroupStatus == ADC_BUSY) &&
        (Det_ErrorCount == 0U) &&
        (Det_RuntimeErrorCount == 0U))
    {
        printf("TC01: PASS - Valid ADC conversion started\n");
    }
    else
    {
        printf("TC01: FAIL - Valid conversion test failed\n");
    }
}

/* -----------------------------------------
   TEST CASE 2:
   Invalid ADC group number
   ----------------------------------------- */

void Test_02_Invalid_Group(void)
{
    Reset_Test();

    Adc_StartGroupConversion(2U);

    if ((Det_ErrorId == ADC_E_PARAM_GROUP) &&
        (Det_ErrorCount == 1U) &&
        (Det_ApiId == ADC_SID_START_GROUP_CONVERSION))
    {
        printf("TC02: PASS - Invalid group detected\n");
    }
    else
    {
        printf("TC02: FAIL - Invalid group not detected\n");
    }
}

/* -----------------------------------------
   TEST CASE 3:
   Uninitialized result buffer
   ----------------------------------------- */

void Test_03_Buffer_Uninitialized(void)
{
    Reset_Test();

    Adc_GroupRuntime[0U].ResultBufferPtr = NULL_PTR;

    Adc_StartGroupConversion(0U);

    if ((Det_ErrorId == ADC_E_BUFFER_UNINIT) &&
        (Det_ErrorCount == 1U) &&
        (Det_ApiId == ADC_SID_START_GROUP_CONVERSION))
    {
        printf("TC03: PASS - Uninitialized buffer detected\n");
    }
    else
    {
        printf("TC03: FAIL - Buffer error not detected\n");
    }
}

/* -----------------------------------------
   TEST CASE 4:
   Hardware trigger instead of software trigger
   ----------------------------------------- */

void Test_04_Wrong_Trigger_Source(void)
{
    Reset_Test();

    adcConfig.AdcHwUnit.AdcGroup[0U].AdcGroupTriggSrc =
        ADC_TRIGG_SRC_HW;

    Adc_GroupRuntime[0U].ResultBufferPtr = ResultBuffer;

    Adc_StartGroupConversion(0U);

    if ((Det_ErrorId == ADC_E_WRONG_TRIGG_SRC) &&
        (Det_ErrorCount == 1U) &&
        (Det_ApiId == ADC_SID_START_GROUP_CONVERSION))
    {
        printf("TC04: PASS - Wrong trigger source detected\n");
    }
    else
    {
        printf("TC04: FAIL - Wrong trigger source not detected\n");
    }
}

/* -----------------------------------------
   TEST CASE 5:
   ADC group is already busy
   ----------------------------------------- */

void Test_05_Group_Already_Busy(void)
{
    Reset_Test();

    Adc_GroupRuntime[0U].ResultBufferPtr = ResultBuffer;

    Adc_GroupRuntime[0U].GroupStatus = ADC_BUSY;

    Adc_StartGroupConversion(0U);

    if ((Det_RuntimeErrorId == ADC_E_BUSY) &&
        (Det_RuntimeErrorCount == 1U) &&
        (Det_ApiId == ADC_SID_START_GROUP_CONVERSION))
    {
        printf("TC05: PASS - Busy ADC group detected\n");
    }
    else
    {
        printf("TC05: FAIL - Busy ADC group not detected\n");
    }
}

/* -----------------------------------------
   MAIN FUNCTION
   ----------------------------------------- */

int main(void)
{
    printf("\n");
    printf("========================================\n");
    printf(" ADC START GROUP CONVERSION TESTING\n");
    printf("========================================\n");

    Test_01_Valid_Group();

    Test_02_Invalid_Group();

    Test_03_Buffer_Uninitialized();

    Test_04_Wrong_Trigger_Source();

    Test_05_Group_Already_Busy();

    printf("========================================\n");
    printf(" ADC TESTING COMPLETED\n");
    printf("========================================\n");

    return 0;
}