MODULE=helloworld
IDL=$(MODULE).idl

all:

%-common.c %-skels.c %-stubs.c %.h %-imodule.c: %.idl
	orbit-idl-2 -I . --add-imodule $<

%-cpp-common.cc %-cpp-skels.cc %-cpp-stubs.cc %-cpp.cc %-cpp-common.h %-cpp-skels.h %-cpp-stubs.h %-cpp.h: %.idl
	orbit-idl-2 -I . -l cpp $<

# imodule stuff is needed only in certain cases, such as using py-orbit.
# need to generalise this for other compilers... 
%-imodule.o: %-imodule.c
	gcc -fPIC -c -o $@ $< `pkg-config ORBit-2.0 --cflags`

%-imodule.so: %-imodule.o
	gcc -shared -o $@ $< `pkg-config ORBit-2.0 --libs`


ORBIT_SRC=\
$(MODULE)-common.c \
$(MODULE)-cpp.cc \
$(MODULE)-skels.c \
$(MODULE)-stubs.c

ORBIT_OBJ=\
$(MODULE)-common.o \
$(MODULE)-cpp.o \
$(MODULE)-skels.o \
$(MODULE)-stubs.o

CFLAGS+=`pkg-config ORBit-2.0-cpp --cflags`
CXXFLAGS+=`pkg-config ORBit-2.0-cpp --cflags`
LOADLIBES+=`pkg-config ORBit-2.0-cpp --libs`

client.o helloworld-impl.o server.o: $(ORBIT_SRC)

client: client.o helloworld-impl.o $(ORBIT_OBJ)
	$(CXX) $(LDFLAGS) -o $@ $+ $(LOADLIBES)

server: server.o helloworld-impl.o $(ORBIT_OBJ)
	$(CXX) $(LDFLAGS) -o $@ $+ $(LOADLIBES)

all: client server
	
clean:
	rm -rf client server client.o server.o $(ORBIT_OBJ) $(MODULE){,-cpp}-{skels,common,stubs,imodule}.{h,cc,c}







