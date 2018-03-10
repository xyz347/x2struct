CC=g++
AR=ar

FLAGS= -g -DUSE_MAKE
INCS=-I. -Ithirdparty

CPPFILES=$(wildcard *.cpp)
HPPFILES=$(wildcard *.h)
HPPFILES+=$(wildcard *.hpp)


# windows remove bson and libconfig that lib is prebuilt
ifeq ($(OS),Windows_NT)
FLAGS+=-DWIN
CPPFILES:=$(filter-out bson%,$(CPPFILES))
CPPFILES:=$(filter-out config%,$(CPPFILES))
HPPFILES:=$(filter-out bson%,$(HPPFILES))
HPPFILES:=$(filter-out config%,$(HPPFILES))
else
LDFLAGS=thirdparty/libconfig/lib/libconfig++.a thirdparty/libbson/lib/libbson-1.0.a -pthread -lrt 
INCS+=-Ithirdparty/libbson/include -Ithirdparty/libconfig/include
endif

OBJFILES=$(foreach n,$(CPPFILES),objs/$(n:.cpp=.o))
DEPFILES=$(OBJFILES:.o=.d)

LIBNAME=libx2struct.a

objs/%.d:%.cpp objs
	$(CC) -MM -MT $(@:.d=.o) $< $(FLAGS) $(INCS) >$@


objs/%.o:%.cpp
	$(CC) -c $< -o $@ $(FLAGS) $(INCS)


$(LIBNAME):$(OBJFILES)
	$(AR) crus $@ $^


objs:
	@mkdir $@

clean:
	-rm -rf $(LIBNAME) objs/* test/xtest*

test:test/xtest

test/xtest:test/x2struct_test.cpp $(LIBNAME)
	$(CC) -o $@ $^ $(FLAGS) $(INCS) $(LDFLAGS)


ifneq ($(MAKECMDGOALS), clean)
-include $(DEPFILES)
endif
