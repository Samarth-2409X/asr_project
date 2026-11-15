



struct{
AdcHwUnitType AdcHwUnit;
}Adc_ConfigType;//container

/* ReqToDesign-ECUC_Adc_00242*/
struct{
uint32 ClkSrc;
uint32 HwUnitId;
AdcChannelType AdcChannel;
AdcGroupType AdcGroup;
}AdcHwUnitType;//container

struct{
uint8 AdcChannelSampTime; /*Sample Time Control for Standard Conversions, 1 -> 150ns*/
uint16 AdcChannelId;
}AdcChannelType;


enum enum_AdcGroupAccessMode
{
ADC_ACCESS_MODE_SINGLE
ADC_ACCESS_MODE_STREAMING
}

enum enum_AdcGroupAccessMode
{
ADC_CONV_MODE_CONTINUOUS
ADC_CONV_MODE_ONESHOT
}

enum enum_AdcGroupTriggSrc
{
ADC_TRIGG_SRC_HW,
ADC_TRIGG_SRC_SW
}

enum enum_AdcStreamingBufferMode  
{
ADC_STREAM_BUFFER_CIRCULAR
ADC_STREAM_BUFFER_LINEAR
}

typedef uin16 Adc_GroupType


struct{
enum_AdcGroupAccessMode AdcGroupAccessMode;  /* Students to write the enum */
enum_AdcGroupConversionMode AdcGroupConversionMode ;
Adc_GroupType AdcGroupId ;
enum_AdcGroupTriggSrc AdcGroupTriggSrc ;
void (*AdcNotification)(void);
enum_AdcStreamingBufferMode AdcStreamingBufferMode ;
uint8 AdcStreamingNumSamples ;
AdcChannelType AdcChannel ;
}AdcGroupType;


extern void Adc_Init (const Adc_ConfigType* ConfigPtr);

extern void Adc_StartGroupConversion (
Adc_GroupType Group
);
