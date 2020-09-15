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

#include <atomic>
#include "ctagSoundProcessor.hpp"
#include "LadderFilterBase.h"
#include "helpers/ctagADEnv.hpp"

using namespace Moog;
using namespace CTAG::SP::HELPERS;

namespace CTAG {
    namespace SP {
        class ctagSoundProcessorMoogFilt : public ctagSoundProcessor {
        public:
            virtual void Process(const ProcessData &) override;

            virtual ~ctagSoundProcessorMoogFilt();

            ctagSoundProcessorMoogFilt();

            virtual const char *GetCStrID() const override;

        private:
            void setParamValueInternal(const string &id, const string &key, const int val) override;
            void loadPresetInternal() override;

// autogenerated code here
// sectionHpp
const string id = "MoogFilt";
void setIsStereo(){isStereo = false;}
atomic<int32_t> flt_model, cv_flt_model;
atomic<int32_t> gain, cv_gain;
atomic<int32_t> cutoff, cv_cutoff;
atomic<int32_t> resonance, cv_resonance;
atomic<int32_t> fm_amt, cv_fm_amt;
atomic<int32_t> enableEG, trig_enableEG;
atomic<int32_t> loopEG, trig_loopEG;
atomic<int32_t> attack, cv_attack;
atomic<int32_t> decay, cv_decay;
// sectionHpp

            // private attributes could go here
            std::unique_ptr<LadderFilterBase> ladderFilters[6];
            const float fCutoffScales[6] = {4095.f, 15000.f, 20000.f, 20000.f, 8000.f, 20000.f};
            const float fResoScales[6] = {1.5f, 1.f, 30.f, 1.f, 1.f, 1.f};
            const float fGainScales[6] = {2.f, 1.f, 1.f, 1.f, 1.f, 1.f};
            ctagADEnv adEnv;
            int prevTrigState = 1;
        };
    }
}