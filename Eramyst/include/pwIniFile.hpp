#ifndef PWINIFILE
#define PWINIFILE

#include <string>
#include <sstream>
#include <fstream>
#include <map>
#include "pwLogSystem.hpp"

////////////////////////////////////////////////////////////////
// A class in an ini-file
////////////////////////////////////////////////////////////////
class pwIniClass
{

public:

    ////////////////////////////////////////////////////////////////
    // Default constructor
    ////////////////////////////////////////////////////////////////
    pwIniClass();

    ////////////////////////////////////////////////////////////////
    // Destructor
    ////////////////////////////////////////////////////////////////
    ~pwIniClass();
    
    ////////////////////////////////////////////////////////////////
    // Gets certain data from the class
    //
    // \param Return: The data, that is to be read
    // \param Data: Which data should be read
    //
    // \return True, if reading the data was successful, false otherwise
    ////////////////////////////////////////////////////////////////
    bool GetData(std::string* Return, std::string Data) const;
    bool GetData(float* Return, std::string Data) const;
    bool GetData(int* Return, std::string Data) const;
    bool GetData(unsigned int* Return, std::string Data) const;
    bool GetData(bool* Return, std::string Data) const;

    ////////////////////////////////////////////////////////////////
    // Adds certain data to the class
    //
    // \param Return: The data, that is to be added
    // \param Data: Which data should be added
    //
    // \return True, if adding the data was successful, false otherwise
    ////////////////////////////////////////////////////////////////
    bool AddData(std::string Insert, std::string Data);
    bool AddData(float Insert, std::string Data);
    bool AddData(int Insert, std::string Data);
    bool AddData(unsigned int Insert, std::string Data);
    bool AddData(bool Insert, std::string Data);

    ////////////////////////////////////////////////////////////////
    // Get the private myDataMap for reading
    //
    // \return Constant pointer to myDataMap
    ////////////////////////////////////////////////////////////////
    const std::map<std::string, std::string> *GetDataMap();


private:

    ////////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////////
    std::map<std::string, std::string> myDataMap;
};

////////////////////////////////////////////////////////////////
// Helper-class for reading and writing ini-files
////////////////////////////////////////////////////////////////
class pwIniFile
{

public:

    ////////////////////////////////////////////////////////////////
    // Default constructor
    ////////////////////////////////////////////////////////////////
    pwIniFile();

    ////////////////////////////////////////////////////////////////
    // Destructor
    ////////////////////////////////////////////////////////////////
    ~pwIniFile();

    ////////////////////////////////////////////////////////////////
    // Loads ini-data from a file
    //
    // \param FileName: Name of the ini-file to read
    //
    // \return True, if reading the file was successful, false otherwise
    ////////////////////////////////////////////////////////////////
    bool LoadFromFile(const std::string& FileName);

    ////////////////////////////////////////////////////////////////
    // Loads ini-data from memory
    //
    // \param Data: Constant pointer to the data that is to be read
    // \param SizeInBytes: The size of the [Data] in bytes
    //
    // \return True, if reading the file was successful, false otherwise
    ////////////////////////////////////////////////////////////////
    bool LoadFromMemory(const char* Data, std::size_t SizeInBytes);

    ////////////////////////////////////////////////////////////////
    // Writes the [propably changed] ini-data into an ini-file
    //
    // \param FileName: Name of the ini-file to write
    //
    // \return True, if writing the file was successful, false otherwise
    ////////////////////////////////////////////////////////////////
    bool WriteToFile(const std::string& FileName);

    ////////////////////////////////////////////////////////////////
    // Gets certain data from a certain class in an opened ini-file
    //
    // \param Data: Which data should be read
    // \param Class: From which class should the data be read (default: "default")
    //
    // \return The Data, that has been read
    ////////////////////////////////////////////////////////////////
    std::string  GetString(std::string Data, std::string Class = "default") const {std::string s;  return (GetData(&s, Data, Class) ? s : "");}
    float        GetFloat(std::string Data, std::string Class = "default") const  {float f;        return (GetData(&f, Data, Class) ? f : 0.0f);}
    int          GetInt(std::string Data, std::string Class = "default") const    {int i;          return (GetData(&i, Data, Class) ? i : 0);}
    unsigned int GetUInt(std::string Data, std::string Class = "default") const   {unsigned int u; return (GetData(&u, Data, Class) ? u : 0);}
    bool         GetBool(std::string Data, std::string Class = "default") const   {bool b;         return (GetData(&b, Data, Class) ? b : false);}


    ////////////////////////////////////////////////////////////////
    // Gets certain data from a certain class in an opened ini-file
    //
    // \param Return: The data, that is to be read
    // \param Data: Which data should be read
    // \param Class: From which class should the data be read (default: "default")
    //
    // \return True, if reading the data was successful, false otherwise
    ////////////////////////////////////////////////////////////////
    bool GetData(std::string* Return, std::string Data, std::string Class = "default")  const {return GetDataTemplate(Return, Data, Class);}
    bool GetData(float* Return, std::string Data, std::string Class = "default")        const {return GetDataTemplate(Return, Data, Class);}
    bool GetData(int* Return, std::string Data, std::string Class = "default")          const {return GetDataTemplate(Return, Data, Class);}
    bool GetData(unsigned int* Return, std::string Data, std::string Class = "default") const {return GetDataTemplate(Return, Data, Class);}
    bool GetData(bool* Return, std::string Data, std::string Class = "default")         const {return GetDataTemplate(Return, Data, Class);}

    ////////////////////////////////////////////////////////////////
    // Adds certain data to a certain class in an opened ini-file
    //
    // \param Return: The data, that is to be added
    // \param Data: Which data should be added
    // \param Class: From which class should the data be read (default: "default")
    //
    // \return True, if adding the data was successful, false otherwise
    ////////////////////////////////////////////////////////////////
    bool AddData(std::string Insert, std::string Data, std::string Class = "default")   {return AddDataTemplate(Insert, Data, Class);}
    bool AddData(float Insert, std::string Data, std::string Class = "default")         {return AddDataTemplate(Insert, Data, Class);}
    bool AddData(int Insert, std::string Data, std::string Class = "default")           {return AddDataTemplate(Insert, Data, Class);}
    bool AddData(unsigned int Insert, std::string Data, std::string Class = "default")  {return AddDataTemplate(Insert, Data, Class);}
    bool AddData(bool Insert, std::string Data, std::string Class = "default")          {return AddDataTemplate(Insert, Data, Class);}

    void SetFileName(std::string FileName)  {myFileName = FileName;}
    std::string GetFileName()               {return myFileName;}

    void clear();


private:

    ////////////////////////////////////////////////////////////////
    // Gets certain data from a certain class in an opened ini-file
    //
    // \param Return: The data, that is to be read
    // \param Data: Which data should be read
    // \param Class: From which class should the data be read (default: "default")
    //
    // \return True, if reading the data was successful, false otherwise
    ////////////////////////////////////////////////////////////////
    template<class T> bool GetDataTemplate(T* Return, std::string Data, std::string Class) const;

    ////////////////////////////////////////////////////////////////
    // Adds certain data to a certain class in an opened ini-file
    //
    // \param Return: The data, that is to be added
    // \param Data: Which data should be added
    // \param Class: From which class should the data be read (default: "default")
    //
    // \return True, if adding the data was successful, false otherwise
    ////////////////////////////////////////////////////////////////
    template<class T> bool AddDataTemplate(T Insert, std::string Data, std::string Class);

    ////////////////////////////////////////////////////////////////
    // Gets the class name from a string: e.g. "default" from "[default]"
    //
    // \param String: The class name with brackets, that is to be changed
    //
    // \return True, if there was a class in [String], and changing it was successful, false otherwise
    ////////////////////////////////////////////////////////////////
    bool GetClassName(std::string *String);

    ////////////////////////////////////////////////////////////////
    // Gets the data-information from a string: e.g. "width" and "800" from "width=800"
    //
    // \param String: The string, where the data is found  
    // \param Pair: The pair, in wich is to be written
    //
    // \return True, if there was data in [String], and changing [Pair] was successful, false otherwise
    ////////////////////////////////////////////////////////////////
    bool GetDataName(std::string String, std::pair<std::string, std::string> *Pair);

    ////////////////////////////////////////////////////////////////
    // Erases comments from a given string
    //
    // \param String: The string, the comments should be erased from
    //
    // \return True, if erasing was successful, false otherwise
    ////////////////////////////////////////////////////////////////
    bool EraseComments(std::string *String);

    ////////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////////
    std::map<std::string, pwIniClass*> myIniFileMap;
    std::string myFileName;
};

////////////////////////////////////////////////////////////////
// Template function definitions
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
// Gets a certain data from a certain class in an opened ini-file
////////////////////////////////////////////////////////////////
template<class T> bool pwIniFile::GetDataTemplate(T* Return, std::string Data, std::string Class) const
{ 
    std::map<std::string, pwIniClass*>::const_iterator ActiveClass; // Active class

    // Does the class exist?
    ActiveClass = myIniFileMap.find(Class);

    if (ActiveClass == myIniFileMap.end())
    {
        // Class could not be found
        pwLog::get()->WriteLine("pwIniFile::GetData: Class \'" + Class + "\' could not be found");
        return false;
    }

    if (!ActiveClass->second->GetData(Return, Data))
    {
        // Data could not be read
        pwLog::get()->WriteLine("pwIniFile::GetData: Data \'" + Data + "\' could not be read from class \'" + Class + "\'");
        return false;
    }

    // Reading the data was successful
    return true;
}

////////////////////////////////////////////////////////////////
// Adds certain data to a certain class in an opened ini-file
////////////////////////////////////////////////////////////////
template<class T> bool pwIniFile::AddDataTemplate(T Insert, std::string Data, std::string Class)
{
    std::map<std::string, pwIniClass*>::iterator ActiveClass; // Active class
    
    // Create the class if not existent and set ActiveClass
    pwIniClass* TmpIniClass = new pwIniClass;                                             // Create new class
    std::pair<std::string, pwIniClass*> TmpPair = std::make_pair(Class, TmpIniClass); // Make new class the "default"-class 
    ActiveClass = myIniFileMap.insert(TmpPair).first;                                     // Insert the defualt-class in the map

    if (!ActiveClass->second->AddData(Insert, Data))
    {
        // Data could not be written
        pwLog::get()->WriteLine("pwIniFile::GetData: Data \'" + Data + "\' could not be written to class \'" + Class + "\'");
        return false;
    }

    // Writing the data was successful
    return true;
}

#endif