/*
  ==============================================================================

    DeckGUI.cpp
    Author:  Shamie

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "DeckGUI.h"    
#include <cmath>
#include "PlaylistComponent.h"


DeckGUI::DeckGUI(DJAudioPlayer* _player,
                PlaylistComponent* _playlistComponent,
                AudioFormatManager& formatManagerToUse,
                AudioThumbnailCache& cacheToUse, 
                int channelToUse
                ) : player(_player), 
                    playlistComponent(_playlistComponent),
                    waveformDisplay(formatManagerToUse,cacheToUse), 
                    channel(channelToUse)
{

    //add buttons for each GUI and add listeners to them 
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(nextButton);
    playButton.addListener(this);
    stopButton.addListener(this);
    nextButton.addListener(this);

    //add sliders for each GUI, format them, add labels, and add listeners to them 
    addAndMakeVisible(posSlider);
    posSlider.addListener(this);
    posSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    posSlider.setRange(0.0, 1.0);
    posSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);

    addAndMakeVisible(volSlider);
    volSlider.addListener(this);
    volSlider.setRange(0.0, 1.0);
    volSlider.setValue(0.5); //default volume half of max vol
    volSlider.setSliderStyle(Slider::SliderStyle::LinearBarVertical);
    volSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    addAndMakeVisible(volLabel);
    volLabel.setText("Volume", juce::dontSendNotification);
    volLabel.attachToComponent(&volSlider, false);
    volLabel.setJustificationType(juce::Justification::centred);

    addAndMakeVisible(speedSlider);
    speedSlider.addListener(this);
    speedSlider.setRange(0.5, 2, 0); //min half speed, max speed 2x
    speedSlider.setValue(1); //default speed at 1x
    speedSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalDrag);
    speedSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    speedSlider.setMouseDragSensitivity(80);
    addAndMakeVisible(speedLabel);
    speedLabel.setText("Speed X", juce::dontSendNotification);
    speedLabel.attachToComponent(&speedSlider, false);
    speedLabel.setJustificationType(juce::Justification::centred);

    //set colour scheme for sliders 
    getLookAndFeel().setColour(juce::Slider::thumbColourId, juce::Colours::mediumspringgreen); //dial
    getLookAndFeel().setColour(juce::Slider::trackColourId, juce::Colours::lightslategrey); //body
    getLookAndFeel().setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::lightslategrey); //body

    //add waveform to each GUI
    addAndMakeVisible(waveformDisplay);

    //add list of songs to be played next
    upNext.getHeader().addColumn("Up Next", 1, 100);
    upNext.setModel(this);
    addAndMakeVisible(upNext);

    //start tread calling 10 times per second (once every 0.1 sec) 
    startTimer(100);
}
    
DeckGUI::~DeckGUI() 
{
    //stop timer when destroying class
    stopTimer();
}

//==============================================================================
void DeckGUI::paint(Graphics & g)
{

}

void DeckGUI::resized()
{
    double rowH = getHeight() / 6;
    double colW = getWidth() / 4;

    /*  _________________________________________________
        |Waveform                                       |
        |                                               |
        _________________________________________________
        |Pos Slider                                     |
        _________________________________________________
        |Vol Slider |SpeedSlider       |Up Next List    |
        |           |                  |                |
        |           _____________________________________
        |           |Play       |Stop       |Load/Next  |
        _________________________________________________
    */

    waveformDisplay.setBounds(0, 0, getWidth(), rowH * 2);

    posSlider.setBounds(0, rowH * 2, getWidth(), rowH);

    volSlider.setBounds(0, rowH * 3 +20, colW, rowH*3 -30);    
    speedSlider.setBounds(colW, rowH * 3 +20, colW*1.5, rowH*2 - 30);
    upNext.setBounds(colW * 2.5, rowH * 3, colW * 1.5 - 20, rowH * 2);

    playButton.setBounds(colW+10, rowH * 5 + 10, colW-20, rowH-20);
    stopButton.setBounds(colW*2+10, rowH * 5 + 10, colW-20, rowH-20);
    nextButton.setBounds(colW * 3 + 10, rowH * 5 + 10, colW - 20, rowH - 20);

}

//==============================================================================
void DeckGUI::buttonClicked(Button* button)
{
    if (button == &playButton)
    {
        player->start(); //need to press load (nextbutton) first to work
    }
    if (button == &stopButton)
    {
        player->stop();
    }
    if (button == &nextButton)
    {   
        //handling next button for left channel
        if (channel == 0 && playlistComponent->playListL.size() > 0) //handle only if there are songs added 
        {
            //get URL to first song of Left playlist
            URL fileURL = URL{ File{playlistComponent->playListL[0]} }; 
            //load the first URL 
            player->loadURL(fileURL);
            //display the waveforms
            waveformDisplay.loadURL(fileURL);
            //pop the first URL of the Left playlist so it doesn't replay
            playlistComponent->playListL.erase(playlistComponent->playListL.begin()); 
        }

        //handling next button for right channel
        if (channel == 1 && playlistComponent->playListR.size() > 0)
        {
            //get URL to first song of playlist
            URL fileURL = URL{ File{playlistComponent->playListR[0]} }; 
            //load the first URL 
            player->loadURL(fileURL);
            //display the waveforms
            waveformDisplay.loadURL(fileURL);
            //pop the first URL of the Right playlist so it doesn't replay
            playlistComponent->playListR.erase(playlistComponent->playListR.begin()); //remove first element
        }

        //Buttons starts with indicating load. Once first songs have been loaded, we can change it to next 
        if (nextButton.getButtonText() == "LOAD")
        {
            nextButton.setButtonText("NEXT");
        }
        else
        {
            player->start(); //starts player each time button labeled next is clicks
        }
    }

    //refresh up next table whenever a button is clicked
    upNext.updateContent();
}

void DeckGUI::sliderValueChanged(Slider* slider)
{
    if (slider == &volSlider)
    {
        player->setGain(slider->getValue());
    }

    if (slider == &speedSlider)
    {
        player->setSpeed(slider->getValue());
    }
    if (slider == &posSlider)
    {
        player->setRelativePosition(slider->getValue());
    }

}


//==============================================================================
int DeckGUI::getNumRows()
{
    //number of rows in the table depends on the number of songs loaded to the respective channels
    if (channel == 0) //left
    {
        return playlistComponent->playListL.size();
    }
    if (channel == 1) //right
    {
        return playlistComponent->playListR.size();
    }
}

void DeckGUI::paintRowBackground(Graphics& g,
    int rowNumber,
    int width,
    int height,
    bool rowIsSelected)
{
    // set background of each row in up next table
    g.fillAll(juce::Colours::mediumspringgreen); 
}

void DeckGUI::paintCell(Graphics& g,
    int rowNumber,
    int columnId,
    int width,
    int height,
    bool rowIsSelected)
{
    std::string filepath = "";

    // get file path depending on channel
    if (channel == 0) //left
    {
        filepath = playlistComponent->playListL[rowNumber];
    }
    if (channel == 1) //right
    {
        filepath = playlistComponent->playListR[rowNumber];
    }

    // extract file name from path 
    std::size_t startFilePos = filepath.find_last_of("\\");
    std::size_t startExtPos = filepath.find_last_of(".");
    std::string extn = filepath.substr(startExtPos + 1, filepath.length() - startExtPos);
    std::string file = filepath.substr(startFilePos + 1, filepath.length() - startFilePos - extn.size() - 2);
    //draw name to each cell
    g.drawText(file,
        1, rowNumber,
        width - 4, height,
        Justification::centredLeft,
        true);
}


void DeckGUI::timerCallback()
{
    waveformDisplay.setRelativePosition(
        player->getRelativePosition());
}
