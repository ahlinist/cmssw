/*
  Author: Adam Hunt
  email:  ahunt@princeton.edu
  Date:   2007-08-24
*/

#ifndef HLXTCP_H
#define HLXTCP_H

#include <string>

// srand rand
#include <ctime>
#include <cstdlib>

// tcp
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>

// Lumi
#include "RecoLuminosity/TCPReceiver/interface/ICTypeDefs.hh"
#include "RecoLuminosity/TCPReceiver/interface/LumiStructures.hh"
#include "RecoLuminosity/TCPReceiver/interface/TimeStamp.h"

/* 
   Error Codes
       0: unknown failure
       1: success
     101: invalid port
        iana.org registers ports from 0 - 1023
     201: invalid mode
        Acceptable modes are 0:  tcp data,  1: constant fake data, 2: random fake data
     301: socket() failed
     302: connect() failed
     401: Disconnect() called without being connected
     501: Failed to Receive Data from server
     601; close() failed

*/

namespace HCAL_HLX{

  class TCPReceiver{

  public:
    TCPReceiver();
    TCPReceiver(unsigned short int, std::string,  unsigned char);
    ~TCPReceiver();
    int Connect();
    int SetPort(unsigned short int);
    int SetMode(unsigned char);
    void SetIP(std::string IP);
    int ReceiveLumiSection(HCAL_HLX::LUMI_SECTION & localSection);
    int Disconnect();
    bool IsConnected();    
    bool VerifyFakeData(HCAL_HLX::LUMI_SECTION & localSection);

    void GenerateFakeData(HCAL_HLX::LUMI_SECTION & localSection);
    void GenerateRandomData(HCAL_HLX::LUMI_SECTION & localSection);

  private:
    unsigned char acquireMode;
    bool Connected;

    unsigned short servPort;
    std::string servIP;
    int tcpSocket;
    struct sockaddr_in servAddr;
  };

}
#endif
