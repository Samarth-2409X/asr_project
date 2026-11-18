/*******************************************************************************
 * File     : Test_Adc_SetupResultBuffer.c
 * Module   : ADC Driver (AUTOSAR CP R25-11, Infineon AURIX TC39x EVADC CPU0)
 * Function : Adc_SetupResultBuffer() - Service ID: 0x0CU [SWS_Adc_91000]
 * Author   : Samarth Shirahatti (Validation Team)
 *
 * Description:
 *   Complete, self-contained white-box unit test suite with full DET & Runtime
 *   error stub logic, driver state reset logic, and extensible test registry.
 *
 * Result Convention (Test_Result):
 *     1U -> PASS (TEST_PASS)
 *     0U -> FAIL (TEST_FAIL)
 ******************************************************************************/

#include "Std_Types.h"
#include "Adc.h"

/* ============================================================================
 * 1. FULL DET & RUNTIME ERROR STUB IMPLEMENTATION (Self-Contained)
 * ============================================================================ */
typedef struct
{
    uint32 CallCount;
    uint16 LastModuleId;
    uint8  LastInstanceId;
    uint8  LastApiId;
    uint8  LastErrorId;
    uint8  IsRuntimeError;
} Det_LogType;

static Det_LogType Det_ErrorLog;

void DetStub_Reset(void)
{
    Det_ErrorLog.CallCount      = 0U;
    Det_ErrorLog.LastModuleId   = 0U;
    Det_ErrorLog.LastInstanceId = 0U;
    Det_ErrorLog.LastApiId      = 0U;
    Det_ErrorLog.LastErrorId    = 0U;
    Det_ErrorLog.IsRuntimeError = 0U;
}

Std_ReturnType Det_ReportError(
    uint16 ModuleId,
    uint8  InstanceId,
    uint8  ApiId,
    uint8  ErrorId
)
{
    Det_ErrorLog.CallCount++;
    Det_ErrorLog.LastModuleId   = ModuleId;
    Det_ErrorLog.LastInstanceId = InstanceId;
    Det_ErrorLog.LastApiId      = ApiId;
    Det_ErrorLog.LastErrorId    = ErrorId;
    Det_ErrorLog.IsRuntimeError = 0U;
    return E_OK;
}

Std_ReturnType Det_ReportRuntimeError(
    uint16 ModuleId,
    uint8  InstanceId,
    uint8  ApiId,
    uint8  ErrorId
)
{
    Det_ErrorLog.CallCount++;
    Det_ErrorLog.LastModuleId   = ModuleId;
    Det_ErrorLog.LastInstanceId = InstanceId;
    Det_ErrorLog.LastApiId      = ApiId;
    Det_ErrorLog.LastErrorId    = ErrorId;
    Det_ErrorLog.IsRuntimeError = 1U;
    return E_OK;
}

/* ============================================================================
 * 2. DRIVER UNDER TEST (White-Box Inclusion for Static State Access)
 * ============================================================================ */
#include "../../../Developement/Adc_SetupResultBuffer.c"

/* Local helper to initialize driver state for standalone unit testing */
void Test_Helper_InitDriver(const Adc_ConfigType* ConfigPtr)
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

/* Resets all internal static driver variables to power-on reset state */
void Test_Helper_ResetDriver(void)
{
    Adc_GroupType grpIdx;

    Adc_DriverState = ADC_STATE_UNINIT;
    for (grpIdx = 0U; grpIdx < ADC_MAX_GROUPS_PER_HW_UNIT; grpIdx++)
    {
        Adc_GroupRuntime[grpIdx].GroupStatus         = ADC_IDLE;
        Adc_GroupRuntime[grpIdx].ResultBufferPtr     = NULL_PTR;
        Adc_GroupRuntime[grpIdx].NotificationEnabled = FALSE;
    }
    DetStub_Reset();
}

/* ============================================================================
 * 3. TEST CONFIGURATION & DATA BUFFERS
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

static const Adc_ConfigType Test_AdcConfig =
{
    .AdcHwUnit =
    {
        .ClkSrc   = 0U,
        .HwUnitId = 0U,
        .AdcChannel =
        {
            { .AdcChannelSampTime = 1U, .AdcChannelId = 0U },
            { .AdcChannelSampTime = 1U, .AdcChannelId = 1U }
        },
        .AdcGroup =
        {
            {
                .AdcGroupAccessMode     = ADC_ACCESS_MODE_SINGLE,
                .AdcGroupConversionMode = ADC_CONV_MODE_ONESHOT,
                .AdcGroupId             = 0U,
                .AdcGroupTriggSrc       = ADC_TRIGG_SRC_SW,
                .AdcNotification        = NULL_PTR,
                .AdcStreamingBufferMode = ADC_STREAM_BUFFER_LINEAR,
                .AdcStreamingNumSamples = 1U,
                .AdcChannel             = { .AdcChannelSampTime = 1U, .AdcChannelId = 0U }
            },
            {
                .AdcGroupAccessMode     = ADC_ACCESS_MODE_SINGLE,
                .AdcGroupConversionMode = ADC_CONV_MODE_ONESHOT,
                .AdcGroupId             = 1U,
                .AdcGroupTriggSrc       = ADC_TRIGG_SRC_SW,
                .AdcNotification        = NULL_PTR,
                .AdcStreamingBufferMode = ADC_STREAM_BUFFER_LINEAR,
                .AdcStreamingNumSamples = 1U,
                .AdcChannel             = { .AdcChannelSampTime = 1U, .AdcChannelId = 1U }
            }
        }
    }
};

uint8 Test_Adc_SetupResultBuffer_GetResult(void)
{
    return Test_Result;
}

/* ============================================================================
 * 4. TEST CASES (Full Concrete Logic)
 * ============================================================================ */

/*******************************************************************************
 * TC_001: Valid Group & Valid Buffer Allocation (Normal Operation)
 * AUTOSAR Req : [SWS_Adc_00420], [SWS_Adc_91000]
 ******************************************************************************/
void Test_Adc_SetupResultBuffer_Tc_001(void)
{
    Std_ReturnType retVal;

    Test_Helper_ResetDriver();
    Test_Helper_InitDriver(&Test_AdcConfig);

    retVal = Adc_SetupResultBuffer(TEST_GROUP_0, Test_BufferA);

    if ((retVal == E_OK) &&
        (Det_ErrorLog.CallCount == 0U) &&
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
 * TC_002: API Called Before Driver Initialization (ADC_E_UNINIT = 0x0AU)
 * AUTOSAR Req : [SWS_Adc_00434], [SWS_Adc_91005]
 ******************************************************************************/
void Test_Adc_SetupResultBuffer_Tc_002(void)
{
    Std_ReturnType retVal;

    Test_Helper_ResetDriver(); /* Adc_DriverState is ADC_STATE_UNINIT */

    retVal = Adc_SetupResultBuffer(TEST_GROUP_0, Test_BufferA);

    if ((retVal == E_NOT_OK) &&
        (Det_ErrorLog.CallCount == 1U) &&
        (Det_ErrorLog.IsRuntimeError == 0U) &&
        (Det_ErrorLog.LastModuleId == ADC_MODULE_ID) &&
        (Det_ErrorLog.LastInstanceId == ADC_INSTANCE_ID) &&
        (Det_ErrorLog.LastApiId == ADC_SID_SETUP_RESULT_BUFFER) &&
        (Det_ErrorLog.LastErrorId == ADC_E_UNINIT) &&
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
 * TC_003: Invalid / Out-of-Range Group ID Check (ADC_E_PARAM_GROUP = 0x15U)
 * AUTOSAR Req : [SWS_Adc_00423], [SWS_Adc_91005]
 ******************************************************************************/
void Test_Adc_SetupResultBuffer_Tc_003(void)
{
    Std_ReturnType retVal;

    Test_Helper_ResetDriver();
    Test_Helper_InitDriver(&Test_AdcConfig);

    /* Pre-assign valid buffer to Group 0 */
    (void)Adc_SetupResultBuffer(TEST_GROUP_0, Test_BufferA);
    DetStub_Reset();

    /* Pass invalid Group ID = 2 */
    retVal = Adc_SetupResultBuffer(TEST_INVALID_GROUP, Test_BufferB);

    if ((retVal == E_NOT_OK) &&
        (Det_ErrorLog.CallCount == 1U) &&
        (Det_ErrorLog.IsRuntimeError == 0U) &&
        (Det_ErrorLog.LastModuleId == ADC_MODULE_ID) &&
        (Det_ErrorLog.LastInstanceId == ADC_INSTANCE_ID) &&
        (Det_ErrorLog.LastApiId == ADC_SID_SETUP_RESULT_BUFFER) &&
        (Det_ErrorLog.LastErrorId == ADC_E_PARAM_GROUP) &&
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
 * TC_004: NULL Buffer Pointer Check (ADC_E_PARAM_POINTER = 0x14U)
 * AUTOSAR Req : [SWS_Adc_00457], [SWS_Adc_91005]
 ******************************************************************************/
void Test_Adc_SetupResultBuffer_Tc_004(void)
{
    Std_ReturnType retVal;

    Test_Helper_ResetDriver();
    Test_Helper_InitDriver(&Test_AdcConfig);

    /* Pre-assign valid buffer to Group 0 */
    (void)Adc_SetupResultBuffer(TEST_GROUP_0, Test_BufferA);
    DetStub_Reset();

    /* Pass NULL_PTR */
    retVal = Adc_SetupResultBuffer(TEST_GROUP_0, NULL_PTR);

    if ((retVal == E_NOT_OK) &&
        (Det_ErrorLog.CallCount == 1U) &&
        (Det_ErrorLog.IsRuntimeError == 0U) &&
        (Det_ErrorLog.LastModuleId == ADC_MODULE_ID) &&
        (Det_ErrorLog.LastInstanceId == ADC_INSTANCE_ID) &&
        (Det_ErrorLog.LastApiId == ADC_SID_SETUP_RESULT_BUFFER) &&
        (Det_ErrorLog.LastErrorId == ADC_E_PARAM_POINTER) &&
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
 * TC_005: Group Busy Runtime Error Check (ADC_E_BUSY = 0x0BU)
 * AUTOSAR Req : [SWS_Adc_00433], [SWS_Adc_91006]
 ******************************************************************************/
void Test_Adc_SetupResultBuffer_Tc_005(void)
{
    Std_ReturnType retVal;

    Test_Helper_ResetDriver();
    Test_Helper_InitDriver(&Test_AdcConfig);

    /* Pre-assign valid buffer to Group 0 */
    (void)Adc_SetupResultBuffer(TEST_GROUP_0, Test_BufferA);
    DetStub_Reset();

    /* Simulate ongoing conversion (group not in ADC_IDLE state) */
    Adc_GroupRuntime[TEST_GROUP_0].GroupStatus = ADC_BUSY;

    /* Attempt to overwrite buffer pointer while Group 0 is BUSY */
    retVal = Adc_SetupResultBuffer(TEST_GROUP_0, Test_BufferB);

    if ((retVal == E_NOT_OK) &&
        (Det_ErrorLog.CallCount == 1U) &&
        (Det_ErrorLog.IsRuntimeError == 1U) &&
        (Det_ErrorLog.LastModuleId == ADC_MODULE_ID) &&
        (Det_ErrorLog.LastInstanceId == ADC_INSTANCE_ID) &&
        (Det_ErrorLog.LastApiId == ADC_SID_SETUP_RESULT_BUFFER) &&
        (Det_ErrorLog.LastErrorId == ADC_E_BUSY) &&
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
 * 5. EXTENSIBLE TEST REGISTRY & RUNNER
 * ============================================================================ */
typedef struct
{
    const char* TestName;
    void (*TestFunc)(void);
} Test_CaseEntryType;

static const Test_CaseEntryType Test_CaseTable[] =
{
    { "TC_001_ValidSetup",      Test_Adc_SetupResultBuffer_Tc_001 },
    { "TC_002_UninitError",     Test_Adc_SetupResultBuffer_Tc_002 },
    { "TC_003_InvalidGroup",    Test_Adc_SetupResultBuffer_Tc_003 },
    { "TC_004_NullPointer",     Test_Adc_SetupResultBuffer_Tc_004 },
    { "TC_005_BusyRuntimeErr",  Test_Adc_SetupResultBuffer_Tc_005 }
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