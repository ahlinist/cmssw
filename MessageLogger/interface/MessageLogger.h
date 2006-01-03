#ifndef Services_MessageLogger_h
#define Services_MessageLogger_h

// -*- C++ -*-
//
// Package:     Services
// Class  :     MessageLogger
//
/**\class MessageLogger MessageLogger.h FWCore/Services/interface/MessageLogger.h

 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Original Author:  W. Brown and M. Fischler
//         Created:  Fri Nov 11 16:38:19 CST 2005
// $Id$
//

// system include files

// user include files

// forward declarations

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/ActivityRegistry.h"
#include "FWCore/MessageLogger/interface/MessageSender.h"
#include "FWCore/MessageLogger/interface/ErrorObj.h"
#include "FWCore/EDProduct/interface/EventID.h"

#include <memory>
#include <string>

namespace edm  {
namespace service  {


class MessageLogger
{
public:
  MessageLogger( ParameterSet const &, ActivityRegistry & );

  void  postBeginJob();
  void  postEndJob();

  void  preEventProcessing ( edm::EventID const &, edm::Timestamp const & );
  void  postEventProcessing( Event const &, EventSetup const & );

  void  preModule ( ModuleDescription const & );
  void  postModule( ModuleDescription const & );

  void  fillErrorObj(edm::ErrorObj& obj) const;

private:
  // put an ErrorLog object here, and maybe more

  edm::EventID curr_event_;
  std::string curr_module_;

};  // MessageLogger


}  // namespace service


class LogWarning
{
public:
  explicit LogWarning( ELstring const & id ) 
    : ap( new MessageSender(ELwarning,id) )
  { }

  template< class T >
    LogWarning & 
    operator<< (T const & t)  { (*ap) << t; return *this; }

private:
  std::auto_ptr<MessageSender> ap; 

};  // LogWarning


class LogError
{
public:
  explicit LogError( ELstring const & id ) 
    : ap( new MessageSender(ELerror,id) )
  { }

  template< class T >
    LogError & 
    operator<< (T const & t)  { (*ap) << t; return *this; }

private:
  std::auto_ptr<MessageSender> ap; 

};  // LogError


class LogInfo
{
public:
  explicit LogInfo( ELstring const & id ) 
    : ap( new MessageSender(ELinfo,id) )
  { }

  template< class T >
    LogInfo & 
    operator<< (T const & t)  { (*ap) << t; return *this; }

private:
  std::auto_ptr<MessageSender> ap; 

};  // LogInfo


class LogDebug_
{
public:
  explicit LogDebug_( ELstring const & id, std::string file, int line ) 
    : ap( new MessageSender(ELsuccess,id) )
  { *this << file << ':' << line << ' '; }

  template< class T >
    LogDebug_ & 
    operator<< (T const & t)  { (*ap) << t; return *this; }

private:
  std::auto_ptr<MessageSender> ap; 

};  // LogDebug_

inline bool ProbeLogError   ( ELstring const & id ) { return true; }
inline bool ProbeLogWarning ( ELstring const & id ) { return true; }
inline bool ProbeLogInfo    ( ELstring const & id ) { return true; }
inline bool ProbeLogDebug   ( ELstring const & id ) { return true; }

}  // namespace edm


#define LogDebug(id)  edm::LogDebug_(id, __FILE__, __LINE__)


#endif  // Services_MessageLogger_h
