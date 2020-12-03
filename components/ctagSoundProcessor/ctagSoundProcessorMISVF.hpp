#include <atomic>
#include "ctagSoundProcessor.hpp"
#include "helpers/ctagADEnv.hpp"
#include "braids/svf.h"

using namespace CTAG::SP::HELPERS;

namespace CTAG {
    namespace SP {
        class ctagSoundProcessorMISVF : public ctagSoundProcessor {
        public:
            virtual void Process(const ProcessData &) override;

            ctagSoundProcessorMISVF();

        private:
            virtual void knowYourself() override;

            // private attributes could go here
            ctagADEnv adEnv;
            int prevTrigState = 1;
            braids::Svf svf;
            // autogenerated code here
// sectionHpp
            atomic<int32_t> flt_mode, cv_flt_mode;
            atomic<int32_t> gain, cv_gain;
            atomic<int32_t> cutoff, cv_cutoff;
            atomic<int32_t> resonance, cv_resonance;
            atomic<int32_t> fm_amt, cv_fm_amt;
            atomic<int32_t> enableEG, trig_enableEG;
            atomic<int32_t> loopEG, trig_loopEG;
            atomic<int32_t> attack, cv_attack;
            atomic<int32_t> decay, cv_decay;
            // sectionHpp
        };
    }
}