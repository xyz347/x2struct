CC=g++
AR=ar

FLAGS= -g -DUSE_MAKE
INCS=-I. -Ithirdparty/json -Ithirdparty/tinyxml  -Ithirdparty/libbson/include -Ithirdparty/libconfig/include

CPPFILES=$(wildcard *.cpp)


HPPFILES=$(wildcard *.hpp)
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
	-rm -rf $(LIBNAME) objs/* test/xtest

test:test/xtest

test/xtest:test/x2struct_test.cpp $(LIBNAME)
	$(CC) -o $@ $^ $(FLAGS) $(INCS) \
	thirdparty/json/lib/libjson.a \
	thirdparty/tinyxml/lib/libtinyxml.a \
	thirdparty/libconfig/lib/libconfig++.a \
	thirdparty/libbson/lib/libbson-1.0.a \
	-pthread -lrt


ifneq ($(MAKECMDGOALS), clean)
-include $(DEPFILES)
endif
