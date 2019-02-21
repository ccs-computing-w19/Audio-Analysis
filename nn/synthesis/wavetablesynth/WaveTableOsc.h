//  WavetableOsc.h

#ifndef Test_WavetableOsc_h
#define Test_WavetableOsc_h

#define doLinearInterp 1

typedef struct {
    double topFreq;
    int WavetableLen;
    float *Wavetable;
} Wavetable;

const int numWavetableSlots = 32;

class WavetableOsc {
protected:
    double phasor;      // phase accumulator
    double phaseInc;    // phase increment
    double phaseOfs;    // phase offset for PWM

    // list of Wavetables
    int numWavetables;
    Wavetable Wavetables[numWavetableSlots];

public:
    WavetableOsc(void);
    ~WavetableOsc(void);
    void setFrequency(double inc);
    void setPhaseOffset(double offset);
    void updatePhase(void);
    float getOutput(void);
    float getOutputMinusOffset(void);
    int addWavetable(int len, float *WavetableIn, double topFreq);
};


// limited between 0 to 1 atm
inline void WavetableOsc::setFrequency(double inc) {
    phaseInc = inc;
}

// limited between 0 to 1 atm
inline void WavetableOsc::setPhaseOffset(double offset) {
    phaseOfs = offset;
}

inline void WavetableOsc::updatePhase() {
    phasor += phaseInc;

    if (phasor >= 1.0)
        phasor -= 1.0;
}

#endif
