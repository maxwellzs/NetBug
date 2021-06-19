//
//  DnsParser.hpp
//  NetBug
//
//  Created by maxwells Turring on 2021/6/18.
//

#ifndef DnsParser_hpp
#define DnsParser_hpp

#include <cstdio>
#include <iostream>
#include <map>
#include <vector>
#include <sstream>
#include <cstring>
#include <string>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "dns/Protocol.hpp"
#include "utils/Buffer.hpp"


namespace NetBug {

using namespace std;
/**
 wrapped types
 */
typedef map<string,vector<in_addr_t> *> ResultMap;
typedef vector<in_addr_t>::iterator innerIterator;
typedef ResultMap::iterator ResultIterator;
typedef vector<string> AddressList;
typedef vector<in_addr> RawAddressList;

class DnsParser {
private:
    const string& domain;
    DNS_HEADER getDefaultHeader();
    /**
     prepare the requset data
     */
    BaseBuffer * prepareData();
    /**
     change the pattern of the domain
     */
    char * parseDomain();
    /**
     skip name and the return the byte skipped
     */
    size_t skipName(stringstream& builder,char * start,char * beg);
    /**
     store all the domain an its candidate name's IPv4 address
     */
    ResultMap result;
public:
    /**
     method to parse the domain
     */
    void parse(int sockfd);
    ~DnsParser();
    DnsParser(const string& domain);
    /**
     return the query result
     MUST parse first
     */
    inline ResultMap& getResult() {
        return this->result;
    }
    /**
     return the raw address
     */
    AddressList getAddress();
    /**
     return the binary address (uint32_t)
     */
    RawAddressList getRawAddress();
};


};


#endif /* DnsParser_hpp */
