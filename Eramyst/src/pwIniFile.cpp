#include "..\include\pwIniFile.hpp"

////////////////////////////////////////////////////////////////
// Default constructor
////////////////////////////////////////////////////////////////
pwIniClass::pwIniClass()
{
    // Nothing to do
}

////////////////////////////////////////////////////////////////
// Destructor
////////////////////////////////////////////////////////////////
pwIniClass::~pwIniClass()
{
    // Nothing to do
}

////////////////////////////////////////////////////////////////
// Gets a string from the class
////////////////////////////////////////////////////////////////
bool pwIniClass::GetData(std::string* Return, std::string Data) const
{
    std::map<std::string, std::string>::const_iterator ActiveData; // Active data

    // Does the data exist?
    ActiveData = myDataMap.find(Data);

    if (ActiveData == myDataMap.end())
    {
        // Data could not be found
        pwLog::get()->WriteLine("pwIniClass::GetData: Data \'" + Data + "\' could not be found");
        return false;
    }
    else
    {
        // Read the data
        *Return = ActiveData->second;
    }

    // Reading the string was successful
    return true;
}

////////////////////////////////////////////////////////////////
// Gets a float-value from the class
////////////////////////////////////////////////////////////////
bool pwIniClass::GetData(float* Return, std::string Data) const
{
    std::string Buffer;
    std::stringstream BufferStream;

    if (!GetData(&Buffer, Data))
    {
        // Data could not be read
        pwLog::get()->WriteLine("pwIniClass::GetData: Float-data \'" + Data + "\' could not be read");
        return false;
    }

    // Get float out of a string
    BufferStream << Buffer;
    BufferStream >> *Return;

    // Reading the float-value was successful
    return true;
}

////////////////////////////////////////////////////////////////
// Gets an int-value from the class
////////////////////////////////////////////////////////////////
bool pwIniClass::GetData(int* Return, std::string Data) const
{
    std::string Buffer;
    std::stringstream BufferStream;

    if (!GetData(&Buffer, Data))
    {
        // Data could not be read
        pwLog::get()->WriteLine("pwIniClass::GetData: Int-data \'" + Data + "\' could not be read");
        return false;
    }

    // Get int out of a string
    BufferStream << Buffer;
    BufferStream >> *Return;

    // Reading the int-value was successful
    return true;
}

////////////////////////////////////////////////////////////////
// Gets an unsigned int-value from the class
////////////////////////////////////////////////////////////////
bool pwIniClass::GetData(unsigned int* Return, std::string Data) const
{
    std::string Buffer;
    std::stringstream BufferStream;

    if (!GetData(&Buffer, Data))
    {
        // Data could not be read
        pwLog::get()->WriteLine("pwIniClass::GetData: Unsigned int-data \'" + Data + "\' could not be read");
        return false;
    }

    // Get int out of a string
    BufferStream << Buffer;
    BufferStream >> *Return;

    // Reading the unsigned int-value was successful
    return true;
}

////////////////////////////////////////////////////////////////
// Gets a bool-value from the class
////////////////////////////////////////////////////////////////
bool pwIniClass::GetData(bool* Return, std::string Data) const
{
    std::string Buffer;

    if (!GetData(&Buffer, Data))
    {
        // Data could not be read
        pwLog::get()->WriteLine("pwIniClass::GetData: Bool-data \'" + Data + "\' could not be read");
        return false;
    }

    // Get bool out of a string
    if (Buffer == "TRUE" || Buffer == "true" || Buffer == "1")
        *Return = true;
    else if (Buffer == "FALSE" || Buffer == "false" || Buffer == "0")
        *Return = false;
    else
    {
        // Type is not a supported BOOL-type
        return false;
    }

    // Reading the bool-value was successful
    return true;
}

////////////////////////////////////////////////////////////////
// Adds a string to the class
////////////////////////////////////////////////////////////////
bool pwIniClass::AddData(std::string Insert, std::string Data)
{
    std::pair<std::string, std::string> TmpPair;                              // Pair for inserting
    std::pair<std::map<std::string, std::string>::iterator, bool> ReturnPair; // Pair for validating the process

    // Insert the data
    TmpPair = std::make_pair(Data, Insert);
    ReturnPair = myDataMap.insert(TmpPair);
    
    // Was this data already existent?
    if (!ReturnPair.second)
    {
        // Then update its value
        ReturnPair.first->second = Insert;
    }

    // Inserting done
    return true;
}

////////////////////////////////////////////////////////////////
// Adds a float-value to the class
////////////////////////////////////////////////////////////////
bool pwIniClass::AddData(float Insert, std::string Data)
{
    std::string Buffer;
    std::stringstream BufferStream;

    // Get string out of a float
    BufferStream << Insert;
    BufferStream >> Buffer;

    if (!AddData(Buffer, Data))
    {
        // Data could not be written
        pwLog::get()->WriteLine("pwIniClass::AddData: Float-data \'" + Data + "\' could not be written");
        return false;
    }

    // Writing the float-value was successful
    return true;
}

////////////////////////////////////////////////////////////////
// Adds an int-value to the class
////////////////////////////////////////////////////////////////
bool pwIniClass::AddData(int Insert, std::string Data)
{
    std::string Buffer;
    std::stringstream BufferStream;

    // Get string out of an int
    BufferStream << Insert;
    BufferStream >> Buffer;

    if (!AddData(Buffer, Data))
    {
        // Data could not be written
        pwLog::get()->WriteLine("pwIniClass::AddData: Int-data \'" + Data + "\' could not be written");
        return false;
    }

    // Writing the int-value was successful
    return true;
}

////////////////////////////////////////////////////////////////
// Adds an unsigned int-value to the class
////////////////////////////////////////////////////////////////
bool pwIniClass::AddData(unsigned int Insert, std::string Data)
{
    std::string Buffer;
    std::stringstream BufferStream;

    // Get string out of an int
    BufferStream << Insert;
    BufferStream >> Buffer;

    if (!AddData(Buffer, Data))
    {
        // Data could not be written
        pwLog::get()->WriteLine("pwIniClass::AddData: Unsigned int-data \'" + Data + "\' could not be written");
        return false;
    }

    // Writing the unsigned int-value was successful
    return true;
}

////////////////////////////////////////////////////////////////
// Adds a bool-value to the class
////////////////////////////////////////////////////////////////
bool pwIniClass::AddData(bool Insert, std::string Data)
{
    std::string Buffer;

    // Get string out of a bool
    if (Insert)
        Buffer = "TRUE";
    else
        Buffer = "FALSE";

    if (!AddData(Buffer, Data))
    {
        // Data could not be written
        pwLog::get()->WriteLine("pwIniClass::AddData: Bool-data \'" + Data + "\' could not be written");
        return false;
    }

    // Writing the bool-value was successful
    return true;
}

const std::map<std::string, std::string> *pwIniClass::GetDataMap()
{
    return &myDataMap;
}

////////////////////////////////////////////////////////////////
// Default constructor
////////////////////////////////////////////////////////////////
pwIniFile::pwIniFile()
{
    // Nothing to do
}

////////////////////////////////////////////////////////////////
// Destructor
////////////////////////////////////////////////////////////////
pwIniFile::~pwIniFile()
{
    clear();
    // Nothing to do
}

////////////////////////////////////////////////////////////////
// Reads in an ini-file 
////////////////////////////////////////////////////////////////
bool pwIniFile::LoadFromFile(const std::string& FileName)
{
    std::ifstream IniFile;                                    // The file
    std::string Buffer;                                       // Buffer for reading
    std::pair<std::string, std::string> BufferPair;           // Buffer for reading data
    std::map<std::string, pwIniClass*>::iterator ActiveClass; // Active class

    // Open ini-file
    IniFile.open(FileName);

    // Ini-file is opened?
    if (!IniFile.good())
    {
        // Ini-file was not opened successfully
        pwLog::get()->WriteLine("pwIniFile::Load: Ini-file \'" + FileName + "\' could not be opened");
        return false;
    }

    // The default class is always existent
    pwIniClass* TmpIniClass = new pwIniClass;                                             // Create new class
    std::pair<std::string, pwIniClass*> TmpPair = std::make_pair("default", TmpIniClass); // Make new class the "default"-class 
    ActiveClass = myIniFileMap.insert(TmpPair).first;                                     // Insert the defualt-class in the map

    // Read, until the end of file is reached
    while (!IniFile.eof())
    {
        // Read the next line
        std::getline(IniFile, Buffer);

        // Was there a class definition in this line?
        if (GetClassName(&Buffer))
        {
            // Class definition found
            pwIniClass* TmpIniClass = new pwIniClass;                                          // Create new class
            std::pair<std::string, pwIniClass*> TmpPair = std::make_pair(Buffer, TmpIniClass); // Link the class-name to the class
            ActiveClass = myIniFileMap.insert(TmpPair).first;                                  // Insert the class in the map and get its position
        }
        // Was there a data definition in this line?
        else if (GetDataName(Buffer, &BufferPair))
        {
            // Data found
            ActiveClass->second->AddData(BufferPair.second, BufferPair.first); // Add data to current class
        }
    }

    // Ini-file was read succesfully
    return true;
}

////////////////////////////////////////////////////////////////
// Loads ini-data from memory
////////////////////////////////////////////////////////////////
bool pwIniFile::LoadFromMemory(const char* Data, std::size_t SizeInBytes)
{
    std::istringstream IniStream;                             // The data stream
    std::string Buffer;                                       // Buffer for reading
    std::pair<std::string, std::string> BufferPair;           // Buffer for reading data
    std::map<std::string, pwIniClass*>::iterator ActiveClass; // Active class

    // Fills the [IniStream] with the [Data]
    IniStream.str(Data);

    // Ini-file is opened?
    if (!IniStream.good())
    {
        // Ini-file was not opened successfully
        pwLog::get()->WriteLine("pwIniFile::LoadFromMemory: Ini-file could not be read from memory");
        return false;
    }

    // The default class is always existent
    pwIniClass* TmpIniClass = new pwIniClass;                                             // Create new class
    std::pair<std::string, pwIniClass*> TmpPair = std::make_pair("default", TmpIniClass); // Make new class the "default"-class 
    ActiveClass = myIniFileMap.insert(TmpPair).first;                                     // Insert the defualt-class in the map

    // Read, until the end of stream is reached
    while (!IniStream.eof())
    {
        // Read the next line
        std::getline(IniStream, Buffer);

        // Was there a class definition in this line?
        if (GetClassName(&Buffer))
        {
            // Class definition found
            pwIniClass* TmpIniClass = new pwIniClass;                                          // Create new class
            std::pair<std::string, pwIniClass*> TmpPair = std::make_pair(Buffer, TmpIniClass); // Link the class-name to the class
            ActiveClass = myIniFileMap.insert(TmpPair).first;                                  // Insert the class in the map and get its position
        }
        // Was there a data definition in this line?
        else if (GetDataName(Buffer, &BufferPair))
        {
            // Data found
            BufferPair.second.resize(BufferPair.second.length()-1);            // Delete the last sign of the data-string (\0)
            ActiveClass->second->AddData(BufferPair.second, BufferPair.first); // Add data to current class
        }
    }

    // Ini-data was read succesfully
    return true;
}

////////////////////////////////////////////////////////////////
// Writes the [propably changed] ini-data into an ini-file
////////////////////////////////////////////////////////////////
bool pwIniFile::WriteToFile(const std::string& FileName)
{
    std::ofstream IniFile;                                          // The file
    std::map<std::string, std::string>::const_iterator DataPair;    // Pair of Data
    std::map<std::string, pwIniClass*>::const_iterator ActiveClass; // Active class

    // Open ini-file
    IniFile.open(FileName);

    // Ini-file is opened?
    if (!IniFile.good())
    {
        // Ini-file was not opened successfully
        pwLog::get()->WriteLine("pwIniFile::WriteToFile: Ini-file \'" + FileName + "\' could not be opened");
        return false;
    }

    // Go through all classes in the pwIniFile
    for (ActiveClass = myIniFileMap.begin(); ActiveClass != myIniFileMap.end(); ActiveClass++)
    {
        // Write the class name
        IniFile << "[" << ActiveClass->first << "]" << std::endl;
        
        // Go through all members of the current class
        for (DataPair = ActiveClass->second->GetDataMap()->begin();DataPair != ActiveClass->second->GetDataMap()->end(); DataPair++)
        {
            // Write the data
            IniFile << DataPair->first << "=" << DataPair->second << std::endl;
        }

        // Write a new line at the end of a class
        IniFile << std::endl;        
    }

    // Ini-file was written succesfully
    return true;
}

////////////////////////////////////////////////////////////////
// Gets the class name from a string: e.g. "default" from "[default]"
////////////////////////////////////////////////////////////////
bool pwIniFile::GetClassName(std::string *String)
{
    // Is string empty?
    if (String->empty())
        return false;

    // Erase comments from string
    if (!EraseComments(String))
        return false;

    // Position of certain signs
    std::string::size_type TmpPosition;

    // Find the beginning of a class definition
    TmpPosition = String->find("[");
    
    // Beginning of a class definition found?
    if (TmpPosition == std::string::npos)
        return false;
    else
        // Erase all characters before the class definition
        String->erase(0, TmpPosition+1);

    // Find the end of a class definition
    TmpPosition = String->find("]");
    
    // End of a class definition found?
    if (TmpPosition == std::string::npos)
        return false;
    else
        // Erase all characters after the class definition
        String->erase(TmpPosition, String->length());

    // [String] was successfully altered and now only contains the class-name 
    return true;
}

////////////////////////////////////////////////////////////////
// Gets the data-information from a string: e.g. "width" and "800" from "width=800"
////////////////////////////////////////////////////////////////
bool pwIniFile::GetDataName(std::string String, std::pair<std::string, std::string> *Pair)
{
    // Is string empty?
    if (String.empty())
        return false;
    
    // Erase comments from string
    if (!EraseComments(&String))
        return false;

    std::string TmpString = String;     // String before the '='
    std::string::size_type TmpPosition; // Position of certain signs

    // Find the equals sign
    TmpPosition = String.find("=");
    
    // Equals sign found?
    if (TmpPosition == std::string::npos)
        return false;
    else
    {
        // Get the content of the data
        String.erase(0, TmpPosition+1);

        // Get the name of the data
        TmpString.erase(TmpPosition, TmpString.length());
    }

    Pair->first = TmpString; // Data-name
    Pair->second = String;   // Data-content

    // Data-name and -content were successfully extracted from the [String] 
    return true;
}

////////////////////////////////////////////////////////////////
// Erases comments from a given string
////////////////////////////////////////////////////////////////
bool pwIniFile::EraseComments(std::string *String)
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

    // Find the start of a comment ('#')
    TmpPosition = String->find("#");
    
    if (TmpPosition != std::string::npos)
        // Comment found: Erase it
        String->erase(TmpPosition, String->length());

    // Erasing comments was successful
    return true;
}

void pwIniFile::clear()
{
    for (std::map<std::string, pwIniClass*>::iterator i = myIniFileMap.begin(); i != myIniFileMap.end(); i++)
    {
        delete i->second;
    }
    myIniFileMap.clear();
}