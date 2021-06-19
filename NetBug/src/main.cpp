//
//  main.cpp
//  NetBug
//
//  Created by maxwells Turring on 2021/6/18.
//

#include <iostream>
#include <memory>
#include <string>
#include "dns/DnsParser.hpp"

int main(int argc, const char * argv[]) {
    // insert code here...
    using namespace std;
    using namespace NetBug;
    
    DnsParser p("www.bilibili.com");
    int fd = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    p.parse(fd);
    
    AddressList list = p.getAddress();
    for(auto ite = list.begin(); ite != list.end(); ite++) {
        cout << "result->" << (*ite) << endl;
    }
    
    close(fd);
    
    return 0;
}
