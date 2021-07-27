#include "Control.hpp"
#include "Calibration.hpp"
#include "gpio.hpp"
#include "adc.hpp"
#include "mk2.hpp"

#ifndef CONFIG_TBD_PLATFORM_MK2
    DRAM_ATTR uint8_t CTAG::CTRL::Control::trig_data[N_TRIGS];
    DRAM_ATTR float CTAG::CTRL::Control::cv_data[N_CVS];
#endif

void CTAG::CTRL::Control::Update(uint8_t **trigs, float **cvs) {
#if defined(CONFIG_TBD_PLATFORM_MK2)
    uint8_t *data = (uint8_t *) DRIVERS::mk2::Update();
    *cvs = (float*) data;
    *trigs = &data[N_CVS*4];
    uint16_t *transmission_count = (uint16_t*) &data[62];
    if(*transmission_count != 0xcafe){
        // Debug transmission
        printf("%5d ", *transmission_count);
        for(int i=0;i<6;i++){
            printf("%d ", (*trigs)[i]);
        }
        for(int i=0;i<14;i++){
            printf("%.3f ", (*cvs)[i]);
        }
        printf("\n");
    }
#else
// update CVs
    CTAG::DRIVERS::ADC::Update();
    CTAG::CAL::Calibration::MapCVData(CTAG::DRIVERS::ADC::data, cv_data);
    *cvs = cv_data;

// update trig data
    trig_data[0] = CTAG::DRIVERS::GPIO::GetTrig0();
    trig_data[1] = CTAG::DRIVERS::GPIO::GetTrig1();
    *trigs = trig_data;
#endif
}

void CTAG::CTRL::Control::SetCVChannelBiPolar(const bool &v0, const bool &v1, const bool &v2, const bool &v3) {
    CTAG::CAL::Calibration::ConfigCVChannels(v0 ? CTAG::CAL::CVConfig::CVBipolar : CTAG::CAL::CVConfig::CVUnipolar,
                                             v1 ? CTAG::CAL::CVConfig::CVBipolar : CTAG::CAL::CVConfig::CVUnipolar,
                                             v2 ? CTAG::CAL::CVConfig::CVBipolar : CTAG::CAL::CVConfig::CVUnipolar,
                                             v3 ? CTAG::CAL::CVConfig::CVBipolar : CTAG::CAL::CVConfig::CVUnipolar);
}

void CTAG::CTRL::Control::Init() {
    ESP_LOGE("Control", "Initializing control!");
#ifndef CONFIG_TBD_PLATFORM_MK2
    DRIVERS::ADC::InitADCSystem();
    DRIVERS::GPIO::InitGPIO();
    CAL::Calibration::Init();
#else
    DRIVERS::mk2::Init();
#endif
}
