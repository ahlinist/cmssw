#ifndef CondCore_IOVService_IOVProxy_h
#define CondCore_IOVService_IOVProxy_h

#include "CondCore/DBCommon/interface/DbSession.h"

#include "CondFormats/Common/interface/IOVSequence.h"
#include "CondFormats/Common/interface/SequenceState.h"
#include "CondFormats/Common/interface/Time.h"

//
#include <boost/shared_ptr.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include <boost/iterator/counting_iterator.hpp>

namespace cond {
  
  class IOVSequence;

  struct IOVProxyData {
    explicit IOVProxyData( cond::DbSession& dbs ) : 
      dbSession(dbs),
      data(), 
      token(""){
    }
      
    IOVProxyData(cond::DbSession& dbs,
		 const std::string & tok) : 
      dbSession(dbs),
      data(), 
      token(tok){
      refresh();
    }

    ~IOVProxyData(){  
    }
      
    void refresh(); 
     
    std::pair<int,int> range( cond::Time_t since, cond::Time_t  till );

    cond::DbSession dbSession;
    boost::shared_ptr<cond::IOVSequence> data;
    std::string token;
  };
  
  class IOVElementProxy {
  public:
    IOVElementProxy() : 
      m_since(cond::invalidTime),
      m_till(cond::invalidTime),
      m_token("") {}
    
    IOVElementProxy(cond::Time_t is, cond::Time_t it, std::string const& itoken):
      m_since(is), 
      m_till(it), 
      m_token(itoken) {}
    
    void set(cond::Time_t is, cond::Time_t it, std::string const& itoken) {
      m_since=is; 
      m_till=it; 
      m_token=itoken;
    }
    
    void set(IOVSequence const & v, int i);
    
    cond::Time_t since() const {
      return m_since;
    }
    
    cond::Time_t till() const {
      return m_till;
    }
    
    std::string const & token() const {
      return m_token;
    }
 
  private:
    cond::Time_t m_since;
    cond::Time_t m_till;
    std::string m_token;
  };
    
  class IterHelp {
  public:
    typedef IOVElementProxy result_type;
    IterHelp() : iov(0){}
    IterHelp( IOVProxyData & in);
    
    result_type const & operator()(int i) const {
      if (iov) elem.set(*iov,i);
      return elem;
    }
    
  private:
    IOVSequence const * iov;
    mutable IOVElementProxy elem;
  };

  typedef boost::transform_iterator<IterHelp,boost::counting_iterator<int> > iov_range_iterator;

  class IOVRange {
  public:
    
    typedef iov_range_iterator const_iterator;

  public:
    IOVRange();
    
    IOVRange( const boost::shared_ptr<IOVProxyData>& iov, int low, int high );
    
    IOVRange( const IOVRange& rhs );
    
    IOVRange& operator=( const IOVRange& rhs );
    
    const_iterator begin() const {
      return  boost::make_transform_iterator(boost::counting_iterator<int>(m_low),
					     IterHelp(*m_iov));
    }
    
    const_iterator end() const {
      return  boost::make_transform_iterator(boost::counting_iterator<int>(m_high),
					     IterHelp(*m_iov));
    }
    
    const_iterator find(cond::Time_t time) const;

    IOVElementProxy front() const;

    IOVElementProxy back() const;

    size_t size() const;

  private:
    boost::shared_ptr<IOVProxyData> m_iov;
    int m_low;
    int m_high;
  };

  /* IOV as the user wants it
   */
  class IOVProxy {
  public:
    typedef iov_range_iterator const_iterator;

  public:  
    IOVProxy();

    ~IOVProxy();
    
    explicit IOVProxy(cond::DbSession& dbSession);

    IOVProxy(cond::DbSession& dbSession, const std::string & token );

    IOVProxy( boost::shared_ptr<IOVProxyData>& data );

    IOVProxy( const IOVProxy& rhs );

    IOVProxy& operator=( const IOVProxy& rhs );

    void load( const std::string & token );

    bool refresh();

    const std::string& token(); 
    
    bool isValid( cond::Time_t currenttime );

    std::pair<cond::Time_t, cond::Time_t> validity( cond::Time_t currenttime );
    
    const_iterator begin() const {
      return  boost::make_transform_iterator(boost::counting_iterator<int>(0),
					     IterHelp(*m_iov));
    }
    
    const_iterator end() const {
      return  boost::make_transform_iterator(boost::counting_iterator<int>(size()),
					     IterHelp(*m_iov));
    }

    // find 
    const_iterator find(cond::Time_t time) const;

    // limit to the first n 
    IOVRange head(int n) const;

    // limit to the last n
    IOVRange tail(int n) const;

    IOVRange range(cond::Time_t since, cond::Time_t  till) const;

    IOVRange rangeHead(cond::Time_t since, cond::Time_t  till, int n) const;

    IOVRange rangeTail(cond::Time_t since, cond::Time_t  till, int n) const;

    int size() const;

    IOVSequence const & iov() const;
    
    TimeType timetype() const;

    cond::Time_t firstSince() const;

    cond::Time_t lastTill() const;

    std::set<std::string> const& payloadClasses() const;

    std::string comment() const;

    int revision() const;

    cond::Time_t timestamp() const;

    SequenceState state() const {
      return SequenceState(iov());
    }

    DbSession& db() const;

  private:
    boost::shared_ptr<IOVProxyData> m_iov;
    
  };
}

#endif // CondCore_IOVService_IOVProxy_h
