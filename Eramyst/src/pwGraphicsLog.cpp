#include "..\include\pwGraphicsLog.hpp"
#include "..\include\pwEngineOptions.hpp"

////////////////////////////////////////////////////////////////
// Declaration of the static member variables
////////////////////////////////////////////////////////////////
pwGraphicsLog*  pwGraphicsLog::p_inst = NULL;

////////////////////////////////////////////////////////////////
// Default constructor
////////////////////////////////////////////////////////////////
pwGraphicsLog::pwGraphicsLog()
{
    myFont = NULL;
    myCharacterSize = 20;
    myStyle = sf::Text::Regular;
    myStream.clear();
    myLogType = pwLogType::pwLTError;
    // Nothing to do
}

////////////////////////////////////////////////////////////////
// Destructor
////////////////////////////////////////////////////////////////
pwGraphicsLog::~pwGraphicsLog()
{
    // Nothing to do
}

////////////////////////////////////////////////////////////////
// Get the static instance of the pwGraphics
////////////////////////////////////////////////////////////////
pwGraphicsLog* pwGraphicsLog::get()
{
    // If the engine doesn't exist, create it
    if (!p_inst)
		p_inst = new pwGraphicsLog;

    // Return the pointer to the engine
	return p_inst;
}

void pwGraphicsLog::update()
{
    myActiveLog.remove_if(pwGraphicsLog::not_alive);

    unsigned int j = 0;
    for (std::list<std::pair<float, sf::Text*>>::iterator i = myActiveLog.begin(); i != myActiveLog.end(); i++)
    {
        i->first += pwGraphics::get()->GetLastFrameTime();
        i->second->setPosition(20.0f, (float)(pwEngineOptions::GetInternGraphicsSize().y - ((j+1) * 32)));
        j++;
    }
}

// a predicate implemented as a function:
bool pwGraphicsLog::not_alive (const std::pair<float, sf::Text*>& value)
{
    if (value.first > 1.0f)
	{
        delete value.second;
        return true;
	}
	return false; 
}

pwGraphicsLog& pwGraphicsLog::_flush() 
{
    std::string TmpString;
    
    while(!myStream.eof())
    {
        std::getline(myStream, TmpString);
    }

    sf::Text* pTmpText = new sf::Text();
    pTmpText->setStyle(myStyle);
    pTmpText->setCharacterSize(myCharacterSize);

    if (myFont != NULL)
        pTmpText->setFont(*myFont);

    if (myLogType == pwLogType::pwLTInfo)
    {
        pTmpText->setColor(sf::Color(0, 255, 0));
        pTmpText->setString("Info: " + TmpString);
    }
    else if (myLogType == pwLogType::pwLTCaution)
    {
        pTmpText->setColor(sf::Color(255, 255, 0));
        pTmpText->setString("Caution: " + TmpString);
    }
    else if (myLogType == pwLogType::pwLTError)
    {
        pTmpText->setColor(sf::Color(255, 0, 0));
        pTmpText->setString("Error: " + TmpString);
    }

    myActiveLog.push_back(std::make_pair(0.0f, pTmpText));
    
    if (myActiveLog.size() > 10)
    {
        myActiveLog.pop_front();
    }

    myStream.clear();

    myLogType = pwLogType::pwLTError;

    return *this;
}


void pwGraphicsLog::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (std::list<std::pair<float, sf::Text*>>::const_iterator i = myActiveLog.begin(); i != myActiveLog.end(); i++)
    {
        target.draw(*i->second, states);
    }
}