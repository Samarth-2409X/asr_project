/*******************************************************************************
 * File     : Test_Adc_SetupResultBuffer.c
 * Layer    : Validation / IntegrationTest / Adc
 * Module   : ADC Driver (AUTOSAR CP R25-11, Infineon AURIX TC39x EVADC CPU0)
 * Function : Adc_SetupResultBuffer() - Service ID: 0x0CU [SWS_Adc_91000]
 * Author   : Samarth Shirahatti (Validation Team)
 *
 * Description:
 *   Integration test suite covering ONLY the 5 requirements implemented in
 *   Adc_SetupResultBuffer.c:
 *     - IT_001: Valid buffer setup ([SWS_Adc_00420], [SWS_Adc_91000])
 *     - IT_002: Driver uninitialized check ([SWS_Adc_00434]) -> ADC_E_UNINIT
 *     - IT_003: Invalid group ID check ([SWS_Adc_00423]) -> ADC_E_PARAM_GROUP
 *     - IT_004: NULL buffer pointer check ([SWS_Adc_00457]) -> ADC_E_PARAM_POINTER
 *     - IT_005: Busy group runtime check ([SWS_Adc_00433]) -> ADC_E_BUSY
 *
 * Result Convention (Test_Result):
 *     1U -> PASS (TEST_PASS)
 *     0U -> FAIL (TEST_FAIL)
 ******************************************************************************/

#include "Std_Types.h"
#include "Adc.h"
#include "Adc_Cfg.h"
#include "Adc_PBcfg.h"
#include "Det.h"

/* Include driver implementation directly to access module static runtime state */
#include "../../../Development/Mcal/Adc/Adc_SetupResultBuffer.c"

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
 * 2. TEST ENVIRONMENT RESET & INITIALIZATION HELPERS
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

static void Test_Env_Reset(void)
{
    Adc_GroupType grpIdx;

    Adc_DriverState = ADC_STATE_UNINIT;
    for (grpIdx = 0U; grpIdx < ADC_MAX_GROUPS_PER_HW_UNIT; grpIdx++)
    {
        Adc_GroupRuntime[grpIdx].GroupStatus         = ADC_IDLE;
        Adc_GroupRuntime[grpIdx].ResultBufferPtr     = NULL_PTR;
        Adc_GroupRuntime[grpIdx].NotificationEnabled = FALSE;
    }

    Test_Det_Reset();
}

static void Test_Env_InitFromPBcfg(const Adc_ConfigType* ConfigPtr)
{
    Adc_GroupType grpIdx;

    if (ConfigPtr != NULL_PTR)
    {
        for (grpIdx = 0U; grpIdx < ADC_MAX_GROUPS_PER_HW_UNIT; grpIdx++)
        {
            Adc_GroupRuntime[grpIdx].GroupStatus         = ADC_IDLE;
            Adc_GroupRuntime[grpIdx].ResultBufferPtr     = NULL_PTR;
            Adc_GroupRuntime[grpIdx].NotificationEnabled = FALSE;
        }
        Adc_DriverState = ADC_STATE_INIT;
    }
}

uint8 Test_Adc_SetupResultBuffer_GetResult(void)
{
    return Test_Result;
}

/* ============================================================================
 * 3. INTEGRATION TEST CASES (Strictly Matching Adc_SetupResultBuffer.c)
 * ============================================================================ */

/*******************************************************************************
 * IT_001: Valid Result Buffer Setup ([SWS_Adc_00420], [SWS_Adc_91000])
 ******************************************************************************/
void Test_Adc_SetupResultBuffer_It_001(void)
{
    Std_ReturnType retVal;

    Test_Env_Reset();
    Test_Env_InitFromPBcfg(&adcConfig);

    retVal = Adc_SetupResultBuffer(TEST_GROUP_0, Test_BufferA);

    if ((retVal == E_OK) &&
        (Det_ErrorCount == 0U) &&
        (Det_RuntimeErrorCount == 0U) &&
        (Adc_GroupRuntime[TEST_GROUP_0].ResultBufferPtr == Test_BufferA) &&
        (Adc_GroupRuntime[TEST_GROUP_1].ResultBufferPtr == NULL_PTR))
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
 ******************************************************************************/
void Test_Adc_SetupResultBuffer_It_002(void)
{
    Std_ReturnType retVal;

    Test_Env_Reset(); /* Driver remains ADC_STATE_UNINIT */

    retVal = Adc_SetupResultBuffer(TEST_GROUP_0, Test_BufferA);

    if ((retVal == E_NOT_OK) &&
        (Det_ErrorCount == 1U) &&
        (Det_LastModuleId == ADC_MODULE_ID) &&
        (Det_LastInstanceId == ADC_INSTANCE_ID) &&
        (Det_LastApiId == ADC_SID_SETUP_RESULT_BUFFER) &&
        (Det_LastErrorId == ADC_E_UNINIT) &&
        (Adc_GroupRuntime[TEST_GROUP_0].ResultBufferPtr == NULL_PTR))
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

    Test_Env_Reset();
    Test_Env_InitFromPBcfg(&adcConfig);

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

    Test_Env_Reset();
    Test_Env_InitFromPBcfg(&adcConfig);

    (void)Adc_SetupResultBuffer(TEST_GROUP_0, Test_BufferA);
    Test_Det_Reset();

    retVal = Adc_SetupResultBuffer(TEST_GROUP_0, NULL_PTR);

    if ((retVal == E_NOT_OK) &&
        (Det_ErrorCount == 1U) &&
        (Det_LastModuleId == ADC_MODULE_ID) &&
        (Det_LastInstanceId == ADC_INSTANCE_ID) &&
        (Det_LastApiId == ADC_SID_SETUP_RESULT_BUFFER) &&
        (Det_LastErrorId == ADC_E_PARAM_POINTER) &&
        (Adc_GroupRuntime[TEST_GROUP_0].ResultBufferPtr == Test_BufferA))
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

    Test_Env_Reset();
    Test_Env_InitFromPBcfg(&adcConfig);

    (void)Adc_SetupResultBuffer(TEST_GROUP_0, Test_BufferA);
    Test_Det_Reset();

    /* Simulate active conversion (GroupStatus != ADC_IDLE) */
    Adc_GroupRuntime[TEST_GROUP_0].GroupStatus = ADC_BUSY;

    retVal = Adc_SetupResultBuffer(TEST_GROUP_0, Test_BufferB);

    if ((retVal == E_NOT_OK) &&
        (Det_RuntimeErrorCount == 1U) &&
        (Det_LastRuntimeModuleId == ADC_MODULE_ID) &&
        (Det_LastRuntimeInstanceId == ADC_INSTANCE_ID) &&
        (Det_LastRuntimeApiId == ADC_SID_SETUP_RESULT_BUFFER) &&
        (Det_LastRuntimeErrorId == ADC_E_BUSY) &&
        (Adc_GroupRuntime[TEST_GROUP_0].ResultBufferPtr == Test_BufferA))
    {
        Test_Result = TEST_PASS;
    }
    else
    {
        Test_Result = TEST_FAIL;
    }
}

/* ============================================================================
 * 4. EXTENSIBLE TEST TABLE & RUNNER
 * ============================================================================ */
typedef struct
{
    const char* TestName;
    void (*TestFunc)(void);
} Test_CaseEntryType;

static const Test_CaseEntryType Test_CaseTable[] =
{
    { "IT_001_ValidBufferSetup",    Test_Adc_SetupResultBuffer_It_001 },
    { "IT_002_UninitDriverError",   Test_Adc_SetupResultBuffer_It_002 },
    { "IT_003_InvalidGroupIdError", Test_Adc_SetupResultBuffer_It_003 },
    { "IT_004_NullBufferPtrError",  Test_Adc_SetupResultBuffer_It_004 },
    { "IT_005_GroupBusyRuntimeErr", Test_Adc_SetupResultBuffer_It_005 }
};

#define TOTAL_TEST_CASES (sizeof(Test_CaseTable) / sizeof(Test_CaseTable[0]))

uint32 Test_Adc_SetupResultBuffer_RunAll(void)
{
    uint32 i;
    uint32 passCount = 0U;

    for (i = 0U; i < TOTAL_TEST_CASES; i++)
    {
        Test_Result = TEST_FAIL;
        Test_CaseTable[i].TestFunc();

        if (Test_Result == TEST_PASS)
        {
            passCount++;
        }
    }

    return passCount;
}