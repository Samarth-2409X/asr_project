/* [SWS_Adc_00364] AUTOSAR Classic Platform R25-11 ADC Driver Header File (TC39x EVADC CPU0) */
#ifndef ADC_H
#define ADC_H

#ifdef __cplusplus
extern "C" {
#endif

/* [SWS_Adc_00364] Include AUTOSAR Standard Types & Pre-Compile Configuration */
#include "Std_Types.h"
#include "Adc_Cfg.h"

/* [SWS_BSW_00059] Module and Vendor Identification */
#define ADC_VENDOR_ID                           (17U)
#define ADC_MODULE_ID                           (123U)
#define ADC_INSTANCE_ID                         (0U)

/* [SWS_BSW_00059] AUTOSAR Release R25-11 Version Information */
#define ADC_AR_RELEASE_MAJOR_VERSION            (25U)
#define ADC_AR_RELEASE_MINOR_VERSION            (11U)
#define ADC_AR_RELEASE_REVISION_VERSION         (0U)

/* [SWS_BSW_00059] Software Module Version Information */
#define ADC_SW_MAJOR_VERSION                    (1U)
#define ADC_SW_MINOR_VERSION                    (0U)
#define ADC_SW_PATCH_VERSION                    (0U)

/* Infineon AURIX TC39x EVADC Hardware Limits (Single Core CPU0) */
#define ADC_MAX_CHANNELS_PER_HW_UNIT            (2U)
#define ADC_MAX_GROUPS_PER_HW_UNIT              (2U)

/* Service IDs for Scoped AUTOSAR SWS Section 8.3 APIs */
#define ADC_SID_INIT                            (0x00U) /* [SWS_Adc_00365] */
#define ADC_SID_START_GROUP_CONVERSION          (0x02U) /* [SWS_Adc_00367] */
#define ADC_SID_STOP_GROUP_CONVERSION           (0x03U) /* [SWS_Adc_00368] */
#define ADC_SID_READ_GROUP                      (0x04U) /* [SWS_Adc_00369] */
#define ADC_SID_ENABLE_GROUP_NOTIFICATION       (0x07U) /* [SWS_Adc_91003] */
#define ADC_SID_GET_GROUP_STATUS                (0x09U) /* [SWS_Adc_00374] */
#define ADC_SID_SETUP_RESULT_BUFFER             (0x0CU) /* [SWS_Adc_91000] */

/* [SWS_Adc_91005] DET Development Error Codes */
#define ADC_E_UNINIT                            (0x0AU)
#define ADC_E_ALREADY_INITIALIZED               (0x0DU)
#define ADC_E_PARAM_POINTER                     (0x14U)
#define ADC_E_PARAM_GROUP                       (0x15U)
#define ADC_E_WRONG_CONV_MODE                   (0x16U)
#define ADC_E_WRONG_TRIGG_SRC                   (0x17U)
#define ADC_E_NOTIF_CAPABILITY                  (0x18U)
#define ADC_E_BUFFER_UNINIT                     (0x19U)

/* [SWS_Adc_91006] Runtime Error Codes */
#define ADC_E_BUSY                              (0x0BU)
#define ADC_E_IDLE                              (0x0CU)
#define ADC_E_NOT_DISENGAGED                    (0x1AU)
#define ADC_E_TRANSITION_NOT_POSSIBLE           (0x1CU)

/* [SWS_Adc_00506] Numeric ID of an ADC channel */
typedef uint16 Adc_ChannelType;

/* [SWS_Adc_00517] Assignment of channels to a channel group */
typedef Adc_ChannelType Adc_GroupDefType;

/* [SWS_Adc_00507] Numeric ID of an ADC channel group */
typedef uint16 Adc_GroupType;

/* [SWS_Adc_00508] Type for reading converted values of a channel group */
typedef uint16 Adc_ValueGroupType;

/* [SWS_Adc_00511] Type of sampling time */
typedef uint8 Adc_SamplingTimeType;

/* [SWS_Adc_00518] Number of group conversions in streaming access mode */
typedef uint8 Adc_StreamNumSampleType;

/* [SWS_Adc_00513] Current conversion status of the requested ADC Channel group */
typedef enum Adc_StatusType_Tag
{
    ADC_IDLE             = 0x00U,
    ADC_BUSY             = 0x01U,
    ADC_COMPLETED        = 0x02U,
    ADC_STREAM_COMPLETED = 0x03U
} Adc_StatusType;

/* [SWS_Adc_00514] Trigger source for an ADC Channel group */
typedef enum Adc_TriggerSourceType_Tag
{
    ADC_TRIGG_SRC_SW = 0x00U,
    ADC_TRIGG_SRC_HW = 0x01U
} Adc_TriggerSourceType;

typedef Adc_TriggerSourceType enum_AdcGroupTriggSrc;

/* [SWS_Adc_00515] Conversion mode of an ADC Channel group */
typedef enum Adc_GroupConvModeType_Tag
{
    ADC_CONV_MODE_ONESHOT    = 0x00U,
    ADC_CONV_MODE_CONTINUOUS = 0x01U
} Adc_GroupConvModeType;

typedef Adc_GroupConvModeType enum_AdcGroupConversionMode;

/* [SWS_Adc_00519] Streaming access mode buffer type */
typedef enum Adc_StreamBufferModeType_Tag
{
    ADC_STREAM_BUFFER_LINEAR   = 0x00U,
    ADC_STREAM_BUFFER_CIRCULAR = 0x01U
} Adc_StreamBufferModeType;

typedef Adc_StreamBufferModeType enum_AdcStreamingBufferMode;

/* [SWS_Adc_00528] Access mode to group conversion results */
typedef enum Adc_GroupAccessModeType_Tag
{
    ADC_ACCESS_MODE_SINGLE    = 0x00U,
    ADC_ACCESS_MODE_STREAMING = 0x01U
} Adc_GroupAccessModeType;

typedef Adc_GroupAccessModeType enum_AdcGroupAccessMode;

/* [SWS_Adc_00525] Alignment of ADC raw results in ADC result buffer */
typedef enum Adc_ResultAlignmentType_Tag
{
    ADC_ALIGN_LEFT  = 0x00U,
    ADC_ALIGN_RIGHT = 0x01U
} Adc_ResultAlignmentType;

/* [SWS_Adc_00084] Callback function pointer type for group conversion notification */
typedef void (*Adc_NotificationType)(void);

/* [ECUC_Adc_00268] Individual ADC Channel Configuration Container */
typedef struct AdcChannelType_Tag
{
    Adc_SamplingTimeType        AdcChannelSampTime;
    Adc_ChannelType             AdcChannelId;
} AdcChannelType;

/* [ECUC_Adc_00028] ADC Channel Group Configuration Container */
typedef struct AdcGroupType_Tag
{
    enum_AdcGroupAccessMode     AdcGroupAccessMode;
    enum_AdcGroupConversionMode AdcGroupConversionMode;
    Adc_GroupType               AdcGroupId;
    enum_AdcGroupTriggSrc       AdcGroupTriggSrc;
    Adc_NotificationType        AdcNotification;
    enum_AdcStreamingBufferMode AdcStreamingBufferMode;
    Adc_StreamNumSampleType     AdcStreamingNumSamples;
    AdcChannelType              AdcChannel;
} AdcGroupType;

/* [ECUC_Adc_00242] ADC Hardware Unit Configuration Container */
typedef struct AdcHwUnitType_Tag
{
    uint32                      ClkSrc;
    uint32                      HwUnitId;
    AdcChannelType              AdcChannel[ADC_MAX_CHANNELS_PER_HW_UNIT];
    AdcGroupType                AdcGroup[ADC_MAX_GROUPS_PER_HW_UNIT];
} AdcHwUnitType;

/* [SWS_Adc_00505] Root ADC Driver Configuration Data Structure */
typedef struct Adc_ConfigType_Tag
{
    AdcHwUnitType               AdcHwUnit;
} Adc_ConfigType;

/* Scoped AUTOSAR SWS Section 8.3 API Prototypes */
extern void Adc_Init(const Adc_ConfigType* ConfigPtr);                                           /* [SWS_Adc_00365] */
extern Std_ReturnType Adc_SetupResultBuffer(Adc_GroupType Group, Adc_ValueGroupType* DataBufferPtr); /* [SWS_Adc_91000] */
extern void Adc_StartGroupConversion(Adc_GroupType Group);                                       /* [SWS_Adc_00367] */
extern void Adc_StopGroupConversion(Adc_GroupType Group);                                        /* [SWS_Adc_00368] */
extern Std_ReturnType Adc_ReadGroup(Adc_GroupType Group, Adc_ValueGroupType* DataBufferPtr);     /* [SWS_Adc_00369] */
extern void Adc_EnableGroupNotification(Adc_GroupType Group);                                    /* [SWS_Adc_91003] */
extern Adc_StatusType Adc_GetGroupStatus(Adc_GroupType Group);                                   /* [SWS_Adc_00374] */

#ifdef __cplusplus
}
#endif

#endif /* ADC_H */
