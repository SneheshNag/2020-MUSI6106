#include "MUSI6106Config.h"

#include "ErrorDef.h"
#include "Util.h"
#include "RingBuffer.h"
#include "Lfo.h"
#include "Vibrato.h"

Vibrato::Vibrato () :
m_bIsInitialized(false),
m_fSampleRate(0),
m_iNumChannels(0),
m_width_sec(0),
m_freq_Hz(0),
m_max_width(0),
m_ppCRingBuffer(0),
m_pCLfo(0)
{
    this->reset ();
}


Error_t Vibrato::create (Vibrato*& pCVibrato)
{
    pCVibrato = new Vibrato ();
    
    if (!pCVibrato)
        return kUnknownError;
    
    return kNoError;
}

Error_t Vibrato::destroy (Vibrato*& pCVibrato)
{
    if (!pCVibrato)
        return kUnknownError;
    
    pCVibrato->reset ();
    
    delete pCVibrato;
    pCVibrato = 0;
    
    return kNoError;
}

Error_t Vibrato::init (float Max_width_sec, float Sample_Rate, int iNumChannels)
{
    reset();
    
    m_fSampleRate = Sample_Rate;
    m_iNumChannels = iNumChannels;
    m_max_width = Max_width_sec;
    int Max_width_samples = CUtil::float2int<int>(m_max_width * m_fSampleRate);
    
    m_ppCRingBuffer = new CRingBuffer<float>*[m_iNumChannels];
    for(int i=0; i<iNumChannels; i++)
    {
        m_ppCRingBuffer[i] = new CRingBuffer<float>(Max_width_samples*2+1);
        m_ppCRingBuffer[i]->setWriteIdx(Max_width_samples);
    }
    
    m_bIsInitialized = true;
    
    return kNoError;
}

Error_t Vibrato::initLfo ()
{
    m_pCLfo = new CLfo(m_fSampleRate, m_width_sec , m_freq_Hz);
    return kNoError;
}

Error_t Vibrato::reset ()
{
    for (int i=0; i<m_iNumChannels; i++)
        delete m_ppCRingBuffer[i];
    delete [] m_ppCRingBuffer;
    m_ppCRingBuffer = 0;
    
    delete m_pCLfo;
    m_pCLfo = 0;
    
    m_fSampleRate = 0;
    m_iNumChannels = 0;
    m_width_sec = 0;
    m_freq_Hz = 0;
    m_max_width = 0;
    m_delay_sec = 0;
    
    m_bIsInitialized = false;
    
    return kNoError;
}

Error_t Vibrato::setParam (param_list eParam, float fParamValue)
{
    if (!m_bIsInitialized)
        return kNotInitializedError;
    
    switch (eParam)
    {
        case delay:
            m_delay_sec = fParamValue;
            if (m_delay_sec < 0 || m_delay_sec > m_max_width)
                return kFunctionInvalidArgsError;
        case width:
            m_width_sec = fParamValue;
            if(m_width_sec>m_max_width){
                return kFunctionInvalidArgsError;
            }
            break;
            
        case mod_freq:
            m_freq_Hz = fParamValue;
            break;
    }
    return kNoError;
}

float   Vibrato::getParam (param_list eParam) const
{
    switch (eParam)
    {
        case delay:
            return delay;
        case width:
            return width;
        case mod_freq:
            return mod_freq;
    }
    return kNoError;
}

Error_t Vibrato::process (float **ppfInputBuffer, float **ppfOutputBuffer, int iNumberOfFrames)
{
    for(int i=0; i<iNumberOfFrames; i++)
    {
        float fOffset = m_pCLfo->getNextVal();
        for (int j=0; j<m_iNumChannels; j++)
        {
            m_ppCRingBuffer[j]->putPostInc(ppfInputBuffer[j][i]);
            ppfOutputBuffer[j][i] = m_ppCRingBuffer[j]->get(-fOffset);
            m_ppCRingBuffer[j]->getPostInc();
            
        }
    }
    return kNoError;
}
