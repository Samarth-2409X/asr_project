
#include "Adc.h"
#include "Adc_Cfg.h"



/* Test case for ADC module initialization */
Test_Adc_Tc_001()
{

Adc_Init(&adcConfig);
if (G0ICLASS0.STCS.B == 1)
{
    // Test passed
}
else if (G0ICLASS1.STCS.B == 2)
{
    // Test passed
}
else
{}

}

/*SWS_Adc_00307*/
Test_Adc_Tc_002()
{

Adc_Init(&adcConfig);
if (Adc_GetGroupStatus(ADC_GROUP_ID0)== 0x0U)/* IDLE */
{
    // Test_Adc_Tc_002 passed
}
else
{
    // Test failed
}

}
/*Software triggered One-Shot conversion without notification*/

Test_Adc_Tc_003()
{

Adc_Init(&adcConfig);


}