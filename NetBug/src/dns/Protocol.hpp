//
//  Protocol.hpp
//  NetBug
//
//  Created by maxwells Turring on 2021/6/18.
//

#ifndef Protocol_h
#define Protocol_h

namespace NetBug {

#define A       1
#define NS      2
#define CNAME   5
#define SOA     6
#define WKS     11
#define PTR     12
#define HINFO   13
#define MX      15
#define AAAA    28
#define AXFR    252
#define ANY     255

typedef struct {
    unsigned short id;
    unsigned char rd:1;
    unsigned char tc:1;
    unsigned char aa:1;
    unsigned char opcode:4;
    unsigned char qr:1;
    unsigned char rcode:1;
    unsigned char cd:1;
    unsigned char ad:1;
    unsigned char z:1;
    unsigned char ra:1;
    unsigned short qCount;
    unsigned short AnsCount;
    unsigned short AuthCount;
    unsigned short AddCount;
} DNS_HEADER;

typedef struct {
    unsigned short qType;
    unsigned short qClass;
} QUESTION;

typedef struct {
    unsigned short _type;
    unsigned short _class;
    uint32_t ttl;
    unsigned short dataLength;
} R_DATA;

};

#endif /* Protocol_h */
