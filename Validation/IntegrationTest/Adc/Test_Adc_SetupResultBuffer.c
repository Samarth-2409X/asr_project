#include "Std_Types.h"
#include "Adc.h"
#include "Adc_Cfg.h"
#include "Adc_PBcfg.h"
#include "Det.h"

/* ============================================================================
 * 1. TEST CONSTANTS & DATA BUFFERS
 * ============================================================================ */
#define TEST_PASS               (1U)
#define TEST_FAIL               (0U)
#define TEST_GROUP_0            (0U)
#define TEST_GROUP_1            (1U)
#define TEST_INVALID_GROUP      (ADC_MAX_GROUPS_PER_HW_UNIT) /* 2U -> Out of bounds */
#define TEST_BUFFER_LEN         (4U)

static uint8              Test_Result = TEST_FAIL;
static Adc_ValueGroupType Test_BufferA[TEST_BUFFER_LEN] = { 0x1111U, 0x2222U, 0x3333U, 0x4444U };
static Adc_ValueGroupType Test_BufferB[TEST_BUFFER_LEN] = { 0x5555U, 0x6666U, 0x7777U, 0x8888U };

/* ============================================================================
 * 2. TEST RESULT GETTER
 * ============================================================================ */
uint8 Test_Adc_SetupResultBuffer_GetResult(void)
{
    return Test_Result;
}

/* ============================================================================
 * 3. INTEGRATION TEST CASES (Sequential Order: IT_001 to IT_005)
 * ============================================================================ */

/*******************************************************************************
 * IT_001: Valid Result Buffer Setup ([SWS_Adc_00420], [SWS_Adc_91000])
 ******************************************************************************/
void Test_Adc_SetupResultBuffer_It_001(void)
{
    Std_ReturnType retVal;

    Det_ResetLogs();
    Adc_Init(&adcConfig);

    retVal = Adc_SetupResultBuffer(TEST_GROUP_0, Test_BufferA);

    if ((retVal == E_OK) &&
        (Det_DevErrorLog.ErrorCount == 0UL) &&
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
 * IT_002: API Called While Driver Uninitialized ([SWS_Adc_00434], [SWS_Adc_91005])
 * Note: Call this test in main() BEFORE calling Adc_Init() (or after Adc_DeInit()).
 ******************************************************************************/
void Test_Adc_SetupResultBuffer_It_002(void)
{
    Std_ReturnType retVal;

    Det_ResetLogs();

    retVal = Adc_SetupResultBuffer(TEST_GROUP_0, Test_BufferA);

    if ((retVal == E_NOT_OK) &&
        (Det_DevErrorLog.ErrorCount == 1UL) &&
        (Det_DevErrorLog.ModuleId == ADC_MODULE_ID) &&
        (Det_DevErrorLog.InstanceId == ADC_INSTANCE_ID) &&
        (Det_DevErrorLog.ApiId == ADC_SID_SETUP_RESULT_BUFFER) &&
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
 * IT_003: Invalid / Out-of-Range Group ID ([SWS_Adc_00423], [SWS_Adc_91005])
 ******************************************************************************/
void Test_Adc_SetupResultBuffer_It_003(void)
{
    Std_ReturnType retVal;

    Det_ResetLogs();

    retVal = Adc_SetupResultBuffer(TEST_INVALID_GROUP, Test_BufferB);

    if ((retVal == E_NOT_OK) &&
        (Det_DevErrorLog.ErrorCount == 1UL) &&
        (Det_DevErrorLog.ModuleId == ADC_MODULE_ID) &&
        (Det_DevErrorLog.InstanceId == ADC_INSTANCE_ID) &&
        (Det_DevErrorLog.ApiId == ADC_SID_SETUP_RESULT_BUFFER) &&
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
 * IT_004: NULL Result Buffer Pointer ([SWS_Adc_00457], [SWS_Adc_91005])
 ******************************************************************************/
void Test_Adc_SetupResultBuffer_It_004(void)
{
    Std_ReturnType retVal;

    Det_ResetLogs();

    retVal = Adc_SetupResultBuffer(TEST_GROUP_0, NULL_PTR);

    if ((retVal == E_NOT_OK) &&
        (Det_DevErrorLog.ErrorCount == 1UL) &&
        (Det_DevErrorLog.ModuleId == ADC_MODULE_ID) &&
        (Det_DevErrorLog.InstanceId == ADC_INSTANCE_ID) &&
        (Det_DevErrorLog.ApiId == ADC_SID_SETUP_RESULT_BUFFER) &&
        (Det_DevErrorLog.ErrorId == ADC_E_PARAM_POINTER))
    {
        Test_Result = TEST_PASS;
    }
    else
    {
        Test_Result = TEST_FAIL;
    }
}

/*******************************************************************************
 * IT_005: Group Busy Runtime Error ([SWS_Adc_00433], [SWS_Adc_91006])
 ******************************************************************************/
void Test_Adc_SetupResultBuffer_It_005(void)
{
    Std_ReturnType retVal;

    /* Bind valid buffer and start conversion to place Group 0 into ADC_BUSY state */
    (void)Adc_SetupResultBuffer(TEST_GROUP_0, Test_BufferA);
    Adc_StartGroupConversion(TEST_GROUP_0);
    Det_ResetLogs();

    /* Calling Adc_SetupResultBuffer while Group 0 is ADC_BUSY must report ADC_E_BUSY */
    retVal = Adc_SetupResultBuffer(TEST_GROUP_0, Test_BufferB);

    if ((retVal == E_NOT_OK) &&
        (Det_RuntimeErrorLog.ErrorCount == 1UL) &&
        (Det_RuntimeErrorLog.ModuleId == ADC_MODULE_ID) &&
        (Det_RuntimeErrorLog.InstanceId == ADC_INSTANCE_ID) &&
        (Det_RuntimeErrorLog.ApiId == ADC_SID_SETUP_RESULT_BUFFER) &&
        (Det_RuntimeErrorLog.ErrorId == ADC_E_BUSY))
    {
        Test_Result = TEST_PASS;
    }
    else
    {
        Test_Result = TEST_FAIL;
    }
}