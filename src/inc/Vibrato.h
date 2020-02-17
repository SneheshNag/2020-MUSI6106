//
//  Vibrato.h
//  MUSI6106
//
//  Created by Snehesh Nag on 2/17/20.
//

#ifndef Vibrato_h
#define Vibrato_h

#include "ErrorDef.h"
#include "RingBuffer.h"
#include "Lfo.h"

class Vibrato
{
public:
    enum param_list
    {
        delay,
        width,
        mod_freq
    };
//    Vibrato (int Num_channels, float Sample_rate, float delay_sec, float width_sec, float mod_freq_sec);
    
//    ~Vibrato (void) {};
    
    static Error_t create (Vibrato*& pVibrato);
    
    static Error_t destroy (Vibrato*& pVibrato);
    
    Error_t init (float fMaxWidthInSec, float fSampleRateInHz, int iNumChannels);
    
    Error_t reset ();
    

    Error_t setParam (param_list eParam, float fParamValue);
    

    float getParam (param_list eParam) const;
    
    /*! processes one block of audio
     \param ppfInputBuffer input buffer [numChannels][iNumberOfFrames]
     \param ppfOutputBuffer output buffer [numChannels][iNumberOfFrames]
     \param iNumberOfFrames buffer length (per channel)
     \return Error_t
     */
    Error_t process (float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames);
    
protected:
    Vibrato ();
    virtual ~Vibrato () {};
    
private:
//    int m_iNumChannels;
//    float m_fSampleRate;
//    
//    float m_fDelayInSamples;
//    float m_fWidthInSamples;
//    float m_fModFreqInSamples;
//    
//    float m_fDelayLengthInSamples;
//    CRingBuffer<float>  **m_ppCDelayLine;
//    Clfo    **m_ppCLFO;
    
    bool                m_bIsInitialized;   //!< internal bool to check whether the init function has been called
    
    float               m_fSampleRate;      //!< audio sample rate in Hz
    int                 m_iNumChannels;     //!< number of channels
    
    float               m_fWidthInSec;      //!< LFO modulation width in seconds
    float               m_fFrequency;       //!< LFO modulation frequency in Hz
    float               m_fMaxWidthInSec;   //!< FLO max modulation width in seconds
    
    CRingBuffer<float>  **m_ppCRingBuffer;  //!< ring buffer
    Clfo                *m_pCLfo;           //!< LFO

};


#endif /* Vibrato_h */
