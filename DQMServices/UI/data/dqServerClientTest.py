import xmlrpclib

run=62803
flag="L1T_Global"
value="GOOD"

tags={}
tags[flag] = value
tags['CSC_Global'] = 'BAD'
tags['DT_Global'] = 'OK'

server = xmlrpclib.ServerProxy("http://localhost:9999")
#print server.querydq(run, flag)
#print server.insertdq(run, flag, value)
#print server.querydq(run, flag)

print server.test(run, tags)
