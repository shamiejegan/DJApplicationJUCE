/*
  ==============================================================================

    DeckGUI.h
    Author:  Shamie

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
#include "PlaylistComponent.h"

//===============================================================================
/*
    This component controls the GUI interface of the application for each channel
*/

class DeckGUI : public Component,
    public Button::Listener,
    public Slider::Listener,
    public TableListBoxModel,
    public Timer
{
public:

    //==============================================================================
    DeckGUI(DJAudioPlayer* player,
        PlaylistComponent* playlistComponent,
        AudioFormatManager& formatManagerToUse,
        AudioThumbnailCache& cacheToUse, 
        int channelToUse);
    ~DeckGUI();


    //==============================================================================
    /**Customise input graphics*/
    void paint(Graphics&) override;
    /**Rescaling of components on the application window*/
    void resized() override;


    //==============================================================================
    /**Override of Button::Listener pure virtual.
    Called when the button is clicked, allowing interacion of play,pause,next buttons with the player*/
    void buttonClicked(Button*) override;

    /**Override of Slider::Listener pure virtual.
    Called when the slider's value is changed, allowing interacion of vol,speed,playback sliders with the player*/
    void sliderValueChanged(Slider* slider) override;


    //==============================================================================
    /**Override of TableListBoxModel pure virtual.For up next table in GUI
    Returns the number of rows currently in the table*/
    int getNumRows() override;
    /**Override of TableListBoxModel pure virtual.For up next table in GUI
    Draws the background behind one of the rows in the table*/
    void paintRowBackground(Graphics&,
        int rowNumber,
        int width,
        int height,
        bool rowIsSelected) override;
    /**Override of TableListBoxModel pure virtual.For up next table in GUI
    Draws one of the cells, used to input details in cells*/
    void paintCell(Graphics&,
        int rowNumber,
        int columnId,
        int width,
        int height,
        bool rowIsSelected) override;


    //==============================================================================
    /**Override of Timer pure virtual.To allow call back for updating waveform visual*/ 
    void timerCallback() override;

    
private: 

    //Create Buttons
    TextButton playButton{ "PLAY" };
    TextButton stopButton{ "PAUSE" };
    TextButton nextButton{ "LOAD" };

    //Create Sliders 
    Slider volSlider;
    Slider speedSlider;
    Slider posSlider;

    //Add labels to sliders 
    Label volLabel; 
    Label speedLabel;

    //Control visual theme
    LookAndFeel_V4 lookandfeel;

    //Create player associated with the GUI
    DJAudioPlayer* player;
    //Create playlist component associated with the GUI
    PlaylistComponent* playlistComponent;

    //Create waveform visual
    WaveformDisplay waveformDisplay;

    //Create table containing list of upcoming songs in the playlist
    TableListBox upNext;

    //variable indicating channel associated with the GUI (0=Left, 1=Right)
    int channel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DeckGUI);
};