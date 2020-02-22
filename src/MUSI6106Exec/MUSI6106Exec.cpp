

#include <iostream>
#include <ctime>

#include "MUSI6106Config.h"

#include "AudioFileIf.h"
#include "Fft.h"
#include "RingBuffer.h"
#include "Vector.h"
//#include "CombFilterIf.h"

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
    float                   **ppfAudioWriteIdx = 0;

    CAudioFileIf            *phAudioFile = 0;
    std::fstream            hOutputFile;
    CAudioFileIf::FileSpec_t stFileSpec;
    CFft                    *pCFft = nullptr;
//    CRingBuffer<float>      *pCRingBuffer = nullptr;
    
    CFft::complex_t         *pfSpectralData = nullptr;
    float                   *pfMag = nullptr;
    
    int BlockLength;
    int HopLength;

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
        sInputFilePath = argv[1];
        sOutputFilePath = sInputFilePath + "_FFTMag.txt";
        BlockLength = atof(argv[2]);
        HopLength = atof(argv[3]);
        
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
    
    // Create FFT
    CFft::createInstance(pCFft);
    


    //////////////////////////////////////////////////////////////////////////////
    // open the output text file
    hOutputFile.open(sOutputFilePath.c_str(), std::ios::out);
    hOutputFile.precision(16);
    if (!hOutputFile.is_open())
    {
        cout << "Text file open error!";
        return -1;
    }
    
    //////////////////////////////////////////////////////////////////////////////
    // allocate memory
    ppfAudioData = new float*[stFileSpec.iNumChannels];
    ppfAudioWriteIdx = new float* [stFileSpec.iNumChannels];
    
    for (int i = 0; i < stFileSpec.iNumChannels; i++)
    {
        ppfAudioData[i] = new float[BlockLength];
        ppfAudioWriteIdx[i] = ppfAudioData[i] + BlockLength - HopLength;
    }

    pfSpectralData = new float[BlockLength] (); // Init to zeros
    pfMag = new float [int(BlockLength / 2 + 1)];
    
    time = clock();
    
//    pCRingBuffer = new CRingBuffer<float>(BlockLength);
    
    //////////////////////////////////////////////////////////////////////////////
    // get audio data and write it to the output file
    long long HopSize = HopLength;
    long long MagSize = int(BlockLength/2 +1);
    
    while (!phAudioFile->isEof())
    {
//        long long iNumFrames = kBlockSize;
        phAudioFile->readData(ppfAudioWriteIdx, HopSize);

        cout << "\r" << "reading and writing";
        
        pCFft->doFft(pfSpectralData, ppfAudioData[0]);
        pCFft->getMagnitude(pfMag, pfSpectralData);
        
        // Move window overlap portion to start of buffer.
        CVectorFloat::moveInMem(ppfAudioData[0], HopLength, 0, BlockLength - HopLength);


        for (long long i = 0; i < MagSize; i++) {
            hOutputFile << pfMag[i] << "\t";
        }
        hOutputFile << std::endl;
    }

    cout << "\nreading/writing done in: \t" << (clock() - time)*1.F / CLOCKS_PER_SEC << " seconds." << endl;

    //////////////////////////////////////////////////////////////////////////////
    // clean-up
    CAudioFileIf::destroy(phAudioFile);
    hOutputFile.close();

    for (int i = 0; i < stFileSpec.iNumChannels; i++)
        delete[] ppfAudioData[i];
    delete[] ppfAudioData;
    ppfAudioData = 0;
    hOutputFile.close();
    return 0;

}


void     showClInfo()
{
    cout << "GTCMT MUSI6106 Executable" << endl;
    cout << "(c) 2014-2020 by Alexander Lerch" << endl;
    cout  << endl;

    return;
}


