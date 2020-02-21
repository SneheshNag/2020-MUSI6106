////
////  Lfo.cpp
////  AudioFileIO
////
////  Created by Snehesh Nag on 2/15/20.
////
//
//#include "Synthesis.h"
//#include "Lfo.h"
//
//
//Clfo::Clfo(float fSampleRate, float fWidthInSec, float fFrequency) :
////    m_pWavetable(new CRingBuffer<float> (table_size)),
//    m_Buffer_size(Buffer_size),
//    m_Sample_Rate(fSampleRate),
//    m_Freq_Hz(fFrequency),
//    m_Phase(0.0f),
//    m_Current_index(0.0f)
//
//{}
//
//
//
//Error_t Clfo::setSamplingRate(int Sample_Rate)
//{
//    m_Sample_Rate = Sample_Rate;
//    return kNoError;
//}
//
//int Clfo::getSamplingRate() const
//{
//    return m_Sample_Rate;
//}
//
//Error_t Clfo::setFrequency(float Freq_Hz)
//{
//    m_Freq_Hz = Freq_Hz;
//    // Phase (step size) = Wavetable size * Freq in Hz / Sample rate
//    m_Phase = (float)m_Table_size * (m_Freq_Hz / (float)m_Sample_Rate);
//    return kNoError;
//}
//
//float Clfo::getFrequency() const
//{
//    return m_Freq_Hz;
//}
//
//
//float Clfo::getPhase() const
//{
//    return m_Phase;
//}
//
//float Clfo::getCurrentIndex() const
//{
//    return m_Current_index;
//}
//
//float Clfo::getNextSample()
//{
//    float fVal = m_fWidthInSec * m_fSampleRate * m_pCRingBuff->get(m_fReadIdx);
//    float fReadIdxNext = m_fReadIdx + m_fFrequency / m_fSampleRate * m_iBufferLength;
//    if(fReadIdxNext >= m_iBufferLength)
//        m_fReadIdx = fReadIdxNext - m_iBufferLength;
//    else
//        m_fReadIdx = fReadIdxNext;
//
//    return fVal;
//}
//
//Error_t Clfo::updateWavetable()
//{
//    float* waveform = new float[m_Table_size];
//
//    CSynthesis::generateSine(waveform, m_Freq_Hz, m_Sample_Rate, m_Table_size);
//
//    m_pWavetable->put(waveform, m_Table_size);
//    return kNoError;
//}
//
//
//
