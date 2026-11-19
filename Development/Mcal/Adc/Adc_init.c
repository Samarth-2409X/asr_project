/* [SWS_Adc_00365] AUTOSAR Classic Platform R25-11 ADC Driver - Adc_Init Implementation (TC39x EVADC CPU0) */

#include "Adc.h"

#ifndef ADC_DEV_ERROR_DETECT
  #define ADC_DEV_ERROR_DETECT                  (STD_ON)
#endif

#if (ADC_DEV_ERROR_DETECT == STD_ON)
  #include "Det.h"
#endif

/* TC39x EVADC Register Bitfield Structures (Base: 0xF0020000UL) */
#ifndef ADC_EVADC_REGS_DEFINED
#define ADC_EVADC_REGS_DEFINED

typedef struct Adc_RegBit_Tag
{
    uint32 B;
} Adc_RegBitType;

typedef struct Adc_EvadcClc_Tag
{
    Adc_RegBitType DISR; /* Clock Disable Request Bit */
    Adc_RegBitType DISS; /* Clock Disable Status Bit */
} Adc_EvadcClcType;

typedef struct Adc_EvadcGlobcfg_Tag
{
    uint32 U;            /* Global Configuration Register */
} Adc_EvadcGlobcfgType;

typedef struct Adc_EvadcIclass_Tag
{
    Adc_RegBitType STCS; /* [SWS_Adc_00511] Sample Time Control for Standard Conversions (Bits [4:0]) */
} Adc_EvadcIclassType;

typedef struct Adc_EvadcChctr_Tag
{
    Adc_RegBitType ICLSEL; /* Input Class Selection (Bits [1:0]) */
} Adc_EvadcChctrType;

#define CLC                                     (*((volatile Adc_EvadcClcType*)0xF0020000UL))
#define GLOBCFG                                 (*((volatile Adc_EvadcGlobcfgType*)0xF0020080UL))
#define G0ICLASS0                               (*((volatile Adc_EvadcIclassType*)0xF00204A0UL))
#define G0ICLASS1                               (*((volatile Adc_EvadcIclassType*)0xF00204A4UL))
#define G0CHCTR0                                (*((volatile Adc_EvadcChctrType*)0xF0020600UL))
#define G0CHCTR1                                (*((volatile Adc_EvadcChctrType*)0xF0020604UL))

#endif /* ADC_EVADC_REGS_DEFINED */

/* EVADC Register Configuration Constants */
#define ADC_GLOBCFG_CPWC_MASK                   (1UL << 15U) /* Write control enable for SUPLEV/USC */
#define ADC_GLOBCFG_USC_SHIFT                   (12U)        /* Unsynchronized clock source shift */
#define ADC_GLOBCFG_CLKSRC_MASK                 (1UL)        /* Clock source bit mask */
#define ADC_STCS_MASK                           (0x1FU)      /* 5-bit STCS bitfield mask (0U..31U) */

/* [SWS_BSW_00071] Internal ADC Driver Initialization State */
typedef enum Adc_StateType_Tag
{
    ADC_STATE_UNINIT = 0x00U,
    ADC_STATE_INIT   = 0x01U
} Adc_StateType;

/* Per-Group Runtime State Structure ([SWS_Adc_00077], [SWS_Adc_00307]) */
typedef struct Adc_GroupRuntimeType_Tag
{
    Adc_StatusType      GroupStatus;         /* [SWS_Adc_00307] Current group conversion status */
    Adc_ValueGroupType* ResultBufferPtr;     /* [SWS_Adc_00420] Application result buffer pointer */
    boolean             NotificationEnabled; /* [SWS_Adc_00077] Group notification state */
} Adc_GroupRuntimeType;

/* Module Internal State Variables */
static Adc_StateType          Adc_DriverState = ADC_STATE_UNINIT;
static const Adc_ConfigType*  Adc_ConfigPtr   = NULL_PTR;
static Adc_GroupRuntimeType   Adc_GroupRuntime[ADC_MAX_GROUPS_PER_HW_UNIT];

/* [SWS_Adc_00365] 8.3.1 Initializes the ADC hardware units and driver (Service ID: 0x00U) */
void Adc_Init(const Adc_ConfigType* ConfigPtr)
{
    uint8         ChIdx;
    Adc_GroupType GrpIdx;

    /* [SWS_Adc_00107] Report ADC_E_ALREADY_INITIALIZED if Adc_Init is called when already initialized */
    if (Adc_DriverState == ADC_STATE_INIT)
    {
#if (ADC_DEV_ERROR_DETECT == STD_ON)
        (void)Det_ReportError(ADC_MODULE_ID, ADC_INSTANCE_ID, ADC_SID_INIT, ADC_E_ALREADY_INITIALIZED);
#endif
        return;
    }

    /* [SWS_Adc_00054] Validate configuration pointer */
    if (ConfigPtr == NULL_PTR)
    {
#if (ADC_DEV_ERROR_DETECT == STD_ON)
        (void)Det_ReportError(ADC_MODULE_ID, ADC_INSTANCE_ID, ADC_SID_INIT, ADC_E_PARAM_POINTER);
#endif
        return;
    }

    /* Store active configuration pointer */
    Adc_ConfigPtr = ConfigPtr;

    /* Enable the EVADC module clock before accessing its registers */
    CLC.DISR.B = 0U;
    while (CLC.DISS.B != 0U)
    {
        /* Wait until EVADC module clock is enabled */
    }

    /* [ECUC_Adc_00489] ClkSrc: 0 = synchronized, 1 = unsynchronized; CPWC enables the write */
    GLOBCFG.U = ADC_GLOBCFG_CPWC_MASK |
                ((ConfigPtr->AdcHwUnit.ClkSrc & ADC_GLOBCFG_CLKSRC_MASK) << ADC_GLOBCFG_USC_SHIFT);

    /* [SWS_Adc_00056] Initialize only configured EVADC channels and sampling times */
    for (ChIdx = 0U; ChIdx < ADC_MAX_CHANNELS_PER_HW_UNIT; ChIdx++)
    {
        const AdcChannelType* ChannelPtr = &ConfigPtr->AdcHwUnit.AdcChannel[ChIdx];

        /* [SWS_Adc_00511] Configure standard-conversion sample time (STCS) for this input class */
        if (ChIdx == 0U)
        {
            G0ICLASS0.STCS.B = (uint32)(ChannelPtr->AdcChannelSampTime & ADC_STCS_MASK);
        }
        else
        {
            G0ICLASS1.STCS.B = (uint32)(ChannelPtr->AdcChannelSampTime & ADC_STCS_MASK);
        }

        /* [SWS_Adc_00506] Select the input class configured above for the requested channel */
        switch (ChannelPtr->AdcChannelId)
        {
            case 0U:
                G0CHCTR0.ICLSEL.B = (uint32)ChIdx;
                break;

            case 1U:
                G0CHCTR1.ICLSEL.B = (uint32)ChIdx;
                break;

            default:
                /* Unmapped channel ID: no hardware register modified */
                break;
        }
    }

    /* [SWS_Adc_00077] [SWS_Adc_00307] Disable notifications and set all groups to ADC_IDLE */
    for (GrpIdx = 0U; GrpIdx < ADC_MAX_GROUPS_PER_HW_UNIT; GrpIdx++)
    {
        Adc_GroupRuntime[GrpIdx].GroupStatus         = ADC_IDLE;
        Adc_GroupRuntime[GrpIdx].ResultBufferPtr     = NULL_PTR;
        Adc_GroupRuntime[GrpIdx].NotificationEnabled = FALSE;
    }

    /* [SWS_BSW_00071] Mark ADC driver state as initialized */
    Adc_DriverState = ADC_STATE_INIT;
}
