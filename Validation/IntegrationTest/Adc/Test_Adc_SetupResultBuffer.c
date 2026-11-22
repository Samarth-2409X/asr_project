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
 * 2. TEST ENVIRONMENT RESET & RESULT HELPERS
 * ============================================================================ */
static void Test_Det_Reset(void)
{
    Det_LastModuleId          = 0U;
    Det_LastInstanceId        = 0U;
    Det_LastApiId             = 0U;
    Det_LastErrorId           = 0U;
    Det_ErrorCount            = 0U;

    Det_LastRuntimeModuleId   = 0U;
    Det_LastRuntimeInstanceId = 0U;
    Det_LastRuntimeApiId      = 0U;
    Det_LastRuntimeErrorId    = 0U;
    Det_RuntimeErrorCount     = 0U;
}

uint8 Test_Adc_SetupResultBuffer_GetResult(void)
{
    return Test_Result;
}

/* ============================================================================
 * 3. INTEGRATION TEST CASES
 * ============================================================================ */

/*******************************************************************************
 * IT_002: API Called While Driver Uninitialized ([SWS_Adc_00434], [SWS_Adc_91005])
 * Note: Run before Adc_Init() while driver is in ADC_STATE_UNINIT.
 ******************************************************************************/
void Test_Adc_SetupResultBuffer_It_002(void)
{
    Std_ReturnType retVal;

    Test_Det_Reset();

    retVal = Adc_SetupResultBuffer(TEST_GROUP_0, Test_BufferA);

    if ((retVal == E_NOT_OK) &&
        (Det_ErrorCount == 1U) &&
        (Det_LastModuleId == ADC_MODULE_ID) &&
        (Det_LastInstanceId == ADC_INSTANCE_ID) &&
        (Det_LastApiId == ADC_SID_SETUP_RESULT_BUFFER) &&
        (Det_LastErrorId == ADC_E_UNINIT))
    {
        Test_Result = TEST_PASS;
    }
    else
    {
        Test_Result = TEST_FAIL;
    }
}

/*******************************************************************************
 * IT_001: Valid Result Buffer Setup ([SWS_Adc_00420], [SWS_Adc_91000])
 ******************************************************************************/
void Test_Adc_SetupResultBuffer_It_001(void)
{
    Std_ReturnType retVal;

    Test_Det_Reset();
    Adc_Init(&adcConfig);

    retVal = Adc_SetupResultBuffer(TEST_GROUP_0, Test_BufferA);

    if ((retVal == E_OK) &&
        (Det_ErrorCount == 0U) &&
        (Det_RuntimeErrorCount == 0U))
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

    Test_Det_Reset();

    retVal = Adc_SetupResultBuffer(TEST_INVALID_GROUP, Test_BufferB);

    if ((retVal == E_NOT_OK) &&
        (Det_ErrorCount == 1U) &&
        (Det_LastModuleId == ADC_MODULE_ID) &&
        (Det_LastInstanceId == ADC_INSTANCE_ID) &&
        (Det_LastApiId == ADC_SID_SETUP_RESULT_BUFFER) &&
        (Det_LastErrorId == ADC_E_PARAM_GROUP))
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

    Test_Det_Reset();

    retVal = Adc_SetupResultBuffer(TEST_GROUP_0, NULL_PTR);

    if ((retVal == E_NOT_OK) &&
        (Det_ErrorCount == 1U) &&
        (Det_LastModuleId == ADC_MODULE_ID) &&
        (Det_LastInstanceId == ADC_INSTANCE_ID) &&
        (Det_LastApiId == ADC_SID_SETUP_RESULT_BUFFER) &&
        (Det_LastErrorId == ADC_E_PARAM_POINTER))
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
    Test_Det_Reset();

    /* Calling Adc_SetupResultBuffer while Group 0 is ADC_BUSY must report ADC_E_BUSY */
    retVal = Adc_SetupResultBuffer(TEST_GROUP_0, Test_BufferB);

    if ((retVal == E_NOT_OK) &&
        (Det_RuntimeErrorCount == 1U) &&
        (Det_LastRuntimeModuleId == ADC_MODULE_ID) &&
        (Det_LastRuntimeInstanceId == ADC_INSTANCE_ID) &&
        (Det_LastRuntimeApiId == ADC_SID_SETUP_RESULT_BUFFER) &&
        (Det_LastRuntimeErrorId == ADC_E_BUSY))
    {
        Test_Result = TEST_PASS;
    }
    else
    {
        Test_Result = TEST_FAIL;
    }
}