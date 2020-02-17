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

    TEST_FIXTURE(VibratoData, TestName)
    {
    }

}

#endif //WITH_TESTS
