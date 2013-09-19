#ifndef PWGRAPHICSLOG
#define PWGRAPHICSLOG

#include <string>
#include <sstream>
#include <list>
#include <SFML\Graphics.hpp>
#include "pwLogSystem.hpp"

#ifdef PWUSEGLOBALSINGLETONS
#ifndef glGLog
#define glGLog (*pwGraphicsLog::get())
#define p_glGLog pwGraphicsLog::get()
#endif
#endif

////////////////////////////////////////////////////////////////
// Writes info-, caution- and error-messages to a graphics-log
////////////////////////////////////////////////////////////////
class pwGraphicsLog : public sf::Drawable
{

public:

    ////////////////////////////////////////////////////////////////
    // Default constructor
    ////////////////////////////////////////////////////////////////
    pwGraphicsLog();

    ////////////////////////////////////////////////////////////////
    // Destructor
    ////////////////////////////////////////////////////////////////
    ~pwGraphicsLog();

    ////////////////////////////////////////////////////////////////
    // Get the static instance of the pwLog
    //
    // \return Static instance of the pwLog
    ////////////////////////////////////////////////////////////////
    static pwGraphicsLog* get();

    void update();
    static pwGraphicsLog& flush()
    {
        pwGraphicsLog::get()->_flush();
        return *pwGraphicsLog::get();
    }

    pwGraphicsLog& _flush();

    template<class T>
    pwGraphicsLog& print(const T & rightOp)
    {
        myStream << rightOp;

        return *this;
    }

    pwGraphicsLog & operator<<(bool rightOp)                              {myStream << rightOp; return *this;}
    pwGraphicsLog & operator<<(short rightOp)                             {myStream << rightOp; return *this;}
    pwGraphicsLog & operator<<(unsigned short rightOp)                    {myStream << rightOp; return *this;}
    pwGraphicsLog & operator<<(int rightOp)                               {myStream << rightOp; return *this;}
    pwGraphicsLog & operator<<(unsigned int rightOp)                      {myStream << rightOp; return *this;}
    pwGraphicsLog & operator<<(long rightOp)                              {myStream << rightOp; return *this;}
    pwGraphicsLog & operator<<(unsigned long rightOp)                     {myStream << rightOp; return *this;}
    pwGraphicsLog & operator<<(float rightOp)                             {myStream << rightOp; return *this;}
    pwGraphicsLog & operator<<(double rightOp)                            {myStream << rightOp; return *this;}
    pwGraphicsLog & operator<<(long double rightOp)                       {myStream << rightOp; return *this;}
    //pwGraphicsLog & operator<<(const void* rightOp)                     {myStream << rightOp; return *this;}
    pwGraphicsLog & operator<<(std::streambuf* rightOp)                   {myStream << rightOp; return *this;}
    pwGraphicsLog & operator<<(std::ostream& (*rightOp)(std::ostream&))   {myStream << rightOp; return *this;}
    pwGraphicsLog & operator<<(std::ios& (*rightOp)(std::ios&))           {myStream << rightOp; return *this;}
    pwGraphicsLog & operator<<(std::ios_base& (*rightOp)(std::ios_base&)) {myStream << rightOp; return *this;}
    pwGraphicsLog & operator<< (char rightOp)                             {myStream << rightOp; return *this;}
    pwGraphicsLog & operator<< (signed char rightOp)                      {myStream << rightOp; return *this;}
    pwGraphicsLog & operator<< (unsigned char rightOp)                    {myStream << rightOp; return *this;}
    pwGraphicsLog & operator<< (const char* rightOp)                      {myStream << rightOp; return *this;}
    pwGraphicsLog & operator<< (const signed char* rightOp)               {myStream << rightOp; return *this;}
    pwGraphicsLog & operator<< (const unsigned char* rightOp)             {myStream << rightOp; return *this;}
    //pwGraphicsLog & operator<<(const pwGraphicsLog & x)  {return *this;}
    pwGraphicsLog & operator<<(pwLogType::pwLogType LogType) {myLogType = LogType; return *this;}

    pwGraphicsLog & operator<<(pwGraphicsLog& (*_function)())
    {
        _function();
        return *this;
    }


    static bool not_alive (const std::pair<float, sf::Text*>& value);

private:

    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    ////////////////////////////////////////////////////////////////
    // Member data
    ////////////////////////////////////////////////////////////////
    static pwGraphicsLog*  p_inst;

    std::stringstream myStream;
    std::list<std::pair<float, sf::Text*>> myActiveLog;

    // Parameters for the whole log
    const sf::Font* myFont;
    unsigned int  myCharacterSize;
    pwLogType::pwLogType myLogType;
    sf::Uint32    myStyle;
    sf::Color     myColor;
};

#endif