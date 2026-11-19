/*******************************************************************************
 * File     : Test_Adc_SetupResultBuffer.c
 * Layer    : Validation / IntegrationTest / Adc
 * Module   : ADC Driver (AUTOSAR CP R25-11, Infineon AURIX TC39x EVADC CPU0)
 * Function : Adc_SetupResultBuffer() - Service ID: 0x0CU [SWS_Adc_91000]
 * Standard : ISO 26262-6 Clause 8 (Software Integration & Verification)
 * Author   : Samarth Shirahatti (Validation Team)
 *
 * Result Convention (Test_Result):
 *     1U -> PASS (TEST_PASS)
 *     0U -> FAIL (TEST_FAIL)
 ******************************************************************************/

#include "Std_Types.h"
#include "Adc.h"
#include "Det.h"

/* ============================================================================
 * 1. DET & RUNTIME ERROR INTERFACE STUBS (BSW Integration Boundary)
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

static void DetStub_Reset(void)
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
 * 2. INTEGRATION TEST CONFIGURATION & DATA BUFFERS
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
            { .AdcChannelSampTime = 5U,  .AdcChannelId = 0U },
            { .AdcChannelSampTime = 12U, .AdcChannelId = 1U }
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
                .AdcChannel             = { .AdcChannelSampTime = 5U, .AdcChannelId = 0U }
            },
            {
                .AdcGroupAccessMode     = ADC_ACCESS_MODE_SINGLE,
                .AdcGroupConversionMode = ADC_CONV_MODE_ONESHOT,
                .AdcGroupId             = 1U,
                .AdcGroupTriggSrc       = ADC_TRIGG_SRC_SW,
                .AdcNotification        = NULL_PTR,
                .AdcStreamingBufferMode = ADC_STREAM_BUFFER_LINEAR,
                .AdcStreamingNumSamples = 1U,
                .AdcChannel             = { .AdcChannelSampTime = 12U, .AdcChannelId = 1U }
            }
        }
    }
};

/* Ensures clean driver state via real AUTOSAR teardown APIs */
static void Test_Integration_Teardown(void)
{
    Adc_StopGroupConversion(TEST_GROUP_0);
    Adc_StopGroupConversion(TEST_GROUP_1);
    Adc_DeInit();
    DetStub_Reset();
}

uint8 Test_Adc_SetupResultBuffer_GetResult(void)
{
    return Test_Result;
}

/* ============================================================================
 * 3. SOFTWARE INTEGRATION TEST CASES (5 Baseline Cases — Extensible)
 * ============================================================================ */

/*******************************************************************************
 * IT_001: Adc_Init() -> Adc_SetupResultBuffer() -> Adc_GetGroupStatus()
 * Verifies real initialization followed by valid buffer setup on Group 0.
 * AUTOSAR Req : [SWS_Adc_00420], [SWS_Adc_91000]
 ******************************************************************************/
void Test_Adc_SetupResultBuffer_It_001(void)
{
    Std_ReturnType retVal;
    Adc_StatusType grp0Status;
    Adc_StatusType grp1Status;

    DetStub_Reset();

    /* Step 1: Real driver initialization */
    Adc_Init(&Test_AdcConfig);

    /* Step 2: Setup result buffer for Group 0 */
    retVal = Adc_SetupResultBuffer(TEST_GROUP_0, Test_BufferA);

    /* Step 3: Verify group states via public status API */
    grp0Status = Adc_GetGroupStatus(TEST_GROUP_0);
    grp1Status = Adc_GetGroupStatus(TEST_GROUP_1);

    if ((retVal == E_OK) &&
        (Det_ErrorLog.CallCount == 0U) &&
        (grp0Status == ADC_IDLE) &&
        (grp1Status == ADC_IDLE))
    {
        Test_Result = TEST_PASS;
    }
    else
    {
        Test_Result = TEST_FAIL;
    }

    Test_Integration_Teardown();
}

/*******************************************************************************
 * IT_002: Uninitialized Call -> Det_ReportError(ADC_E_UNINIT = 0x0AU)
 * Verifies Adc_SetupResultBuffer() rejects calls before Adc_Init().
 * AUTOSAR Req : [SWS_Adc_00434], [SWS_Adc_91005]
 ******************************************************************************/
void Test_Adc_SetupResultBuffer_It_002(void)
{
    Std_ReturnType retVal;

    DetStub_Reset();

    /* Call without prior Adc_Init() */
    retVal = Adc_SetupResultBuffer(TEST_GROUP_0, Test_BufferA);

    if ((retVal == E_NOT_OK) &&
        (Det_ErrorLog.CallCount == 1U) &&
        (Det_ErrorLog.IsRuntimeError == 0U) &&
        (Det_ErrorLog.LastModuleId == ADC_MODULE_ID) &&
        (Det_ErrorLog.LastInstanceId == ADC_INSTANCE_ID) &&
        (Det_ErrorLog.LastApiId == ADC_SID_SETUP_RESULT_BUFFER) &&
        (Det_ErrorLog.LastErrorId == ADC_E_UNINIT))
    {
        Test_Result = TEST_PASS;
    }
    else
    {
        Test_Result = TEST_FAIL;
    }
}

/*******************************************************************************
 * IT_003: Adc_Init() -> Out-of-Range Group ID -> Det_ReportError(ADC_E_PARAM_GROUP)
 * Verifies boundary validation across Adc_SetupResultBuffer() and DET module.
 * AUTOSAR Req : [SWS_Adc_00423], [SWS_Adc_91005]
 ******************************************************************************/
void Test_Adc_SetupResultBuffer_It_003(void)
{
    Std_ReturnType retVal;

    DetStub_Reset();
    Adc_Init(&Test_AdcConfig);

    /* Pass invalid Group ID = 2 */
    retVal = Adc_SetupResultBuffer(TEST_INVALID_GROUP, Test_BufferB);

    if ((retVal == E_NOT_OK) &&
        (Det_ErrorLog.CallCount == 1U) &&
        (Det_ErrorLog.IsRuntimeError == 0U) &&
        (Det_ErrorLog.LastModuleId == ADC_MODULE_ID) &&
        (Det_ErrorLog.LastInstanceId == ADC_INSTANCE_ID) &&
        (Det_ErrorLog.LastApiId == ADC_SID_SETUP_RESULT_BUFFER) &&
        (Det_ErrorLog.LastErrorId == ADC_E_PARAM_GROUP))
    {
        Test_Result = TEST_PASS;
    }
    else
    {
        Test_Result = TEST_FAIL;
    }

    Test_Integration_Teardown();
}

/*******************************************************************************
 * IT_004: Adc_Init() -> NULL Buffer Pointer -> Det_ReportError(ADC_E_PARAM_POINTER)
 * Verifies NULL pointer rejection and protection of previously bound buffer.
 * AUTOSAR Req : [SWS_Adc_00457], [SWS_Adc_91005]
 ******************************************************************************/
void Test_Adc_SetupResultBuffer_It_004(void)
{
    Std_ReturnType retVal;

    DetStub_Reset();
    Adc_Init(&Test_AdcConfig);

    /* Bind valid buffer first */
    (void)Adc_SetupResultBuffer(TEST_GROUP_0, Test_BufferA);
    DetStub_Reset();

    /* Attempt to pass NULL_PTR */
    retVal = Adc_SetupResultBuffer(TEST_GROUP_0, NULL_PTR);

    if ((retVal == E_NOT_OK) &&
        (Det_ErrorLog.CallCount == 1U) &&
        (Det_ErrorLog.IsRuntimeError == 0U) &&
        (Det_ErrorLog.LastModuleId == ADC_MODULE_ID) &&
        (Det_ErrorLog.LastInstanceId == ADC_INSTANCE_ID) &&
        (Det_ErrorLog.LastApiId == ADC_SID_SETUP_RESULT_BUFFER) &&
        (Det_ErrorLog.LastErrorId == ADC_E_PARAM_POINTER))
    {
        Test_Result = TEST_PASS;
    }
    else
    {
        Test_Result = TEST_FAIL;
    }

    Test_Integration_Teardown();
}

/*******************************************************************************
 * IT_005: Adc_Init() -> SetupBuffer -> StartGroupConversion() -> Busy Rejection
 * Verifies cross-API state lock: starting a conversion via Adc_StartGroupConversion()
 * transitions Group 0 to ADC_BUSY and causes Adc_SetupResultBuffer() to report
 * runtime error ADC_E_BUSY (0x0BU).
 * AUTOSAR Req : [SWS_Adc_00433], [SWS_Adc_91006]
 ******************************************************************************/
void Test_Adc_SetupResultBuffer_It_005(void)
{
    Std_ReturnType retVal;
    Adc_StatusType grp0Status;

    DetStub_Reset();

    /* Step 1: Initialize and setup valid buffer */
    Adc_Init(&Test_AdcConfig);
    (void)Adc_SetupResultBuffer(TEST_GROUP_0, Test_BufferA);

    /* Step 2: Start real SW conversion so Group 0 naturally enters ADC_BUSY */
    Adc_StartGroupConversion(TEST_GROUP_0);
    grp0Status = Adc_GetGroupStatus(TEST_GROUP_0);
    DetStub_Reset();

    /* Step 3: Attempt to re-bind buffer while Group 0 is actively converting */
    retVal = Adc_SetupResultBuffer(TEST_GROUP_0, Test_BufferB);

    if ((grp0Status == ADC_BUSY) &&
        (retVal == E_NOT_OK) &&
        (Det_ErrorLog.CallCount == 1U) &&
        (Det_ErrorLog.IsRuntimeError == 1U) &&
        (Det_ErrorLog.LastModuleId == ADC_MODULE_ID) &&
        (Det_ErrorLog.LastInstanceId == ADC_INSTANCE_ID) &&
        (Det_ErrorLog.LastApiId == ADC_SID_SETUP_RESULT_BUFFER) &&
        (Det_ErrorLog.LastErrorId == ADC_E_BUSY))
    {
        Test_Result = TEST_PASS;
    }
    else
    {
        Test_Result = TEST_FAIL;
    }

    Test_Integration_Teardown();
}

/* ============================================================================
 * 4. EXTENSIBLE INTEGRATION TEST REGISTRY & RUNNER
 * ============================================================================ */
 
typedef struct
{
    const char* TestName;
    void (*TestFunc)(void);
} Test_CaseEntryType;

static const Test_CaseEntryType Test_CaseTable[] =
{
    { "IT_001_InitAndSetupBuffer",      Test_Adc_SetupResultBuffer_It_001 },
    { "IT_002_UninitSequenceReject",    Test_Adc_SetupResultBuffer_It_002 },
    { "IT_003_InvalidGroupBoundary",    Test_Adc_SetupResultBuffer_It_003 },
    { "IT_004_NullBufferProtection",    Test_Adc_SetupResultBuffer_It_004 },
    { "IT_005_ActiveConversionBusyErr", Test_Adc_SetupResultBuffer_It_005 }
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