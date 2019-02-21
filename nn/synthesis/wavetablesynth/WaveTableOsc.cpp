//  WavetableOsc.cpp

#include "WavetableOsc.h"


WavetableOsc::WavetableOsc(void) {
    phasor = 0.0;
    phaseInc = 0.0;
    phaseOfs = 0.5;
    numWavetables = 0;
    for (int idx = 0; idx < numWavetableSlots; idx++) {
        Wavetables[idx].topFreq = 0;
        Wavetables[idx].WavetableLen = 0;
        Wavetables[idx].Wavetable = 0;
    }
}


WavetableOsc::~WavetableOsc(void) {
    for (int idx = 0; idx < numWavetableSlots; idx++) {
        float *temp = Wavetables[idx].Wavetable;
        if (temp != 0)
            delete [] temp;
    }
}


// addWavetable
// add Wavetables in order of lowest frequency to highest
// topFreq is the highest frequency supported by a Wavetable
// Wavetables within an oscillator can be different lengths
//
// returns 0 upon success, or the number of Wavetables if no more room is available
//
int WavetableOsc::addWavetable(int len, float *WavetableIn, double topFreq) {
    if (this->numWavetables < numWavetableSlots) {
        float *Wavetable = this->Wavetables[this->numWavetables].Wavetable = new float[len];
        this->Wavetables[this->numWavetables].WavetableLen = len;
        this->Wavetables[this->numWavetables].topFreq = topFreq;
        ++this->numWavetables;

        // fill in wave
        for (long idx = 0; idx < len; idx++)
            Wavetable[idx] = WavetableIn[idx];

        return 0;
    }
    return this->numWavetables;
}


//
// getOutput
//
// returns the current oscillator output
//
float WavetableOsc::getOutput() {
    // grab the appropriate Wavetable
    int WavetableIdx = 0;
    while ((this->phaseInc >= this->Wavetables[WavetableIdx].topFreq) && (WavetableIdx < (this->numWavetables - 1))) {
        ++WavetableIdx;
    }
    Wavetable *Wavetable = &this->Wavetables[WavetableIdx];

#if !doLinearInterp
    // truncate
    return Wavetable->Wavetable[int(this->phasor * Wavetable->WavetableLen)];
#else
    // linear interpolation
    double temp = this->phasor * Wavetable->WavetableLen;
    int intPart = temp;
    double fracPart = temp - intPart;
    float samp0 = Wavetable->Wavetable[intPart];
    if (++intPart >= Wavetable->WavetableLen)
        intPart = 0;
    float samp1 = Wavetable->Wavetable[intPart];

    return samp0 + (samp1 - samp0) * fracPart;
#endif
}


//
// getOutputMinusOffset
//
// for variable pulse width: initialize to sawtooth,
// set phaseOfs to duty cycle, use this for osc output
//
// returns the current oscillator output
//
float WavetableOsc::getOutputMinusOffset() {
    // grab the appropriate Wavetable
    int WavetableIdx = 0;
    while ((this->phaseInc >= this->Wavetables[WavetableIdx].topFreq) && (WavetableIdx < (this->numWavetables - 1))) {
        ++WavetableIdx;
    }
    Wavetable *Wavetable = &this->Wavetables[WavetableIdx];

#if !doLinearInterp
    // truncate
    double offsetPhasor = this->phasor + this->phaseOfs;
    if (offsetPhasor >= 1.0)
        offsetPhasor -= 1.0;
    return Wavetable->Wavetable[int(this->phasor * Wavetable->WavetableLen)] - Wavetable->Wavetable[int(offsetPhasor * Wavetable->WavetableLen)];
#else
    // linear
    double temp = this->phasor * Wavetable->WavetableLen;
    int intPart = temp;
    double fracPart = temp - intPart;
    float samp0 = Wavetable->Wavetable[intPart];
    if (++intPart >= Wavetable->WavetableLen)
        intPart = 0;
    float samp1 = Wavetable->Wavetable[intPart];
    float samp = samp0 + (samp1 - samp0) * fracPart;

    // and linear again for the offset part
    double offsetPhasor = this->phasor + this->phaseOfs;
    if (offsetPhasor > 1.0)
        offsetPhasor -= 1.0;
    temp = offsetPhasor * Wavetable->WavetableLen;
    intPart = temp;
    fracPart = temp - intPart;
    samp0 = Wavetable->Wavetable[intPart];
    if (++intPart >= Wavetable->WavetableLen)
        intPart = 0;
    samp1 = Wavetable->Wavetable[intPart];

    return samp - (samp0 + (samp1 - samp0) * fracPart);
#endif
}
