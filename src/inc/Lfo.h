//
//  Lfo.h
//  MUSI6106
//
//  Created by Snehesh Nag on 2/13/20.
//

#ifndef Lfo_h
#define Lfo_h


#include "RingBuffer.h"
#include "Synthesis.h"
#include "ErrorDef.h"

class Clfo {
    
public:

    
    Clfo (int table_size);
    
    ~Clfo();
    
    Error_t init(float Sample_Rate, float Freq_Hz);
    
    
    Error_t setSamplingRate(int Sample_Rate);
    int getSamplingRate() const;
    
    Error_t setFrequency(float Freq_Hz);
    float getFrequency() const;
    
    float getPhase() const;
    float getCurrentIndex() const;
    float getNextSample();
    Error_t updateWavetable();
    
private:
    CRingBuffer<float>* m_pWavetable;
    int m_Table_size;
    int m_Sample_Rate;
    float m_Freq_Hz;
    float m_Phase;
    float m_Current_index;
    
};



#endif /* Lfo_h */
