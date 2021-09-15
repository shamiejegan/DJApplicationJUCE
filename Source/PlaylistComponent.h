/*
  ==============================================================================

    PlaylistComponent.h
    Author:  Shamie

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <vector>
#include <string>

//===============================================================================
/*
    This component controls the playlist, where files are uploaded from OS to app 
*/

class PlaylistComponent : public juce::Component,
    public TableListBoxModel,
    public AudioSource,
    public Button::Listener,
    public FileDragAndDropTarget,
    public TextEditor::Listener
{
public:

    //==============================================================================
    PlaylistComponent(AudioFormatManager& formatManager);
    ~PlaylistComponent() override;


    //==============================================================================
    /**Customise input graphics*/
    void paint (juce::Graphics&) override;
    /**Rescaling of components on the application window*/
    void resized() override;


    //==============================================================================
    /**Override of TableListBoxModel pure virtual. 
    Returns the number of rows currently in the table*/
    int getNumRows() override;
    /**Override of TableListBoxModel pure virtual. 
    Draws the background behind one of the rows in the table*/
    void paintRowBackground(Graphics&,
        int rowNumber,
        int width,
        int height,
        bool rowIsSelected) override; 
    /**Override of TableListBoxModel pure virtual. 
    Draws one of the cells, used to input details in cells*/
    void paintCell(Graphics &, 
        int rowNumber,
        int columnId,
        int width,
        int height,
        bool rowIsSelected) override;
    /**Override of TableListBoxModel function. 
    To create & update custom components such as buttons to go in a cell*/
    Component* refreshComponentForCell(int rowNumber,
        int columnId,
        bool isRowSelected,
        Component* existingComponentToUpdate) override;


    //==============================================================================
    /**Placeholder function override for Audio Source component to tell the source to prepare for playing*/
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    /**Placeholder function override for Audio Source component to fetch subsequent blocks of audio data*/
    void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill);
    /**Placeholder function override for Audio Source component to release anything it no longer needs*/
    void releaseResources();


    //==============================================================================
    /**Override of Button::Listener pure virtual. 
    Called when the button is clicked, allowing users to load files to deck*/
    void buttonClicked(Button* button) override;


    //==============================================================================
    /**Override of FileDragAndDropTarget pure virtual. 
    Callback to check whether this target is interested in the set of files being offered. 
    If set to true, allows files to be dragged and dropped into the area*/
    bool isInterestedInFileDrag(const StringArray& files) override;
    /**Override of FileDragAndDropTarget pure virtual.
    Callback to indicate that the user has dropped the files onto this component. 
    Processes file once it has been dropped (mouse released)*/
    void filesDropped(const StringArray& files, int x, int y) override;


    //==============================================================================
    /**Override of TextEditor::Listener function to be called whenever the user changes 
    the text in the object in some way*/
    void textEditorTextChanged(TextEditor&) override;


    //==============================================================================
    /**Vector of songs to be added to the Left Channel Player, utilised by DeckGUI*/
    std::vector<std::string> playListL;
    /**Vector of songs to be added to the Right Channel Player, utilised by DeckGUI*/
    std::vector<std::string> playListR;


private:

    AudioFormatManager& formatManager;
    std::unique_ptr<AudioFormatReaderSource> readerSource;
    AudioTransportSource transportSource;

    //Playlist displayed as a table list
    TableListBox tableComponent; 

    //vectors to store music file metadata
    std::vector<std::string> inputFiles;
    std::vector<std::string> interestedFiles;
    std::vector<std::string> trackTitles; 
    std::vector<std::string> interestedTitle;
    std::vector<int> trackDurations;
    std::vector<int> interestedDuration;

    // Search bar and label to allow for searching functionality 
    TextEditor searchBar;
    Label searchLabel;

    //==============================================================================
    //user defined variables to process data
    void addToChannelList(std::string filepath, int channel);
    void getAudioLength(URL audioURL);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};

