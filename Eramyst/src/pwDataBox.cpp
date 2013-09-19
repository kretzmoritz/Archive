#include "..\include\pwDataBox.hpp"
#include "..\include\pwEngineOptions.hpp"

////////////////////////////////////////////////////////////////
// Declaration of the static member variable
////////////////////////////////////////////////////////////////
pwDataBox* pwDataBox::p_inst = NULL;

////////////////////////////////////////////////////////////////
// Default constructor
////////////////////////////////////////////////////////////////
pwDataBox::pwDataBox()
{
    // Nothing to do
}

////////////////////////////////////////////////////////////////
// Destructor
////////////////////////////////////////////////////////////////
pwDataBox::~pwDataBox()
{
    // Nothing to do
}

////////////////////////////////////////////////////////////////
// Get the static instance of the engine
////////////////////////////////////////////////////////////////
pwDataBox* pwDataBox::get()
{
    // If the engine doesn't exist, create it
    if (!p_inst)
		p_inst = new pwDataBox;

    // Return the pointer to the engine
	return p_inst;
}

////////////////////////////////////////////////////////////////
// Load all objects, which are named in the scripted list [FileName]
////////////////////////////////////////////////////////////////
bool pwDataBox::LoadDataList(std::string ListName, std::string FileName, std::string ClassName)
{
    void* DataLoaded = NULL;
    pwScript LoadScript;
    const pwScriptClass* LoadClass;
    const pwScriptDataExContainer* LoadList;
    bool TmpError = false;
    bool Error = false;
    
    if (LoadScript.LoadFromFile("data\\script\\" + FileName) && 
       (LoadClass = LoadScript.Class(ClassName)) != NULL &&
       (LoadList = LoadClass->List(ListName)) != NULL)
    {
        for (std::list<std::pair<std::string, std::string>>::const_iterator i = LoadList->begin(); i != LoadList->end(); i++)
        {
            if (i->first == "texture")
            {
                TmpError = (DataLoaded = GetTexture(i->second)) == NULL;
            } 
            else if (i->first == "font")
            {
                TmpError = (DataLoaded = GetFont(i->second)) == NULL;
            } 
            else if (i->first == "sound")
            {
                TmpError = (DataLoaded = GetSoundBuffer(i->second)) == NULL;
            } 
            else if (i->first == "shader")
            {
                TmpError = (DataLoaded = GetPixelShader(i->second)) == NULL;
            } 
            else if (i->first == "music")
            {
                TmpError = (DataLoaded = GetMusic(i->second)) == NULL;
            } 
            else if (i->first == "ini")
            {
                TmpError = (DataLoaded = GetIniFile(i->second)) == NULL;
            }

            if (TmpError)
                Error = true;
        }

        return !Error;
    }

    pwLog::get()->WriteLine("pwDataBox::LoadDataList: Scripted list " + FileName + " could not be loaded");
    return false;
}

////////////////////////////////////////////////////////////////
// Delete all objects, which are named in the scripted list [FileName]
////////////////////////////////////////////////////////////////
bool pwDataBox::FreeDataList(std::string ListName, std::string FileName, std::string ClassName)
{
    pwScript LoadScript;
    const pwScriptClass* LoadClass;
    const pwScriptDataExContainer* LoadList;
    bool TmpError = false;
    bool Error = false;
    
    if (LoadScript.LoadFromFile("data\\script\\" + FileName) && 
       (LoadClass = LoadScript.Class(ClassName)) != NULL &&
       (LoadList = LoadClass->List(ListName)) != NULL)
    {
        for (std::list<std::pair<std::string, std::string>>::const_iterator i = LoadList->begin(); i != LoadList->end(); i++)
        {
            if (i->first == "texture")
            {
                TmpError = FreeTexture(i->second);
            } 
            else if (i->first == "font")
            {
                TmpError = FreeFont(i->second);
            } 
            else if (i->first == "sound")
            {
                TmpError = FreeSoundBuffer(i->second);
            } 
            else if (i->first == "shader")
            {
                TmpError = FreePixelShader(i->second);
            } 
            else if (i->first == "music")
            {
                TmpError = FreeMusic(i->second);
            } 
            else if (i->first == "ini")
            {
                TmpError = FreeIniFile(i->second);
            }

            if (TmpError)
                Error = true;
        }

        return !Error;
    }

    pwLog::get()->WriteLine("pwDataBox::FreeDataList: Scripted list " + FileName + " could not be loaded");
    return false;
}

////////////////////////////////////////////////////////////////
// Get the image linked with the given file-name
// Load it if it doesn't already exist
////////////////////////////////////////////////////////////////
sf::Texture* pwDataBox::GetTexture(std::string FileName, unsigned int AssignIDOnLoad)
{
    // Iterator to find the file-name in myImageBox
    std::map<std::string, sf::Texture*>::iterator myTextureBoxIterator;
    
    // Search for the file-name
    myTextureBoxIterator = myTextureBox.find(FileName);

    // Entry with this file-name was found?
    if (myTextureBoxIterator != myTextureBox.end())
    {
        if (AssignIDOnLoad != 0)
        {
            myTextureIDBox.insert(std::make_pair(AssignIDOnLoad, myTextureBoxIterator->second));
        }

        if (pwEngineOptions::GetScaled())
        {
            myTextureBoxIterator->second->setSmooth(true);
        }
        else
        {
            myTextureBoxIterator->second->setSmooth(false);
        }

        // Then return it
        return myTextureBoxIterator->second;
    }

    // Load the file, as it wasn't found in the box
    char* Buffer = NULL;                                // Buffer to hold the loaded data
    unsigned int FileSize = 0;                          // Size of the file to load
    std::string TmpFileName = "data\\img\\" + FileName; // Temporary file-name to get the right file-type

    // Get the data from a file
    if ((FileSize = LoadDataFromFile(&Buffer, TmpFileName)) != 0)
    {
        // Loading the file was successful
        sf::Texture* TmpTexture = new sf::Texture; // Image to add to the box

        // Get the image from the data  
        if (!TmpTexture->loadFromMemory(Buffer, FileSize))
	    {
            pwLog::get()->WriteLine("Image " + FileName + " could not be loaded from memory");
		    
            if (FileName == "default")
            {
                return NULL;
            }
            else
            {
                pwLog::get()->WriteLine("Default-image is to be loaded");
                return GetTexture("default");
            }
	    }
        
        // Make a pair and insert it to the box
        myTextureBox.insert(std::make_pair(FileName, TmpTexture));

        if (AssignIDOnLoad != 0)
        {
            myTextureIDBox.insert(std::make_pair(AssignIDOnLoad, TmpTexture));
        }

        // Delete the temporary buffer
        if (Buffer != NULL)
            delete Buffer;

        // Return the image
        pwLog::get()->WriteLine("Loading the image " + FileName + " from memory was successful", pwLogType::pwLTInfo);
        return TmpTexture;
    }
    else
    {
        if (FileName == "default")
        {
            pwLog::get()->WriteLine("Default-image could not be loaded from file");
            return NULL;
        }
        else
        {
            pwLog::get()->WriteLine("Image " + FileName + " could not be loaded from file");
            pwLog::get()->WriteLine("Default-image is to be loaded");
            return GetTexture("default");
        }
    }
}

////////////////////////////////////////////////////////////////
// Get the font linked with the given file-name
// Load it if it doesn't already exist
////////////////////////////////////////////////////////////////
sf::Font* pwDataBox::GetFont(std::string FileName, unsigned int AssignIDOnLoad)
{
    // Iterator to find the file-name in myFontBox
    std::map<std::string, pwFontData>::iterator myFontBoxIterator;
    
    // Search for the file-name
    myFontBoxIterator = myFontBox.find(FileName);

    // Entry with this file-name was found?
    if (myFontBoxIterator != myFontBox.end())
    {
        if (AssignIDOnLoad != 0)
        {
            myFontIDBox.insert(std::make_pair(AssignIDOnLoad, myFontBoxIterator->second.Font));
        }

        // Then return it
        return myFontBoxIterator->second.Font;
    }

    // Load the file, as it wasn't found in the box
    char* Buffer = NULL;                                  // Buffer to hold the loaded data
    unsigned int FileSize = 0;                            // Size of the file to load
    std::string TmpFileName = "data\\fonts\\" + FileName; // Temporary file-name to get the right file-type

    // Get the data from a file
    if ((FileSize = LoadDataFromFile(&Buffer, TmpFileName)) != 0)
    {
        // Loading the file was successful
        sf::Font* TmpFont = new sf::Font; // Font to add to the box

        // Get the font from the data
        if (!TmpFont->loadFromMemory(Buffer, FileSize))
	    {
            pwLog::get()->WriteLine("Font " + FileName + " could not be loaded from memory");
		    
            if (FileName == "default")
            {
                return NULL;
            }
            else
            {
                pwLog::get()->WriteLine("Default-font is to be loaded");
                return GetFont("default");
            }
	    }
        
        // Make a pair and insert it to the box
        std::pair<std::string, pwFontData> TmpPair;
        TmpPair.first = FileName;
        TmpPair.second.Font = TmpFont;
        TmpPair.second.Data = Buffer;
        myFontBox.insert(TmpPair);

        if (AssignIDOnLoad != 0)
        {
            myFontIDBox.insert(std::make_pair(AssignIDOnLoad, TmpFont));
        }

        // Return the font
        pwLog::get()->WriteLine("Loading the font " + FileName + " from memory was successful", pwLogType::pwLTInfo);
        return TmpFont;
    }
    else
    {
        if (FileName == "default")
        {
            pwLog::get()->WriteLine("Default-font could not be loaded from file");
            return NULL;
        }
        else
        {
            pwLog::get()->WriteLine("Font " + FileName + " could not be loaded from file");
            pwLog::get()->WriteLine("Default-font is to be loaded");
            return GetFont("default");
        }
    }
}

////////////////////////////////////////////////////////////////
// Get the SoundBuffer linked with the given file-name
// Load it if it doesn't already exist
////////////////////////////////////////////////////////////////
sf::SoundBuffer* pwDataBox::GetSoundBuffer(std::string FileName, unsigned int AssignIDOnLoad)
{
    // Iterator to find the file-name in mySoundBufferBox
    std::map<std::string, sf::SoundBuffer*>::iterator mySoundBufferBoxIterator;
    
    // Search for the file-name
    mySoundBufferBoxIterator = mySoundBufferBox.find(FileName);

    // Entry with this file-name was found?
    if (mySoundBufferBoxIterator != mySoundBufferBox.end())
    {
        if (AssignIDOnLoad != 0)
        {
            mySoundBufferIDBox.insert(std::make_pair(AssignIDOnLoad, mySoundBufferBoxIterator->second));
        }

        // Then return it
        return mySoundBufferBoxIterator->second;
    }

    // Load the file, as it wasn't found in the box
    char* Buffer = NULL;                                   // Buffer to hold the loaded data
    unsigned int FileSize = 0;                             // Size of the file to load
    std::string TmpFileName = "data\\sounds\\" + FileName; // Temporary file-name to get the right file-type

    // Get the data from a file
    if ((FileSize = LoadDataFromFile(&Buffer, TmpFileName)) != 0)
    {
        // Loading the file was successful
        sf::SoundBuffer* TmpSoundBuffer = new sf::SoundBuffer; // SoundBuffer to add to the box

        // Get the SoundBuffer from the data
        if (!TmpSoundBuffer->loadFromMemory(Buffer, FileSize))
	    {
            pwLog::get()->WriteLine("SoundBuffer " + FileName + " could not be loaded from memory");
		    
            if (FileName == "default")
            {
                return NULL;
            }
            else
            {
                pwLog::get()->WriteLine("Default-SoundBuffer is to be loaded");
                return GetSoundBuffer("default");
            }
	    }
        
        // Make a pair and insert it to the box
        mySoundBufferBox.insert(std::make_pair(FileName, TmpSoundBuffer));

        if (AssignIDOnLoad != 0)
        {
            mySoundBufferIDBox.insert(std::make_pair(AssignIDOnLoad, TmpSoundBuffer));
        }

        // Delete the temporary buffer
        if (Buffer != NULL)
            delete Buffer;

        // Return the SoundBuffer
        pwLog::get()->WriteLine("Loading the SoundBuffer " + FileName + " from memory was successful", pwLogType::pwLTInfo);
        return TmpSoundBuffer;
    }
    else
    {
        if (FileName == "default")
        {
            pwLog::get()->WriteLine("Default-SoundBuffer could not be loaded from file");
            return NULL;
        }
        else
        {
            pwLog::get()->WriteLine("SoundBuffer " + FileName + " could not be loaded from file");
            pwLog::get()->WriteLine("Default-SoundBuffer is to be loaded");
            return GetSoundBuffer("default");
        }
    }
}

////////////////////////////////////////////////////////////////
// Get the shader linked with the given file-name
// Load it if it doesn't already exist
////////////////////////////////////////////////////////////////
sf::Shader* pwDataBox::GetPixelShader(std::string FileName, unsigned int AssignIDOnLoad)
{
    // Iterator to find the file-name in myPixelShaderBox
    std::map<std::string, sf::Shader*>::iterator myPixelShaderBoxIterator;
    
    // Search for the file-name
    myPixelShaderBoxIterator = myPixelShaderBox.find(FileName);

    // Entry with this file-name was found?
    if (myPixelShaderBoxIterator != myPixelShaderBox.end())
    {
        if (AssignIDOnLoad != 0)
        {
            myPixelShaderIDBox.insert(std::make_pair(AssignIDOnLoad, myPixelShaderBoxIterator->second));
        }

        // Then return it
        return myPixelShaderBoxIterator->second;
    }

    // Load the file, as it wasn't found in the box
    std::string Buffer = "";                            // Buffer to hold the loaded data
    unsigned int FileSize = 0;                          // Size of the file to load
    std::string TmpFileName = "data\\shader\\" + FileName; // Temporary file-name to get the right file-type

    // Get the data from a file
    if (LoadTextFromFile(&Buffer, TmpFileName))
    {
        // Loading the file was successful
        sf::Shader* TmpShader = new sf::Shader; // Shader to add to the box

        // Get the shader from the data  
        if (!TmpShader->loadFromMemory(Buffer, sf::Shader::Fragment))
	    {
            pwLog::get()->WriteLine("Shader " + FileName + " could not be loaded from memory");
		    
            if (FileName == "default")
            {
                return NULL;
            }
            else
            {
                pwLog::get()->WriteLine("Default-shader is to be loaded");
                return GetPixelShader("default");
            }
	    }

        TmpShader->setParameter("texture", sf::Shader::CurrentTexture);
        
        // Make a pair and insert it to the box
        myPixelShaderBox.insert(std::make_pair(FileName, TmpShader));

        if (AssignIDOnLoad != 0)
        {
            myPixelShaderIDBox.insert(std::make_pair(AssignIDOnLoad, TmpShader));
        }

        // Return the image
        pwLog::get()->WriteLine("Loading the shader " + FileName + " from memory was successful", pwLogType::pwLTInfo);
        return TmpShader;
    }
    else
    {
        if (FileName == "default")
        {
            pwLog::get()->WriteLine("Default-shader could not be loaded from file");
            return NULL;
        }
        else
        {
            pwLog::get()->WriteLine("Shader " + FileName + " could not be loaded from file");
            pwLog::get()->WriteLine("Default-shader is to be loaded");
            return GetPixelShader("default");
        }
    }
}

////////////////////////////////////////////////////////////////
// Get the Music linked with the given file-name
// Load it if it doesn't already exist / preserve the data
////////////////////////////////////////////////////////////////
sf::Music* pwDataBox::GetMusic(std::string FileName, unsigned int AssignIDOnLoad)
{
    // Iterator to find the file-name in mySoundBufferBox
    std::map<std::string, pwMusicData>::iterator myMusicBoxIterator;
    
    // Search for the file-name
    myMusicBoxIterator = myMusicBox.find(FileName);

    // Entry with this file-name was found?
    if (myMusicBoxIterator != myMusicBox.end())
    {
        if (AssignIDOnLoad != 0)
        {
            myMusicIDBox.insert(std::make_pair(AssignIDOnLoad, myMusicBoxIterator->second.Music));
        }

        // Then return it
        return myMusicBoxIterator->second.Music;
    }

    // Load the file, as it wasn't found in the box
    char* Buffer = NULL;                                   // Buffer to hold the loaded data
    unsigned int FileSize = 0;                             // Size of the file to load
    std::string TmpFileName = "data\\music\\" + FileName;  // Temporary file-name to get the right file-type

    // Get the data from a file
    if ((FileSize = LoadDataFromFile(&Buffer, TmpFileName)) != 0)
    {
        // Loading the file was successful
        sf::Music* TmpMusic = new sf::Music; // Music to add to the box

        // Get the font from the data
        if (!TmpMusic->openFromMemory(Buffer, FileSize))
	    {
            pwLog::get()->WriteLine("Music " + FileName + " could not be opened from memory");
		    
            if (FileName == "default")
            {
                return NULL;
            }
            else
            {
                pwLog::get()->WriteLine("Default-music is to be loaded");
                return GetMusic("default");
            }
	    }

        // Make a pair and insert it to the box
        std::pair<std::string, pwMusicData> TmpPair;
        TmpPair.first = FileName;
        TmpPair.second.Music = TmpMusic;
        TmpPair.second.Data = Buffer;
        myMusicBox.insert(TmpPair);

        if (AssignIDOnLoad != 0)
        {
            myMusicIDBox.insert(std::make_pair(AssignIDOnLoad, TmpMusic));
        }

        // Return the SoundBuffer
        pwLog::get()->WriteLine("Loading the Music " + FileName + " from memory was successful", pwLogType::pwLTInfo);
        return TmpMusic;
    }
    else if (FileName == "default")
    {
        pwLog::get()->WriteLine("Default-Music could not be loaded from file");
        return NULL;
    }
    else
    {
        pwLog::get()->WriteLine("Music " + FileName + " could not be loaded from file");
        pwLog::get()->WriteLine("Default-Music is to be loaded");
        return GetMusic("default");
    }
}

////////////////////////////////////////////////////////////////
// Get the pwIniFile linked with the given file-name
// Load it if it doesn't already exist
////////////////////////////////////////////////////////////////
pwIniFile* pwDataBox::GetIniFile(std::string FileName, unsigned int AssignIDOnLoad)
{
    // Iterator to find the file-name in myIniBox
    std::map<std::string, pwIniFile*>::iterator myIniBoxIterator;
    
    // Search for the file-name
    myIniBoxIterator = myIniBox.find(FileName);

    // Entry with this file-name was found?
    if (myIniBoxIterator != myIniBox.end())
    {
        if (AssignIDOnLoad != 0)
        {
            myIniIDBox.insert(std::make_pair(AssignIDOnLoad, myIniBoxIterator->second));
        }

        // Then return it
        return myIniBoxIterator->second;
    }

    // Load the file, as it wasn't found in the box
    char* Buffer = NULL;                                   // Buffer to hold the loaded data
    unsigned int FileSize = 0;                             // Size of the file to load
    std::string TmpFileName = "data\\ini\\" + FileName;    // Temporary file-name to get the right file-type

    // Get the data from a file
    if ((FileSize = LoadDataFromFile(&Buffer, TmpFileName)) != 0)
    {
        // Loading the file was successful
        pwIniFile* TmpIniFile = new pwIniFile; // SoundBuffer to add to the box

        // Get the pwIniFile from the data
        if (!TmpIniFile->LoadFromMemory(Buffer, FileSize))
	    {
            pwLog::get()->WriteLine("Ini-file " + FileName + " could not be loaded from memory");
		    
            if (FileName == "default")
            {
                return NULL;
            }
            else
            {
                pwLog::get()->WriteLine("Default-pwIniFile is to be loaded");
                return GetIniFile("default");
            }
	    }

        TmpIniFile->SetFileName(TmpFileName);
        
        // Make a pair and insert it to the box
        myIniBox.insert(std::make_pair(FileName, TmpIniFile));

        if (AssignIDOnLoad != 0)
        {
            myIniIDBox.insert(std::make_pair(AssignIDOnLoad, TmpIniFile));
        }

        // Delete the temporary buffer
        if (Buffer != NULL)
            delete Buffer;

        // Return the IniFile
        pwLog::get()->WriteLine("Loading the ini-file " + FileName + " from memory was successful", pwLogType::pwLTInfo);
        return TmpIniFile;
    }
    else
    {
        if (FileName == "default")
        {
            pwLog::get()->WriteLine("Default-pwIniFile could not be loaded from file");
            return NULL;
        }
        else
        {
            pwLog::get()->WriteLine("Ini-file " + FileName + " could not be loaded from file");
            pwLog::get()->WriteLine("Default-pwIniFile is to be loaded");
            return GetIniFile("default");
        }
    }
}


////////////////////////////////////////////////////////////////
// Delete the image linked with the given file-name
////////////////////////////////////////////////////////////////
bool pwDataBox::FreeTexture(std::string FileName)
{ 
    // Iterator to find the file-name in myImageBox
    std::map<std::string, sf::Texture*>::iterator myTextureBoxIterator;
    sf::Texture* TmpPointer = NULL;
    
    // Search for the file-name
    myTextureBoxIterator = myTextureBox.find(FileName);

    // Entry with this file-name was found?
    if (myTextureBoxIterator != myTextureBox.end())
    {
        // Then delete it
        if ((TmpPointer = myTextureBoxIterator->second) != NULL)
            delete(myTextureBoxIterator->second);
        myTextureBox.erase(myTextureBoxIterator);

        for (std::map<unsigned int, sf::Texture*>::iterator i = myTextureIDBox.begin(); i != myTextureIDBox.end(); i++)
        {
            if (i->second == TmpPointer)
                myTextureIDBox.erase(i);
        }

        pwLog::get()->WriteLine("Unloading the image " + FileName + " from memory was successful", pwLogType::pwLTInfo);
    }

    // Nothing can go wrong
    return true;
}

////////////////////////////////////////////////////////////////
// Delete the font linked with the given file-name
////////////////////////////////////////////////////////////////
bool pwDataBox::FreeFont(std::string FileName)
{
    // Iterator to find the file-name in myFontBox
    std::map<std::string, pwFontData>::iterator myFontBoxIterator;
    sf::Font* TmpPointer = NULL;
    
    // Search for the file-name
    myFontBoxIterator = myFontBox.find(FileName);

    // Entry with this file-name was found?
    if (myFontBoxIterator != myFontBox.end())
    {
        // Then delete it
        TmpPointer = myFontBoxIterator->second.Font;
        delete myFontBoxIterator->second.Data;
        delete myFontBoxIterator->second.Font;

        myFontBox.erase(myFontBoxIterator);
        
        for (std::map<unsigned int, sf::Font*>::iterator i = myFontIDBox.begin(); i != myFontIDBox.end(); i++)
        {
            if (i->second == TmpPointer)
                myFontIDBox.erase(i);
        }

        pwLog::get()->WriteLine("Unloading the font " + FileName + " from memory was successful", pwLogType::pwLTInfo);
    }

    // Nothing can go wrong
    return true;
}

////////////////////////////////////////////////////////////////
// Delete the SoundBuffer linked with the given file-name
////////////////////////////////////////////////////////////////
bool pwDataBox::FreeSoundBuffer(std::string FileName)
{
    // Iterator to find the file-name in mySoundBufferBox
    std::map<std::string, sf::SoundBuffer*>::iterator mySoundBufferBoxIterator;
    sf::SoundBuffer* TmpPointer = NULL;
    
    // Search for the file-name
    mySoundBufferBoxIterator = mySoundBufferBox.find(FileName);

    // Entry with this file-name was found?
    if (mySoundBufferBoxIterator != mySoundBufferBox.end())
    {
        // Then delete it
        if ((TmpPointer = mySoundBufferBoxIterator->second) != NULL)
            delete(mySoundBufferBoxIterator->second);
        mySoundBufferBox.erase(mySoundBufferBoxIterator);

        for (std::map<unsigned int, sf::SoundBuffer*>::iterator i = mySoundBufferIDBox.begin(); i != mySoundBufferIDBox.end(); i++)
        {
            if (i->second == TmpPointer)
                mySoundBufferIDBox.erase(i);
        }

        pwLog::get()->WriteLine("Unloading the SoundBuffer " + FileName + " from memory was successful", pwLogType::pwLTInfo);
    }

    // Nothing can go wrong
    return true;
}

////////////////////////////////////////////////////////////////
// Delete the shader linked with the given file-name
////////////////////////////////////////////////////////////////
bool pwDataBox::FreePixelShader(std::string FileName)
{ 
    // Iterator to find the file-name in myPixelShaderBox
    std::map<std::string, sf::Shader*>::iterator myPixelShaderBoxIterator;
    sf::Shader* TmpPointer = NULL;
    
    // Search for the file-name
    myPixelShaderBoxIterator = myPixelShaderBox.find(FileName);

    // Entry with this file-name was found?
    if (myPixelShaderBoxIterator != myPixelShaderBox.end())
    {
        // Then delete it
        if ((TmpPointer = myPixelShaderBoxIterator->second) != NULL)
            delete(myPixelShaderBoxIterator->second);
        myPixelShaderBox.erase(myPixelShaderBoxIterator);

        for (std::map<unsigned int, sf::Shader*>::iterator i = myPixelShaderIDBox.begin(); i != myPixelShaderIDBox.end(); i++)
        {
            if (i->second == TmpPointer)
                myPixelShaderIDBox.erase(i);
        }

        pwLog::get()->WriteLine("Unloading the shader " + FileName + " from memory was successful", pwLogType::pwLTInfo);
    }

    // Nothing can go wrong
    return true;
}

//////////////////////////////////////////////////////////////
// Delete the Music linked with the given file-name
//////////////////////////////////////////////////////////////
bool pwDataBox::FreeMusic(std::string FileName)
{
    // Iterator to find the file-name in mySoundBufferBox
    std::map<std::string, pwMusicData>::iterator myMusicBoxIterator;
    sf::Music* TmpPointer = NULL;
    
    // Search for the file-name
    myMusicBoxIterator = myMusicBox.find(FileName);

    // Entry with this file-name was found?
    if (myMusicBoxIterator != myMusicBox.end())
    {
        // Then delete it
        TmpPointer = myMusicBoxIterator->second.Music;
        delete myMusicBoxIterator->second.Data;
        delete myMusicBoxIterator->second.Music;
        
        myMusicBox.erase(myMusicBoxIterator);

        for (std::map<unsigned int, sf::Music*>::iterator i = myMusicIDBox.begin(); i != myMusicIDBox.end(); i++)
        {
            if (i->second == TmpPointer)
                myMusicIDBox.erase(i);
        }

        pwLog::get()->WriteLine("Unloading the Music " + FileName + " from memory was successful", pwLogType::pwLTInfo);
    }

    // Nothing can go wrong
    return true;
}

////////////////////////////////////////////////////////////////
// Delete the pwIniFile linked with the given file-name
////////////////////////////////////////////////////////////////
bool pwDataBox::FreeIniFile(std::string FileName)
{
    // Iterator to find the file-name in myIniBox
    std::map<std::string, pwIniFile*>::iterator myIniBoxIterator;
    pwIniFile* TmpPointer = NULL;
    
    // Search for the file-name
    myIniBoxIterator = myIniBox.find(FileName);

    // Entry with this file-name was found?
    if (myIniBoxIterator != myIniBox.end())
    {
        // Then delete it
        if ((TmpPointer = myIniBoxIterator->second) != NULL)
            delete(myIniBoxIterator->second);
        myIniBox.erase(myIniBoxIterator);

        for (std::map<unsigned int, pwIniFile*>::iterator i = myIniIDBox.begin(); i != myIniIDBox.end(); i++)
        {
            if (i->second == TmpPointer)
                myIniIDBox.erase(i);
        }

        pwLog::get()->WriteLine("Unloading the ini-file " + FileName + " from memory was successful", pwLogType::pwLTInfo);
    }

    // Nothing can go wrong
    return true;
}

////////////////////////////////////////////////////////////////
// Loads a file into memory
////////////////////////////////////////////////////////////////
unsigned int pwDataBox::LoadDataFromFile(char** Data, std::string FileName)
{
    std::ifstream TmpFile;   // The file
    unsigned int length = 0; // The lenght of the file
	
    // Open the file
    TmpFile.open(FileName, std::ios::binary);

    // Test, if file was opened
    if (!TmpFile.is_open())
        return length; // = 0

    // Tell the length of the file
    TmpFile.seekg (0, std::ios::end);
	length = static_cast<unsigned int>(TmpFile.tellg());

    // Create the [*Data], so it can hold the whole file
	(*Data) = new char[length];
    
    // Read the whole file into the [*Data]
    TmpFile.seekg (0, std::ios::beg);
    TmpFile.read((*Data), static_cast<std::streamsize>(length));
    TmpFile.close();

    // Return the length of the file
    pwLog::get()->WriteLine("Loading the file " + FileName + " was successful", pwLogType::pwLTInfo);
    return length;
}

bool pwDataBox::LoadTextFromFile(std::string* Data, std::string FileName)
{
    std::ifstream TmpFile;   // The file
    std::string Buffer;
    *Data = "";
	
    // Open the file
    TmpFile.open(FileName);

    // Test, if file was opened
    if (!TmpFile.good())
        return false;

    while (!TmpFile.eof())
    {
        // Read the next line
        std::getline(TmpFile, Buffer);
        *Data += Buffer + "\n";
    }

    TmpFile.close();

    pwLog::get()->WriteLine("Loading the file " + FileName + " was successful", pwLogType::pwLTInfo);
    return true;
}

std::string pwDataBox::TextureIDToFileName(unsigned int ID)
{
    // Iterator to find the file-name in myImageBox
    std::map<unsigned int, sf::Texture*>::iterator myTextureBoxIterator;
    sf::Texture* TmpPointer = NULL;
    
    // Search for the file-name
    myTextureBoxIterator = myTextureIDBox.find(ID);

    // Entry with this file-name was found?
    if (myTextureBoxIterator != myTextureIDBox.end())
    {
        // Then delete it
        if ((TmpPointer = myTextureBoxIterator->second) != NULL)
        {
            for (std::map<std::string, sf::Texture*>::iterator i = myTextureBox.begin(); i != myTextureBox.end(); i++)
            {
                if (i->second == TmpPointer)
                    return i->first;
            }
        }
    }

    return "";
}
std::string pwDataBox::FontIDToFileName(unsigned int ID)
{
    // Iterator to find the file-name in myImageBox
    std::map<unsigned int, sf::Font*>::iterator myFontBoxIterator;
    sf::Font* TmpPointer = NULL;
    
    // Search for the file-name
    myFontBoxIterator = myFontIDBox.find(ID);

    // Entry with this file-name was found?
    if (myFontBoxIterator != myFontIDBox.end())
    {
        if ((TmpPointer = myFontBoxIterator->second) != NULL)
        {
            for (std::map<std::string, pwFontData>::iterator i = myFontBox.begin(); i != myFontBox.end(); i++)
            {
                if (i->second.Font == TmpPointer)
                    return i->first;
            }
        }
    }

    return "";
}
std::string pwDataBox::SoundBufferIDToFileName(unsigned int ID)
{
    // Iterator to find the file-name in myImageBox
    std::map<unsigned int, sf::SoundBuffer*>::iterator mySoundBufferBoxIterator;
    sf::SoundBuffer* TmpPointer = NULL;
    
    // Search for the file-name
    mySoundBufferBoxIterator = mySoundBufferIDBox.find(ID);

    // Entry with this file-name was found?
    if (mySoundBufferBoxIterator != mySoundBufferIDBox.end())
    {
        if ((TmpPointer = mySoundBufferBoxIterator->second) != NULL)
        {
            for (std::map<std::string, sf::SoundBuffer*>::iterator i = mySoundBufferBox.begin(); i != mySoundBufferBox.end(); i++)
            {
                if (i->second == TmpPointer)
                    return i->first;
            }
        }
    }

    return "";
}
std::string pwDataBox::PixelShaderIDToFileName(unsigned int ID)
{
    // Iterator to find the file-name in myImageBox
    std::map<unsigned int, sf::Shader*>::iterator myPixelShaderBoxIterator;
    sf::Shader* TmpPointer = NULL;
    
    // Search for the file-name
    myPixelShaderBoxIterator = myPixelShaderIDBox.find(ID);

    // Entry with this file-name was found?
    if (myPixelShaderBoxIterator != myPixelShaderIDBox.end())
    {
        if ((TmpPointer = myPixelShaderBoxIterator->second) != NULL)
        {
            for (std::map<std::string, sf::Shader*>::iterator i = myPixelShaderBox.begin(); i != myPixelShaderBox.end(); i++)
            {
                if (i->second == TmpPointer)
                    return i->first;
            }
        }
    }

    return "";
}
std::string pwDataBox::IniFileIDToFileName(unsigned int ID)
{
    // Iterator to find the file-name in myImageBox
    std::map<unsigned int, pwIniFile*>::iterator myIniFileBoxIterator;
    pwIniFile* TmpPointer = NULL;
    
    // Search for the file-name
    myIniFileBoxIterator = myIniIDBox.find(ID);

    // Entry with this file-name was found?
    if (myIniFileBoxIterator != myIniIDBox.end())
    {
        if ((TmpPointer = myIniFileBoxIterator->second) != NULL)
        {
            for (std::map<std::string, pwIniFile*>::iterator i = myIniBox.begin(); i != myIniBox.end(); i++)
            {
                if (i->second == TmpPointer)
                    return i->first;
            }
        }
    }

    return "";
}
std::string pwDataBox::MusicIDToFileName(unsigned int ID)
{
    // Iterator to find the file-name in myImageBox
    std::map<unsigned int, sf::Music*>::iterator myMusicBoxIterator;
    sf::Music* TmpPointer = NULL;
    
    // Search for the file-name
    myMusicBoxIterator = myMusicIDBox.find(ID);

    // Entry with this file-name was found?
    if (myMusicBoxIterator != myMusicIDBox.end())
    {
        if ((TmpPointer = myMusicBoxIterator->second) != NULL)
        {
            for (std::map<std::string, pwMusicData>::iterator i = myMusicBox.begin(); i != myMusicBox.end(); i++)
            {
                if (i->second.Music == TmpPointer)
                    return i->first;
            }
        }
    }

    return "";
}
