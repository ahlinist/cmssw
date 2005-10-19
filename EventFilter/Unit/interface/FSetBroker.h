#ifndef FSET_BROKER_H
#define FSET_BROKER_H

class FSetBroker
{
 public:
  
  virtual ~FSetBroker(){}
  static FSetBroker* instance()
    {
      if(instance_==0)
	instance_ = new FSetBroker();
      return instance_;
    }
  static void registerInstance(FSetBroker *inst)
    {
      instance_ = inst;
    }
  virtual unsigned int getNSF(int ind)
    {
      return nRUs_;
    }
  virtual unsigned int getFS(int fedid)
    {
      return GLOBAL_FSET;
    }

 private:
  
  FSetBroker(){}

  static FSetBroker *instance_;
  static const int nRUs_ = 64; 
  static const int GLOBAL_FSET = 1;
};
#endif
