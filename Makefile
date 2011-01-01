all: 
	mkdir -p build
	cd test; make all
	cd examples; make all

clean: 
	cd test; make clean
	cd examples; make clean
