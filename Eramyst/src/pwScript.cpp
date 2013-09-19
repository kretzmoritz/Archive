#include "..\include\pwScript.hpp"

bool pwScriptResourceDataDefinition::LoadFromMemory(const std::string& Data)
{
    std::string TmpStringBefore = Data;       // String before the ':'
    std::string TmpStringAfter = Data;
    std::string::size_type TmpPosition; // Position of certain signs

    // Find the colon
    TmpPosition = Data.find(":");
    
    // Colon found?
    if (TmpPosition == std::string::npos)
    {
        // RecursiveLoad done -> load last Parameter + return false
    }
    else
    {
        // Get the content of the data
        TmpStringBefore.erase(0, TmpPosition+1);

        // Get the type of the data
        TmpStringAfter.erase(TmpPosition, TmpStringAfter.length());

        // Add TmpStringBefore To myParametres + return LoadFromMemory(TmpStringAfter);
    }

    //// Container-type and -name were successfully extracted from the [String]
    //return TmpString; // Container-type
    return true;
}

std::string pwScriptResourceDataDefinition::at(const std::string& Parameter)
{
    if (Parameter == "source")
    {
        return myParametres.at(0);
    }
    else if (Parameter == "id")
    {
        return myParametres.at(1);
    }
    else
    {
        return "";
    }
}

////////////////////////////////////////////////////////////////
// Default constructor
////////////////////////////////////////////////////////////////
pwScriptDataExContainer::pwScriptDataExContainer()
{
    // Nothing to do
}

////////////////////////////////////////////////////////////////
// Destructor
////////////////////////////////////////////////////////////////
pwScriptDataExContainer::~pwScriptDataExContainer()
{
    // Nothing to do
}

////////////////////////////////////////////////////////////////
// Loads pwScriptDataExContainer-data from memory
////////////////////////////////////////////////////////////////
bool pwScriptDataExContainer::LoadFromMemory(const std::string& Data)
{
    std::istringstream ScriptStream;                          // The data stream
    std::string Buffer;                                       // Buffer for reading
    std::string DataBuffer;                                   // Buffer for reading data

    // Fills the [ScriptStream] with the [Data]
    ScriptStream.str(Data);

    // ScriptStream ok?
    if (!ScriptStream.good())
    {
        // Script was not opened successfully
        pwLog::get()->WriteLine("pwScriptDataExContainer::LoadFromMemory: Script could not be read from memory");
        return false;
    }

    // Read, until the end of stream is reached
    while (!ScriptStream.eof())
    {
        // Read the next line
        std::getline(ScriptStream, Buffer);

        // Was there a data definition in this line?
        if ((DataBuffer = GetDataDefinition(&Buffer)) != "")
        {
            // Class definition found
            std::pair<std::string, std::string> TmpPair = std::make_pair(DataBuffer, Buffer); // Link the data to the data-type
            myData.push_back(TmpPair);                                                        // Insert the data into the list
        }
    }

    // Script-data was read succesfully
    return true;
}

////////////////////////////////////////////////////////////////
// Gets the data definition-input [in a DataEx Container] from a string: e.g. "image" and "default.png" from "image:default.png"
////////////////////////////////////////////////////////////////
std::string pwScriptDataExContainer::GetDataDefinition(std::string *String)
{
    // Erase white-spaces from string
    if (!pwScript::EraseWhiteSpaces(String))
        return "";

    // Erase comments from string
    if (!pwScript::EraseComments(String))
        return "";

    // Is string empty?
    if (String->empty())
        return "";

    std::string TmpString = *String;     // String before the ':'
    std::string::size_type TmpPosition;  // Position of certain signs

    // Find the colon
    TmpPosition = String->find(":");
    
    // Colon found?
    if (TmpPosition == std::string::npos)
        return "";
    else
    {
        // Get the content of the data
        String->erase(0, TmpPosition+1);

        // Get the type of the data
        TmpString.erase(TmpPosition, TmpString.length());
    }

    // Container-type and -name were successfully extracted from the [String]
    return TmpString; // Container-type
}

////////////////////////////////////////////////////////////////
// Default constructor
////////////////////////////////////////////////////////////////
pwScriptClass::pwScriptClass()
{
    // Nothing to do
}

////////////////////////////////////////////////////////////////
// Destructor
////////////////////////////////////////////////////////////////
pwScriptClass::~pwScriptClass()
{
    // Nothing to do
}

////////////////////////////////////////////////////////////////
// Loads pwScriptClass-data from memory
////////////////////////////////////////////////////////////////
bool pwScriptClass::LoadFromMemory(const std::string& Data)
{
    std::istringstream ScriptStream;               // The data stream
    std::string Buffer;                            // Buffer for reading
    std::string DataBuffer;                        // Buffer for reading data
    std::string ContainerType;
    pwScriptContainer* ActiveContainer = NULL; //Active container

    // Fills the [ScriptStream] with the [Data]
    ScriptStream.str(Data);

    // ScriptStream ok?
    if (!ScriptStream.good())
    {
        // Script was not opened successfully
        pwLog::get()->WriteLine("pwScriptClass::LoadFromMemory: Script could not be read from memory");
        return false;
    }

    // Read, until the end of stream is reached
    while (!ScriptStream.eof())
    {
        // Read the next line
        std::getline(ScriptStream, Buffer);

        // Was there a container definition in this line?
        if ((ContainerType = GetContainer(&Buffer)) != "")
        {
            // Load all data for the old container
            if (ActiveContainer != NULL)
            {
                ActiveContainer->LoadFromMemory(DataBuffer);
            }
            
            // Container definition found
            pwScriptContainer* TmpScriptContainer;
            
            if (ContainerType == "fn")
            {
                // Create new function container
                TmpScriptContainer = new pwScriptContainer;
            } else if (ContainerType == "list") {
                TmpScriptContainer = new pwScriptDataExContainer;
            } else {
                TmpScriptContainer = new pwScriptContainer;
            }
            
            pwScriptContainerType TmpCType;
            TmpCType.Container  = TmpScriptContainer;
            TmpCType.Name       = Buffer;
            TmpCType.Type       = ContainerType;

            myScriptContainer.push_back(TmpCType); // Insert the class in the map and get its position
            ActiveContainer = TmpScriptContainer;
            DataBuffer = "";
        }
        // No container definition -> container content
        else
        {
            DataBuffer += Buffer + "\n";
        }
    }

    // Load all data for the old container
    if (ActiveContainer != NULL)
    {
        ActiveContainer->LoadFromMemory(DataBuffer);
    }

    // Script-data was read succesfully
    return true;
}

const pwScriptContainerType* pwScriptClass::Container(std::string& ContainerName) const
{
    std::list<pwScriptContainerType>::const_iterator ActiveContainer; // Active container

    // Does the container exist?
    for (ActiveContainer = myScriptContainer.begin(); ActiveContainer != myScriptContainer.end(); ActiveContainer++)
    {
        if (ActiveContainer->Name == ContainerName)
        {
            return  &(*ActiveContainer);
        }
    }

    // Class could not be found
    pwLog::get()->WriteLine("pwScriptClass::Container: Container \'" + ContainerName + "\' could not be found");
    return NULL;
}
const pwScriptDataExContainer* pwScriptClass::List(std::string& ContainerName) const
{
    const pwScriptContainerType* TmpContainer = Container(ContainerName);
    if (TmpContainer != NULL && TmpContainer->Type == "list")
    {
        return (pwScriptDataExContainer*) TmpContainer->Container;
    }

    return NULL;
}

////////////////////////////////////////////////////////////////
// Gets the container definition from a string: e.g. "default" from "[list:default]"
////////////////////////////////////////////////////////////////
std::string pwScriptClass::GetContainer(std::string *String)
{
    // Erase white-spaces from string
    if (!pwScript::EraseWhiteSpaces(String))
        return "";

    // Erase comments from string
    if (!pwScript::EraseComments(String))
        return "";

    // Is string empty?
    if (String->empty())
        return "";

    // Beginning of a container definition found?
    if (String->at(0) == '[')
         // Erase the open-bracket in the class definition
        String->erase(String->begin());
    else
        return "";

    // Beginning of a container definition found?
    if (String->at(String->length()-1) == ']')
         // Erase the close-bracket in the class definition
         String->pop_back();
    else
        return "";

    std::string TmpString = *String;     // String before the ':'
    std::string::size_type TmpPosition;  // Position of certain signs

    // Find the colon
    TmpPosition = String->find(":");
    
    // Colon found?
    if (TmpPosition == std::string::npos)
        return "";
    else
    {
        // Get the name of the container
        String->erase(0, TmpPosition+1);

        // Get the type of the container
        TmpString.erase(TmpPosition, TmpString.length());
    }

    // Container-type and -name were successfully extracted from the [String]
    return TmpString; // Container-type
}

////////////////////////////////////////////////////////////////
// Default constructor
////////////////////////////////////////////////////////////////
pwScript::pwScript()
{
    // Nothing to do
}

////////////////////////////////////////////////////////////////
// Destructor
////////////////////////////////////////////////////////////////
pwScript::~pwScript()
{
    // Nothing to do
}

////////////////////////////////////////////////////////////////
// Loads pwScript-data from a file
////////////////////////////////////////////////////////////////
bool pwScript::LoadFromFile(const std::string& FileName)
{
    std::ifstream TmpFile;   // The file
    std::string Buffer;
    std::string Data;
	
    // Open the file
    TmpFile.open(FileName);

    // Test, if file was opened
    if (!TmpFile.good())
    {
        // Script-file was not opened successfully
        pwLog::get()->WriteLine("pwScript::LoadFromFile: Script-file \'" + FileName + "\' could not be opened");
        return false;
    }

    while (!TmpFile.eof())
    {
        // Read the next line
        std::getline(TmpFile, Buffer);
        Data += Buffer + "\n";
    }

    TmpFile.close();

    return LoadFromMemory(Data);
}

////////////////////////////////////////////////////////////////
// Loads pwScript-data from memory
////////////////////////////////////////////////////////////////
bool pwScript::LoadFromMemory(const std::string& Data)
{
    std::istringstream ScriptStream;                          // The data stream
    std::string Buffer;                                       // Buffer for reading
    std::string DataBuffer;                                   // Buffer for reading data
    std::map<std::string, pwScriptClass*>::iterator ActiveClass; // Active class

    // Fills the [ScriptStream] with the [Data]
    ScriptStream.str(Data);

    // ScriptStream ok?
    if (!ScriptStream.good())
    {
        // Script was not opened successfully
        pwLog::get()->WriteLine("pwScript::LoadFromMemory: Script could not be read from memory");
        return false;
    }

    // The default class is always existent
    pwScriptClass* TmpScriptClass = new pwScriptClass;                                          // Create new class
    std::pair<std::string, pwScriptClass*> TmpPair = std::make_pair("default", TmpScriptClass); // Make new class the "default"-class 
    ActiveClass = myScriptClass.insert(TmpPair).first;                                          // Insert the defualt-class in the map

    // Read, until the end of stream is reached
    while (!ScriptStream.eof())
    {
        // Read the next line
        std::getline(ScriptStream, Buffer);

        // Was there a class definition in this line?
        if (GetClass(&Buffer))
        {
            // Load all data for the old class
            ActiveClass->second->LoadFromMemory(DataBuffer);

            // Class definition found
            pwScriptClass* TmpScriptClass = new pwScriptClass;                                          // Create new class
            std::pair<std::string, pwScriptClass*> TmpPair = std::make_pair(Buffer, TmpScriptClass);    // Link the class-name to the class
            ActiveClass = myScriptClass.insert(TmpPair).first;                                          // Insert the class in the map and get its position
            DataBuffer = "";
        }
        // No class definition -> class content
        else
        {
            DataBuffer += Buffer + "\n";
        }
    }

    ActiveClass->second->LoadFromMemory(DataBuffer);

    // Script-data was read succesfully
    return true;
}

const pwScriptClass* pwScript::Class(std::string& ClassName) const
{
    std::map<std::string, pwScriptClass*>::const_iterator ActiveClass; // Active class

    // Does the class exist?
    ActiveClass = myScriptClass.find(ClassName);

    if (ActiveClass == myScriptClass.end())
    {
        // Class could not be found
        pwLog::get()->WriteLine("pwScript::Class: Class \'" + ClassName + "\' could not be found");
        return NULL;
    }

    // Reading the data was successful
    return ActiveClass->second;
}

////////////////////////////////////////////////////////////////
// Gets the class name from a string: e.g. "default" from ".default"
////////////////////////////////////////////////////////////////
bool pwScript::GetClass(std::string *String)
{
    // Erase white-spaces from string
    if (!EraseWhiteSpaces(String))
        return false;

    // Erase comments from string
    if (!EraseComments(String))
        return false;

    // Is string empty?
    if (String->empty())
        return false;

    // Beginning of a class definition found?
    if (String->at(0) == '.')
         // Erase the dot in the class definition
        String->erase(String->begin());
    else
        return false;

    // [String] was successfully altered and now only contains the class-name 
    return true;
}

////////////////////////////////////////////////////////////////
// Erases white-spaces from a given string
////////////////////////////////////////////////////////////////
bool pwScript::EraseWhiteSpaces(std::string *String)
{
    while(true)
    {
        // Is string empty?
        if (String->empty())
            return false;
        
        // Find a white-space (' ', '\t') from the beginning of the string
        if (String->at(0) == ' ' || String->at(0) == '\t')
            // Erase the white-space
            String->erase(String->begin());
        else
            break;
    }
    while(true)
    {
        // Is string empty?
        if (String->empty())
            return false;
        
        // Find a white-space (' ', '\t') from the end of the string
        if (String->at(String->length()-1) == ' ' || String->at(String->length()-1) == '\t')
            // Erase the white-space
            String->pop_back();
        else
            break;
    }

    // Erasing white-spaces was successful
    return true;
}

////////////////////////////////////////////////////////////////
// Erases comments from a given string
////////////////////////////////////////////////////////////////
bool pwScript::EraseComments(std::string *String)
{
    // Is string empty?
    if (String->empty())
        return false;

    // Position of certain signs
    std::string::size_type TmpPosition;

    // Find the start of a comment (';')
    TmpPosition = String->find(";");
    
    if (TmpPosition != std::string::npos)
        // Comment found: Erase it
        String->erase(TmpPosition, String->length());

    // Erasing comments was successful
    return true;
}