#include "MUSI6106Config.h"

#ifdef WITH_TESTS
#include <cassert>
#include <cstdio>
#include <algorithm>

#include "UnitTest++.h"
#include "Synthesis.h"
#include "Vector.h"

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
        m_fMaxWidthSecs(0.1F),
        m_iBlockLength(1024),
        m_iNumChannels(2),
        m_fSampleRate(44100),
        m_fWidthSecs(.05F),
        m_fModfreqInHz(10.0F)
        {
            Vibrato::create(m_pVibrato);
            m_pVibrato->init(m_fMaxWidthSecs,m_fSampleRate,m_iNumChannels);
            
            m_ppfInputData  = new float*[m_iNumChannels];
            m_ppfOutputData = new float*[m_iNumChannels];
            m_ppfInputTmp   = new float*[m_iNumChannels];
            m_ppfOutputTmp  = new float*[m_iNumChannels];
            for (int i = 0; i < m_iNumChannels; i++)
            {
                m_ppfInputData[i]   = new float [m_iDataLength];
                CVectorFloat::setZero(m_ppfInputData[i], m_iDataLength);
                m_ppfOutputData[i]  = new float [m_iDataLength];
                CVectorFloat::setZero(m_ppfOutputData[i], m_iDataLength);
                
            }
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
            
            Vibrato::destroy(m_pVibrato);
        }
        
        void TestProcess()
        {
            int iNumFramesRemaining = m_iDataLength;
            while (iNumFramesRemaining > 0)
            {
                int iNumFrames = std::min(iNumFramesRemaining, m_iBlockLength);
                
                for (int c = 0; c < m_iNumChannels; c++)
                {
                    m_ppfInputTmp[c]    = &m_ppfInputData[c][m_iDataLength - iNumFramesRemaining];
                    m_ppfOutputTmp[c]   = &m_ppfOutputData[c][m_iDataLength - iNumFramesRemaining];
                }
                m_pVibrato->process(m_ppfInputTmp, m_ppfOutputTmp, iNumFrames);
                
                iNumFramesRemaining -= iNumFrames;
            }
        }
        
        void TestProcessInplace()
        {
            int iNumFramesRemaining = m_iDataLength;
            while (iNumFramesRemaining > 0)
            {
                int iNumFrames = std::min(iNumFramesRemaining, m_iBlockLength);
                
                for (int c = 0; c < m_iNumChannels; c++)
                {
                    m_ppfInputTmp[c]    = &m_ppfInputData[c][m_iDataLength - iNumFramesRemaining];
                }
                m_pVibrato->process(m_ppfInputTmp, m_ppfInputTmp, iNumFrames);
                
                iNumFramesRemaining -= iNumFrames;
            }
        }
        
        Vibrato  *m_pVibrato;
        float**  m_ppfInputData;
        float** m_ppfOutputData;
        float** m_ppfInputTmp;
        float** m_ppfOutputTmp;
        int     m_iDataLength;
        float   m_fMaxWidthSecs;
        int     m_iBlockLength;
        int     m_iNumChannels;
        float   m_fSampleRate,
        m_fWidthSecs,
        m_fModfreqInHz;
        
    };

    
    
    TEST_FIXTURE(VibratoData, Input_Zero)
    {
        
        m_pVibrato->reset();
        
        TestProcess();
        
        for (int c = 0; c < m_iNumChannels; c++)
            CHECK_ARRAY_CLOSE(m_ppfInputData[c], m_ppfOutputData[c], m_iDataLength, 1e-3);
        
        m_pVibrato->reset();
    }
    
    TEST_FIXTURE(VibratoData, DC_Input)
    {
        m_pVibrato->reset();
        
        for (int c = 0; c < m_iNumChannels; c++)
            CSynthesis::generateDc(m_ppfInputData[c], m_iDataLength);
        
        TestProcess();
        
        for (int c = 0; c < m_iNumChannels; c++)
            CHECK_ARRAY_CLOSE(m_ppfInputData[c], m_ppfOutputData[c], m_iDataLength, 1e-3);
        
        m_pVibrato->reset();
    }
    
    TEST_FIXTURE(VibratoData, Mod_Freq_Zero)
    {
        m_pVibrato->setParam(Vibrato::mod_freq, 0);
        m_pVibrato->setParam(Vibrato::width, 0.05);
        m_pVibrato->initLfo();
        
        for (int c = 0; c < m_iNumChannels; c++)
            CSynthesis::generateSine (m_ppfInputData[c], 220.0F, m_fSampleRate, m_iDataLength, .8F, static_cast<float>(c*M_PI_2));
        
        TestProcess();
        
        for (int c = 0; c < m_iNumChannels; c++)
            CHECK_ARRAY_CLOSE(m_ppfInputData[c], &m_ppfOutputData[c][int(m_fMaxWidthSecs*m_fSampleRate)], m_iDataLength - int(m_fMaxWidthSecs*m_fSampleRate), 1e-3);
    }
}

#endif //WITH_TESTS
