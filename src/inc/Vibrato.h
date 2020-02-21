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
    
    Error_t init (float Max_width_sec, float Sample_Rate, int iNumChannels);
    
    Error_t initLfo ();
    
    Error_t reset ();
    

    Error_t setParam (param_list eParam, float fParamValue);
    

    float getParam (param_list eParam) const;
    
    Error_t process (float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames);
    
protected:
    Vibrato ();
    virtual ~Vibrato () {};
    
private:

    
    bool                m_bIsInitialized;
    
    float               m_fSampleRate;
    int                 m_iNumChannels;
    float               m_delay_sec;
    float               m_width_sec;
    float               m_freq_Hz;
    float               m_max_width;
    
    CRingBuffer<float>  **m_ppCRingBuffer;
    CLfo                *m_pCLfo;

};


#endif /* Vibrato_h */
