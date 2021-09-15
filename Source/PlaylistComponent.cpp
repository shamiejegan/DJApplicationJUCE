/*
  ==============================================================================

    PlaylistComponent.cpp
    Author:  Shamie

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"

//==============================================================================
PlaylistComponent::PlaylistComponent(AudioFormatManager& _formatManager)
    : formatManager(_formatManager)
{
    //set up playlist library table 
    tableComponent.getHeader().addColumn("Track Title",1, 250);
    tableComponent.getHeader().addColumn("Duration", 2, 100);
    tableComponent.getHeader().addColumn("Add to L", 3, 100);
    tableComponent.getHeader().addColumn("Add to R", 4, 100);
    tableComponent.setModel(this);
    addAndMakeVisible(tableComponent);

    //add search bar and listener
    addAndMakeVisible(searchBar);
    searchBar.addListener(this);
    //add label for search bar
    addAndMakeVisible(searchLabel);
    searchLabel.setText("Find Track: ", juce::dontSendNotification);

}

PlaylistComponent::~PlaylistComponent()
{
}


//==============================================================================
void PlaylistComponent::paint (juce::Graphics& g)
{
}

void PlaylistComponent::resized()
{
    double rowH = getHeight() / 8;
    double colW = getWidth() / 6;

    //set position of search functionality
    searchLabel.setBounds(0, 0, colW, rowH);
    searchBar.setBounds(colW, 0, colW * 5, rowH);
    //set position of table
    tableComponent.setBounds(0, rowH, getWidth(), rowH*7);

}


//==============================================================================
int PlaylistComponent::getNumRows()
{
    return interestedTitle.size(); // length of filtered vector
}

void PlaylistComponent::paintRowBackground(Graphics& g,
    int rowNumber,
    int width,
    int height,
    bool rowIsSelected)
{
    if (rowIsSelected)
    {
        g.fillAll(juce::Colours::orange); // colour background of row orange if selected
    }
    else {
        g.fillAll(juce::Colours::grey); //unselected rows will be of colour grey
    }
}

void PlaylistComponent::paintCell(Graphics& g,
    int rowNumber,
    int columnId,
    int width,
    int height,
    bool rowIsSelected)
{
    // Draw Track Title Name to first column
    if (columnId == 1)
    {
        g.drawText(interestedTitle[rowNumber],
            1, rowNumber,
            width - 4, height,
            Justification::centredLeft,
            true);
    }
    // Draw duration in seconds to second column
    if (columnId == 2)
    {
        g.drawText(std::to_string(interestedDuration[rowNumber]) + "s",
            1, rowNumber,
            width - 4, height,
            Justification::centredLeft,
            true);
    }
}

Component* PlaylistComponent::refreshComponentForCell(int rowNumber,
    int columnId,
    bool isRowSelected,
    Component* existingComponentToUpdate)
{
    // Create buttons for each line in the 3rd column to add to the Left channel deck
    if (columnId == 3)
    {
        if (existingComponentToUpdate == nullptr)
        {
            TextButton* btn = new TextButton{ "Add to L" };
            String id{ std::to_string(rowNumber) };
            btn->setComponentID(id);
            btn->addListener(this);
            existingComponentToUpdate = btn;
            btn->setColour(TextButton::buttonColourId, juce::Colours::darkslategrey);
        }
    }
    // Create buttons for each line in the 3rd column to add to the Right channel deck
    if (columnId == 4)
    {
        if (existingComponentToUpdate == nullptr)
        {
            TextButton* btn = new TextButton{ "Add to R" };
            // add 1000 to id to allow buttons to be processed differently between channels
            String id{ std::to_string(rowNumber + 1000) }; 
            btn->setComponentID(id);
            btn->addListener(this);
            existingComponentToUpdate = btn;
            btn->setColour(TextButton::buttonColourId, juce::Colours::darkslategrey);
        }
    }
    return existingComponentToUpdate;
}


//==============================================================================
//AudioSource pure virtual functions
void PlaylistComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate){}
void PlaylistComponent::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill){}
void PlaylistComponent::releaseResources(){}


//==============================================================================
void PlaylistComponent::buttonClicked(Button* button)
{
    //get id of button 
    int id = std::stoi(button->getComponentID().toStdString()); 
    //if id is less than 1000, it should be allocated to the left channel GUI player. 
    if (id < 1000)
    {
        addToChannelList(interestedFiles[id], 0);
    }
    //if id is 1000 of more, it should be allocated to the right chanel GUI player
    else 
    {
        addToChannelList(interestedFiles[id - 1000], 1);
    }
}


//==============================================================================
bool PlaylistComponent::isInterestedInFileDrag(const StringArray& files)
{
    return true; // allows files to be dragged and dropped 
}

void PlaylistComponent::filesDropped(const StringArray& files, int x, int y)
{
    //perform if files have been dropped (mouse released with files) 
    for (String filename : files)
    {
        //for each file URL, get filepath and file name 
        std::string filepath = String(filename).toStdString();
        std::size_t startFilePos = filepath.find_last_of("\\");
        std::size_t startExtPos = filepath.find_last_of(".");
        std::string extn = filepath.substr(startExtPos + 1, filepath.length() - startExtPos);
        std::string file = filepath.substr(startFilePos + 1, filepath.length() - startFilePos - extn.size() - 2);

        //update vectors for file details
        inputFiles.push_back(filepath);
        trackTitles.push_back(file);

        //compute adudio length of the file and update vectors for file details 
        getAudioLength(URL{ File{filepath} });

    }
    //Initialise interested titles as the full list. 
    //This will be updated when text is entered in the search bar
    interestedTitle = trackTitles;
    interestedFiles = inputFiles;

    //update the music library table to include added files 
    tableComponent.updateContent();
}


//==============================================================================
void PlaylistComponent::textEditorTextChanged(TextEditor& textEditor)
{
    //whenever the search box is modified, clear the vectors that will be used for the table 
    interestedTitle.clear(); 
    interestedDuration.clear();
    interestedFiles.clear();

    // start at position 0 of the original library list and increment until the last element of the list
    int pos = 0;
    for (std::string track : trackTitles)
    {   
        //check if the texts typed in the search box is a substring of the track title we are currently processing
        if (track.find(searchBar.getText().toStdString()) != std::string::npos)
        {
            //if there is a match, push the item to the vector used for displaying values in the table
            interestedTitle.push_back(trackTitles[pos]);
            interestedDuration.push_back(trackDurations[pos]);
            interestedFiles.push_back(inputFiles[pos]);
        }
        ++pos; 
    }
    //update the contents of the table after looping
    tableComponent.updateContent();
}


//==============================================================================
// Add music file to list of the respective Left/Right channel's playlist
void PlaylistComponent::addToChannelList(std::string filepath, int channel)
{
    if (channel == 0) //left
    {
        playListL.push_back(filepath);
    }
    if (channel == 1) //right
    {
        playListR.push_back(filepath);
    }
}

// get audio length metadata
void PlaylistComponent::getAudioLength(URL audioURL)
{
    double trackLen = 0.0;

    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));

    if (reader != nullptr) // good file!
    {
        std::unique_ptr<AudioFormatReaderSource> newSource(new AudioFormatReaderSource(reader,
            true));
        transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource.reset(newSource.release());
        double trackLen = transportSource.getLengthInSeconds(); // get length of audio
        trackDurations.push_back(trackLen); // add audio length to vector
    }

    //Initialise interested durations as the full list. 
    //This will be updated when text is entered in the search bar
    interestedDuration = trackDurations;
}