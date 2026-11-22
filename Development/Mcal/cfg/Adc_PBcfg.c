#include "Adc_PBcfg.h"

/* [SWS_Adc_00505] [ECUC_Adc_00390] Primary Post-Build Configuration Set */
const Adc_ConfigType adcConfig =
{
    .AdcHwUnit =
    {
        .ClkSrc   = 0UL, /* [ECUC_Adc_00087] 0 = Synchronized Clock */
        .HwUnitId = 0UL, /* [ECUC_Adc_00389] EVADC Group 0 Kernel */
        .AdcChannel =
        {
            {
                .AdcChannelSampTime = 1U,             /* [ECUC_Adc_00290] */
                .AdcChannelId       = ADC_CHANNEL_ID0 /* [ECUC_Adc_00392] */
            },
            {
                .AdcChannelSampTime = 2U,             /* [ECUC_Adc_00290] */
                .AdcChannelId       = ADC_CHANNEL_ID1 /* [ECUC_Adc_00392] */
            }
        },
        .AdcGroup =
        {
            /* [ECUC_Adc_00028] Group 0: SW Trigger, One-Shot, Single Access, NULL Callback */
            {
                .AdcGroupAccessMode     = ADC_ACCESS_MODE_SINGLE,   /* [ECUC_Adc_00317] */
                .AdcGroupConversionMode = ADC_CONV_MODE_ONESHOT,    /* [ECUC_Adc_00397] */
                .AdcGroupId             = ADC_GROUP_ID0,            /* [ECUC_Adc_00398] */
                .AdcGroupTriggSrc       = ADC_TRIGG_SRC_SW,         /* [ECUC_Adc_00399] */
                .AdcNotification        = NULL_PTR,                 /* [ECUC_Adc_00402] */
                .AdcStreamingBufferMode = ADC_STREAM_BUFFER_LINEAR, /* [ECUC_Adc_00316] */
                .AdcStreamingNumSamples = 1U,                       /* [ECUC_Adc_00292] */
                .AdcChannel             = { .AdcChannelSampTime = 1U, .AdcChannelId = ADC_CHANNEL_ID0 }
            },
            /* [ECUC_Adc_00028] Group 1: SW Trigger, One-Shot, Single Access, NotifySensor Callback */
            {
                .AdcGroupAccessMode     = ADC_ACCESS_MODE_SINGLE,   /* [ECUC_Adc_00317] */
                .AdcGroupConversionMode = ADC_CONV_MODE_ONESHOT,    /* [ECUC_Adc_00397] */
                .AdcGroupId             = ADC_GROUP_ID1,            /* [ECUC_Adc_00398] */
                .AdcGroupTriggSrc       = ADC_TRIGG_SRC_SW,         /* [ECUC_Adc_00399] */
                .AdcNotification        = NotifySensor,             /* [ECUC_Adc_00402] */
                .AdcStreamingBufferMode = ADC_STREAM_BUFFER_LINEAR, /* [ECUC_Adc_00316] */
                .AdcStreamingNumSamples = 1U,                       /* [ECUC_Adc_00292] */
                .AdcChannel             = { .AdcChannelSampTime = 2U, .AdcChannelId = ADC_CHANNEL_ID1 }
            }
        }
    }
};

/* [SWS_Adc_00505] [ECUC_Adc_00390] Alternate Post-Build Configuration Set */
const Adc_ConfigType adcConfig1 =
{
    .AdcHwUnit =
    {
        .ClkSrc   = 1UL, /* [ECUC_Adc_00087] 1 = Unsynchronized Clock */
        .HwUnitId = 0UL, /* [ECUC_Adc_00389] EVADC Group 0 Kernel */
        .AdcChannel =
        {
            {
                .AdcChannelSampTime = 5U,             /* [ECUC_Adc_00290] */
                .AdcChannelId       = ADC_CHANNEL_ID0 /* [ECUC_Adc_00392] */
            },
            {
                .AdcChannelSampTime = 7U,             /* [ECUC_Adc_00290] */
                .AdcChannelId       = ADC_CHANNEL_ID1 /* [ECUC_Adc_00392] */
            }
        },
        .AdcGroup =
        {
            {
                .AdcGroupAccessMode     = ADC_ACCESS_MODE_SINGLE,
                .AdcGroupConversionMode = ADC_CONV_MODE_ONESHOT,
                .AdcGroupId             = ADC_GROUP_ID0,
                .AdcGroupTriggSrc       = ADC_TRIGG_SRC_SW,
                .AdcNotification        = NULL_PTR,
                .AdcStreamingBufferMode = ADC_STREAM_BUFFER_LINEAR,
                .AdcStreamingNumSamples = 1U,
                .AdcChannel             = { .AdcChannelSampTime = 5U, .AdcChannelId = ADC_CHANNEL_ID0 }
            },
            /* Group 1 configured with ADC_TRIGG_SRC_HW for [SWS_Adc_00133] / [SWS_Adc_00164] */
            {
                .AdcGroupAccessMode     = ADC_ACCESS_MODE_SINGLE,
                .AdcGroupConversionMode = ADC_CONV_MODE_ONESHOT,
                .AdcGroupId             = ADC_GROUP_ID1,
                .AdcGroupTriggSrc       = ADC_TRIGG_SRC_HW,
                .AdcNotification        = NotifySensor,
                .AdcStreamingBufferMode = ADC_STREAM_BUFFER_LINEAR,
                .AdcStreamingNumSamples = 1U,
                .AdcChannel             = { .AdcChannelSampTime = 7U, .AdcChannelId = ADC_CHANNEL_ID1 }
            }
        }
    }
};
