all: 
	mkdir -p build
	cd gtest; make all
	cd test; make all
	cd examples; make all

clean: 
	cd gtest; make clean
	cd test; make clean
	cd examples; make clean
