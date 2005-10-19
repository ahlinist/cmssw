#ifndef EVENTSINK_H
#define EVENTSINK_H

class FUAdapter;

class EventSink
{
 public:
  virtual ~EventSink(){}
  virtual void sink();
  static void setFwk(FUAdapter *fwk){fwk_=fwk;}

 protected:
  static FUAdapter *fwk_;

};

#endif
