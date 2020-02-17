//
//  Lfo.cpp
//  AudioFileIO
//
//  Created by Snehesh Nag on 2/15/20.
//

#include "Synthesis.h"
#include "Lfo.h"


Clfo::Clfo(int table_size) :
    m_pWavetable(new CRingBuffer<float> (table_size)),
    m_Table_size(table_size),
    m_Sample_Rate(0),
    m_Freq_Hz(0.0f),
    m_Phase(0.0f),
    m_Current_index(0.0f)

{}

Clfo::~Clfo(void) {}

Error_t Clfo::init(int Sample_Rate, float Freq_Hz)
{
    
    m_Sample_Rate = Sample_Rate;
    m_Freq_Hz = Freq_Hz;
    updateWavetable();
}


Error_t Clfo::setSamplingRate(int Sample_Rate)
{
    m_Sample_Rate = Sample_Rate;
    return kNoError;
}

int Clfo::getSamplingRate() const
{
    return m_Sample_Rate;
}

Error_t Clfo::setFrequency(float Freq_Hz)
{
    m_Freq_Hz = Freq_Hz;
    // Phase (step size) = Wavetable size * Freq in Hz / Sample rate
    m_Phase = (float)m_Table_size * (m_Freq_Hz / (float)m_Sample_Rate);
    return kNoError;
}

float Clfo::getFrequency() const
{
    return m_Freq_Hz;
}


float Clfo::getPhase() const
{
    return m_Phase;
}

float Clfo::getCurrentIndex() const
{
    return m_Current_index;
}

Error_t Clfo::updateWavetable()
{
    float* waveform = new float[m_Table_size];
    
    CSynthesis::generateSine(waveform, m_Freq_Hz, m_Sample_Rate, m_Table_size);
    
    m_pWavetable->put(waveform, m_Table_size);
    return kNoError;
}



