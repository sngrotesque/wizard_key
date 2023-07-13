# Copyright (C) 2022 Valentin-Ioan VINTILA.
# All rights reserved.

# Used for C++ files (wi + sb)
CPP=g++
CPPFLAGS=-Wall -Wextra -std=c++20

# Used for C files (dh)
CC=gcc
CFLAGS=-Wall -Wextra

# Used in debugging
VG=DEBUGINFOD_URLS="https://debuginfod.archlinux.org" valgrind
VGFLAGS=--leak-check=full \
		--show-leak-kinds=all \
		--track-origins=yes \
		--verbose \
		--log-file=../logs/valgrind-out.txt

# Default targets (non-debugging)
TARGETS=../lib/wi-utilities.o ../lib/wi-aes.o \
		../lib/sb-aes.o \
		../lib/dh-gmult.o ../lib/dh-aes.o

# Debugging targets
DTARGETS=../lib/wi-utilities-d.o ../lib/wi-aes-d.o \
		 ../lib/sb-aes-d.o \
		 ../lib/dh-gmult-d.o ../lib/dh-aes-d.o

# End result file
ENDRES=./check

# Build & Run
run: all
	clear
	$(ENDRES)

# Build (-g) & Debug
debug: all-d
	clear
	gdb $(ENDRES)-d

# Build (-g) & Valgrind
valgrind: all-d
	clear
	$(VG) $(VGFLAGS) $(ENDRES)-d

# Build
all: $(TARGETS)
	$(CPP) $(CPPFLAGS) -o $(ENDRES) $(TARGETS) check.cpp

# Build (-g)
all-d: $(DTARGETS)
	$(CPP) $(CPPFLAGS) -g -o $(ENDRES)-d $(DTARGETS) check.cpp

# WI files
../lib/wi-utilities.o: ../src/wi-utilities.cpp
	$(CPP) $(CPPFLAGS) -c -o $@ $^
../lib/wi-aes.o: ../src/wi-aes.cpp
	$(CPP) $(CPPFLAGS) -c -o $@ $^

../lib/wi-utilities-d.o: ../src/wi-utilities.cpp
	$(CPP) $(CPPFLAGS) -g -c -o $@ $^
../lib/wi-aes-d.o: ../src/wi-aes.cpp
	$(CPP) $(CPPFLAGS) -g -c -o $@ $^

# SB files
../lib/sb-aes.o: SergeyBel-AES/AES.cpp
	$(CPP) $(CPPFLAGS) -c -o $@ $^

../lib/sb-aes-d.o: SergeyBel-AES/AES.cpp
	$(CPP) $(CPPFLAGS) -g -c -o $@ $^

# DH files
../lib/dh-gmult.o: dhuertas-AES/gmult.c
	$(CC) $(CFLAGS) -c -o $@ $^
../lib/dh-aes.o: dhuertas-AES/aes.c
	$(CC) $(CFLAGS) -c -o $@ $^

../lib/dh-gmult-d.o: dhuertas-AES/gmult.c
	$(CC) $(CFLAGS) -g -c -o $@ $^
../lib/dh-aes-d.o: dhuertas-AES/aes.c
	$(CC) $(CFLAGS) -g -c -o $@ $^

# Cleaning our mess
clean:
	rm -f $(TARGETS) $(ENDRES) \
		  $(DTARGETS) $(ENDRES)-d
