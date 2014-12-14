# Makefile for Serpent Virtual Machine (SVM)
# Author:  Lawrence Daniels
# Created: March 20, 2010
 
OBJS = core.o 
SRC = ./main/src/cpp
OPTS = -g -Wall -Wno-deprecated

sera : $(OBJS)

core.o : 
	g++ $(OPTS) -o app/test/sera \
	$(SRC)/*.cpp \
	$(SRC)/class/*.cpp \
	$(SRC)/class/member/*.cpp \
	$(SRC)/compiler/*.cpp \
	$(SRC)/platform/*.cpp \
	$(SRC)/platform/builtin/*.cpp \
	$(SRC)/runtime/*.cpp \
	$(SRC)/token/*.cpp	\
	$(SRC)/vm/*.cpp \
	$(SRC)/debugger/*.cpp
	
