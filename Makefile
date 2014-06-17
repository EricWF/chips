SHELL := /bin/bash

NOPRINTD ?= --no-print-directory
CMD_VERB ?= @

.PHONY: all
all: 
ifeq ($(wildcard build/Makefile),)
	$(error make configure must be run)
else
	$(CMD_VERB) $(MAKE) $(NOPRINTD) -C build
endif

	
.PHONY: everything
everything:
	$(CMD_VERB) $(MAKE) $(NOPRINTD) distclean
	$(CMD_VERB) $(MAKE) $(NOPRINTD) configure
	$(CMD_VERB) $(MAKE) $(NOPRINTD) -C build
	
	
.PHONY: clean
clean:
	$(CMD_VERB) if [ -f build/Makefile ]; \
		then $(MAKE) $(NOPRINTD) -C build clean ; \
	fi
	
.PHONY: configure
configure:
	$(CMD_VERB) rm -rf build/ ; mkdir -p build/ ; cd build/ ; cmake .. ; cd ..

.PHONY: redep
redep: 
	$(CMD_VERB) cd build/ ; cmake .. ; cd ..
	
.PHONY: distclean
distclean: 
	$(CMD_VERB) $(MAKE) $(NOPRINTD) clean
	$(CMD_VERB) rm -rf ./build ./bin
	
.PHONY: check
check:
	$(CMD_VERB) echo === Building tests ===
	$(CMD_VERB) $(MAKE) $(NOPRINTD) -C build
	$(CMD_VERB) echo
	$(CMD_VERB) echo === Running tests ===
	$(CMD_VERB) ./bin/chips_test --log_level=message --report_level=short

################################################################################
.PHONY: scan-build
scan-build:
	$(CMD_VERB) rm -rf build/ ; mkdir -p build ; cd build/ ; cmake .. ; scan-build make $(NOPRINTD) -C all; cd ..

	
	
