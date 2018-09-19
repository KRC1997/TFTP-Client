IDIR = include
SDIR = src
ODIR = obj

CC = g++
CFLAGS = -I $(IDIR)

_DEPS = encoderDecoder.h packetStructure.h supportFunctions.h
DEPS = $(patsubst %, $(IDIR)/%, $(_DEPS))

_SRCS = packetStructure.cpp encoderDecoder.cpp supportFunctions.cpp main.cpp
SRCS = $(patsubst %, $(SDIR)/%, $(_SRCS))

tftpc: $(SRCS)
	$(CC) -o $@ $^ $(CFLAGS)
