// main.cpp

#include <ctime> // for time testing
#include <iostream>
using namespace std;

#include <math.h>

#include "WavetableOsc.h"



//
// some things to tweak for testing and experimenting
//
#define sampleRate (44100)

// sound file
#define numSecs (20.0)      /* length of sound file to generate (seconds) */
#define gainMult (0.5)      /* some control over sound file gain */

// oscillator
#define overSamp (2)        /* oversampling factor (positive integer) */
#define baseFrequency (20)  /* starting frequency of first table */
#define constantRatioLimit (99999)    /* set to a large number (greater than or equal to the length of the lowest octave table) for constant table size; set to 0 for a constant oversampling ratio (each higher ocatave table length reduced by half); set somewhere between (64, for instance) for constant oversampling but with a minimum limit */

#define myFloat double      /* float or double, to set the resolution of the FFT, etc. (the resulting Wavetables are always float) */

// test selector
enum {
    c_testPWM,
    c_testThreeOsc,
    c_testSawSweep
};
const int testType = c_testThreeOsc;    // set this to select the test to run

// end tweak section



// tests
void testPWM(void);
void testThreeOsc(void);
void testSawSweep(void);

int main(void) {
    switch (testType) {
        case c_testPWM:
            testPWM();
            break;
        case c_testThreeOsc:
            testThreeOsc();
            break;
        case c_testSawSweep:
            testSawSweep();
            break;
        default:
            break;
    }

    return 0;
}



void writeFloatSound(int len, float *wave);
void fft(int N, myFloat *ar, myFloat *ai);
void defineSawtooth(int len, int numHarmonics, myFloat *ar, myFloat *ai);
float makeWavetable(WavetableOsc *osc, int len, myFloat *ar, myFloat *ai, myFloat scale, double topFreq);
void setSawtoothOsc(WavetableOsc *osc, float baseFreq);


// sawtooth sweep
void testSawSweep(void) {
    WavetableOsc *osc = new WavetableOsc();
    setSawtoothOsc(osc, baseFrequency);

    // time test
    clock_t start, finish;
    start = clock();

    // run the oscillator
    const int numSamples = sampleRate * numSecs;
    float *soundBuf = new float [numSamples];

    double freqVal = 20.0 / sampleRate;
    double freqMult = 1.0 + (log(20000.0 / sampleRate) - log(freqVal)) / numSamples;

    for (int idx = 0; idx < numSamples; idx ++) {
        osc->setFrequency(freqVal);
        soundBuf[idx] = osc->getOutput() * gainMult;
        osc->updatePhase();
        freqVal *= freqMult;    // exponential frequency sweep
    }

    // time test
    finish = clock();
    double elapsed = (finish - start)/(double)CLOCKS_PER_SEC;
    cout << "Elapsed time: " << elapsed<< "\n";

    // q&d fade to avoid tick at end (0.05s)
    for (int count = sampleRate * 0.05; count >= 0; --count) {
        soundBuf[numSamples-count] *= count / (sampleRate * 0.05);
    }

    writeFloatSound(numSamples, soundBuf);
    delete [] soundBuf;

    return;
}


// pwm
void testPWM(void) {
    // make an oscillator with Wavetable
    WavetableOsc *osc = new WavetableOsc();
    setSawtoothOsc(osc, baseFrequency);

    // pwm
    WavetableOsc *mod = new WavetableOsc();
    const int sineTableLen = 2048;
    float sineTable[sineTableLen];
    for (int idx = 0; idx < sineTableLen; ++idx)
        sineTable[idx] = sin((float)idx / sineTableLen * M_PI * 2);
    mod->addWavetable(sineTableLen, sineTable, 1.0);
    mod->setFrequency(0.3/sampleRate);

    osc->setFrequency(110.0/sampleRate);

    // time test
    clock_t start, finish;
    start = clock();

    // run the oscillator
    const int numSamples = sampleRate * numSecs;
    float *soundBuf = new float [numSamples];

    for (int idx = 0; idx < numSamples; idx ++) {
        osc->setPhaseOffset((mod->getOutput() * 0.95 + 1.0) * 0.5);
        soundBuf[idx] = osc->getOutputMinusOffset() * gainMult;    // square wave from sawtooth
        mod->updatePhase();
        osc->updatePhase();
    }

    // time test
    finish = clock();
    double elapsed = (finish - start)/(double)CLOCKS_PER_SEC;
    cout << "Elapsed time: " << elapsed<< "\n";

    // q&d fade to avoid tick at end (0.05s)
    for (int count = sampleRate * 0.05; count >= 0; --count) {
        soundBuf[numSamples-count] *= count / (sampleRate * 0.05);
    }

    writeFloatSound(numSamples, soundBuf);
    delete [] soundBuf;

    return;
}


// three unison detuned saws
void testThreeOsc(void) {
    // make an oscillator with Wavetable
    WavetableOsc *osc1 = new WavetableOsc();
    WavetableOsc *osc2 = new WavetableOsc();
    WavetableOsc *osc3 = new WavetableOsc();

    setSawtoothOsc(osc1, baseFrequency);
    setSawtoothOsc(osc2, baseFrequency);
    setSawtoothOsc(osc3, baseFrequency);

    // time test
    clock_t start, finish;
    start = clock();

    // run the oscillator
    const int numSamples = sampleRate * numSecs;
    float *soundBuf = new float [numSamples];

    osc1->setFrequency(111.0*.5/sampleRate);
    osc2->setFrequency(112.0*.5/sampleRate);
    osc3->setFrequency(55.0/sampleRate);

    for (int idx = 0; idx < numSamples; idx ++) {
        soundBuf[idx] = (osc1->getOutput() + osc2->getOutput() + osc3->getOutput()) * 0.5 * gainMult;    // square wave from sawtooth
        osc1->updatePhase();
        osc2->updatePhase();
        osc3->updatePhase();
    }

    // time test
    finish = clock();
    double elapsed = (finish - start)/(double)CLOCKS_PER_SEC;
    cout << "Elapsed time: " << elapsed<< "\n";

    // q&d fade to avoid tick at end (0.05s)
    for (int count = sampleRate * 0.05; count >= 0; --count) {
        soundBuf[numSamples-count] *= count / (sampleRate * 0.05);
    }

    writeFloatSound(numSamples, soundBuf);
    delete [] soundBuf;

    return;
}

//
// setSawtoothOsc
//
// make set of Wavetables for sawtooth oscillator
//
void setSawtoothOsc(WavetableOsc *osc, float baseFreq) {
    // calc number of harmonics where the highest harmonic baseFreq and lowest alias an octave higher would meet
    int maxHarms = sampleRate / (3.0 * baseFreq) + 0.5;

    // round up to nearest power of two
    unsigned int v = maxHarms;
    v--;            // so we don't go up if already a power of 2
    v |= v >> 1;    // roll the highest bit into all lower bits...
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    v++;            // and increment to power of 2
    int tableLen = v * 2 * overSamp;  // double for the sample rate, then oversampling

    myFloat ar[tableLen], ai[tableLen];   // for ifft

    double topFreq = baseFreq * 2.0 / sampleRate;
    myFloat scale = 0.0;
    for (; maxHarms >= 1; maxHarms >>= 1) {
        defineSawtooth(tableLen, maxHarms, ar, ai);
        scale = makeWavetable(osc, tableLen, ar, ai, scale, topFreq);
        topFreq *= 2;
        if (tableLen > constantRatioLimit) // variable table size (constant oversampling but with minimum table size)
            tableLen >>= 1;
    }
}


//
// if scale is 0, auto-scales
// returns scaling factor (0.0 if failure), and Wavetable in ai array
//
float makeWavetable(WavetableOsc *osc, int len, myFloat *ar, myFloat *ai, myFloat scale, double topFreq) {
    fft(len, ar, ai);

    if (scale == 0.0) {
        // calc normal
        myFloat max = 0;
        for (int idx = 0; idx < len; idx++) {
            myFloat temp = fabs(ai[idx]);
            if (max < temp)
                max = temp;
        }
        scale = 1.0 / max * .999;
    }

    // normalize
    float wave[len];
    for (int idx = 0; idx < len; idx++)
        wave[idx] = ai[idx] * scale;

    if (osc->addWavetable(len, wave, topFreq))
        scale = 0.0;

    return scale;
}


//
// fft
//
//
//
//
void fft(int N, myFloat *ar, myFloat *ai)
/*
 in-place complex fft

 After Cooley, Lewis, and Welch; from Rabiner & Gold (1975)

 program adapted from FORTRAN
 by K. Steiglitz  (ken@princeton.edu)
 Computer Science Dept.
 Princeton University 08544          */
{
    int i, j, k, L;            /* indexes */
    int M, TEMP, LE, LE1, ip;  /* M = log N */
    int NV2, NM1;
    myFloat t;               /* temp */
    myFloat Ur, Ui, Wr, Wi, Tr, Ti;
    myFloat Ur_old;

    // if ((N > 1) && !(N & (N - 1)))   // make sure we have a power of 2

    NV2 = N >> 1;
    NM1 = N - 1;
    TEMP = N; /* get M = log N */
    M = 0;
    while (TEMP >>= 1) ++M;

    /* shuffle */
    j = 1;
    for (i = 1; i <= NM1; i++) {
        if(i<j) {             /* swap a[i] and a[j] */
            t = ar[j-1];
            ar[j-1] = ar[i-1];
            ar[i-1] = t;
            t = ai[j-1];
            ai[j-1] = ai[i-1];
            ai[i-1] = t;
        }

        k = NV2;             /* bit-reversed counter */
        while(k < j) {
            j -= k;
            k /= 2;
        }

        j += k;
    }

    LE = 1.;
    for (L = 1; L <= M; L++) {            // stage L
        LE1 = LE;                         // (LE1 = LE/2)
        LE *= 2;                          // (LE = 2^L)
        Ur = 1.0;
        Ui = 0.;
        Wr = cos(M_PI/(float)LE1);
        Wi = -sin(M_PI/(float)LE1); // Cooley, Lewis, and Welch have "+" here
        for (j = 1; j <= LE1; j++) {
            for (i = j; i <= N; i += LE) { // butterfly
                ip = i+LE1;
                Tr = ar[ip-1] * Ur - ai[ip-1] * Ui;
                Ti = ar[ip-1] * Ui + ai[ip-1] * Ur;
                ar[ip-1] = ar[i-1] - Tr;
                ai[ip-1] = ai[i-1] - Ti;
                ar[i-1]  = ar[i-1] + Tr;
                ai[i-1]  = ai[i-1] + Ti;
            }
            Ur_old = Ur;
            Ur = Ur_old * Wr - Ui * Wi;
            Ui = Ur_old * Wi + Ui * Wr;
        }
    }
}


//
// defineSawtooth
//
// prepares sawtooth harmonics for ifft
//
void defineSawtooth(int len, int numHarmonics, myFloat *ar, myFloat *ai) {
    if (numHarmonics > (len >> 1))
        numHarmonics = (len >> 1);

    // clear
    for (int idx = 0; idx < len; idx++) {
        ai[idx] = 0;
        ar[idx] = 0;
    }

    // sawtooth
    for (int idx = 1, jdx = len - 1; idx <= numHarmonics; idx++, jdx--) {
        myFloat temp = -1.0 / idx;
        ar[idx] = -temp;
        ar[jdx] = temp;
    }

    // examples of other waves
    /*
     // square
     for (int idx = 1, jdx = len - 1; idx <= numHarmonics; idx++, jdx--) {
     myFloat temp = idx & 0x01 ? 1.0 / idx : 0.0;
     ar[idx] = -temp;
     ar[jdx] = temp;
     }
     */
    /*
     // triangle
     float sign = 1;
     for (int idx = 1, jdx = len - 1; idx <= numHarmonics; idx++, jdx--) {
     myFloat temp = idx & 0x01 ? 1.0 / (idx * idx) * (sign = -sign) : 0.0;
     ar[idx] = -temp;
     ar[jdx] = temp;
     }
     */
}


//
// quick & dirty wave file
//

enum {
    /* keep sorted for wav_w64_format_str() */
	WAVE_FORMAT_UNKNOWN					= 0x0000,		/* Microsoft Corporation */
	WAVE_FORMAT_PCM	 					= 0x0001, 		/* Microsoft PCM format */
	WAVE_FORMAT_MS_ADPCM				= 0x0002,		/* Microsoft ADPCM */
	WAVE_FORMAT_IEEE_FLOAT				= 0x0003,		/* Micrososft 32 bit float format */
};

inline void writeFourCC(uint32_t val, uint8_t **bytePtr) {
	*(*bytePtr)++ = val >> 24;
	*(*bytePtr)++ = val >> 16;
	*(*bytePtr)++ = val >> 8;
	*(*bytePtr)++ = val;
}

inline void write4Bytes(uint32_t val, uint8_t **bytePtr) {
	*(*bytePtr)++ = val;
	*(*bytePtr)++ = val >> 8;
	*(*bytePtr)++ = val >> 16;
	*(*bytePtr)++ = val >> 24;
}

inline void write2Bytes(uint32_t val, uint8_t **bytePtr) {
	*(*bytePtr)++ = val;
	*(*bytePtr)++ = val >> 8;
}

void writeFloatSound(int len, float *wave) {
	const int numChannels = 1;

	// build file
	const int bytesPersample = 4;
	const int soundChunkLen = len * bytesPersample;

	uint8_t bytes[58];
	uint8_t *bytePtr = bytes;





    writeFourCC(0x52494646, &bytePtr);                      //'RIFF'
    write4Bytes(4 + 26 + 12 + 8 + soundChunkLen, &bytePtr); // chunk size
    writeFourCC(0x57415645, &bytePtr);                      //'WAVE'
    writeFourCC(0x666d7420, &bytePtr);                      //'fmt '
    write4Bytes(18, &bytePtr);                              // size of subchunk that follows
    write2Bytes(WAVE_FORMAT_IEEE_FLOAT, &bytePtr);          // format
    write2Bytes(numChannels, &bytePtr);                     // number of channels
    write4Bytes(sampleRate, &bytePtr);                      // sample rate
    write4Bytes(sampleRate * numChannels * bytesPersample, &bytePtr);  // byte rate
    write2Bytes(numChannels * bytesPersample, &bytePtr);    // block align
    write2Bytes(bytesPersample * 8, &bytePtr);              // bits per sample
    write2Bytes(0, &bytePtr);                               // cbSize
    writeFourCC(0x66616374, &bytePtr);                     //'fact'
    write4Bytes(4, &bytePtr);                               // size of subchunk that follows
    write4Bytes(numChannels * len, &bytePtr);               // size of subchunk that follows
	  writeFourCC(0x64617461, &bytePtr);                        //'data'
    write4Bytes(len * numChannels * bytesPersample, &bytePtr); // subchunk size

	// write array to file
	FILE *pFile;
	pFile = fopen("oscillator test.wav", "wb");
	if (pFile) {
        fwrite(bytes, 1, sizeof(bytes), pFile);
        fwrite(wave, sizeof(*wave), len, pFile);            // note: little endian
		fclose(pFile);
	}

    return;
}
