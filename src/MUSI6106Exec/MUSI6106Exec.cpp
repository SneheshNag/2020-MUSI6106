
#include <iostream>
#include <ctime>

#include "MUSI6106Config.h"

#include "AudioFileIf.h"
//#include "CombFilterIf.h"
#include "Vibrato.h"

using std::cout;
using std::endl;

// local function declarations
void    showClInfo ();

/////////////////////////////////////////////////////////////////////////////////
// main function
int main(int argc, char* argv[])
{
    std::string             sInputFilePath,                 //!< file paths
                            sOutputFilePath;

    static const int        kBlockSize = 1024;

    clock_t                 time = 0;

    float                   **ppfAudioData = 0;
    float                   **ppfAudioOutput = 0;

    CAudioFileIf            *phAudioFileOutput = 0;
    CAudioFileIf            *phAudioFile = 0;
    std::fstream            hOutputFile;
    CAudioFileIf::FileSpec_t stFileSpec;
    Vibrato                 *pVibrato;
    
    float delay_sec;
    float width_sec;
    float mod_freq_Hz;

    //CCombFilterIf   *pInstance = 0;
    //CCombFilterIf::create(pInstance);
    showClInfo();

    //////////////////////////////////////////////////////////////////////////////
    // parse command line arguments
    if (argc < 2)
    {
        cout << "Missing audio input path!";
        return -1;
    }
    else
    {
        if (argc > 2)
        {
            delay_sec = atof(argv[2]);
            width_sec = atof(argv[3]);
            mod_freq_Hz = atof(argv[4]);
        }
        sInputFilePath = argv[1];
        sOutputFilePath = sInputFilePath + ".wav";
    }

    //////////////////////////////////////////////////////////////////////////////
    // open the input wave file
    CAudioFileIf::create(phAudioFile);
    phAudioFile->openFile(sInputFilePath, CAudioFileIf::kFileRead);
    if (!phAudioFile->isOpen())
    {
        cout << "Wave file open error!";
        return -1;
    }
    phAudioFile->getFileSpec(stFileSpec);

    //////////////////////////////////////////////////////////////////////////////
    // open the output text file
//    hOutputFile.open(sOutputFilePath.c_str(), std::ios::out);
//    if (!hOutputFile.is_open())
//    {
//        cout << "Text file open error!";
//        return -1;
//    }

    // open output wav file
    CAudioFileIf::create(phAudioFileOutput);
    phAudioFileOutput->openFile(sOutputFilePath, CAudioFileIf::kFileWrite, &stFileSpec);
    if (!phAudioFileOutput->isOpen())
    {
        cout << "Output wave file open error!";
        return -1;
    }
    //////////////////////////////////////////////////////////////////////////////
    // allocate memory
    ppfAudioData = new float*[stFileSpec.iNumChannels];
    for (int i = 0; i < stFileSpec.iNumChannels; i++)
        ppfAudioData[i] = new float[kBlockSize];

    ppfAudioOutput = new float*[stFileSpec.iNumChannels];
    for (int i = 0; i < stFileSpec.iNumChannels; i++)
        ppfAudioOutput[i] = new float[kBlockSize];

    time = clock();
    
//    pVibrato = new Vibrato(stFileSpec.iNumChannels, stFileSpec.fSampleRateInHz, delay_sec, width_sec, mod_freq_Hz);
    Vibrato::create(pVibrato);
    pVibrato->init(delay_sec, stFileSpec.fSampleRateInHz, stFileSpec.iNumChannels);
    pVibrato->setParam(Vibrato::delay, delay_sec);
    pVibrato->setParam(Vibrato::width, width_sec);
    pVibrato->setParam(Vibrato::mod_freq, mod_freq_Hz);
    pVibrato->initLfo();
//    pVibrato->initLfo();
    //////////////////////////////////////////////////////////////////////////////
    // get audio data and write it to the output file
    while (!phAudioFile->isEof())
    {
        long long iNumFrames = kBlockSize;
        phAudioFile->readData(ppfAudioData, iNumFrames);
        
        pVibrato->process(ppfAudioData, ppfAudioOutput, iNumFrames);

        cout << "\r" << "reading and writing";
        
        phAudioFileOutput->writeData(ppfAudioOutput, iNumFrames);

//        for (int i = 0; i < iNumFrames; i++)
//        {
//            for (int c = 0; c < stFileSpec.iNumChannels; c++)
//            {
//                hOutputFile << ppfAudioData[c][i] << "\t";
//            }
//            hOutputFile << endl;
//        }
    }

    cout << "\nreading/writing done in: \t" << (clock() - time)*1.F / CLOCKS_PER_SEC << " seconds." << endl;

    //////////////////////////////////////////////////////////////////////////////
    // clean-up
    CAudioFileIf::destroy(phAudioFile);
    CAudioFileIf::destroy(phAudioFileOutput);
    Vibrato::destroy(pVibrato);
    for (int i = 0; i < stFileSpec.iNumChannels; i++) {
        delete[] ppfAudioData[i];
        delete[] ppfAudioOutput[i];
    }
    delete[] ppfAudioData;
    delete[] ppfAudioOutput;
    ppfAudioData = nullptr;
    ppfAudioOutput = nullptr;

}


void     showClInfo()
{
    cout << "GTCMT MUSI6106 Executable" << endl;
    cout << "(c) 2014-2020 by Alexander Lerch" << endl;
    cout  << endl;

    return;
}

