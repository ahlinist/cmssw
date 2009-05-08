#!/usr/bin/env python

import sys, os, optparse, SimpleXMLRPCServer

def insertdq(run, tags):
  print "Received: ", run, tags
  return "OK"
		
def querydq(dataset, run, tag):
  return 'NOTFOUND'

if __name__ == "__main__":
  server = SimpleXMLRPCServer.SimpleXMLRPCServer(('localhost', 9999))
  server.register_function(insertdq);
  server.register_function(querydq);
  server.serve_forever()

