#ifndef ADC_CFG_H
#define ADC_CFG_H

#include "Adc.h"

/* Pre-Compile Development Error Detection Switch ([ECUC_Adc_00405]) */
#define ADC_DEV_ERROR_DETECT                    (STD_ON)

/* Pre-Compile Optional API Switches (AUTOSAR SWS 10.2.2 AdcGeneral) */
#define ADC_START_STOP_CONV_API                 (0U)              /* STD_OFF (Set to STD_ON when enabling Start/Stop API) */
#define ADC_ENABLE_START_STOP_GROUP_API         (ADC_START_STOP_CONV_API) /* [ECUC_Adc_00406] */
#define ADC_GRP_NOTIF_CAPABILITY                (STD_ON)          /* [ECUC_Adc_00105] */
#define ADC_READ_GROUP_API                      (STD_ON)          /* [ECUC_Adc_00394] */
#define ADC_RESULT_ALIGNMENT                    (ADC_ALIGN_RIGHT) /* [ECUC_Adc_00444] */

/* [SWS_Adc_00099] Configured Symbolic Channel and Group IDs */
#define ADC_CHANNEL_ID0                         (0U)
#define ADC_CHANNEL_ID1                         (1U)
#define ADC_GROUP_ID0                           (0U)
#define ADC_GROUP_ID1                           (1U)
#define ADC_INVALID_GROUP_ID                    (0xFFU)

/* [SWS_Adc_00082] External IoHwAb Notification Callback */
extern void NotifySensor(void);

/* [SWS_Adc_00505] External Post-Build Configuration Declarations (Defined in Adc_PBcfg.c) */
extern const Adc_ConfigType adcConfig;
extern const Adc_ConfigType adcConfig_HwTrigg;

/* Infineon AURIX TC39x EVADC Hardware Register Definitions (Base: 0xF0020000UL) */
#ifndef ADC_EVADC_REGS_DEFINED
#define ADC_EVADC_REGS_DEFINED

typedef struct Adc_RegBit_Tag
{
    uint32 B;
} Adc_RegBitType;

typedef struct Adc_EvadcClc_Tag
{
    Adc_RegBitType DISR;
    Adc_RegBitType DISS;
} Adc_EvadcClcType;

typedef struct Adc_EvadcGlobcfg_Tag
{
    uint32 U;
} Adc_EvadcGlobcfgType;

typedef struct Adc_EvadcIclass_Tag
{
    Adc_RegBitType STCS;
} Adc_EvadcIclassType;

typedef struct Adc_EvadcChctr_Tag
{
    Adc_RegBitType ICLSEL;
} Adc_EvadcChctrType;

typedef struct Adc_EvadcQmrBits_Tag
{
    uint32 ENGT  : 2;
    uint32 ENTR  : 1;
    uint32 RES1  : 5;
    uint32 CLRV  : 1;
    uint32 TREV  : 1;
    uint32 FLUSH : 1;
    uint32 CEV   : 1;
    uint32 RES2  : 20;
} Adc_EvadcQmrBitsType;

typedef union Adc_EvadcQmr_Tag
{
    uint32               U;
    Adc_EvadcQmrBitsType B;
} Adc_EvadcQmrType;

#define CLC                                     (*((volatile Adc_EvadcClcType*)0xF0020000UL))
#define GLOBCFG                                 (*((volatile Adc_EvadcGlobcfgType*)0xF0020080UL))
#define G0ICLASS0                               (*((volatile Adc_EvadcIclassType*)0xF00204A0UL))
#define G0ICLASS1                               (*((volatile Adc_EvadcIclassType*)0xF00204A4UL))
#define G0QMR0                                  (*((volatile Adc_EvadcQmrType*)0xF0020504UL))
#define G0QINR0                                 (*((volatile uint32*)0xF0020510UL))
#define G0CHCTR0                                (*((volatile Adc_EvadcChctrType*)0xF0020600UL))
#define G0CHCTR1                                (*((volatile Adc_EvadcChctrType*)0xF0020604UL))

#endif /* ADC_EVADC_REGS_DEFINED */

#endif /* ADC_CFG_H */
