CC=g++
AR=ar

FLAGS= -g -DUSE_MAKE
INCS=-I.

CPPFILES=$(wildcard *.cpp)
HPPFILES=$(wildcard *.h)
HPPFILES+=$(wildcard *.hpp)


# windows remove bson and libconfig that lib is prebuilt
ifeq ($(OS),Windows_NT)
CPPFILES:=$(filter-out bson%,$(CPPFILES))
CPPFILES:=$(filter-out config%,$(CPPFILES))
HPPFILES:=$(filter-out bson%,$(HPPFILES))
HPPFILES:=$(filter-out config%,$(HPPFILES))
EXE=.exe
else
FLAGS+=-DXTOSTRUCT_BSON -DXTOSTRUCT_LIBCONFIG 
LDFLAGS=thirdparty/libconfig/lib/libconfig++.a thirdparty/libbson/lib/libbson-1.0.a -pthread -lrt 
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
	-rm -rf $(LIBNAME) objs/* test/xtest* gocode*

test:test/xtest$(EXE)

test/xtest$(EXE):test/x2struct_test.cpp $(LIBNAME)
	$(CC) -o $@ $^ $(FLAGS) $(INCS) $(LDFLAGS)

gocode:test/x2struct_test.cpp  $(LIBNAME)
	$(CC) -o $@$(EXE) $^ $(FLAGS) $(INCS)  $(LDFLAGS) -DXTOSTRUCT_GOCODE
	

ifneq ($(MAKECMDGOALS), clean)
-include $(DEPFILES)
endif
