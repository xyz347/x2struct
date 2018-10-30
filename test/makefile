INC=-I ../
LIBCONFIG=../thirdparty/libconfig/lib/libconfig++.a
LIBBSON=../thirdparty/libbson/lib/libbson-1.0.a -pthread -lrt

json:
	g++ -o $@ example_json.cpp $(INC)
	./$@
	@-rm $@

xml:
	g++ -o $@ example_xml.cpp $(INC)
	./$@
	@-rm $@

libconfig:
	g++ -o $@ -DXTOSTRUCT_LIBCONFIG example_libconfig.cpp $(INC) $(LIBCONFIG)
	./$@
	@-rm $@

bson:
	g++ -o $@ -DXTOSTRUCT_BSON example_bson.cpp $(INC) $(LIBBSON)
	./$@
	@-rm $@

gocode:
	g++ -o $@ -DXTOSTRUCT_GOCODE check.cpp $(INC) $(LIBCONFIG) $(LIBBSON)
	./$@
	@-rm $@

check:
	g++ -o $@ check.cpp $(INC) $(LIBCONFIG) $(LIBBSON)
	./$@
	@-rm $@
