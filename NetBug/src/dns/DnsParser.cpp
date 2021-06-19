//
//  DnsParser.cpp
//  NetBug
//
//  Created by maxwells Turring on 2021/6/18.
//

#include "DnsParser.hpp"

using namespace std;
using namespace NetBug;

DnsParser::DnsParser(const string& domain) : domain(domain) {
    // initialize
}

BaseBuffer * DnsParser::prepareData() {
    BaseBuffer * b = new UnsafeBuffer();
    // write in data
    DNS_HEADER h;
    memset(&h, 0x0, sizeof(h));
    // set header content
    h.id     = (unsigned short)htons(getpid());
    h.rd     = 1;
    h.qCount = htons(1);
    b->pushData(&h, sizeof(h));
    // parse domain
    unique_ptr<char> parsed(parseDomain());
    b->pushData(parsed.get(), strlen(parsed.get()) + 1);
    
    QUESTION question;
    question.qClass = htons(A);
    question.qType  = htons(1);
    b->pushData(&question, sizeof(QUESTION));
    
    return b;
}

char * DnsParser::parseDomain() {
    char * dataBuf = new char[domain.length() + 2];
    memset(dataBuf, 0x0, domain.length() + 2);
    size_t domainLength = domain.length();
    const char * source  = domain.c_str();
    size_t offset = 1;
    size_t count  = 0;
    for(size_t idx = 0 ; idx < domainLength + 1 ; idx ++) {
        if(source[idx] == '.' || source[idx] == '\0') {
            // is end or seperator
            dataBuf[offset - count - 1] = count;
            count = 0;
        } else {
            dataBuf[offset] = source[idx];
            count ++;
        }
        offset ++;
    }
    return dataBuf;
}

void DnsParser::parse(int sockfd) {
    unique_ptr<BaseBuffer> b(prepareData());
    // prepare address
    sockaddr_in addr;
    memset(&addr, 0x0, sizeof(addr));
    addr.sin_addr.s_addr = inet_addr("180.76.76.76");
    addr.sin_port        = htons(53);
    addr.sin_family      = AF_INET;
    socklen_t len = sizeof(addr);
    
    timespec spec;
    spec.tv_nsec = 0;
    spec.tv_sec = 10;
    // timeout option
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &spec, sizeof(spec));
    
    // start to receive data
    sendto(sockfd, b->getPointer(), b->getAvailable() + 1, 0, (sockaddr *)&addr, sizeof(addr));
    char buf[65535];
    bzero(buf, 65535);
    size_t recv = recvfrom(sockfd, buf, 65535, 0, (sockaddr *)&addr, &len);
    if(recv > 65535) {
        cout << "warning : dns timed out" << endl;
        return;
    }
    
    // response header
    DNS_HEADER h = ((DNS_HEADER *)buf)[0];
    // answer count
    unsigned short anscount = htons(h.AnsCount);
    // skip query first
    size_t skiped = 0;
    stringstream stm;
    char * start = buf + 12;
    char * beg   = buf;
    skiped = skipName(stm, start, beg);
    // move ptr
    start += skiped + sizeof(QUESTION);
    for(ushort i = 0 ; i < anscount ; i ++) {
        stm.clear();
        stm.str("");
        // skip two names
        skiped = skipName(stm, start, beg);
        string currentAnswer = stm.str().substr(1);
        start += skiped;
        // skip rdata section
        R_DATA * data = (R_DATA *)start;
        start += 10;
        // data length
        stm.clear();
        stm.str("");
        if(data->_type == htons(CNAME)) {
            // CNAME data
            skiped = skipName(stm, start, beg);
            // remeber to clean the memory of the vector
            result[currentAnswer] = new vector<in_addr_t>();
            start += skiped;
        } else if(data->_type == htons(A)) {
            // IPv4 address
            // vector not found
            if(result.find(currentAnswer) == result.end()) {
                result[currentAnswer] = new vector<in_addr_t>();
            }
            // get address
            result[currentAnswer] -> push_back(*((in_addr_t *)start));
            start += sizeof(in_addr_t);
        }
    }
}

size_t DnsParser::skipName(stringstream& builder,char * start,char * beg) {
    char * p = start;
    size_t skiped = 0;
    bool isCompressed = false;
    while(*p != '\0') {
        unsigned char c = *((unsigned short *)p);
        if(c >> 4 == 0xC) {
            // compressed
            unsigned short offset = ntohs(*((unsigned short *)p)) & 0b0011111111111111;
            char * newStart = beg + offset;
            skipName(builder, newStart, beg);
            // move two more bytes
            skiped += 2;
            isCompressed = true;
            break;
        } else {
            // read data chuck
            char byte_count = *p;
            char tmp[byte_count + 1];
            tmp[byte_count] = 0;
            memcpy(tmp, p + 1, byte_count);
            builder << "." << tmp;
            p += byte_count + 1;
            skiped += byte_count + 1;
        }
    }
    if(!isCompressed) skiped ++;
    return skiped;
}

DnsParser::~DnsParser() {
    // delete all vectors
    for(ResultIterator ite = result.begin(); ite != result.end(); ite ++) {
        delete ite->second;
    }
}

AddressList DnsParser::getAddress()  {
    AddressList list;
    for(ResultIterator ite = result.begin(); ite != result.end(); ite ++) {
        for(innerIterator i = ite->second->begin(); i != ite->second->end(); i ++) {
            // walk all the address
            in_addr addr;
            addr.s_addr = (*i);
            string rc = inet_ntoa(addr);
            list.push_back(rc);
        }
    }
    return list;
}

RawAddressList DnsParser::getRawAddress() {
    RawAddressList list;
    for(ResultIterator ite = result.begin(); ite != result.end(); ite ++) {
        for(innerIterator i = ite->second->begin(); i != ite->second->end(); i ++) {
            // walk all the address
            in_addr addr;
            addr.s_addr = (*i);;
            list.push_back(addr);
        }
    }
    return list;
}
