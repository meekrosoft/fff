all:
	mkdir -p build
	cd gtest; $(MAKE) all
	cd test; $(MAKE) all
	cd examples; $(MAKE) all

clean: 
	cd gtest; $(MAKE) clean
	cd test; $(MAKE) clean
	cd examples; $(MAKE) clean
