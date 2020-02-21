//
//  Lfo.h
//  MUSI6106
//
//  Created by Snehesh Nag on 2/13/20.
//

#ifndef Lfo_h
#define Lfo_h


#include "ErrorDef.h"
#include "RingBuffer.h"
#include "Synthesis.h"

class CLfo
{
public:
    CLfo(float Sample_rate, float Width_sec, float Freq_Hz) :
    m_pRingBuffer(0),
    m_fSampleRate(Sample_rate),
    m_iBufferLength(int(Sample_rate)),
    m_fReadIdx(0),
    m_Width_sec(Width_sec),
    m_Freq_Hz(Freq_Hz)
    {
        m_pRingBuffer = new CRingBuffer<float>(m_iBufferLength);
        float *pWavetable = new float [m_iBufferLength];
        CSynthesis::generateSine(pWavetable, 1.f, 1.f*m_iBufferLength, m_iBufferLength);
        m_pRingBuffer->put(pWavetable, m_iBufferLength);
        delete [] pWavetable;
    }
    
    ~CLfo()
    {
        delete m_pRingBuffer;
    }
    
    float getNextVal()
    {
        float fVal = m_Width_sec * m_fSampleRate * m_pRingBuffer->get(m_fReadIdx);
        float fReadIdxNext = m_fReadIdx + m_Freq_Hz / m_fSampleRate * m_iBufferLength;
        if(fReadIdxNext >= m_iBufferLength)
            m_fReadIdx = fReadIdxNext - m_iBufferLength;
        else
            m_fReadIdx = fReadIdxNext;
        
        return fVal;
    }
    
    
private:
    CRingBuffer<float>  *m_pRingBuffer;
    float               m_fSampleRate;
    int                 m_iBufferLength;
    float               m_fReadIdx;
    float               m_Width_sec;
    float               m_Freq_Hz;
    
};



#endif /* Lfo_h */
