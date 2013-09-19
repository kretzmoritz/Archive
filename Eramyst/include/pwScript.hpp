#ifndef PWSCRIPT
#define PWSCRIPT

#include <string>
#include <sstream>
#include <fstream>
#include <map>
#include <list>
#include <vector>
#include "pwLogSystem.hpp"

class pwScriptDataDefinition
{

public:

    ////////////////////////////////////////////////////////////////
    // Default constructor
    ////////////////////////////////////////////////////////////////
    pwScriptDataDefinition() {}

    ////////////////////////////////////////////////////////////////
    // Destructor
    ////////////////////////////////////////////////////////////////
    ~pwScriptDataDefinition() {}

    ////////////////////////////////////////////////////////////////
    // Loads pwScriptContainer-data from memory
    //
    // \param Data: Data of the pwScriptContainer to read
    //
    // \return True, if reading the data was successful, false otherwise
    ////////////////////////////////////////////////////////////////
    virtual bool LoadFromMemory(const std::string& Data) {return true;}

    virtual std::string at(const std::string& Parameter) {return "";}
};

class pwScriptResourceDataDefinition : public pwScriptDataDefinition
{

public:

    ////////////////////////////////////////////////////////////////
    // Default constructor
    ////////////////////////////////////////////////////////////////
    pwScriptResourceDataDefinition() {myParametres.resize(2, "");}

    ////////////////////////////////////////////////////////////////
    // Destructor
    ////////////////////////////////////////////////////////////////
    ~pwScriptResourceDataDefinition() {}

    ////////////////////////////////////////////////////////////////
    // Loads pwScriptContainer-data from memory
    //
    // \param Data: Data of the pwScriptContainer to read
    //
    // \return True, if reading the data was successful, false otherwise
    ////////////////////////////////////////////////////////////////
    bool LoadFromMemory(const std::string& Data);

    std::string at(const std::string& Parameter);

private:
    
    std::vector<std::string> myParametres;
};

class pwScriptContainer
{
    
public:

    ////////////////////////////////////////////////////////////////
    // Default constructor
    ////////////////////////////////////////////////////////////////
    pwScriptContainer() {}

    ////////////////////////////////////////////////////////////////
    // Destructor
    ////////////////////////////////////////////////////////////////
    ~pwScriptContainer() {}

    ////////////////////////////////////////////////////////////////
    // Loads pwScriptContainer-data from memory
    //
    // \param Data: Data of the pwScriptContainer to read
    //
    // \return True, if reading the data was successful, false otherwise
    ////////////////////////////////////////////////////////////////
    virtual bool LoadFromMemory(const std::string& Data) {return true;}
};

class pwScriptDataExContainer : public pwScriptContainer
{
    
public:

    ////////////////////////////////////////////////////////////////
    // Default constructor
    ////////////////////////////////////////////////////////////////
    pwScriptDataExContainer();

    ////////////////////////////////////////////////////////////////
    // Destructor
    ////////////////////////////////////////////////////////////////
    ~pwScriptDataExContainer();

    ////////////////////////////////////////////////////////////////
    // Loads pwScriptDataExContainer-data from memory
    //
    // \param Data: Data of the pwScriptDataExContainer to read
    //
    // \return True, if reading the data was successful, false otherwise
    ////////////////////////////////////////////////////////////////
    bool LoadFromMemory(const std::string& Data);

    std::list<std::pair<std::string, std::string>>::const_iterator begin() const {return myData.begin();}
    std::list<std::pair<std::string, std::string>>::const_iterator end() const   {return myData.end();}
    bool empty() const                                                     {return myData.empty();}
    std::list<std::pair<std::string, std::string>>::size_type size() const {return myData.size();}


private:

    ////////////////////////////////////////////////////////////////
    // Gets the data definition-input [in a DataEx Container] from a string: e.g. "image" and "default.png" from "image:default.png"
    //
    // \param String: The data definition, that is to be changed
    //
    // \return The name of the data-type, if there was data-definition in [String], and changing it was successful, "" otherwise
    ////////////////////////////////////////////////////////////////
    std::string GetDataDefinition(std::string *String);

    std::list<std::pair<std::string, std::string>>  myData;
};

struct pwScriptContainerType
{
    std::string Type;
    std::string Name;
    pwScriptContainer* Container; 
};

class pwScriptClass
{

public:

    ////////////////////////////////////////////////////////////////
    // Default constructor
    ////////////////////////////////////////////////////////////////
    pwScriptClass();

    ////////////////////////////////////////////////////////////////
    // Destructor
    ////////////////////////////////////////////////////////////////
    ~pwScriptClass();

    ////////////////////////////////////////////////////////////////
    // Loads pwScriptClass-data from memory
    //
    // \param Data: Data of the pwScriptClass to read
    //
    // \return True, if reading the data was successful, false otherwise
    ////////////////////////////////////////////////////////////////
    bool LoadFromMemory(const std::string& Data);

    const pwScriptContainerType* Container(std::string& ContainerName) const;
    const pwScriptDataExContainer* List(std::string& ContainerName) const;

private:

    ////////////////////////////////////////////////////////////////
    // Gets the container definition from a string: e.g. "default" from "[list:default]"
    //
    // \param String: The container definition with brackets, that is to be changed
    //
    // \return The name of the container-type, if there was container-definition in [String], and changing it was successful, "" otherwise
    ////////////////////////////////////////////////////////////////
    std::string GetContainer(std::string *String);
    
    std::list<pwScriptContainerType> myScriptContainer;
};

////////////////////////////////////////////////////////////////
// Helper-class for reading and running pwScript-files
////////////////////////////////////////////////////////////////
class pwScript
{
    friend pwScriptClass;
    friend pwScriptContainer;
    friend pwScriptDataExContainer;

public:

    ////////////////////////////////////////////////////////////////
    // Default constructor
    ////////////////////////////////////////////////////////////////
    pwScript();

    ////////////////////////////////////////////////////////////////
    // Destructor
    ////////////////////////////////////////////////////////////////
    ~pwScript();

    ////////////////////////////////////////////////////////////////
    // Loads pwScript-data from a file
    //
    // \param FileName: Name of the pwScript-file to read
    //
    // \return True, if reading the file was successful, false otherwise
    ////////////////////////////////////////////////////////////////
    bool LoadFromFile(const std::string& FileName);

    ////////////////////////////////////////////////////////////////
    // Loads pwScript-data from memory
    //
    // \param Data: Data of the pwScript to read
    //
    // \return True, if reading the data was successful, false otherwise
    ////////////////////////////////////////////////////////////////
    bool LoadFromMemory(const std::string& Data);

    const pwScriptClass* Class(std::string& ClassName) const;

protected:

    ////////////////////////////////////////////////////////////////
    // Erases white-spaces from a given string
    //
    // \param String: The string, the white-spaces should be erased from
    //
    // \return True, if erasing was successful, false otherwise
    ////////////////////////////////////////////////////////////////
    static bool EraseWhiteSpaces(std::string *String);

    ////////////////////////////////////////////////////////////////
    // Erases comments from a given string
    //
    // \param String: The string, the comments should be erased from
    //
    // \return True, if erasing was successful, false otherwise
    ////////////////////////////////////////////////////////////////
    static bool EraseComments(std::string *String);

private:

    ////////////////////////////////////////////////////////////////
    // Gets the class name from a string: e.g. "default" from ".default"
    //
    // \param String: The list name with the dot, that is to be changed
    //
    // \return True, if there was a class in [String], and changing it was successful, false otherwise
    ////////////////////////////////////////////////////////////////
    bool GetClass(std::string *String);

    ////////////////////////////////////////////////////////////////
    // Gets the function name and type from a string: e.g. "gameobject" and "default"  from "[fn.gameobject:default]"
    //
    // \param String: The function name with brackets
    // \param Pair: The pair, in wich is to be written
    //
    // \return True, if there was a function in [String], and changing [Pair] was successful, false otherwise
    ////////////////////////////////////////////////////////////////
    //bool GetFuncDefinition(std::string String, std::pair<std::string, std::string> *Pair);

    ////////////////////////////////////////////////////////////////
    // Gets the variable-type and data from a string: e.g. "int" and "hello=800" from "int:hello=800"
    //
    // \param String: The string, where the data is found  
    // \param Pair: The pair, in wich is to be written
    //
    // \return True, if there was data in [String], and changing [Pair] was successful, false otherwise
    ////////////////////////////////////////////////////////////////
    //bool GetVarDeclaration(std::string String, std::pair<std::string, std::string> *Pair);

    ////////////////////////////////////////////////////////////////
    // Gets the variable-name and value from a string: e.g. "hello" and "800" from "hello=800"
    //
    // \param String: The string, where the data is found  
    // \param Pair: The pair, in wich is to be written
    //
    // \return True, if there was data in [String], and changing [Pair] was successful, false otherwise
    ////////////////////////////////////////////////////////////////
    //bool GetVarDefinition(std::string String, std::pair<std::string, std::string> *Pair);

    ////////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////////
    std::map<std::string, pwScriptClass*> myScriptClass;
};

#endif