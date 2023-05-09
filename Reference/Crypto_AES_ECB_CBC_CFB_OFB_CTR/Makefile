# Copyright (C) 2022 Valentin-Ioan VINTILA.
# All rights reserved.

# Used for C++ files (wi + sb)
CPP=g++
CPPFLAGS=-Wall -Wextra -std=c++20 -DWI_AES_LOG_CRYPT

# Used in debugging
VG=DEBUGINFOD_URLS="https://debuginfod.archlinux.org" valgrind
VGFLAGS=--leak-check=full \
		--show-leak-kinds=all \
		--track-origins=yes \
		--verbose \
		--log-file=logs/valgrind-out.txt

# Default targets (non-debugging)
TARGETS=lib/wi-utilities.o lib/wi-aes.o

# Debugging targets
DTARGETS=lib/wi-utilities-d.o lib/wi-aes-d.o

# End result file
ENDRES=./main

# Build & Run
run: all
	clear
	$(ENDRES)

# Build (-g) & Valgrind
valgrind: clean all-d
	clear
	$(VG) $(VGFLAGS) $(ENDRES)-d

# Build (-g) & Debug
debug: all-d
	clear
	gdb $(ENDRES)-d

# Build
all: $(TARGETS)
	$(CPP) $(CPPFLAGS) -o $(ENDRES) $(TARGETS) main.cpp

# Build (-g)
all-d: $(DTARGETS)
	$(CPP) $(CPPFLAGS) -g -o $(ENDRES)-d $(DTARGETS) main.cpp

# WI files
lib/wi-utilities.o: src/wi-utilities.cpp
	$(CPP) $(CPPFLAGS) -c -o $@ $^
lib/wi-aes.o: src/wi-aes.cpp
	$(CPP) $(CPPFLAGS) -c -o $@ $^

lib/wi-utilities-d.o: src/wi-utilities.cpp
	$(CPP) $(CPPFLAGS) -g -c -o $@ $^
lib/wi-aes-d.o: src/wi-aes.cpp
	$(CPP) $(CPPFLAGS) -g -c -o $@ $^

# Cleanup
clean:
	rm -f $(TARGETS) $(ENDRES) \
		  $(DTARGETS) $(ENDRES)-d

# Line counting
cloc:
	clear
	cloc .
