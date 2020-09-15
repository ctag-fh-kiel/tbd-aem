/***************
CTAG TBD >>to be determined<< is an open source eurorack synthesizer module.

A project conceived within the Creative Technologies Arbeitsgruppe of
Kiel University of Applied Sciences: https://www.creative-technologies.de

(c) 2020 by Robert Manzke. All rights reserved.

The CTAG TBD software is licensed under the GNU General Public License
(GPL 3.0), available here: https://www.gnu.org/licenses/gpl-3.0.txt

The CTAG TBD hardware design is released under the Creative Commons
Attribution-NonCommercial-ShareAlike 4.0 International (CC BY-NC-SA 4.0).
Details here: https://creativecommons.org/licenses/by-nc-sa/4.0/

CTAG TBD is provided "as is" without any express or implied warranties.

License and copyright details for specific submodules are included in their
respective component folders / files if different from this license.
***************/

#include "ctagSoundProcessorMIPShft.hpp"
#include <iostream>
#include "helpers/ctagFastMath.hpp"
#include "clouds/dsp/frame.h"
#include "esp_log.h"
#include "esp_heap_caps.h"

using namespace CTAG::SP;

ctagSoundProcessorMIPShft::ctagSoundProcessorMIPShft() {
    setIsStereo();
    model = std::make_unique<ctagSPDataModel>(id, isStereo);
    model->LoadPreset(0);

    fx_buffer = (float *) heap_caps_malloc(4096 * sizeof(float),
                                           MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
    if (fx_buffer == NULL) {
        ESP_LOGE("MIPShift", "Could not allocate shared buffer!");
    };

    fx.Init(fx_buffer);
}

void ctagSoundProcessorMIPShft::Process(const ProcessData &data) {
    float fRatio = ratio / 4095.f;
    float fSize = size / 4095.f;
    if(cv_ratio != -1){
        fRatio = HELPERS::fastfabs(data.cv[cv_ratio]);
    }
    if(cv_size != -1){
        fSize = HELPERS::fastfabs(data.cv[cv_size]);
    }
    fx.set_size(fSize);
    fx.set_ratio(fRatio);

    clouds::FloatFrame frames[bufSz];
    for(int i=0;i<bufSz;i++){
        frames[i].l = data.buf[i*2];
        frames[i].r = data.buf[i*2 + 1];
    }

    fx.Process(frames, bufSz);

    for(int i=0;i<bufSz;i++){
        data.buf[i*2] = frames[i].l;
        data.buf[i*2 + 1] = frames[i].r;
    }
}

ctagSoundProcessorMIPShft::~ctagSoundProcessorMIPShft() {
    heap_caps_free(fx_buffer);
}

const char *ctagSoundProcessorMIPShft::GetCStrID() const {
    return id.c_str();
}


void ctagSoundProcessorMIPShft::setParamValueInternal(const string& id, const string& key, const int val) {
// autogenerated code here
// sectionCpp0
if(id.compare("ratio") == 0){
	if(key.compare("current") == 0){
		ratio = val;
		return;
	}else if(key.compare("cv") == 0){
		if(val >= -1 && val <= 3)
			cv_ratio = val;
	}
	return;
}
if(id.compare("size") == 0){
	if(key.compare("current") == 0){
		size = val;
		return;
	}else if(key.compare("cv") == 0){
		if(val >= -1 && val <= 3)
			cv_size = val;
	}
	return;
}
// sectionCpp0

}

void ctagSoundProcessorMIPShft::loadPresetInternal() {
// autogenerated code here
// sectionCpp1
ratio = model->GetParamValue("ratio", "current");
cv_ratio = model->GetParamValue("ratio", "cv");
size = model->GetParamValue("size", "current");
cv_size = model->GetParamValue("size", "cv");
// sectionCpp1

}
