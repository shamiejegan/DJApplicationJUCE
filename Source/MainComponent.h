/*
  ==============================================================================

    MainComponent.h
    Author:  Shamie

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "DeckGUI.h"
#include "PlaylistComponent.h"


//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent   : public AudioAppComponent
{
public:

    //==============================================================================
    MainComponent();
    ~MainComponent();


    //==============================================================================
    /*Override of AudioAppComponent pure virtual. Tells the source to prepare for playing */
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    /*Override of AudioAppComponent pure virtual. Called repeatedly to fetch subsequent blocks of audio data */
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    /*Override of AudioAppComponent pure virtual. Allows the source to release anything it no longer needs after playback has stopped */
    void releaseResources() override;


    //==============================================================================
    /**Customise input graphics*/
    void paint (Graphics& g) override;
    /**Rescaling of components on the application window*/
    void resized() override;


private:

    //==============================================================================
    AudioFormatManager formatManager; 
    AudioThumbnailCache thumbCache{ 100 }; //cache up to 100 files 

    int channelL = 0;
    int channelR = 1;

    PlaylistComponent playlistComponent{formatManager};
    DJAudioPlayer playerLeft{formatManager};
    DeckGUI deckGUILeft{&playerLeft,&playlistComponent, formatManager, thumbCache, channelL};

    DJAudioPlayer playerRight{ formatManager };
    DeckGUI deckGUIRight{&playerRight, &playlistComponent, formatManager, thumbCache, channelR};

    //==============================================================================
    Label waveformLabel;
    Label posLabel;
    Label widgetLabel;
    Label playlistLabel;

    //==============================================================================
    MixerAudioSource mixerSource; 


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent); 
};
