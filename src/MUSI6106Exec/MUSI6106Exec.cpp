


#include <iostream>
#include <ctime>

#include "MUSI6106Config.h"

#include "AudioFileIf.h"
#include "Fft.h"
//#include "RingBuffer.h"
#include "Vector.h"

using std::cout;
using std::endl;

// local function declarations
void    showClInfo ();
void convertToMono(float* pMonoAudioBuffer, float** ppInputAudioBuffer, int iNumChannels, int iLength);


/////////////////////////////////////////////////////////////////////////////////
// main function
int main(int argc, char* argv[])
{
    std::string             sInputFilePath,                 //!< file paths
    sOutputFilePath;
    
//    static const int        kBlockSize = 1024;
    
    clock_t                 time = 0;
    
    float                   **ppfAudioData = nullptr;
    float                   **ppfAudioWriteIdx = nullptr;
    
    CFft::complex_t         *pfSpectralData = nullptr;
    float                   *pfMag = nullptr;
    
    CAudioFileIf            *phAudioFile = 0;
    std::fstream            hOutputFile;
    CAudioFileIf::FileSpec_t stFileSpec;
    
    CFft                    *pFft = nullptr;
    
    int                     BlockLength;
    int                     HopLength;
    
    showClInfo();
    
    //////////////////////////////////////////////////////////////////////////////
    // parse command line arguments
    sInputFilePath = argv[1];
    sOutputFilePath = sInputFilePath.substr(0, sInputFilePath.size() - 4).append("_fftmag.txt");
    BlockLength = atoi(argv[2]);
    HopLength = atoi(argv[3]);
    
    //////////////////////////////////////////////////////////////////////////////
    // open the input wave file
    
    CAudioFileIf::create(phAudioFile);
    phAudioFile->openFile(sInputFilePath, CAudioFileIf::FileIoType_t::kFileRead, &stFileSpec);
    phAudioFile->getFileSpec(stFileSpec);
    
    //////////////////////////////////////////////////////////////////////////////
    // open the output text file
    
    hOutputFile.open(sOutputFilePath, std::fstream::out);
    hOutputFile.precision(16);
    
    //////////////////////////////////////////////////////////////////////////////
    // allocate memory
    ppfAudioData = new float* [stFileSpec.iNumChannels];
    ppfAudioWriteIdx = new float* [stFileSpec.iNumChannels];
    
    for (int i=0; i<stFileSpec.iNumChannels; i++) {
        ppfAudioData[i] = new float[BlockLength] (); // Init to zeros
        ppfAudioWriteIdx[i] = ppfAudioData[i] + BlockLength - HopLength;  // Pointer to write data read from input file to end of buffer
    }
    pfSpectralData = new float[BlockLength] (); // Init to zeros
    pfMag = new float [int(BlockLength / 2 + 1)];
    
    time = clock();
    
    //////////////////////////////////////////////////////////////////////////////
    // Create FFT class
    CFft::createInstance(pFft);
    pFft->initInstance(BlockLength);
    
    //////////////////////////////////////////////////////////////////////////////
    // FFT calc
    long long HopSize = HopLength;
    long long MagSize = int(BlockLength/2 + 1);
    
    while(!phAudioFile->isEof()) {
        
        phAudioFile->readData(ppfAudioWriteIdx, HopSize);
        

        pFft->doFft(pfSpectralData, ppfAudioData[0]);
        pFft->getMagnitude(pfMag, pfSpectralData);

        CVectorFloat::moveInMem(ppfAudioData[0], 0, HopLength, BlockLength - HopLength);
        
        for (long long i = 0; i < MagSize; i++) {
            hOutputFile << pfMag[i] << "\t";
        }
        hOutputFile << std::endl;
        
    }
    
    
    //////////////////////////////////////////////////////////////////////////////
    // clean-up
    CFft::destroyInstance(pFft);
    for (int i=0; i<stFileSpec.iNumChannels; i++)
        free(ppfAudioData[i]);
    free(ppfAudioData);
    phAudioFile->closeFile();
    CAudioFileIf::destroy(phAudioFile);
    hOutputFile.close();
    
    // all done
    return 0;
    
}


void     showClInfo()
{
    cout << "GTCMT MUSI6106 Executable" << endl;
    cout << "(c) 2014-2020 by Alexander Lerch" << endl;
    cout  << endl;
    
    return;
}


