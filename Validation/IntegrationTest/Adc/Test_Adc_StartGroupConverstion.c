#include "Std_Types.h"
#include "Adc.h"
#include "Adc_Cfg.h"
#include "Adc_PBcfg.h"
#include "Det.h"

/* Include driver implementation directly to access module static runtime state */
#include "../../../Development/Mcal/Adc/Adc_StartGroupConversion.c"

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
    Adc_ConfigPtr   = NULL_PTR;

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
        Adc_ConfigPtr = ConfigPtr;
        for (grpIdx = 0U; grpIdx < ADC_MAX_GROUPS_PER_HW_UNIT; grpIdx++)
        {
            Adc_GroupRuntime[grpIdx].GroupStatus         = ADC_IDLE;
            Adc_GroupRuntime[grpIdx].ResultBufferPtr     = NULL_PTR;
            Adc_GroupRuntime[grpIdx].NotificationEnabled = FALSE;
        }
        Adc_DriverState = ADC_STATE_INIT;
    }
}

uint8 Test_Adc_StartGroupConversion_GetResult(void)
{
    return Test_Result;
}


/*******************************************************************************
 * IT_001: Valid Software Conversion Start ([SWS_Adc_00061], [SWS_Adc_00367])
 ******************************************************************************/
void Test_Adc_StartGroupConversion_It_001(void)
{
    Test_Env_Reset();
    Test_Env_InitFromPBcfg(&adcConfig);

    Adc_GroupRuntime[TEST_GROUP_0].ResultBufferPtr = Test_Buffer;

    Adc_StartGroupConversion(TEST_GROUP_0);

    if ((Det_ErrorCount == 0U) &&
        (Det_RuntimeErrorCount == 0U) &&
        (Adc_GroupRuntime[TEST_GROUP_0].GroupStatus == ADC_BUSY) &&
        (Adc_GroupRuntime[TEST_GROUP_1].GroupStatus == ADC_IDLE))
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
 ******************************************************************************/
void Test_Adc_StartGroupConversion_It_002(void)
{
    Test_Env_Reset(); /* Driver remains ADC_STATE_UNINIT */

    Adc_StartGroupConversion(TEST_GROUP_0);

    if ((Det_ErrorCount == 1U) &&
        (Det_LastModuleId == ADC_MODULE_ID) &&
        (Det_LastInstanceId == ADC_INSTANCE_ID) &&
        (Det_LastApiId == ADC_SID_START_GROUP_CONVERSION) &&
        (Det_LastErrorId == ADC_E_UNINIT) &&
        (Adc_GroupRuntime[TEST_GROUP_0].GroupStatus == ADC_IDLE))
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
    Test_Env_Reset();
    Test_Env_InitFromPBcfg(&adcConfig);

    Adc_StartGroupConversion(TEST_INVALID_GROUP);

    if ((Det_ErrorCount == 1U) &&
        (Det_LastModuleId == ADC_MODULE_ID) &&
        (Det_LastInstanceId == ADC_INSTANCE_ID) &&
        (Det_LastApiId == ADC_SID_START_GROUP_CONVERSION) &&
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
 * IT_004: Result Buffer Not Initialized ([SWS_Adc_00424], [SWS_Adc_91005])
 ******************************************************************************/
void Test_Adc_StartGroupConversion_It_004(void)
{
    Test_Env_Reset();
    Test_Env_InitFromPBcfg(&adcConfig);

    /* ResultBufferPtr intentionally left NULL_PTR */
    Adc_StartGroupConversion(TEST_GROUP_0);

    if ((Det_ErrorCount == 1U) &&
        (Det_LastModuleId == ADC_MODULE_ID) &&
        (Det_LastInstanceId == ADC_INSTANCE_ID) &&
        (Det_LastApiId == ADC_SID_START_GROUP_CONVERSION) &&
        (Det_LastErrorId == ADC_E_BUFFER_UNINIT) &&
        (Adc_GroupRuntime[TEST_GROUP_0].GroupStatus == ADC_IDLE))
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
    Test_Env_Reset();
    Test_Env_InitFromPBcfg(&adcConfig_HwTrigg);

    Adc_GroupRuntime[TEST_GROUP_0].ResultBufferPtr = Test_Buffer;

    Adc_StartGroupConversion(TEST_GROUP_0);

    if ((Det_ErrorCount == 1U) &&
        (Det_LastModuleId == ADC_MODULE_ID) &&
        (Det_LastInstanceId == ADC_INSTANCE_ID) &&
        (Det_LastApiId == ADC_SID_START_GROUP_CONVERSION) &&
        (Det_LastErrorId == ADC_E_WRONG_TRIGG_SRC) &&
        (Adc_GroupRuntime[TEST_GROUP_0].GroupStatus == ADC_IDLE))
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
    Test_Env_Reset();
    Test_Env_InitFromPBcfg(&adcConfig);

    Adc_GroupRuntime[TEST_GROUP_0].ResultBufferPtr = Test_Buffer;

    /* First call starts conversion and transitions Group 0 to ADC_BUSY */
    Adc_StartGroupConversion(TEST_GROUP_0);
    Test_Det_Reset();

    /* Second call while already ADC_BUSY must report runtime error ADC_E_BUSY */
    Adc_StartGroupConversion(TEST_GROUP_0);

    if ((Det_RuntimeErrorCount == 1U) &&
        (Det_LastRuntimeModuleId == ADC_MODULE_ID) &&
        (Det_LastRuntimeInstanceId == ADC_INSTANCE_ID) &&
        (Det_LastRuntimeApiId == ADC_SID_START_GROUP_CONVERSION) &&
        (Det_LastRuntimeErrorId == ADC_E_BUSY) &&
        (Adc_GroupRuntime[TEST_GROUP_0].GroupStatus == ADC_BUSY))
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
    { "IT_001_ValidStartConversion", Test_Adc_StartGroupConversion_It_001 },
    { "IT_002_UninitDriverError",    Test_Adc_StartGroupConversion_It_002 },
    { "IT_003_InvalidGroupIdError",  Test_Adc_StartGroupConversion_It_003 },
    { "IT_004_BufferUninitError",    Test_Adc_StartGroupConversion_It_004 },
    { "IT_005_WrongTriggerSrcError", Test_Adc_StartGroupConversion_It_005 },
    { "IT_006_GroupBusyRuntimeErr",  Test_Adc_StartGroupConversion_It_006 }
};

#define TOTAL_TEST_CASES (sizeof(Test_CaseTable) / sizeof(Test_CaseTable[0]))

uint32 Test_Adc_StartGroupConversion_RunAll(void)
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