/*
  ==============================================================================

    WaveformDisplay.h
    Author:  Shamie

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
* This component enables waveforms of audio being played to be painted to the screen 
*/

class WaveformDisplay  : public juce::Component,
                        public ChangeListener
{
public:

    WaveformDisplay(AudioFormatManager &formatManagerToUse, 
            AudioThumbnailCache &cacheToUse);
    ~WaveformDisplay() override;

    //==============================================================================
    /**Customise input graphics*/
    void paint(juce::Graphics&) override;
    /**Rescaling of components on the application window*/
    void resized() override;

    //==============================================================================
    /**Set source for audiothumb based on URL path and generates the name of nowPlaying song*/
    void loadURL(URL audioURL);

    /**Listener to repaint whenever there has been a change to enable moving playhead*/
    void changeListenerCallback(ChangeBroadcaster* source) override;

    /** Set relative position of the playhead*/
    void setRelativePosition(double pos);

private:

    AudioThumbnail audioThumb;

    bool fileLoaded;
    double position;
    std::string nowPlaying;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};
