#include "MUSI6106Config.h"

#ifdef WITH_TESTS
#include <cassert>
#include <cstdio>
#include <algorithm>

#include "UnitTest++.h"


#include "Vibrato.h"

SUITE(Vibrato)
{
    struct VibratoData
    {
        VibratoData() :
        m_pVibrato(0),
        m_ppfInputData(0),
        m_ppfOutputData(0),
        m_iDataLength(35131),
        m_fMaxDelayLength(3.F),
        m_iBlockLength(171),
        m_iNumChannels(3),
        m_fSampleRate(8000),
        m_fDelay(.1F),
        m_fGain(.5F)
        {
//            CCombFilterIf::create(m_pCombFilter);
            
            m_ppfInputData  = new float*[m_iNumChannels];
            m_ppfOutputData = new float*[m_iNumChannels];
            m_ppfInputTmp   = new float*[m_iNumChannels];
            m_ppfOutputTmp  = new float*[m_iNumChannels];
            for (int i = 0; i < m_iNumChannels; i++)
            {
                m_ppfInputData[i]   = new float [m_iDataLength];
                CVectorFloat::setZero(m_ppfInputData[i], m_iDataLength);
                m_ppfOutputData[i]  = new float [m_iDataLength];
                CVectorFloat::setZero(m_ppfOutputData[i], m_iDataLength);            }
        }
        ~VibratoData()
        {
            for (int i = 0; i < m_iNumChannels; i++)
            {
                delete [] m_ppfOutputData[i];
                delete [] m_ppfInputData[i];
            }
            delete [] m_ppfOutputTmp;
            delete [] m_ppfInputTmp;
            delete [] m_ppfOutputData;
            delete [] m_ppfInputData;
            
        }
        
    };

    TEST_FIXTURE(VibratoData, VaryingBlockSize)
    {
        m_pVibrato->setParam(Vibrato::mod_freq, 5);
        m_pVibrato->setParam(Vibrato::width, 0.2);
        m_pVibrato->initLfo();
        
        Testprocess();
        
        m_pVibrato->reset();
        m_pVibrato->init(m_max_width,m_fSampleRate,m_iNumChannels);
        m_pVibrato->setParam(Vibrato::mod_freq, 5);
        m_pVibrato->setParam(Vibrato::width, 0.2);
//        m_pVibrato->initLfo();
        {
            int iNumFramesRemaining = m_iDataLength;
            while (iNumFramesRemaining > 0)
            {
                int iNumFrames = std::min(static_cast<float>(iNumFramesRemaining), static_cast<float>(rand())/RAND_MAX*17000.F);
                
                for (int c = 0; c < m_iNumChannels; c++)
                {
                    m_ppfInputTmp[c] = &m_ppfInputData[c][m_iDataLength - iNumFramesRemaining];
                }
                m_pVibrato->process(m_ppfInputTmp, m_ppfInputTmp, iNumFrames);
                
                iNumFramesRemaining -= iNumFrames;
            }
        }
        
        for (int c = 0; c < m_iNumChannels; c++)
            CHECK_ARRAY_CLOSE(m_ppfInputData[c], m_ppfOutputData[c], m_iDataLength, 1e-3);
    }

}

#endif //WITH_TESTS
