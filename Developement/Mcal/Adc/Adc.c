


void Adc_Init (const Adc_ConfigType* ConfigPtr)
{
    /* Enable the EVADC module clock before accessing its registers. */
    CLC.DISR.B = 0U;
    while (CLC.DISS.B != 0U)
    {
    }

    /* ClkSrc: 0 = synchronized, 1 = unsynchronized; CPWC enables the write. */
    GLOBCFG.U = (1UL << 15U) |
                ((ConfigPtr->AdcHwUnit.ClkSrc & 1UL) << 12U);

    for (uint8 ChIdx = 0U; ChIdx < 2U; ChIdx++)
    {
        const AdcChannelType* ChannelPtr = &ConfigPtr->AdcHwUnit.AdcChannel[ChIdx];

        /* Configure the standard-conversion sample time for this input class. */
        if (ChIdx == 0U)
        {
            G0ICLASS0.STCS.B = ChannelPtr->AdcChannelSampTime;
        }
        else
        {
            G0ICLASS1.STCS.B = ChannelPtr->AdcChannelSampTime;
        }

        /* Select the input class configured above for the requested channel. */
        switch (ChannelPtr->AdcChannelId)
        {
            case 0U:
                G0CHCTR0.ICLSEL.B = ChIdx;
                break;
            case 1U:
                G0CHCTR1.ICLSEL.B = ChIdx;
                break;
            default:
                break;
        }
    }
}

#if ADC_START_STOP_CONV_API == 1
1KB ROM - saving
void Adc_StartGroupConversion (
Adc_GroupType Group
)
{
    // Start group conversion code goes here
}

#endif