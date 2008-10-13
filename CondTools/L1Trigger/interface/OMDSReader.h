#ifndef CondTools_L1Trigger_OMDSReader_h
#define CondTools_L1Trigger_OMDSReader_h
// -*- C++ -*-
//
// Package:     L1Trigger
// Class  :     OMDSReader
// 
/**\class OMDSReader OMDSReader.h CondTools/L1Trigger/interface/OMDSReader.h

 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Original Author:  Werner Sun
//         Created:  Sun Mar  2 01:36:06 CET 2008
// $Id: OMDSReader.h,v 1.4 2008/07/23 16:38:08 wsun Exp $
//

// system include files
#include <memory>
#include "boost/shared_ptr.hpp"

// user include files
#include "CondCore/DBCommon/interface/CoralTransaction.h"
#include "CondTools/L1Trigger/interface/DataManager.h"
#include "RelationalAccess/IQuery.h"
#include "CoralBase/AttributeList.h"
#include "CoralBase/AttributeSpecification.h"
#include "CoralBase/Attribute.h"

// forward declarations

namespace l1t
{

  class OMDSReader : public DataManager
  {

  public:
    // std::vector< std::string > is the list of attribute names.
    // We need to store the column names because there is no way to ask
    // AttributeList for this information.  We have a vector of AttributeLists
    // because the query may return more than one row, each of which is
    // encapsulated in an AttributeList.
    class QueryResults
	{
	public:
	  QueryResults() {}
	  QueryResults( const std::vector< std::string >& columnNames,
			const std::vector< coral::AttributeList >& attLists )
	    : m_columnNames( columnNames), m_attributeLists( attLists ) {}

	  virtual ~QueryResults() {}

	  const std::vector< std::string >& columnNames() const
	    { return m_columnNames ; }
	  const std::vector< coral::AttributeList >& attributeLists() const
	    { return m_attributeLists ; }
	  bool queryFailed() const { return m_attributeLists.size() == 0 ; }
	  int numberRows() const { return m_attributeLists.size() ; }

	  // Return value is false if variable is null.
	  template< class T >
	    bool fillVariable( const std::string& columnName,
			       T& outputVariable ) const ;

	  template< class T >
	    bool fillVariableFromRow( const std::string& columnName,
				      int rowNumber,
				      T& outputVariable ) const ;

	  // If there is only one column, no need to give column name
	  template< class T >
	    bool fillVariable( T& outputVariable ) const ;

	  template< class T >
	    bool fillVariableFromRow( int rowNumber,
				      T& outputVariable ) const ;

	private:
	  std::vector< std::string > m_columnNames ;
	  std::vector< coral::AttributeList > m_attributeLists ;
	} ;

    OMDSReader( const std::string& connectString,
		const std::string& authenticationPath ) ;

    virtual ~OMDSReader();

      // ---------- const member functions ---------------------

      // These functions encapsulate basic SQL queries of the form
      //
      // SELECT <columns> FROM <schema.table> WHERE <conditionLHS> = <conditionRHS>
      //
      // where
      //
      // <columns> can be one or many column names
      // <conditionRHS> can be a string or the result of another query

      const QueryResults basicQuery(
	const std::vector< std::string >& columnNames,
	const std::string& schemaName, // for nominal schema, use ""
	const std::string& tableName,
	const std::string& conditionLHS = "",
	const QueryResults conditionRHS = QueryResults(),
	                                           // must have only one row
	const std::string& conditionRHSName = ""
	                 // if empty, conditionRHS must have only one column
	) const ;

      const QueryResults basicQuery(
	const std::string& columnName,
	const std::string& schemaName, // for nominal schema, use ""
	const std::string& tableName,
	const std::string& conditionLHS = "",
	const QueryResults conditionRHS = QueryResults(),
	                                           // must have only one row
	const std::string& conditionRHSName = ""
	                 // if empty, conditionRHS must have only one column
	) const ;

      template< class T >
	const QueryResults singleAttribute( const T& data ) const ;

      // ---------- static member functions --------------------

      // ---------- member functions ---------------------------

   private:
      OMDSReader(const OMDSReader&); // stop default

      const OMDSReader& operator=(const OMDSReader&); // stop default

      // ---------- member data --------------------------------
      cond::CoralTransaction* m_coralTransaction ;
};

  template< class T > const OMDSReader::QueryResults
  OMDSReader::singleAttribute( const T& data ) const
  {
    std::vector< std::string > names ;
    names.push_back( "dummy" ) ;

    coral::AttributeList attList ;
    attList.extend( "dummy", typeid( std::string ) ) ;
    attList[ "dummy" ].data< T >() = data ;

    std::vector< coral::AttributeList > atts ;
    atts.push_back( attList ) ;

    return QueryResults( names, atts ) ;
  }

  template< class T > bool
    OMDSReader::QueryResults::fillVariable(
      const std::string& columnName,
      T& outputVariable ) const
    {
      return fillVariableFromRow( columnName, 0, outputVariable ) ;
    }

  template< class T > bool
    OMDSReader::QueryResults::fillVariableFromRow(
      const std::string& columnName,
      int rowNumber,
      T& outputVariable ) const
    {
      // Check index in bounds
      if( rowNumber < 0 || rowNumber >= numberRows() ) return false ;
      const coral::AttributeList& row = m_attributeLists[ rowNumber ] ;
      if( row[ columnName ].isNull() ) return false ;
      outputVariable = row[ columnName ].template data< T >() ;
      return true ;
    }

  template< class T > bool
    OMDSReader::QueryResults::fillVariable( T& outputVariable ) const
    {
      return fillVariableFromRow( 0, outputVariable ) ;
    }

  template< class T > bool
    OMDSReader::QueryResults::fillVariableFromRow( int rowNumber,
						   T& outputVariable ) const
    {
      // Check index in bounds and only one column
      if( rowNumber < 0 || rowNumber >= numberRows() ||
	  m_columnNames.size() != 1 ) return false ;
      const coral::AttributeList& row = m_attributeLists[ rowNumber ] ;
      if( row[ m_columnNames.front() ].isNull() ) return false ;
      outputVariable = row[ m_columnNames.front() ].template data< T >() ;
      return true ;
    }

}
#endif
