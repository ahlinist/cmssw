#include "cgicc/HTTPHTMLHeader.h"
#include "cgicc/HTTPPlainHeader.h"
#include "EventFilter/TriggerAdapter/interface/Application.h"
#include "EventFilter/TriggerAdapter/interface/Constants.h"
#include "EventFilter/TriggerAdapter/interface/ForceFailedEvent.h"
#include "rubuilder/utils/XoapUtils.h"
#include "i2o/Method.h"
#include "interface/evb/i2oEVBMsgs.h"
#include "interface/shared/i2oXFunctionCodes.h"
#include "toolbox/utils.h"
#include "toolbox/fsm/FailedEvent.h"
#include "toolbox/mem/HeapAllocator.h"
#include "xcept/tools.h"
#include "xdaq/NamespaceURI.h"
#include "xdaq/exception/ApplicationNotFound.h"
#include "xgi/Method.h"
#include "xoap/domutils.h"
#include "xoap/MessageFactory.h"
#include "xoap/MessageReference.h"
#include "xoap/Method.h"
#include "xoap/SOAPBody.h"
#include "xoap/SOAPBodyElement.h"
#include "xoap/SOAPEnvelope.h"

#include <ctime>
#include <netinet/in.h>


rubuilder::ta::Application::Application(xdaq::ApplicationStub *s)
throw (xdaq::exception::Exception) :
xdaq::WebApplication(s),

//logger_(Logger::getInstance(generateLoggerName())),
logger_(getApplicationLogger()),

applicationBSem_(toolbox::BSem::FULL),

soapParameterExtractor_(this)
{
    tid_           = 0;
    i2oAddressMap_ = i2o::utils::getAddressMap();
    poolFactory_   = toolbox::mem::getMemoryPoolFactory();
    appInfoSpace_  = getApplicationInfoSpace();
    appDescriptor_ = getApplicationDescriptor();
    appContext_    = getApplicationContext();
    xmlClass_      = appDescriptor_->getClassName();
    instance_      = appDescriptor_->getInstance();
    urn_           = appDescriptor_->getURN();

    appDescriptor_->setAttribute("icon", APP_ICON);

    // Note that rubuilderTesterDescriptor_ will be zero if the
    // RUBuilderTester application is not found
    rubuilderTesterDescriptor_ = getRUBuilderTester();

    i2oExceptionHandler_ = toolbox::exception::bind
    (
        this,
        &rubuilder::ta::Application::onI2oException,
        "onI2oException"
    );

    // Create a memory pool for dummy triggers
    try
    {
        toolbox::net::URN urn("toolbox-mem-pool", "Dummy trigger pool");
        toolbox::mem::HeapAllocator* a = new toolbox::mem::HeapAllocator();

        triggerPool_ = poolFactory_->createPool(urn, a);
    }
    catch (toolbox::mem::exception::Exception e)
    {
        XCEPT_RETHROW(xdaq::exception::Exception,
            "Failed to create dummy trigger pool", e);
    }

    try
    {
        defineFsm();
    }
    catch(xcept::Exception &e)
    {
        XCEPT_RETHROW(xdaq::exception::Exception,
            "Failed to define finite state machine", e);
    }

    // Initialise and group parameters
    stdConfigParams_  = initAndGetStdConfigParams();
    stdMonitorParams_ = initAndGetStdMonitorParams();
    monitorCounters_  = getMonitorCounters();
    resetCounters(monitorCounters_);
    addCountersToParams(monitorCounters_, stdMonitorParams_);
    dbgMonitorParams_ = initAndGetDbgMonitorParams();

    // Create info space for monitoring
    monitoringInfoSpaceName_ =
        generateMonitoringInfoSpaceName(xmlClass_, instance_);
    monitoringInfoSpace_ =
        xdata::InfoSpace::get(monitoringInfoSpaceName_);

    try
    {
        putParamsIntoInfoSpace(stdConfigParams_ , appInfoSpace_       );
        putParamsIntoInfoSpace(stdMonitorParams_, appInfoSpace_       );
        putParamsIntoInfoSpace(stdMonitorParams_, monitoringInfoSpace_);
    }
    catch(xcept::Exception &e)
    {
        XCEPT_RETHROW(xdaq::exception::Exception,
            "Failed to put parameters into info spaces", e);
    }
    
    bindFsmSoapCallbacks();
    bindI2oCallbacks();
    bindXgiCallbacks();

    LOG4CPLUS_INFO(logger_, "End of constructor");
}


std::string rubuilder::ta::Application::generateLoggerName()
{
    xdaq::ApplicationDescriptor *appDescriptor = getApplicationDescriptor();
    std::string                 appClass       = appDescriptor->getClassName();
    unsigned int                appInstance    = appDescriptor->getInstance();
    std::stringstream           oss;


    oss << appClass << appInstance;

    return oss.str();
}


xdaq::ApplicationDescriptor *rubuilder::ta::Application::getRUBuilderTester()
{
    xdaq::ApplicationDescriptor *appDescriptor = 0;


    try
    {
        appDescriptor =
            getApplicationContext()->
            getDefaultZone()->
            getApplicationDescriptor("rubuilder::tester::Application", 0);
    }
    catch(xcept::Exception &e)
    {
        appDescriptor = 0;
    }

    return appDescriptor;
}


void rubuilder::ta::Application::defineFsm()
throw (rubuilder::ta::exception::Exception)
{
    try
    {
        // Define FSM states
        fsm_.addState('H', "Halted"   , this,
            &rubuilder::ta::Application::stateChanged);
        fsm_.addState('R', "Ready"    , this,
            &rubuilder::ta::Application::stateChanged);
        fsm_.addState('E', "Enabled"  , this,
            &rubuilder::ta::Application::stateChanged);
        fsm_.addState('S', "Suspended", this,
            &rubuilder::ta::Application::stateChanged);
    }
    catch(xcept::Exception &e)
    {
        XCEPT_RETHROW(rubuilder::ta::exception::Exception,
            "Failed to define FSM states", e);
    }

    try
    {
        // Explicitly set the name of the Failed state
        fsm_.setStateName('F', "Failed");
    }
    catch(xcept::Exception &e)
    {
        XCEPT_RETHROW(rubuilder::ta::exception::Exception,
            "Failed to explicitly set the name of the Failed state", e);
    }

    try
    {
        // Define FSM transitions
        fsm_.addStateTransition('H', 'R', "Configure", this,
            &rubuilder::ta::Application::configureAction);
        fsm_.addStateTransition('R', 'E', "Enable"   , this,
            &rubuilder::ta::Application::enableAction);
        fsm_.addStateTransition('E', 'S', "Suspend"  , this,
            &rubuilder::ta::Application::suspendAction);
        fsm_.addStateTransition('S', 'E', "Resume"   , this,
            &rubuilder::ta::Application::resumeAction);
        fsm_.addStateTransition('H', 'H', "Halt"     , this,
            &rubuilder::ta::Application::haltAction);
        fsm_.addStateTransition('R', 'H', "Halt"     , this,
            &rubuilder::ta::Application::haltAction);
        fsm_.addStateTransition('E', 'H', "Halt"     , this,
            &rubuilder::ta::Application::haltAction);
        fsm_.addStateTransition('S', 'H', "Halt"     , this,
            &rubuilder::ta::Application::haltAction);

        fsm_.addStateTransition('H', 'F', "Fail"     , this,
            &rubuilder::ta::Application::failAction);
        fsm_.addStateTransition('R', 'F', "Fail"     , this,
            &rubuilder::ta::Application::failAction);
        fsm_.addStateTransition('E', 'F', "Fail"     , this,
            &rubuilder::ta::Application::failAction);

        fsm_.addStateTransition('F', 'F', "Fail"     , this,
            &rubuilder::ta::Application::failAction);
    }
    catch(xcept::Exception &e)
    {
        XCEPT_RETHROW(rubuilder::ta::exception::Exception,
            "Failed to define FSM transitions", e);
    }

    try
    {
        fsm_.setFailedStateTransitionAction(this,
            &rubuilder::ta::Application::failAction);
    }
    catch(xcept::Exception &e)
    {
        XCEPT_RETHROW(rubuilder::ta::exception::Exception,
            "Failed to set action for failed state transition", e);
    }

    try
    {
        fsm_.setFailedStateTransitionChanged(this,
            &rubuilder::ta::Application::stateChanged);
    }
    catch(xcept::Exception &e)
    {
        XCEPT_RETHROW(rubuilder::ta::exception::Exception,
            "Failed to set state changed callback for failed state transition",
            e);
    }

    try
    {
        fsm_.setInitialState('H');
    }
    catch(xcept::Exception &e)
    {
        XCEPT_RETHROW(rubuilder::ta::exception::Exception,
            "Failed to set state initial state of FSM", e);
    }

    try
    {
        fsm_.reset();
    }
    catch(xcept::Exception &e)
    {
        XCEPT_RETHROW(rubuilder::ta::exception::Exception,
            "Failed to reset FSM", e);
    }
}


std::string rubuilder::ta::Application::generateMonitoringInfoSpaceName
(
    const std::string  appClass,
    const unsigned int appInstance
)
{
    std::stringstream oss;


    oss <<  appClass;

    return oss.str();
}


std::vector< std::pair<std::string, xdata::Serializable*> >
rubuilder::ta::Application::initAndGetStdConfigParams()
{
    std::vector< std::pair<std::string, xdata::Serializable*> > params;


    evmInstance_     = -1; // Explicitly indicate parameter not set
    triggerSourceId_ = 0;
    fedPayloadSize_  = 2048;

    params.push_back(std::pair<std::string,xdata::Serializable *>
        ("evmInstance", &evmInstance_));
    params.push_back(std::pair<std::string,xdata::Serializable *>
        ("triggerSourceId", &triggerSourceId_));
    params.push_back(std::pair<std::string,xdata::Serializable *>
        ("fedPayloadSize", &fedPayloadSize_));

    return params;
}


std::vector< std::pair<std::string, xdata::Serializable*> >
rubuilder::ta::Application::initAndGetDbgConfigParams()
{
    std::vector< std::pair<std::string, xdata::Serializable*> > params;


    return params;
}


void rubuilder::ta::Application::resetCounters
(
    std::vector< std::pair<std::string, xdata::UnsignedInteger32*> > &counters
)
{
    std::vector
    <
        std::pair<std::string, xdata::UnsignedInteger32*>
    >::const_iterator itor;
    xdata::UnsignedInteger32 *counter = 0;


    for(itor=counters.begin(); itor!=counters.end(); itor++)
    {
        counter = (*itor).second;
        *counter = 0;
    }
}


void rubuilder::ta::Application::addCountersToParams
(
    std::vector < std::pair<std::string, xdata::UnsignedInteger32*> > &counters,
    std::vector < std::pair<std::string, xdata::Serializable*> > &params
)
{
    std::vector
    <
        std::pair<std::string, xdata::UnsignedInteger32*>
    >::const_iterator itor;


    for(itor=counters.begin(); itor!=counters.end(); itor++)
    {
        params.push_back(std::pair<std::string,xdata::Serializable *>
            ((*itor).first, (*itor).second));
    }
}


std::vector< std::pair<std::string, xdata::UnsignedInteger32*> >
rubuilder::ta::Application::getMonitorCounters()
{
    std::vector< std::pair<std::string, xdata::UnsignedInteger32*> > counters;


    counters.push_back(std::pair<std::string,xdata::UnsignedInteger32*>
        ("I2O_TA_CREDIT_Payload", &I2O_TA_CREDIT_Payload_));
    counters.push_back(std::pair<std::string,xdata::UnsignedInteger32*>
        ("I2O_TA_CREDIT_LogicalCount", &I2O_TA_CREDIT_LogicalCount_));
    counters.push_back(std::pair<std::string,xdata::UnsignedInteger32*>
        ("I2O_TA_CREDIT_I2oCount", &I2O_TA_CREDIT_I2oCount_));
    counters.push_back(std::pair<std::string,xdata::UnsignedInteger32*>
        ("I2O_EVM_TRIGGER_Payload", &I2O_EVM_TRIGGER_Payload_));
    counters.push_back(std::pair<std::string,xdata::UnsignedInteger32*>
        ("I2O_EVM_TRIGGER_LogicalCount", &I2O_EVM_TRIGGER_LogicalCount_));
    counters.push_back(std::pair<std::string,xdata::UnsignedInteger32*>
        ("I2O_EVM_TRIGGER_I2oCount", &I2O_EVM_TRIGGER_I2oCount_));

    return counters;
}


std::vector< std::pair<std::string, xdata::Serializable*> >
rubuilder::ta::Application::initAndGetStdMonitorParams()
{
    std::vector< std::pair<std::string, xdata::Serializable*> > params;


    stateName_       = "Halted";
    nbCreditsHeld_   = 0;
    eventNumber_     = 1;

    params.push_back(std::pair<std::string,xdata::Serializable *>
        ("stateName", &stateName_));
    params.push_back(std::pair<std::string,xdata::Serializable *>
        ("nbCreditsHeld", &nbCreditsHeld_));
    params.push_back(std::pair<std::string,xdata::Serializable *>
        ("eventNumber", &eventNumber_));

    return params;
}


std::vector< std::pair<std::string, xdata::Serializable*> >
rubuilder::ta::Application::initAndGetDbgMonitorParams()
{
    std::vector< std::pair<std::string, xdata::Serializable*> > params;


    return params;
}


void rubuilder::ta::Application::putParamsIntoInfoSpace
(
    std::vector< std::pair<std::string, xdata::Serializable*> > &params,
    xdata::InfoSpace                                            *s
)
throw (rubuilder::ta::exception::Exception)
{
    std::vector
    <
        std::pair<std::string, xdata::Serializable*>
    >::const_iterator itor;


    for(itor=params.begin(); itor!= params.end(); itor++)
    {
        try
        {
            s->fireItemAvailable(itor->first, itor->second);
        }
        catch(xcept::Exception &e)
        {
            std::stringstream oss;

            oss << "Failed to put " << itor->first;
            oss << " into infospace " << s->name();

            XCEPT_RETHROW(rubuilder::ta::exception::Exception, oss.str(), e);
        }
    }
}


void rubuilder::ta::Application::stateChanged
(
    toolbox::fsm::FiniteStateMachine & fsm
)
throw (toolbox::fsm::exception::Exception)
{
    toolbox::fsm::State state = fsm.getCurrentState();


    try
    {
        stateName_ = fsm.getStateName(state);
    }
    catch(xcept::Exception &e)
    {
        XCEPT_RETHROW(toolbox::fsm::exception::Exception,
            "Failed to set exported parameter stateName", e);
    }
}


void rubuilder::ta::Application::bindFsmSoapCallbacks()
{
    xoap::bind
    (
        this,
        &rubuilder::ta::Application::processSoapFsmEvent,
        "Configure",
        XDAQ_NS_URI
    );

    xoap::bind
    (
        this,
        &rubuilder::ta::Application::processSoapFsmEvent,
        "Enable",
        XDAQ_NS_URI
    );

    xoap::bind
    (
        this,
        &rubuilder::ta::Application::processSoapFsmEvent,
        "Suspend",
        XDAQ_NS_URI
    );

    xoap::bind
    (
        this,
        &rubuilder::ta::Application::processSoapFsmEvent,
        "Resume",
        XDAQ_NS_URI
    );

    xoap::bind
    (
        this,
        &rubuilder::ta::Application::processSoapFsmEvent,
        "Halt",
        XDAQ_NS_URI
    );

    xoap::bind
    (
        this,
        &rubuilder::ta::Application::processSoapFsmEvent,
        "Fail",
        XDAQ_NS_URI
    );

    xoap::bind
    (
        this,
        &rubuilder::ta::Application::resetMonitoringCountersMsg,
        "resetMonitoringCounters",
        XDAQ_NS_URI
    );
}


void rubuilder::ta::Application::fsmWebPage
(
    xgi::Input  *in,
    xgi::Output *out
)
throw (xgi::exception::Exception)
{
    try
    {
         processFsmForm(in);
    }
    catch(xcept::Exception &e)
    {
        XCEPT_RETHROW(xgi::exception::Exception,
            "Failed to process FSM form", e);
    }

    toolbox::fsm::State state = fsm_.getCurrentState();
    std::vector<std::string> events;

    out->getHTTPResponseHeader().addHeader("Content-Type", "text/html");

    *out << "<html>"                                              << std::endl;
    *out << "<head>"                                              << std::endl;
    *out << "<link type=\"text/css\" rel=\"stylesheet\"";
    *out << " href=\"/" << urn_ << "/styles.css\"/>"              << std::endl;
    *out << "<title>"                                             << std::endl;
    *out << xmlClass_ << instance_ << "  FSM"                     << std::endl;
    *out << "</title>"                                            << std::endl;
    *out << "</head>"                                             << std::endl;
    *out << "<body>"                                              << std::endl;

    printWebPageTitleBar
    (
        out,
        rubuilder::ta::FSM_ICON,
        "FSM",
        rubuilder::ta::APP_ICON,
        rubuilder::ta::DBG_ICON,
        rubuilder::ta::FSM_ICON
    );

    *out << "<form method=\"get\" action=\"/" << urn_ << "/fsm\">";
    *out << std::endl;

    switch(state)
    {
    case 'H': // Halted
        events.push_back("Configure");
        break;
    case 'R': // Ready
        events.push_back("Enable");
        break;
    case 'E': // Enabled
        events.push_back("Suspend");
        events.push_back("Halt");
        break;
    case 'S': // Suspended
        events.push_back("Resume");
        break;
    case 'F': // Failed
        // Nothing can be done
        break;
    default:
        std::stringstream oss;

        oss << "Unknown state: " << state;

        XCEPT_RAISE(xgi::exception::Exception, oss.str());
    }

    // If the application is not in the "Failed" state
    if(state != 'F')
    {
        *out << "<table width=\"100%\">"                          << std::endl;
        *out << "<tr align=\"left\" width=\"50%\">"               << std::endl;
        *out << "  <td>"                                          << std::endl;

        std::vector<std::string>::const_iterator itor;

        for(itor=events.begin(); itor!=events.end(); itor++)
        {
            *out << "    <input"                                  << std::endl;
            *out << "     type=\"submit\""                        << std::endl;
            *out << "     name=\"event\""                         << std::endl;
            *out << "     value=\"" << *itor << "\""              << std::endl;
            *out << "    />"                                      << std::endl;
        }

        *out << "  </td>"                                         << std::endl;
        *out << "  <td align=\"left\"width=\"50%\">"              << std::endl;
        *out << "    <font color=\"red\"><b>DEBUGGING ONLY!!!!</b></font>";
        *out                                                      << std::endl;
        *out << "    <input"                                      << std::endl;
        *out << "     type=\"submit\""                            << std::endl;
        *out << "     name=\"event\""                             << std::endl;
        *out << "     value=\"Fail\""                             << std::endl;
        *out << "    />"                                          << std::endl;
        *out << "  </td>"                                         << std::endl;
        *out << "</tr>"                                           << std::endl;
        *out << "</table>"                                        << std::endl;
    }

    *out << "</form>"                                             << std::endl;
    *out << "</body>"                                             << std::endl;

    *out << "</html>"                                             << std::endl;
}


void rubuilder::ta::Application::processFsmForm(xgi::Input *in)
throw (xgi::exception::Exception)
{
    cgicc::Cgicc         cgi(in);
    cgicc::form_iterator element = cgi.getElement("event");
    std::string          event   = "";


    // If their is an event from from the html form
    if(element != cgi.getElements().end())
    {
        event = (*element).getValue();

        try
        {
            toolbox::Event::Reference evtRef(new toolbox::Event(event, this));

            applicationBSem_.take();
            fsm_.fireEvent(evtRef);
            applicationBSem_.give();
        }
        catch(xcept::Exception &e)
        {
            applicationBSem_.give();

            std::stringstream oss;

            oss << "Failed to process FSM event: " << event;

            LOG4CPLUS_ERROR(logger_,
                oss.str() << " : " << xcept::stdformat_exception_history(e));
            XCEPT_RETHROW(xgi::exception::Exception, oss.str(), e);
        }
    }
}


xoap::MessageReference rubuilder::ta::Application::processSoapFsmEvent
(
    xoap::MessageReference msg
)
throw (xoap::exception::Exception)
{
    std::string event = "";


    try
    {
        event = soapParameterExtractor_.extractParameters(msg);
    }
    catch(xcept::Exception &e)
    {
        std::string s = "Failed to extract FSM event and parameters from SOAP message";

        LOG4CPLUS_ERROR(logger_,
            s << " : " << xcept::stdformat_exception_history(e));
        XCEPT_RETHROW(xoap::exception::Exception, s, e);
    }

    try
    {
        toolbox::Event::Reference evtRef(new toolbox::Event(event, this));

        applicationBSem_.take();
        fsm_.fireEvent(evtRef);
        applicationBSem_.give();
    }
    catch(xcept::Exception &e)
    {
        applicationBSem_.give();

        std::stringstream oss;

        oss << "Failed to process FSM event: " << event;

        LOG4CPLUS_ERROR(logger_,
            oss.str() << " : " << xcept::stdformat_exception_history(e));
        XCEPT_RETHROW(xoap::exception::Exception, oss.str(), e);
    }

    if(fsm_.getCurrentState() == 'F')
    {
        XCEPT_RAISE(xoap::exception::Exception, reasonForFailed_.getValue());
    }

    return rubuilder::utils::createFsmSoapResponseMsg(event, stateName_.toString());
}


void rubuilder::ta::Application::bindI2oCallbacks()
{
    i2o::bind
    (
        this,
        &rubuilder::ta::Application::taCreditMsg,
        I2O_TA_CREDIT,
        XDAQ_ORGANIZATION_ID
    );
}


void rubuilder::ta::Application::bindXgiCallbacks()
{
    xgi::bind
    (
        this,
        &rubuilder::ta::Application::css,
        "styles.css"
    );

    xgi::bind
    (
        this,
        &rubuilder::ta::Application::defaultWebPage,
        "Default"
    );

    xgi::bind
    (
        this,
        &rubuilder::ta::Application::debugWebPage,
        "debug"
    );

    xgi::bind
    (
        this,
        &rubuilder::ta::Application::fsmWebPage,
        "fsm"
    );
}


void rubuilder::ta::Application::css
(
    xgi::Input  *in,
    xgi::Output *out
)
throw (xgi::exception::Exception)
{
    out->getHTTPResponseHeader().addHeader("Content-Type", "text/css");

    *out << "body"                                                << std::endl;
    *out << "{"                                                   << std::endl;
    *out << "background-color: white;"                            << std::endl;
    *out << "font-family:Arial;"                                  << std::endl;
    *out << "}"                                                   << std::endl;
    *out                                                          << std::endl;
    *out << ".app_links"                                          << std::endl;
    *out << "{"                                                   << std::endl;
    *out << "font-size: 14px;"                                    << std::endl;
    *out << "line-height: 14px;"                                  << std::endl;
    *out << "}"                                                   << std::endl;
    *out                                                          << std::endl;
    *out << "table.params th"                                     << std::endl;
    *out << "{"                                                   << std::endl;
    *out << "color: white;"                                       << std::endl;
    *out << "background-color: #66F;"                             << std::endl;
    *out << "}"                                                   << std::endl;
}


void rubuilder::ta::Application::defaultWebPage(xgi::Input *in, xgi::Output *out)
throw (xgi::exception::Exception)
{
    out->getHTTPResponseHeader().addHeader("Content-Type", "text/html");

    *out << "<html>"                                              << std::endl;

    *out << "<head>"                                              << std::endl;
    *out << "<link type=\"text/css\" rel=\"stylesheet\"";
    *out << " href=\"/" << urn_ << "/styles.css\"/>"              << std::endl;
    *out << "<title>"                                             << std::endl;
    *out << xmlClass_ << instance_ << " MAIN"                     << std::endl;
    *out << "</title>"                                            << std::endl;
    *out << "</head>"                                             << std::endl;

    *out << "<body>"                                              << std::endl;

    printWebPageTitleBar
    (
        out,
        rubuilder::ta::APP_ICON,
        "Main",
        rubuilder::ta::APP_ICON,
        rubuilder::ta::DBG_ICON,
        rubuilder::ta::FSM_ICON
    );

    *out << "<table>"                                             << std::endl;
    *out << "<tr valign=\"top\">"                                 << std::endl;
    *out << "  <td>"                                              << std::endl;
    try
    {
        printParamsTable(in, out, "Standard configuration", stdConfigParams_);
    }
    catch(xcept::Exception &e)
    {
        XCEPT_RETHROW(xgi::exception::Exception,
            "Failed to print standard configuration table", e);
    }
    *out << "  </td>"                                             << std::endl;
    *out << "  <td width=\"64\">"                                 << std::endl;
    *out << "  </td>"                                             << std::endl;
    *out << "  <td>"                                              << std::endl;
    try
    {
        printParamsTable(in, out, "Standard monitoring", stdMonitorParams_);
    }
    catch(xcept::Exception &e)
    {
        XCEPT_RETHROW(xgi::exception::Exception,
            "Failed to print standard monitoring table", e);
    }
    *out << "  </td>"                                             << std::endl;
    *out << "</tr>"                                               << std::endl;
    *out << "</table>"                                            << std::endl;
    *out << "</body>"                                             << std::endl;
    *out << "</html>"                                             << std::endl;
}

void rubuilder::ta::Application::debugWebPage
(
    xgi::Input  *in,
    xgi::Output *out
)
throw (xgi::exception::Exception)
{
    out->getHTTPResponseHeader().addHeader("Content-Type", "text/html");

    *out << "<html>"                                              << std::endl;
    *out << "<head>"                                              << std::endl;
    *out << "<link type=\"text/css\" rel=\"stylesheet\"";
    *out << " href=\"/" << urn_ << "/styles.css\"/>"              << std::endl;
    *out << "<title>"                                             << std::endl;
    *out << xmlClass_ << instance_ << " DEBUG"                    << std::endl;
    *out << "</title>"                                            << std::endl;
    *out << "</head>"                                             << std::endl;
    *out << "<body>"                                              << std::endl;

    printWebPageTitleBar
    (
        out,
        rubuilder::ta::DBG_ICON,
        "Debug",
        rubuilder::ta::APP_ICON,
        rubuilder::ta::DBG_ICON,
        rubuilder::ta::FSM_ICON
    );

    *out << "<table>"                                             << std::endl;
    *out << "<tr valign=\"top\">"                                 << std::endl;
    *out << "  <td>"                                              << std::endl;
    try
    {
        printParamsTable(in, out, "Debug configuration", dbgConfigParams_);
    }
    catch(xcept::Exception &e)
    {
        XCEPT_RETHROW(xgi::exception::Exception,
            "Failed to print debug configuration table", e);
    }
    *out << "  </td>"                                             << std::endl;
    *out << "  <td width=\"64\">"                                 << std::endl;
    *out << "  </td>"                                             << std::endl;
    *out << "  <td>"                                              << std::endl;
    try
    {
        printParamsTable(in, out, "Debug monitoring", dbgMonitorParams_);
    }
    catch(xcept::Exception &e)
    {
        XCEPT_RETHROW(xgi::exception::Exception,
            "Failed to print debug monitoring table", e);
    }
    *out << "  </td>"                                             << std::endl;
    *out << "</tr>"                                               << std::endl;
    *out << "</table>"                                            << std::endl;

    std::string reasonForFailed = reasonForFailed_.getValue();

    if(reasonForFailed == "")
    {
        reasonForFailed = "N/A";
    }

    *out << "<p>"                                                 << std::endl;
    *out << "<table frame=\"void\" rules=\"rows\" class=\"params\">";
    *out << std::endl;
    *out << "  <tr>"                                              << std::endl;
    *out << "    <th>Reason state machine moved to \"Failed\"</th>";
    *out << std::endl;
    *out << "  </tr>"                                             << std::endl;
    *out << "  <tr>"                                              << std::endl;
    *out << "    <td>" << reasonForFailed << "</td>"              << std::endl;
    *out << "  </tr>"                                             << std::endl;
    *out << "</table>"                                            << std::endl;
    *out << "</p>"                                                << std::endl;

    *out << "</body>"                                             << std::endl;
    *out << "</html>"                                             << std::endl;
}


void rubuilder::ta::Application::printParamsTable
(
    xgi::Input                                                  *in,
    xgi::Output                                                 *out,
    const std::string                                           name,
    std::vector< std::pair<std::string, xdata::Serializable*> > &params
)
throw (xgi::exception::Exception)
{
    *out << "<table frame=\"void\" rules=\"rows\" class=\"params\">";
    *out << std::endl;

    *out << "  <tr>"                                              << std::endl;
    *out << "    <th colspan=2>"                                  << std::endl;
    *out << "      " << name                                      << std::endl;
    *out << "    </th>"                                           << std::endl;
    *out << "  </tr>"                                             << std::endl;


    std::vector
    <
        std::pair<std::string, xdata::Serializable*>
    >::const_iterator itor;

    for(itor=params.begin(); itor!=params.end(); itor++)
    {
        *out << "  <tr>"                                          << std::endl;

        // Name
        *out << "    <td>"                                        << std::endl;
        *out << "      " << itor->first                           << std::endl;
        *out << "    </td>"                                       << std::endl;

        // Value
        *out << "    <td>"                                        << std::endl;

        std::string str;

        try
        {
            str = itor->second->toString();
        }
        catch(xcept::Exception &e)
        {
            str = e.what();
        }
        *out << "      " << str << std::endl;

        *out << "    </td>"                                       << std::endl;

        *out << "  </tr>"                                         << std::endl;
    }

    *out << "</table>"                                            << std::endl;
}


xoap::MessageReference rubuilder::ta::Application::resetMonitoringCountersMsg
(
    xoap::MessageReference msg
)
throw (xoap::exception::Exception)
{
    applicationBSem_.take();
    resetCounters(monitorCounters_);
    applicationBSem_.give();

    std::string responseString = "resetMonitoringCountersResponse";

    try
    {
        xoap::MessageReference message = xoap::createMessage();
        xoap::SOAPEnvelope envelope = message->getSOAPPart().getEnvelope();
        xoap::SOAPBody body = envelope.getBody();
        xoap::SOAPName responseName =
            envelope.createName(responseString, "xdaq", XDAQ_NS_URI);

        body.addBodyElement(responseName);

        return message;
    }
    catch(xcept::Exception &e)
    {
        std::stringstream oss;

        oss << "Failed to create " << responseString << " message";

        XCEPT_RETHROW(xoap::exception::Exception, oss.str(), e);
    }
}


void rubuilder::ta::Application::configureAction(toolbox::Event::Reference e)
throw (toolbox::fsm::exception::Exception)
{
    try
    {
        getAppDescriptorsAndTids();
    }
    catch(xcept::Exception &e)
    {
        XCEPT_RETHROW(toolbox::fsm::exception::Exception,
            "Failed to get application descriptors and tids", e);
    }
}


void rubuilder::ta::Application::getAppDescriptorsAndTids()
throw (rubuilder::ta::exception::Exception)
{
    try
    {
        tid_ = i2oAddressMap_->getTid(appDescriptor_);
    }
    catch(xcept::Exception &e)
    {
        XCEPT_RETHROW(rubuilder::ta::exception::Exception,
            "Failed to get the I2O TID of this application", e);
    }

    // If the instance number of the EVM has not been given
    if(evmInstance_.value_ < 0)
    {
        // Try to find instance number by assuming the first EVM found is the
        // one to be used.

        std::set< xdaq::ApplicationDescriptor* > evms;

        try
        {
            evms =
                getApplicationContext()->
                getDefaultZone()->
                getApplicationDescriptors("rubuilder::evm::Application");
        }
        catch(xcept::Exception &e)
        {
            XCEPT_RETHROW(rubuilder::ta::exception::Exception,
                "Failed to get EVM application descriptor", e);
        }

        if(evms.size() == 0)
        {
            XCEPT_RAISE(rubuilder::ta::exception::Exception,
                "Failed to get EVM application descriptor");
        }

        evmDescriptor_ = *(evms.begin());
        evmInstance_   = evmDescriptor_->getInstance();
    }
    else
    {
        try
        {
            evmDescriptor_ =
                getApplicationContext()->
                getDefaultZone()->
                getApplicationDescriptor("rubuilder::evm::Application",
                    evmInstance_.value_);
        }
        catch(xcept::Exception &e)
        {
            std::stringstream oss;

            oss << "Failed to get application descriptor of EVM";
            oss << evmInstance_.toString();

            XCEPT_RETHROW(rubuilder::ta::exception::Exception, oss.str(), e);
        }
    }

    try
    {
        evmTid_ = i2oAddressMap_->getTid(evmDescriptor_);
    }
    catch(xcept::Exception &e)
    {
        XCEPT_RETHROW(rubuilder::ta::exception::Exception,
            "Failed to get the I2O TID of the EVM", e);
    }
}


void rubuilder::ta::Application::enableAction(toolbox::Event::Reference e)
throw (toolbox::fsm::exception::Exception)
{
    // If there are some held credits
    if(nbCreditsHeld_.value_ != 0)
    {
        // Send triggers for the held credits
        try
        {
            sendNTriggers(nbCreditsHeld_);
        }
        catch(xcept::Exception &e)
        {
            std::stringstream oss;

            oss << "Failed to send " << nbCreditsHeld_ << " triggers";

            XCEPT_RETHROW(toolbox::fsm::exception::Exception, oss.str(), e);
        }

        nbCreditsHeld_ = 0;  // Credits have been used up
    }
}


void rubuilder::ta::Application::suspendAction(toolbox::Event::Reference e)
throw (toolbox::fsm::exception::Exception)
{
    // Do nothing
}


void rubuilder::ta::Application::resumeAction(toolbox::Event::Reference e)
throw (toolbox::fsm::exception::Exception)
{
    // If there are some held credits
    if(nbCreditsHeld_.value_ != 0)
    {
        // Send triggers for the held credits
        try
        {
            sendNTriggers(nbCreditsHeld_);
        }
        catch(xcept::Exception &e)
        {
            std::stringstream oss;

            oss << "Failed to send " << nbCreditsHeld_ << " triggers";

            XCEPT_RETHROW(toolbox::fsm::exception::Exception, oss.str(), e);
        }

        nbCreditsHeld_ = 0;  // Credits have been used up
    }
}


void rubuilder::ta::Application::haltAction(toolbox::Event::Reference e)
throw (toolbox::fsm::exception::Exception)
{
    // Reset the dummy event number
    eventNumber_ = 1;

    // Reset the number of credits held
    nbCreditsHeld_ = 0;
}


void rubuilder::ta::Application::failAction(toolbox::Event::Reference event)
throw (toolbox::fsm::exception::Exception)
{
    std::string reason       = "";
    std::string xceptMessage = "";
    std::string errorMessage = "";


    // Determine reason for failure if there is one
    if(typeid(*event) == typeid(toolbox::fsm::FailedEvent))
    {
        toolbox::fsm::FailedEvent &failedEvent =
            dynamic_cast<toolbox::fsm::FailedEvent&>(*event);
        xcept::Exception exception = failedEvent.getException();

        std::stringstream oss;

        oss << "Failure occurred when performing transition from: ";
        oss << failedEvent.getFromState();
        oss <<  " to: ";
        oss << failedEvent.getToState();

        reason = oss.str();

        oss << " exception history: ";
        oss << xcept::stdformat_exception_history(exception);
	
	xceptMessage = oss.str();
    }
    else if(typeid(*event) == typeid(rubuilder::ta::ForceFailedEvent))
    {
        rubuilder::ta::ForceFailedEvent &forceFailedEvent =
            dynamic_cast<rubuilder::ta::ForceFailedEvent&>(*event);

        reason = forceFailedEvent.getReason();
    }

    // Update web-page data
    reasonForFailed_.setValue(reason);

    // Prepare error message for logging and the sentinel
    if(reason != "")
    {
        errorMessage = "Moved to Failed state: " + reason;
    }
    else
    {
        errorMessage = "Moved to Failed state";
    }

    LOG4CPLUS_FATAL(logger_, errorMessage << xceptMessage);

    // Notify the sentinel
    XCEPT_DECLARE(rubuilder::ta::exception::Exception, sentinelException,
        errorMessage);
    this->notifyQualified("fatal",sentinelException);
}


void rubuilder::ta::Application::sendNTriggers(const unsigned int n)
throw (rubuilder::ta::exception::Exception)
{
    toolbox::mem::Reference *bufRef = 0;
    unsigned int            i       = 0;


    for(i=0; i<n; i++)
    {
        bufRef = triggerGenerator_.generate
        (
            poolFactory_,            // poolFactory
            triggerPool_,            // pool
            tid_,                    // initiatorAddress
            evmTid_,                 // targetAddress
            triggerSourceId_.value_, // triggerSourceId
            eventNumber_.value_,     // eventNumber
            fedPayloadSize_.value_   // fedPayloadSize
        );

        // Update parameters showing message payloads and counts
        {
            I2O_MESSAGE_FRAME *stdMsg =
                (I2O_MESSAGE_FRAME*)bufRef->getDataLocation();

            I2O_EVM_TRIGGER_Payload_.value_ += (stdMsg->MessageSize << 2) -
                sizeof(I2O_EVENT_DATA_BLOCK_MESSAGE_FRAME);
            I2O_EVM_TRIGGER_LogicalCount_.value_++;
            I2O_EVM_TRIGGER_I2oCount_.value_++;
        }

        try
        {
            appContext_->postFrame
            (
                bufRef,
                appDescriptor_,
                evmDescriptor_,
                i2oExceptionHandler_,
                evmDescriptor_
            );
        }
        catch(xcept::Exception &e)
        {
            std::stringstream oss;

            oss << "Failed to send dummy trigger";
            oss << " (eventNumber=" << eventNumber_ << ")";

            XCEPT_RETHROW(rubuilder::ta::exception::Exception, oss.str(), e);
        }
        catch(...)
        {
            std::stringstream oss;

            oss << "Failed to send dummy trigger";
            oss << " (eventNumber=" << eventNumber_ << ")";
            oss << " : Unknown exception";

            XCEPT_RAISE(rubuilder::ta::exception::Exception, oss.str());
        }

        // Increment the event number taking into account the event number of
        // CMS will be 24-bits (2 to the power of 24 = 16777216)
        eventNumber_ = (eventNumber_ + 1) % 16777216;
    }
}


void rubuilder::ta::Application::taCreditMsg(toolbox::mem::Reference *bufRef)
{
    I2O_TA_CREDIT_MESSAGE_FRAME *msg =
        (I2O_TA_CREDIT_MESSAGE_FRAME*)bufRef->getDataLocation();


    applicationBSem_.take();

    // Update parameters showing message payloads and counts
    I2O_TA_CREDIT_Payload_.value_ += sizeof(I2O_TA_CREDIT_MESSAGE_FRAME) -
        sizeof(I2O_PRIVATE_MESSAGE_FRAME);
    I2O_TA_CREDIT_LogicalCount_.value_ += msg->nbCredits;
    I2O_TA_CREDIT_I2oCount_.value_++;

    try
    {
        switch(fsm_.getCurrentState())
        {
        case 'H': // Halted
        case 'F': // Failed
            break;
        case 'E': // Enabled
            sendNTriggers(msg->nbCredits);
            break;
        case 'R': // Ready
        case 'S': // Suspended
            nbCreditsHeld_.value_ += msg->nbCredits;  // Hold credits
            break;
        default:
            LOG4CPLUS_ERROR(logger_,
                "TA in undefined state");
        }
    }
    catch(xcept::Exception &e)
    {
        LOG4CPLUS_ERROR(logger_,
            "Failed to process trigger credit message : "
             << stdformat_exception_history(e));
    }
    catch(...)
    {
        LOG4CPLUS_ERROR(logger_,
            "Failed to process trigger credit message : Unknown exception");
    }

    applicationBSem_.give();

    // Free the trigger credits message
    bufRef->release();
}


std::string rubuilder::ta::Application::getUrl(xdaq::ApplicationDescriptor *appDescriptor)
{
    std::string url;


    url  = appDescriptor->getContextDescriptor()->getURL();
    url += "/";
    url += appDescriptor->getURN();

    return url;
}


bool rubuilder::ta::Application::onI2oException(xcept::Exception &exception, void *context)
{
    xdaq::ApplicationDescriptor *destDescriptor =
        (xdaq::ApplicationDescriptor *)context;
    std::string errorMessage =
        createI2oErrorMsg(appDescriptor_, destDescriptor);


    XCEPT_DECLARE_NESTED(rubuilder::ta::exception::Exception, sentinelException, errorMessage, exception);
    this->notifyQualified("error",sentinelException);

    LOG4CPLUS_ERROR(logger_,
        errorMessage << " : " << xcept::stdformat_exception_history(exception));

    return true;
}


std::string rubuilder::ta::Application::createI2oErrorMsg
(
    xdaq::ApplicationDescriptor *source,
    xdaq::ApplicationDescriptor *destination
)
{
    std::stringstream oss;


    oss << "I2O exception from ";
    oss << source->getClassName();
    oss << " instance ";
    oss << source->getInstance();
    oss << " to ";
    oss << destination->getClassName();
    oss << " instance ";
    oss << destination->getInstance();

    return oss.str();
}


void rubuilder::ta::Application::printWebPageTitleBar
(
    xgi::Output       *out,
    const std::string pageIconSrc,
    const std::string pageIconAlt,
    const std::string appIconSrc,
    const std::string dbgIconSrc,
    const std::string fsmIconSrc
)
throw (xgi::exception::Exception)
{
    std::time_t rawtime;  // Time at which the page is being updated
    std::time(&rawtime);

    *out << "<table border=\"0\" width=\"100%\">"                 << std::endl;
    *out << "<tr>"                                                << std::endl;
    *out << "  <td width=\"64\">"                                 << std::endl;
    *out << "    <img"                                            << std::endl;
    *out << "     align=\"middle\""                               << std::endl;
    *out << "     src=\"" << pageIconSrc << "\""                  << std::endl;
    *out << "     alt=\"" << pageIconAlt << "\""                  << std::endl;
    *out << "     width=\"64\""                                   << std::endl;
    *out << "     height=\"64\""                                  << std::endl;
    *out << "     border=\"\"/>"                                  << std::endl;
    *out << "  </td>"                                             << std::endl;
    *out << "  <td align=\"left\">"                               << std::endl;
    *out << "    <b>" << xmlClass_ << instance_ << " " << stateName_.toString();
    *out << "</b><br/>"                                           << std::endl;
    *out << "    <a href=\"/" << urn_ << "/ParameterQuery\">XML</a>";
    *out << "<br/>"                                               << std::endl;
    *out << "    Page last updated: ";
    *out << std::asctime(std::gmtime(&rawtime)) << " UTC"         << std::endl;
    *out << "  </td>"                                             << std::endl;
    *out << "  <td class=\"app_links\" align=\"center\" width=\"70\">";
    *out << std::endl;
    printWebPageIcon(out, "/hyperdaq/images/HyperDAQ.jpg", "HyperDAQ",
        "/urn:xdaq-application:service=hyperdaq");
    *out << "  </td>"                                             << std::endl;

    if(rubuilderTesterDescriptor_ != 0)
    {
        *out << "  <td width=\"32\">"                             << std::endl;
        *out << "  </td>"                                         << std::endl;
        *out << "  <td class=\"app_links\" align=\"center\" width=\"64\">"
       << std::endl;
        printWebPageIcon(out, TESTER_ICON, "Tester",
            getUrl(rubuilderTesterDescriptor_));
        *out << "  </td>"                                         << std::endl;
    }

    *out << "  <td width=\"32\">"                                 << std::endl;
    *out << "  </td>"                                             << std::endl;
    *out << "  <td class=\"app_links\" align=\"center\" width=\"64\">";
    *out << std::endl;
    printWebPageIcon(out, fsmIconSrc, "FSM", "/" + urn_ + "/fsm");
    *out << "  </td>"                                             << std::endl;

    *out << "  <td width=\"32\">"                                 << std::endl;
    *out << "  </td>"                                             << std::endl;
    *out << "  <td class=\"app_links\" align=\"center\" width=\"64\">";
    *out << std::endl;
    printWebPageIcon(out, dbgIconSrc, "Debug", "/" + urn_ + "/debug");
    *out << "  </td>"                                             << std::endl;

    *out << "  <td width=\"32\">"                                 << std::endl;
    *out << "  </td>"                                             << std::endl;
    *out << "  <td class=\"app_links\" align=\"center\" width=\"64\">";
    *out << std::endl;
    printWebPageIcon(out, appIconSrc, "Main", "/" + urn_);
    *out << "  </td>"                                             << std::endl;

    *out << "</tr>"                                               << std::endl;
    *out << "</table>"                                            << std::endl;

    *out << "<hr/>"                                               << std::endl;
}


void rubuilder::ta::Application::printWebPageIcon
(
    xgi::Output       *out,
    const std::string imgSrc,
    const std::string label,
    const std::string href
)
throw (xgi::exception::Exception)
{
    *out << "<center><a href=\"" << href << "\">";
    *out << "<img";
    *out << " src=\"" << imgSrc << "\"";
    *out << " alt=\"" << label << "\"";
    *out << " width=\"64\"";
    *out << " height=\"64\"";
    *out << " border=\"0\"/>";
    *out << "</a><br/>"                                           << std::endl;
    *out << "<a href=\"" << href << "\">" << label << "</a></center>";
    *out << std::endl;
}


/**
 * Provides the factory method for the instantiation of TA applications.
 */
XDAQ_INSTANTIATOR_IMPL(rubuilder::ta::Application)

