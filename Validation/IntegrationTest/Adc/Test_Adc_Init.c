#include "Adc.h"
#include "Det.h"

/* Spy variables provided by the Det stub to record reported errors */
extern uint16 Det_LastModuleId;
extern uint8  Det_LastInstanceId;
extern uint8  Det_LastApiId;
extern uint8  Det_LastErrorId;
extern uint32 Det_ErrorCount;

/* Valid Post-Build Configuration Structure matching Adc.h */
static const Adc_ConfigType Test_ValidAdcConfig = {
    .AdcHwUnit = {
        .ClkSrc   = 0U, /* 0 = Synchronized clock source */
        .HwUnitId = 0U, /* EVADC HW Unit 0 */
        .AdcChannel = {
            { .AdcChannelSampTime = 5U,  .AdcChannelId = 0U },
            { .AdcChannelSampTime = 10U, .AdcChannelId = 1U }
        },
        .AdcGroup = {
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
                .AdcChannel             = { .AdcChannelSampTime = 10U, .AdcChannelId = 1U }
            }
        }
    }
};

/* Helper function to clear DET spy logs before each test case */
static void Reset_DetSpy(void)
{
    Det_LastModuleId   = 0U;
    Det_LastInstanceId = 0U;
    Det_LastApiId      = 0U;
    Det_LastErrorId    = 0U;
    Det_ErrorCount     = 0U;
}

/* TC_ADC_INIT_006: Pass NULL_PTR before initialization -> Expect ADC_E_PARAM_POINTER (0x14) */
void TC_ADC_INIT_006_NullConfigPtr(void)
{
    Reset_DetSpy();

    Adc_Init(NULL_PTR);

    TEST_ASSERT_EQUAL(1U, Det_ErrorCount);
    TEST_ASSERT_EQUAL(ADC_MODULE_ID, Det_LastModuleId);
    TEST_ASSERT_EQUAL(ADC_SID_INIT, Det_LastApiId);
    TEST_ASSERT_EQUAL(ADC_E_PARAM_POINTER, Det_LastErrorId);
}

/* TC_ADC_INIT_001, 005, 007, 008: Pre-Init Uninit Check -> Valid Init -> Post-Init Integration */
void TC_ADC_INIT_001_005_007_008_ValidInitIntegration(void)
{
    Adc_ValueGroupType ResultBuf[2];
    Std_ReturnType ret;

    Reset_DetSpy();

    /* Step 1 (TC_ADC_INIT_008): Call another API before Adc_Init -> Must fail with ADC_E_UNINIT (0x0A) */
    ret = Adc_SetupResultBuffer(0U, ResultBuf);
    TEST_ASSERT_EQUAL(E_NOT_OK, ret);
    TEST_ASSERT_EQUAL(ADC_E_UNINIT, Det_LastErrorId);

    /* Step 2 (TC_ADC_INIT_005 & 007): Initialize driver with valid config */
    Reset_DetSpy();
    Adc_Init(&Test_ValidAdcConfig);
    TEST_ASSERT_EQUAL(0U, Det_ErrorCount);

    /* Step 3 (TC_ADC_INIT_001): Verify all groups are in ADC_IDLE state */
    TEST_ASSERT_EQUAL(ADC_IDLE, Adc_GetGroupStatus(0U));
    TEST_ASSERT_EQUAL(ADC_IDLE, Adc_GetGroupStatus(1U));

    /* Step 4 (Integration check): Now Adc_SetupResultBuffer must succeed (E_OK) */
    ret = Adc_SetupResultBuffer(0U, ResultBuf);
    TEST_ASSERT_EQUAL(E_OK, ret);
}

/* TC_ADC_INIT_002: Call Adc_Init again when already initialized -> Expect ADC_E_ALREADY_INITIALIZED (0x0D) */
void TC_ADC_INIT_002_AlreadyInitialized(void)
{
    Reset_DetSpy();

    Adc_Init(&Test_ValidAdcConfig);

    TEST_ASSERT_EQUAL(1U, Det_ErrorCount);
    TEST_ASSERT_EQUAL(ADC_SID_INIT, Det_LastApiId);
    TEST_ASSERT_EQUAL(ADC_E_ALREADY_INITIALIZED, Det_LastErrorId);
}