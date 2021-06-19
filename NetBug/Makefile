SRC 	:= 	src/main.cpp \
			src/dns/DnsParser.cpp \
			src/utils/Buffer.cpp \
			src/message/BlockingDeque.cpp
INCLUDE :=	-Isrc
CXX		:= clang++
C_FLAG 	:= -Wall -pipe -O3 -std=c++11

all: $(SRC)
	$(CXX) $(SRC) $(INCLUDE) $(C_FLAG)
	mkdir -p obj
	mv a.out NetBug
	mv NetBug obj
