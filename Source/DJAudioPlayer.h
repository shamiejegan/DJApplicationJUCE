/*
  ==============================================================================
    DJAudioPlayer.h
    Author:  Shamie
  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//===============================================================================
/*
    This component controls the actual playing of the audio files
*/
class DJAudioPlayer : public AudioSource
{
public:

    DJAudioPlayer(AudioFormatManager& formatManager);
    ~DJAudioPlayer();

    //==============================================================================
    /**Override of AudioSource pure virtual. 
    For Audio Source component to tell the source to prepare for playing*/
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    /**Override of AudioSource pure virtual.
    For Audio Source component to fetch subsequent blocks of audio data*/
    void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
    /**Override of AudioSource pure virtual.
    For Audio Source component to release anything it no longer needs*/
    void releaseResources() override;


    //==============================================================================
    /**Set source for the transport and reader based on URL path*/
    void loadURL(URL audioURL);
    /**Set gain (volume) based on input value between 0-1, received from the slider*/
    void setGain(double gain);
    /**Set speed based on input value as a ratio of speed, where 1 is the default 1x speed */
    void setSpeed(double ratio);
    /**Identifies position in seconds based on input value 0-1, received from playback slider*/
    void setRelativePosition(double pos);
    /**Set position of the transport source playhead to the input value in seconds*/
    void setPosition(double posInSecs);

    /**Get relative position of the playhead and returns value, used for plotting playhead on waveform*/
    double getRelativePosition();

    /**Start the transport source */
    void start();
    /**Stop the transport source */
    void stop();


private: 
    //==============================================================================
    AudioFormatManager& formatManager;

    std::unique_ptr<AudioFormatReaderSource> readerSource;

    AudioTransportSource transportSource;

    ResamplingAudioSource resampleSource{ &transportSource, false, 2 };

};