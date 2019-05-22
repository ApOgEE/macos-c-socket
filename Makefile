# Makefile for MacOS Mojave
#
# Author: M. Fauzilkamil Zainuddin
#

CC= gcc

APOSOCK= aposock.c
EXECFILE= socklient

all: aposocket
	
aposocket:
	$(CC) -o $(EXECFILE) $(APOSOCK)

clean:
	rm -f $(EXECFILE)

