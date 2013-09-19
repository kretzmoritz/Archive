#include "..\include\pwLogSystem.hpp"
#include "..\include\pwEngineOptions.hpp"

////////////////////////////////////////////////////////////////
// Declaration of the static member variables
////////////////////////////////////////////////////////////////
pwLog*  pwLog::p_inst = NULL;

////////////////////////////////////////////////////////////////
// Default constructor
////////////////////////////////////////////////////////////////
pwLog::pwLog()
{
    // Nothing to do
    myInitialized = false;
}

////////////////////////////////////////////////////////////////
// Destructor
////////////////////////////////////////////////////////////////
pwLog::~pwLog()
{
    // Nothing to do
    if (myInitialized)
    {
        Shutdown();
    }
}

////////////////////////////////////////////////////////////////
// Get the static instance of the pwGraphics
////////////////////////////////////////////////////////////////
pwLog* pwLog::get()
{
    // If the engine doesn't exist, create it
    if (!p_inst)
		p_inst = new pwLog;

    // Return the pointer to the engine
	return p_inst;
}

////////////////////////////////////////////////////////////////
// Initialize the logging-system
////////////////////////////////////////////////////////////////
bool pwLog::Initialize(std::string FileName)
{
    if (myInitialized || !pwEngineOptions::LogEnabled)
        return true;

    // Open up the log-file
    myLogFile.open(FileName.c_str());

    // Log-file is opened?
    if (!myLogFile.good())
    {
        // Log-file was not opened successfully
        return false;
    }

    // Get the current time
    const time_t Time = time(0);

#if (_MSC_VER >= 1500)
    tm timestamp;
    // localtime_s is definately defined
    if (localtime_s(&timestamp, &Time) != 0)
    {
        // Some error occured
        return false;
    }
#else
    // localtime_s is propably not defined - use localtime instead
    tm* timestamp = localtime(&Time);
#endif

    // Write the header to the log-file
    myLogFile << "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\">\n";
    myLogFile << "<html xmlns=\"http://www.w3.org/1999/xhtml\">\n";
    myLogFile << "<head>\n";
    myLogFile << "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />\n";
    myLogFile << "<title>pwEngine - LoggingSystem</title>\n";
    myLogFile << "<style type=\"text/css\">\n";
    myLogFile << "<!--\n";
    myLogFile << "body {\n";
	myLogFile << "    background-color:#999;\n";
	myLogFile << "    background-image:url('data/img/log.png');\n";
	myLogFile << "    background-repeat:repeat;\n";
	myLogFile << "    margin:0px;\n";
	myLogFile << "    padding:0px;\n}\n\n";
    myLogFile << "div#wrapper {\n";
    myLogFile << "    margin:0 auto;\n";
    myLogFile << "    padding:0;\n}\n\n";
    myLogFile << "div#logo {\n";
    myLogFile << "    background-image: url('data/img/default.png');\n";
    myLogFile << "    background-position: 0px -48px;\n";
    myLogFile << "    background-repeat: no-repeat;\n";
    myLogFile << "    padding: 20px 50px 10px 0px;\n";
    myLogFile << "    width: 244px;\n";
    myLogFile << "    height: 190px;\n";
    myLogFile << "    text-indent: -1000px;\n";
    myLogFile << "    float: left;\n}\n\n";
    myLogFile << "div#header {\n";
	myLogFile << "    position:relative;\n";
    myLogFile << "    top:62px;\n}\n\n";
    myLogFile << "div#title {\n";
	myLogFile << "    font-family:Georgia, \"Times New Roman\", Times, serif;\n";
    myLogFile << "    font-size:64px;\n";
    myLogFile << "    color:#CCC;\n}\n\n";
    myLogFile << "div#date {\n";
	myLogFile << "    font-family:Georgia, \"Times New Roman\", Times, serif;\n";
    myLogFile << "    font-size:24px;\n";
    myLogFile << "    color:#CCC;\n}\n\n";
    myLogFile << "div.log {\n";
    myLogFile << "    clear:both;\n";
    myLogFile << "    border:none;\n";
    myLogFile << "    background-color:#CCC;\n";
    myLogFile << "    color:black;\n";
    myLogFile << "    font-family:Georgia, \"Times New Roman\", Times, serif;\n";
    myLogFile << "    font-size:12px;\n";
	myLogFile << "    margin:0px 25px;\n}\n\n";
    myLogFile << "div.text {\n";
    myLogFile << "    padding:5px 5px 5px 5px;\n}\n\n";
    myLogFile << "div.info {\n";
    myLogFile << "    border-left:15px #0F0 solid;\n";
    myLogFile << "    color:#004C00;\n}\n\n";
    myLogFile << "div.error {\n";
    myLogFile << "    border-left:15px #F00 solid;\n";
    myLogFile << "    color:#900;\n}\n\n";
    myLogFile << "div.caution {\n";
    myLogFile << "    border-left:15px #FF0 solid;\n";
    myLogFile << "    color:#4C4C00;\n}\n";
    myLogFile << "-->\n";
    myLogFile << "</style>\n";
    myLogFile << "</head>\n\n";
    myLogFile << "<body>\n";
    myLogFile << "    <div id=\"wrapper\">\n";
    myLogFile << "        <div id=\"logo\">pwEngine</div>\n";
    myLogFile << "        <div id=\"header\">\n";
    myLogFile << "            <div id=\"title\">Logging-System</div>\n";
    myLogFile << "            <div id=\"date\">Written on " << timestamp.tm_year+1900 << "-" << std::setw(2) << std::setfill('0') << std::right <<  timestamp.tm_mon+1 << "-" << std::setw(2) << std::setfill('0') << std::right << timestamp.tm_mday << ", " << std::setw(2) << std::setfill('0') << std::right << timestamp.tm_hour << ":" << std::setw(2) << std::setfill('0') << std::right << timestamp.tm_min << "</div>\n";
    myLogFile << "        </div>\n";

    // The log-file is now initialized
    myInitialized = true;

    // No error occured
    return true;
}

////////////////////////////////////////////////////////////////
// Shuts the logging-system down
////////////////////////////////////////////////////////////////
bool pwLog::Shutdown()
{
    if (!myInitialized)
    {
        // Do nothing, if the logging-system wasn't even started
        return true;
    }

    // Write the footer to the log-file
    myLogFile << "    </div>\n";
    myLogFile << "</body>\n";
    myLogFile << "</html>";

     // Close the log-file
    myLogFile.close();

    // The log-file isn't initialized anymore
    myInitialized = false;

    if (p_inst != NULL)
    {
        delete (p_inst);
        p_inst = NULL;
    }

    // No error occured
    return true;
}

////////////////////////////////////////////////////////////////
// Write a new line in the log-file
////////////////////////////////////////////////////////////////
bool pwLog::WriteLine(std::string Text, pwLogType::pwLogType Type)
{
    // Was the logging-system already initialized
    if (!myInitialized)
    {
        // If not, initialize now
        Initialize();
    }
    
    // What type of line shoud be written
    switch(Type)
    {
        // Write an info-line to the log-file
    case (pwLogType::pwLTInfo):
        myLogFile << "        <div class=\"log\"><div class=\"text info\">Info: " << Text << "</div></div>\n";
        break;
        // Write an error-line to the log-file
    case (pwLogType::pwLTError):
        myLogFile << "        <div class=\"log\"><div class=\"text error\">Error: " << Text << "</div></div>\n";
        break;
        // Write a caution-line to the log-file
    case (pwLogType::pwLTCaution):
        myLogFile << "        <div class=\"log\"><div class=\"text caution\">Caution: " << Text << "</div></div>\n";
        break;
    default:
        // Something weard happened
        return false;
    }
    
    myLogFile.flush();

    // No errors occured
    return true;
}