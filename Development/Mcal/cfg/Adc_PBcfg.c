Adc_ConfigType adcConfig = {
    .AdcHwUnit = {
        .ClkSrc = 0,
        .HwUnitId = 0,
        .AdcChannel = {
            {   /* AdcCh0*/         
                .AdcChannelSampTime = 1,
                .AdcChannelId = 0
            },
            {   /* AdcCh1*/         
                .AdcChannelSampTime = 2,
                .AdcChannelId = 1
            }
        },
        .AdcGroup[0]
    }
};

AdcGroupType AdcGroup = {   
    {
    .AdcGroupAccessMode = ADC_ACCESS_MODE_SINGLE,
    .AdcGroupConversionMode = ADC_CONV_MODE_ONESHOT,
    .AdcGroupId = 0,
    .AdcGroupTriggSrc = ADC_TRIGG_SRC_SW,
    .AdcNotification = NULL,
    .AdcStreamingBufferMode = ADC_STREAM_BUFFER_LINEAR,
    .AdcStreamingNumSamples = 1,
    .AdcChannel = {
        .AdcChannelSampTime = 1,
        .AdcChannelId = 0
    }
    {
        .AdcGroupAccessMode = ADC_ACCESS_MODE_SINGLE,
        .AdcGroupConversionMode = ADC_CONV_MODE_ONESHOT,
        .AdcGroupId = 1,
        .AdcGroupTriggSrc = ADC_TRIGG_SRC_SW,
        .AdcNotification = NotifySensor,
        .AdcStreamingBufferMode = ADC_STREAM_BUFFER_LINEAR,
        .AdcStreamingNumSamples = 1,
        .AdcChannel = {
            .AdcChannelSampTime = 2,
            .AdcChannelId = 1
        }
    }
    }
};


Adc_ConfigType adcConfig1 = { 
    .AdcHwUnit = {
        .ClkSrc = 0,
        .HwUnitId = 1,
        .AdcChannel = {
            {   /* AdcCh0*/         
                .AdcChannelSampTime = 1,
                .AdcChannelId = 0
            },
            {   /* AdcCh1*/         
                .AdcChannelSampTime = 2,
                .AdcChannelId = 1
            }
        },
        .AdcGroup[1]
    }
};