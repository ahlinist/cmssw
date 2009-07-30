#ifndef _rubuilder_ta_Application_h_
#define _rubuilder_ta_Application_h_

#include "EventFilter/TriggerAdapter/interface/SynchronizedString.h"
#include "EventFilter/TriggerAdapter/interface/TriggerGenerator.h"
#include "EventFilter/TriggerAdapter/interface/Exception.h"
#include "i2o/i2oDdmLib.h"
#include "i2o/utils/AddressMap.h"
#include "toolbox/BSem.h"
#include "toolbox/fsm/FiniteStateMachine.h"
#include "toolbox/mem/MemoryPoolFactory.h"
#include "xdaq/ApplicationGroup.h"
#include "xdaq/WebApplication.h"
#include "xdaq2rc/SOAPParameterExtractor.hh"
#include "xdata/Boolean.h"
#include "xdata/Double.h"
#include "xdata/InfoSpace.h"
#include "xdata/Integer32.h"
#include "xdata/String.h"
#include "xdata/UnsignedInteger32.h"


namespace rubuilder { namespace ta { // namespace rubuilder::ta

/**
 * Example trigger adapter (TA) to be copied and modified by end-users.
 */
class Application :
public xdaq::WebApplication
{
public:

    /**
     * Define factory method for the instantion of TA applications.
     */
    XDAQ_INSTANTIATOR();

    /**
     * Constructor.
     */
    Application(xdaq::ApplicationStub *s)
    throw (xdaq::exception::Exception);
    
    virtual ~Application(){}

protected:

    /**
     * Sends n triggers to the EVM.
     */
    virtual void sendNTriggers(const unsigned int n)
    throw (rubuilder::ta::exception::Exception);


    /**
     * Callback implementing the action to be executed on the
     * Ready->Enabled transition.
     */
    virtual void enableAction(toolbox::Event::Reference e)
    throw (toolbox::fsm::exception::Exception);

    /**
     * Callback implementing the action to be executed on the
     * ANY STATE->Halted transition.
     */
    virtual void haltAction(toolbox::Event::Reference e)
    throw (toolbox::fsm::exception::Exception);


    /**
     * The default web page of the application.
     */
    virtual void defaultWebPage(xgi::Input *in, xgi::Output *out)
    throw (xgi::exception::Exception);


    /**
     * Prints the specified parameters as an HTML table with the specified name.
     */
    void printParamsTable
    (
        xgi::Input                                                  *in,
        xgi::Output                                                 *out,
        const std::string                                           name,
        std::vector< std::pair<std::string, xdata::Serializable*> > &params
    )
    throw (xgi::exception::Exception);


    /**
     * The application's standard configuration parameters.
     */
    std::vector< std::pair<std::string, xdata::Serializable *> >
        stdConfigParams_;

    /**
     * The application's debug (private to developer) configuration parameters.
     */
    std::vector< std::pair<std::string, xdata::Serializable *> >
        dbgConfigParams_;

    /**
     * The application's monitor counters.
     */
    std::vector< std::pair<std::string, xdata::UnsignedInteger32 *> >
        monitorCounters_;

    /**
     * The application's standard monitoring parameters.
     */
    std::vector< std::pair<std::string, xdata::Serializable *> >
        stdMonitorParams_;

    /**
     * The application's debug (private to developer) monitoring parameters.
     */
    std::vector< std::pair<std::string, xdata::Serializable *> >
        dbgMonitorParams_;

    /**
     * I2o exception handler.
     */
    toolbox::exception::HandlerSignature *i2oExceptionHandler_;


    /**
     * Exported read-only parameter specifying the next trigger event number.
     */
    xdata::UnsignedInteger32 eventNumber_;

    /**
     * Used to access the memory pool factory without a function call.
     */
    toolbox::mem::MemoryPoolFactory *poolFactory_;

    /**
     * Memory pool from which buffers for dummy triggers are taken.
     */
    toolbox::mem::Pool *triggerPool_;

    /**
     * The I2O tid of the application.
     */
    I2O_TID tid_;

    /**
     * The I2O tid of the EVM.
     */
    I2O_TID evmTid_;

    /**
     * Exported read/write parameter specifying the trigger source id that is
     * to be put into each dummy trigger.
     */
    xdata::UnsignedInteger32 triggerSourceId_;

    /**
     * Exported read-only parameter specifying the payload transfered
     * by I2O_EVM_TRIGGER messages.
     */
    xdata::UnsignedInteger32 I2O_EVM_TRIGGER_Payload_;

    /**
     * Exported read-only parameter specifying the logical message
     * count for I2O_EVM_TRIGGER messages.
     */
    xdata::UnsignedInteger32 I2O_EVM_TRIGGER_LogicalCount_;

    /**
     * Exported read-only parameter specifying the i2o message frame
     * count for I2O_EVM_TRIGGER messages.
     */
    xdata::UnsignedInteger32 I2O_EVM_TRIGGER_I2oCount_;

    /**
     * Used to access the application's descriptor without a function call.
     */
    xdaq::ApplicationDescriptor *appDescriptor_;

    /**
     * Used to access the application's context without a function call.
     */
    xdaq::ApplicationContext *appContext_;

    /**
     * The application descriptor of the EVM.
     */
    xdaq::ApplicationDescriptor *evmDescriptor_;

    /**
     * Pointer to the descriptor of the RUBuilderTester application.
     *
     * It is normal for this pointer to be 0 if the RUBuilderTester application      * cannot be found.
     */
    xdaq::ApplicationDescriptor *rubuilderTesterDescriptor_;

    /**
     * The logger of this application.
     */
    Logger logger_;

    /**
     * The name of the info space that contains exported parameters used for
     * monitoring.
     */
    std::string monitoringInfoSpaceName_;

    /**
     * Info space that contains exported parameters used for monitoring.
     */
    xdata::InfoSpace *monitoringInfoSpace_;

    /**
     * Binary semaphore used to protect the internal data structures of the
     * application from multithreaded access.
     */
    toolbox::BSem applicationBSem_;

    /**
     * Used to display on the application web-pages the reason why the finite
     * state machine moved to the failed state if it did.
     */
    SynchronizedString reasonForFailed_;

    /**
     * The finite state machine of the application.
     */
    toolbox::fsm::FiniteStateMachine fsm_;

    /**
     * The generator of dummy triggers.
     */
    rubuilder::ta::TriggerGenerator triggerGenerator_;

    /**
     * Helper class to extract FSM command and parameters from SOAP command
     */
    xdaq2rc::SOAPParameterExtractor soapParameterExtractor_;

    /**
     * Used to access the I2O address map without a function call.
     */
    i2o::utils::AddressMap *i2oAddressMap_;


    /**
     * Used to access the application's info space without a function call.
     */
    xdata::InfoSpace *appInfoSpace_;

    /**
     * The XML class name of the application.
     */
    std::string xmlClass_;

    /**
     * The instance number of the application.
     */
    unsigned int instance_;

    /**
     * The application's URN.
     */
    std::string urn_;

    ////////////////////////////////////////////////////////
    // Beginning of exported parameters for configuration //
    ////////////////////////////////////////////////////////

    /**
     * Exported read/write parameter specifying the instance number of the
     * EVM.
     *
     * A negative value is interpretted as meaning this parameter has not been
     * set.
     *
     * If this parameter is not set when the TA is configured, then the TA
     * will attempt to find the instance number itself.
     */
    xdata::Integer32 evmInstance_;


    /**
     * Exported read/write parameter specifying the FED payload of dummy
     * triggers in bytes.  The FED payload must be a multiple of 8 bytes.
     */
    xdata::UnsignedInteger32 fedPayloadSize_;

    ////////////////////////////////////////////////////////
    // End of exported parameters for configuration       //
    ////////////////////////////////////////////////////////


    //////////////////////////////////////////////////////////
    // Beginning of exported parameters used for monitoring //
    //////////////////////////////////////////////////////////

    /**
     * Exported read-only parameter specifying the current state of the
     * application.
     */
    xdata::String stateName_;

    /**
     * Exported read-only parameter specifying the number of trigger credits
     * currently held by the TA.
     */
    xdata::UnsignedInteger32 nbCreditsHeld_;


    //////////////////////////////////////////////////////////
    // End of exported parameters used for monitoring       //
    //////////////////////////////////////////////////////////


    //////////////////////////////////////////////////////////////////////////
    // Beginning of exported parameters showing message payloads and counts //
    //////////////////////////////////////////////////////////////////////////

    /**
     * Exported read-only parameter specifying the payload transfered
     * by I2O_TA_CREDIT messages.
     */
    xdata::UnsignedInteger32 I2O_TA_CREDIT_Payload_;

    /**
     * Exported read-only parameter specifying the logical message
     * count for I2O_TA_CREDIT messages.
     */
    xdata::UnsignedInteger32 I2O_TA_CREDIT_LogicalCount_;

    /**
     * Exported read-only parameter specifying the i2o message frame
     * count for I2O_TA_CREDIT messages.
     */
    xdata::UnsignedInteger32 I2O_TA_CREDIT_I2oCount_;


    //////////////////////////////////////////////////////////////////////////
    // End of exported parameters showing message payloads and counts       //
    //////////////////////////////////////////////////////////////////////////


    /////////////////////////////////////////////////////////
    // Beginning of exported parameters used for debugging //
    /////////////////////////////////////////////////////////

    /////////////////////////////////////////////////////////
    // End of exported parameters used for debugging       //
    /////////////////////////////////////////////////////////


    /**
     * Returns the name to be given to the logger of this application.
     */
    std::string generateLoggerName();

    /**
     * Returns a pointer to the descriptor of the RUBuilderTester application,
     * or 0 if the application cannot be found, which will be the case when
     * testing is not being performed.
     */
    xdaq::ApplicationDescriptor *getRUBuilderTester();	

    /**
     * Returns the name of the info space that contains exported parameters
     * for monitoring.
     */
    std::string generateMonitoringInfoSpaceName
    (
        const std::string   appClass,
        const unsigned int  appInstance
    );

    /**
     * Initialises and returns the application's standard configuration
     * parameters.
     */
    std::vector< std::pair<std::string, xdata::Serializable*> >
    initAndGetStdConfigParams();

    /**
     * Initialises and returns the application's debug (private to developer)
     * configuration parameters.
     */
    std::vector< std::pair<std::string, xdata::Serializable*> >
    initAndGetDbgConfigParams();

    /**
     * Resets the specified counters to zero.
     */
    void resetCounters
    (
        std::vector
        <
            std::pair<std::string, xdata::UnsignedInteger32*>
        > &counters
    );

    /**
     * Adds the specified counters vector to specified general parameters
     * vector.
     */
    void addCountersToParams
    (
        std::vector
        <
            std::pair<std::string, xdata::UnsignedInteger32*>
        > &counters,
        std::vector
        <
            std::pair<std::string, xdata::Serializable*>
        > &params
    );

    /**
     * Returns the application's monitor counters.
     */
    std::vector< std::pair<std::string, xdata::UnsignedInteger32*> >
    getMonitorCounters();

    /**
     * Initialises and returns the application's standard monitoring
     * parameters.
     */
    std::vector< std::pair<std::string, xdata::Serializable*> >
    initAndGetStdMonitorParams();

    /**
     * Initialises and returns the application's debug (private to developer)
     * monitoring parameters.
     */
    std::vector< std::pair<std::string, xdata::Serializable*> >
    initAndGetDbgMonitorParams();

    /**
     * Puts the specified parameters into the specified info space.
     */
    void putParamsIntoInfoSpace
    (
        std::vector< std::pair<std::string, xdata::Serializable*> > &params,
        xdata::InfoSpace                                            *s
    )
    throw (rubuilder::ta::exception::Exception);

    /**
     * Defines the finite state machine of the application.
     */
    void defineFsm()
    throw (rubuilder::ta::exception::Exception);

    /**
     * Binds the SOAP callbacks required to implement the finite state machine
     * of the application.
     */
    void bindFsmSoapCallbacks();

    /**
     * Web page used to visualise and control the finite state machine of the
     * application.
     */
    void fsmWebPage(xgi::Input *in, xgi::Output *out)
    throw (xgi::exception::Exception);

    /**
     * Processes the form sent from the finite state machine web page.
     */
    void processFsmForm(xgi::Input *in)
    throw (xgi::exception::Exception);

    /**
     * Callback responsible for processing FSM events received via
     * SOAP messages.
     */
    xoap::MessageReference processSoapFsmEvent(xoap::MessageReference msg)
    throw (xoap::exception::Exception);

    /**
     * Binds the I2O callbacks of the application.
     */
    void bindI2oCallbacks();

    /**
     * Binds the XGI callbacks of the application.
     */
    void bindXgiCallbacks();

    /**
     * Creates the CSS file for this application.
     */
    void css
    (
        xgi::Input  *in,
        xgi::Output *out
    )
    throw (xgi::exception::Exception);

    /**
     * Displays the developer's web page used for debugging.
     */
    void debugWebPage
    (
        xgi::Input  *in,
        xgi::Output *out
    )
    throw (xgi::exception::Exception);

    /**
     * SOAP callback thats reset all the monitoring counters.
     */
    xoap::MessageReference resetMonitoringCountersMsg
    (
        xoap::MessageReference msg
    )
    throw (xoap::exception::Exception);

    /**
     * Callback implementing the action to be executed on the
     * Halted->Ready transition.
     */
    void configureAction(toolbox::Event::Reference e)
    throw (toolbox::fsm::exception::Exception);

    /**
     * Gets the application descriptors and tids of this application and
     * those with which it communicates.
     */
    void getAppDescriptorsAndTids()
    throw (rubuilder::ta::exception::Exception);


    /**
     * Callback implementing the action to be executed on the
     * Enabled->Suspended transition.
     */
    void suspendAction(toolbox::Event::Reference e)
    throw (toolbox::fsm::exception::Exception);

    /**
     * Callback implementing the action to be executed on the
     * Suspended->Enabled transition.
     */
    void resumeAction(toolbox::Event::Reference e)
    throw (toolbox::fsm::exception::Exception);

    /**
     * Callback implementing the action to be executed on the
     * ANY STATE->Failed transition.
     */
    void failAction(toolbox::Event::Reference event)
    throw (toolbox::fsm::exception::Exception);

    /**
     * Callback invoked when the state machine of the application has changed.
     */
    void stateChanged(toolbox::fsm::FiniteStateMachine & fsm)
    throw (toolbox::fsm::exception::Exception);


    /**
     * I2O callback routine invoked when a trigger credit count has been
     * received from the EVM.
     */
    void taCreditMsg(toolbox::mem::Reference *bufRef);

    /**
     * Returns the url of the specified application.
     */
    std::string getUrl(xdaq::ApplicationDescriptor *appDescriptor);

    /**
     * Invoked when there is an I2O exception.
     */
    bool onI2oException(xcept::Exception &exception, void *context);

    /**
     * Creates and returns the error message of an I2O exception by specifying
     * the source and destination involved.
     */
    std::string createI2oErrorMsg
    (
        xdaq::ApplicationDescriptor *source,
        xdaq::ApplicationDescriptor *destination
    );

    /**
     * Prints the title bar found at the top of each web page.
     */
    void printWebPageTitleBar
    (
        xgi::Output       *out,
        const std::string pageIconSrc,
        const std::string pageIconAlt,
        const std::string appIconSrc,
        const std::string dbgIconSrc,
        const std::string fsmIconSrc
    )
    throw (xgi::exception::Exception);

    /**
     * Prints one of the icons used to navigate this application's web site.
     */
    void printWebPageIcon
    (
        xgi::Output       *out,
        const std::string imgSrc,
        const std::string label,
        const std::string href
    )
    throw (xgi::exception::Exception);

}; // class Application

} } // namespace rubuilder::ta

#endif
