#ifndef PWDATABOX
#define PWDATABOX

#ifndef PWSYSTEM
#ifdef PWUSEGLOBALSINGLETONS
#define glDataBox pwDataBox::get()
#endif
#endif

#include <fstream>
#include <string>
#include <map>
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include "pwIniFile.hpp"
#include "pwScript.hpp"

struct pwMusicData
{
    sf::Music* Music;
    void* Data;
};

struct pwFontData
{
    sf::Font* Font;
    void* Data;
};

class pwDataBox
{

public:

    ////////////////////////////////////////////////////////////////
    // Default constructor
    ////////////////////////////////////////////////////////////////
    pwDataBox();

    ////////////////////////////////////////////////////////////////
    // Destructor
    ////////////////////////////////////////////////////////////////
    ~pwDataBox();

    ////////////////////////////////////////////////////////////////
    // Get the static instance of the data box
    //
    // \return Static instance of the data box
    ////////////////////////////////////////////////////////////////
    static pwDataBox* get();

    ////////////////////////////////////////////////////////////////
    // Load all objects, which are named in the scripted list [FileName]
    //
    // \param FileName: File-name of the scripted list, containing all the object-names, which are to be loaded
    //
    // \return True, if loading [of all!] objects was successful, false otherwise
    ////////////////////////////////////////////////////////////////
    bool LoadDataList(std::string ListName, std::string FileName, std::string ClassName = "default");

    ////////////////////////////////////////////////////////////////
    // Delete all objects, which are named in the scripted list [FileName]
    //
    // \param FileName: File-name of the scripted list, containing all the object-names, which are to be deleted
    //
    // \return True, if deletion was successful, false otherwise
    ////////////////////////////////////////////////////////////////
    bool FreeDataList(std::string ListName, std::string FileName, std::string ClassName = "default");

    ////////////////////////////////////////////////////////////////
    // Get the objects linked with the given file-name
    // Load it if it doesn't already exist
    //
    // \param FileName: File-name of the object to get
    //
    // \return Object to get if existent, default object otherwise
    ////////////////////////////////////////////////////////////////

    //pwAnimationTexture GetAnimationTexture(std::string FileName);
    sf::Texture*     GetTexture     (std::string FileName, unsigned int AssignIDOnLoad = 0);
    sf::Font*        GetFont        (std::string FileName, unsigned int AssignIDOnLoad = 0);
    sf::SoundBuffer* GetSoundBuffer (std::string FileName, unsigned int AssignIDOnLoad = 0);
    sf::Shader*      GetPixelShader (std::string FileName, unsigned int AssignIDOnLoad = 0);
    sf::Music*       GetMusic       (std::string FileName, unsigned int AssignIDOnLoad = 0);
    pwIniFile*       GetIniFile     (std::string FileName, unsigned int AssignIDOnLoad = 0);
	
    sf::Texture*     GetTexture     (unsigned int FileID);
    sf::Font*        GetFont        (unsigned int FileID);
    sf::SoundBuffer* GetSoundBuffer (unsigned int FileID);
    sf::Shader*      GetPixelShader (unsigned int FileID);
    sf::Music*       GetMusic       (unsigned int FileID);
    pwIniFile*       GetIniFile     (unsigned int FileID);

    ////////////////////////////////////////////////////////////////
    // Delete the objects linked with the given file-name
    //
    // \param FileName: File-name of the object, that is to be deleted
    //
    // \return True, if deletion was successful, false otherwise
    ////////////////////////////////////////////////////////////////
    bool FreeTexture    (std::string FileName);
    bool FreeFont       (std::string FileName);
    bool FreeSoundBuffer(std::string FileName);
    bool FreePixelShader(std::string FileName);
    bool FreeIniFile    (std::string FileName);
    bool FreeMusic      (std::string FileName);
	
    bool FreeTexture    (unsigned int FileID)   {return FreeTexture     (TextureIDToFileName    (FileID));}
    bool FreeFont       (unsigned int FileID)   {return FreeFont        (FontIDToFileName       (FileID));}
    bool FreeSoundBuffer(unsigned int FileID)   {return FreeSoundBuffer (SoundBufferIDToFileName(FileID));}
    bool FreePixelShader(unsigned int FileID)   {return FreePixelShader (PixelShaderIDToFileName(FileID));}
    bool FreeIniFile    (unsigned int FileID)   {return FreeIniFile     (IniFileIDToFileName    (FileID));}
    bool FreeMusic      (unsigned int FileID)   {return FreeMusic       (MusicIDToFileName      (FileID));}


private:

    ////////////////////////////////////////////////////////////////
    // Loads a file into memory
    //
    // \param Data: Pointer to fill with the data
    // \param Filename: Name of the file to load
    //
    // \return The size of the file, if loading the file was successful, 0 otherwise
    ////////////////////////////////////////////////////////////////
    unsigned int LoadDataFromFile(char** Data, std::string FileName);
    bool LoadTextFromFile(std::string* Data, std::string FileName);

    std::string TextureIDToFileName     (unsigned int ID);
    std::string FontIDToFileName        (unsigned int ID);
    std::string SoundBufferIDToFileName (unsigned int ID);
    std::string PixelShaderIDToFileName (unsigned int ID);
    std::string IniFileIDToFileName     (unsigned int ID);
    std::string MusicIDToFileName       (unsigned int ID);

    ////////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////////
    static pwDataBox*  p_inst;

    std::map<std::string, sf::Texture*>     myTextureBox;
	std::map<std::string, pwFontData>      myFontBox;
    std::map<std::string, sf::SoundBuffer*> mySoundBufferBox;
    std::map<std::string, sf::Shader*>      myPixelShaderBox;
    std::map<std::string, pwMusicData>     myMusicBox;
    std::map<std::string, pwIniFile*>       myIniBox;

    std::map<unsigned int, sf::Texture*>     myTextureIDBox;
	std::map<unsigned int, sf::Font*>        myFontIDBox;
    std::map<unsigned int, sf::SoundBuffer*> mySoundBufferIDBox;
    std::map<unsigned int, sf::Shader*>      myPixelShaderIDBox;
    std::map<unsigned int, sf::Music*>       myMusicIDBox;
    std::map<unsigned int, pwIniFile*>       myIniIDBox;
};

#endif