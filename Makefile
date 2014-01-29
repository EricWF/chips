SHELL := /bin/bash

.PHONY: all
all: 
	@ mkdir -p build/ ; cd build/ ; cmake .. ; cd ..
	@ $(MAKE) --no-print-directory -C build

	
.PHONY: e
e:
	@ $(MAKE) --no-print-directory distclean
	@ $(MAKE) --no-print-directory redep
	@ $(MAKE) --no-print-directory -C build
	
.PHONY: ej
ej:
	@ $(MAKE) --no-print-directory distclean
	@ $(MAKE) --no-print-directory redep
	@ $(MAKE) --no-print-directory -j2 -C build
	
	
.PHONY: clean
clean:
	@ if [ -f build/Makefile ]; \
		then $(MAKE) --no-print-directory -C build clean ; \
	fi
	
.PHONY: redep
redep: 
	@ rm -rf build/ ; mkdir -p build/ ; cd build/ ; cmake .. ; cd ..
	
.PHONY: distclean
distclean: 
	@ $(MAKE) --no-print-directory clean
	@ rm -rf ./build ./bin
	
.PHONY: check
check:
	@ echo === Building tests ===
	@ $(MAKE) --no-print-directory -C build
	@ echo
	@ echo === Running tests ===
	@ ./bin/chips_test --log_level=message --report_level=short
	
	
