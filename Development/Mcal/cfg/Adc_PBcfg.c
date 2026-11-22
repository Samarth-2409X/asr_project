#include "Adc.h"
#include "Adc_PBcfg.h"

/* Standard Post-Build Configuration (SW Triggered Groups) */
const Adc_ConfigType adcConfig =
{
    .AdcHwUnit =
    {
        .ClkSrc   = 0U,
        .HwUnitId = 0U,
        .AdcChannel =
        {
            { .AdcChannelSampTime = 1U, .AdcChannelId = 0U },
            { .AdcChannelSampTime = 2U, .AdcChannelId = 1U }
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
                .AdcNotification        = NotifySensor,
                .AdcStreamingBufferMode = ADC_STREAM_BUFFER_LINEAR,
                .AdcStreamingNumSamples = 1U,
                .AdcChannel             = { .AdcChannelSampTime = 2U, .AdcChannelId = 1U }
            }
        }
    }
};

/* Post-Build Configuration with HW Trigger on Group 0 (For [SWS_Adc_00133] Verification) */
const Adc_ConfigType adcConfig_HwTrigg =
{
    .AdcHwUnit =
    {
        .ClkSrc   = 0U,
        .HwUnitId = 0U,
        .AdcChannel =
        {
            { .AdcChannelSampTime = 1U, .AdcChannelId = 0U },
            { .AdcChannelSampTime = 2U, .AdcChannelId = 1U }
        },
        .AdcGroup =
        {
            {
                .AdcGroupAccessMode     = ADC_ACCESS_MODE_SINGLE,
                .AdcGroupConversionMode = ADC_CONV_MODE_ONESHOT,
                .AdcGroupId             = 0U,
                .AdcGroupTriggSrc       = ADC_TRIGG_SRC_HW,
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
                .AdcNotification        = NotifySensor,
                .AdcStreamingBufferMode = ADC_STREAM_BUFFER_LINEAR,
                .AdcStreamingNumSamples = 1U,
                .AdcChannel             = { .AdcChannelSampTime = 2U, .AdcChannelId = 1U }
            }
        }
    }
};