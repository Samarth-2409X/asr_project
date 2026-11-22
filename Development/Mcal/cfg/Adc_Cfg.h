#ifndef ADC_CFG_H
#define ADC_CFG_H

#include "Adc.h"

/* Pre-Compile Development Error Detection Switch */
#define ADC_DEV_ERROR_DETECT                    (STD_ON)

/* Pre-Compile Optional API Switches */
#define ADC_START_STOP_CONV_API                 (0U) /* STD_OFF */

/* Configured Symbolic Group IDs */
#define ADC_GROUP_ID0                           (0U)

/* External Post-Build Configuration Declarations */
extern const Adc_ConfigType adcConfig;
extern const Adc_ConfigType adcConfig_HwTrigg;

#endif /* ADC_CFG_H */