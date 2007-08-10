#ifndef ICException_HH
#define ICException_HH

#include <list>
#include <exception>
#include <string>
#include <sstream>

#include "ICTypeDefs.hh"

namespace ICCoreUtils
{
 

  
  /*
   * macro for raising an exception of type ICException, or a
   * derivitive. 
   * 
   * Adds function, file and line information about the origin 
   * of the exception to the ICException history mechanism.
   *
   */
#define RAISE(__Exc_Object) \
if(true){__Exc_Object.AddLocation(__PRETTY_FUNCTION__,__FILE__,__LINE__);\
__Exc_Object.Raise();}else (void)0


  /*
   * macro for re-throwing a caught exception. 
   *
   * Adds function, file and line information about the origin 
   * of the exception to the IImaSException history mechanism.
   *
   */

#define RETHROW(___Exc_Object) \
if(true){___Exc_Object.AddLocation(__PRETTY_FUNCTION__,__FILE__,__LINE__);throw;}\
else (void)0


  
  /*
   * abstract base class for exceptions in the IC DAQ project. 
   *
   * you cannot create a ICException directly, you must derive 
   * a more meaningful error description. 
   *  
   *  
   * ICExampleException is an example of how this 
   * class may be implemented. The essential method
   * to overide is:
   *
   *  virtual void Raise()=0;
   *
   * the body is only required to contain
   *
   *    throw *this; 
   *
   * and ensures polymorphic throwing, which is essential
   * to the error reporting mechanism.
   *
   * m.noy 
   * Wed Apr  6 18:00:58 BST 2005
   * 
   */
  class ICException: public std::exception
  {
  public:
    
    /*
     * This is the only constructor that should be used.
     * The latter 4 arguments can be filled in automatically
     * by using the RAISE() macro declared above.
     *
     * Derived classes need to call this constructor explicitly
     * in the constructor list of the 
     *
     * added the streamers, and will make this have a default.
     */
    ICException(const std::string & aErrorDescription="");
    
    /*
     * Default destructor has nothing to do
     * except be declared virtual and non-throwing.
     *
     */
    virtual ~ICException()throw(){}
   


    /*
     * mechanism for raising an exception. 
     * -> must be implemented in the derived class as
     *    virtual inline void Raise(){throw *this;} 
     */
    virtual inline void Raise()=0;
    
    
    /*
     * mechanism for raising an exception
     * whilst adding the function, file and 
     * line info. Use through the RAISE() macro.
     *
     */
    void AddLocation(std::string aFunc,
		     std::string aFile,
		     int aLine);
    



    /*
     * implementation of the std::exception base class 
     * method returning a constant pointer to a c-style 
     * string contining the message, exception name and 
     * history of the exception.  
     *
     */
    virtual const char * what()const throw();
    
    /*
     * returns std::string contining the message, exception name and 
     * history of the exception.  
     *
     * this function also does formatting for the function: 
     * 
     *   virtual const char * what()const throw()
     *
     * which will keep changes in the formatting consistent.
     * 
     *  
     */
    virtual std::string What()const throw();


    /*
     * formats and returns a string containing the history
     * of the exception. 
     *
     */
    std::string History()const throw();


    /*
     * Returns a string of the name of 
     * the function where the exception was raised.
     *
     * returns the string "History unpopulated" if not 
     * recorded. 
     */
    std::string GetFunctionOfOrigin()const;

    /*
     * Returns a string of the file 
     * containing the function where the exception
     * was raised.
     * returns the string "History unpopulated" if not 
     * recorded. 
     */
    std::string GetFileOfOrigin()const;

    /*
     * returns the line of the file where 
     * the exception was raised. 
     * returns -1 if not recorded.  
     */
    int GetLineOfOrigin()const;
    
  protected:
    /*
     * template function for converting anything that 
     * has an overloaded insertion operator (<<) to 
     * string format. designed to work inline, just to 
     * convert data from T to std::string for the 
     * purposes of the derived Excaption classes. 
     *
     */
    template <class T> static std::string ToString(const T & aT)
    {
      std::ostringstream oss;
      oss<<aT;
      return oss.str();
    }
    
    void AddHistoryElement(const std::string & aFunc,
			   const std::string & aFile,
			   int aLine);
 
    void SetType();

    void SetErrorDescription(const std::string & aNewDesc);

    
    // streamers to make is easier to fill the internal buffer.
    
    friend ICException & operator<<(ICException & aExc, u8 aData);  
    friend ICException & operator<<(ICException & aExc, i8 aData);
  
    friend ICException & operator<<(ICException & aExc, u16 aData);
    friend ICException & operator<<(ICException & aExc, i16 aData);
  
    friend ICException & operator<<(ICException & aExc, u32 aData);
    friend ICException & operator<<(ICException & aExc, i32 aData);

    friend ICException & operator<<(ICException & aExc, const HardwareAddress & aData);

    friend ICException & operator<<(ICException & aExc, const char * aData);
    friend ICException & operator<<(ICException & aExc, const std::string & aData);

  private: 
    /*
     * must use the implpemented constructor from derived classes for
     * the history mechanism to work. 
     *
     * see comment from constructor with std::string argument at the 
     * top of the class declaration.  
     */

    //ICException();
    
    
  private:
    
    /*
     * internal structure for containing the 
     * points along the path of the exception
     * route.  
     *
     */
    struct ExceptionHistoryPoint{
      std::string mFunction;
      std::string mFile;
      int mLine;      
    };
    

  private:
    std::string mExceptionName;
  protected: // sometimes derived classes want to manipulate this one.
    std::string mErrorDescription;
  private:
    std::list<ExceptionHistoryPoint> mHistory;
  };//~class ICException

  //ICException & operator<<(ICException & aExc, const char * aData);  
  
  ICException & operator<<(ICException & aExc, u8 aData);  
  ICException & operator<<(ICException & aExc, i8 aData);
  
  ICException & operator<<(ICException & aExc, u16 aData);
  ICException & operator<<(ICException & aExc, i16 aData);
  
  ICException & operator<<(ICException & aExc, u32 aData);
  ICException & operator<<(ICException & aExc, i32 aData);

  ICException & operator<<(ICException & aExc, const HardwareAddress & aData);

  ICException & operator<<(ICException & aExc, const char * aData);
  ICException & operator<<(ICException & aExc, const std::string & aData);


  
    
}//~namespace ICCoreUtils

#endif //~ICException_HH
