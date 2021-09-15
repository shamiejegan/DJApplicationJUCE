/*
  ==============================================================================

    WaveformDisplay.cpp
    Author:  Shamie

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WaveformDisplay.h"

//==============================================================================
WaveformDisplay::WaveformDisplay(AudioFormatManager & formatManagerToUse,
                                AudioThumbnailCache & cacheToUse) : 
                                audioThumb(1000, formatManagerToUse, cacheToUse),
                                fileLoaded(false), 
                                position(0)
{
  
    audioThumb.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay(){}


//==============================================================================
void WaveformDisplay::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

    //draw borders 
    g.setColour(juce::Colours::grey);
    g.drawRect(getLocalBounds(), 1);

    //if file is loaded, draw waveforms
    if (fileLoaded)
    {
        //draw waveforms in red
        g.setColour(juce::Colours::crimson);
        audioThumb.drawChannel(g,
            getLocalBounds(), // area
            0, //start time
            audioThumb.getTotalLength(), //length of file as end time
            0,
            1.0f
        );

        //draw playhead in green
        g.setColour(juce::Colours::mediumspringgreen);
        g.fillRect(position * getWidth(), 0, 2, getHeight());

        //display name of currently playing track on the waveform in white
        g.setColour(juce::Colours::floralwhite);
        g.setFont(16.0f);
        g.drawText(nowPlaying, getLocalBounds(),
            juce::Justification::centred, true);
    }
    else
    {
        //file not loaded 
        g.setColour(juce::Colours::mediumspringgreen);
        g.setFont(20.0f);
        g.drawText("Load File to Channel to Begin...", getLocalBounds(),
            juce::Justification::centred, true);
    }
}
void WaveformDisplay::resized()
{

}

//==============================================================================
void WaveformDisplay::loadURL(URL audioURL)
{
    audioThumb.clear();
    fileLoaded = audioThumb.setSource(new URLInputSource(audioURL));
    if (fileLoaded)
    {
        std::string justFile = audioURL.toString(false).toStdString();
        std::size_t startFilePos = justFile.find_last_of("/");
        std::size_t startExtPos = justFile.find_last_of(".");
        std::string extn = justFile.substr(startExtPos + 1, justFile.length() - startExtPos);
        std::string file = justFile.substr(startFilePos + 1, justFile.length() - startFilePos - extn.size() - 2);

        nowPlaying = file;
        repaint();
    }
    else
    {

    }
}

void WaveformDisplay::changeListenerCallback(ChangeBroadcaster* source)
{
    repaint();
}

void WaveformDisplay::setRelativePosition(double pos)
{
    if (pos != position)
    {
        position = pos; //update position 
        repaint(); //then repaint
    }
}