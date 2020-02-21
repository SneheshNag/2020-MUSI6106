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
    CLfo(float fSampleRate, float fWidthInSec, float fFrequency) :
    m_pRingBuffer(0),
    m_fSampleRate(fSampleRate),
    m_iBufferLength(int(fSampleRate)),
    m_fReadIdx(0),
    m_fWidthInSec(fWidthInSec),
    m_fFrequency(fFrequency)
    {
        m_pRingBuffer = new CRingBuffer<float>(m_iBufferLength);
        float *pSineBuffer = new float [m_iBufferLength];
        CSynthesis::generateSine(pSineBuffer, 1.f, 1.f*m_iBufferLength, m_iBufferLength);
        m_pRingBuffer->put(pSineBuffer, m_iBufferLength);
        delete [] pSineBuffer;
    }
    
    ~CLfo()
    {
        delete m_pRingBuffer;
    }
    
    float getNextVal()
    {
        float fVal = m_fWidthInSec * m_fSampleRate * m_pRingBuffer->get(m_fReadIdx);
        float fReadIdxNext = m_fReadIdx + m_fFrequency / m_fSampleRate * m_iBufferLength;
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
    float               m_fWidthInSec;
    float               m_fFrequency;
    
};



#endif /* Lfo_h */
