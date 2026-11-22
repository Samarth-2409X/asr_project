#include "Std_Types.h"
#include "Adc.h"
#include "Adc_Cfg.h"
#include "Adc_PBcfg.h"
#include "Det.h"

/* ============================================================================
 * 1. TEST CONSTANTS & APPLICATION DATA BUFFERS
 * ============================================================================ */
#define TEST_PASS               (1U)
#define TEST_FAIL               (0U)
#define TEST_GROUP_0            (0U)
#define TEST_GROUP_1            (1U)
#define TEST_INVALID_GROUP      (ADC_MAX_GROUPS_PER_HW_UNIT) /* 2U -> Out of bounds */
#define TEST_BUFFER_LEN         (4U)

static uint8              Test_Result = TEST_FAIL;
static Adc_ValueGroupType Test_Buffer[TEST_BUFFER_LEN] = { 0x1111U, 0x2222U, 0x3333U, 0x4444U };

/* ============================================================================
 * 2. TEST RESULT GETTER
 * ============================================================================ */
uint8 Test_Adc_StartGroupConversion_GetResult(void)
{
    return Test_Result;
}

/* ============================================================================
 * 3. INTEGRATION TEST CASES (Sequential Order: IT_001 to IT_006)
 * ============================================================================ */

/*******************************************************************************
 * IT_001: Valid Software Conversion Start ([SWS_Adc_00061], [SWS_Adc_00367])
 ******************************************************************************/
void Test_Adc_StartGroupConversion_It_001(void)
{
    Det_ResetLogs();

    (void)Adc_SetupResultBuffer(TEST_GROUP_0, Test_Buffer);

    Adc_StartGroupConversion(TEST_GROUP_0);

    if ((Det_DevErrorLog.ErrorCount == 0UL) &&
        (Det_RuntimeErrorLog.ErrorCount == 0UL))
    {
        Test_Result = TEST_PASS;
    }
    else
    {
        Test_Result = TEST_FAIL;
    }
}

/*******************************************************************************
 * IT_002: API Called While Driver Uninitialized ([SWS_Adc_00294], [SWS_Adc_91005])
 * Note: Call this test in main() BEFORE calling Adc_Init() (or after Adc_DeInit()).
 ******************************************************************************/
void Test_Adc_StartGroupConversion_It_002(void)
{
    Det_ResetLogs();

    Adc_StartGroupConversion(TEST_GROUP_0);

    if ((Det_DevErrorLog.ErrorCount == 1UL) &&
        (Det_DevErrorLog.ModuleId == ADC_MODULE_ID) &&
        (Det_DevErrorLog.InstanceId == ADC_INSTANCE_ID) &&
        (Det_DevErrorLog.ApiId == ADC_SID_START_GROUP_CONVERSION) &&
        (Det_DevErrorLog.ErrorId == ADC_E_UNINIT))
    {
        Test_Result = TEST_PASS;
    }
    else
    {
        Test_Result = TEST_FAIL;
    }
}

/*******************************************************************************
 * IT_003: Invalid / Out-of-Range Group ID ([SWS_Adc_00125], [SWS_Adc_91005])
 ******************************************************************************/
void Test_Adc_StartGroupConversion_It_003(void)
{
    Det_ResetLogs();

    Adc_StartGroupConversion(TEST_INVALID_GROUP);

    if ((Det_DevErrorLog.ErrorCount == 1UL) &&
        (Det_DevErrorLog.ModuleId == ADC_MODULE_ID) &&
        (Det_DevErrorLog.InstanceId == ADC_INSTANCE_ID) &&
        (Det_DevErrorLog.ApiId == ADC_SID_START_GROUP_CONVERSION) &&
        (Det_DevErrorLog.ErrorId == ADC_E_PARAM_GROUP))
    {
        Test_Result = TEST_PASS;
    }
    else
    {
        Test_Result = TEST_FAIL;
    }
}

/*******************************************************************************
 * IT_004: Result Buffer Not Initialized ([SWS_Adc_00424], [SWS_Adc_91005])
 * Note: Must be called on a group whose ResultBufferPtr is still NULL_PTR.
 ******************************************************************************/
void Test_Adc_StartGroupConversion_It_004(void)
{
    Det_ResetLogs();

    Adc_StartGroupConversion(TEST_GROUP_0);

    if ((Det_DevErrorLog.ErrorCount == 1UL) &&
        (Det_DevErrorLog.ModuleId == ADC_MODULE_ID) &&
        (Det_DevErrorLog.InstanceId == ADC_INSTANCE_ID) &&
        (Det_DevErrorLog.ApiId == ADC_SID_START_GROUP_CONVERSION) &&
        (Det_DevErrorLog.ErrorId == ADC_E_BUFFER_UNINIT))
    {
        Test_Result = TEST_PASS;
    }
    else
    {
        Test_Result = TEST_FAIL;
    }
}

/*******************************************************************************
 * IT_005: Wrong Trigger Source (HW Trigger Configured) ([SWS_Adc_00133])
 ******************************************************************************/
void Test_Adc_StartGroupConversion_It_005(void)
{
    Det_ResetLogs();

    (void)Adc_SetupResultBuffer(TEST_GROUP_1, Test_Buffer);

    Adc_StartGroupConversion(TEST_GROUP_1);

    if ((Det_DevErrorLog.ErrorCount == 1UL) &&
        (Det_DevErrorLog.ModuleId == ADC_MODULE_ID) &&
        (Det_DevErrorLog.InstanceId == ADC_INSTANCE_ID) &&
        (Det_DevErrorLog.ApiId == ADC_SID_START_GROUP_CONVERSION) &&
        (Det_DevErrorLog.ErrorId == ADC_E_WRONG_TRIGG_SRC))
    {
        Test_Result = TEST_PASS;
    }
    else
    {
        Test_Result = TEST_FAIL;
    }
}

/*******************************************************************************
 * IT_006: Group Already Busy Runtime Error ([SWS_Adc_00346], [SWS_Adc_00426])
 ******************************************************************************/
void Test_Adc_StartGroupConversion_It_006(void)
{
    /* Ensure Group 0 has a valid buffer and start conversion so it becomes ADC_BUSY */
    (void)Adc_SetupResultBuffer(TEST_GROUP_0, Test_Buffer);
    Adc_StartGroupConversion(TEST_GROUP_0);
    Det_ResetLogs();

    /* Second call while Group 0 is already ADC_BUSY must report ADC_E_BUSY */
    Adc_StartGroupConversion(TEST_GROUP_0);

    if ((Det_RuntimeErrorLog.ErrorCount == 1UL) &&
        (Det_RuntimeErrorLog.ModuleId == ADC_MODULE_ID) &&
        (Det_RuntimeErrorLog.InstanceId == ADC_INSTANCE_ID) &&
        (Det_RuntimeErrorLog.ApiId == ADC_SID_START_GROUP_CONVERSION) &&
        (Det_RuntimeErrorLog.ErrorId == ADC_E_BUSY))
    {
        Test_Result = TEST_PASS;
    }
    else
    {
        Test_Result = TEST_FAIL;
    }
}