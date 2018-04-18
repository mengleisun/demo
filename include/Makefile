# =============================== 
# Makefile for ana 
# =============================== 

ROOTGLIBS     = $(shell $(ROOTSYS)/bin/root-config --glibs)
ROOTCFLAGS    = $(shell $(ROOTSYS)/bin/root-config --cflags) 

CXX           = g++ 
CXXFLAGS      = -g -Wall -fPIC   	
SOFLAGS       = -shared 

CXXFLAGS     += $(ROOTCFLAGS) 

ANACLASSES = ../lib/analysis_photon.o ../lib/analysis_ele.o ../lib/analysis_rawData.o ../lib/analysis_muon.o ../lib/analysis_scalefactor.o ../lib/analysis_photonDict.o 
ROOFITCLASSES = ../lib/RooDCBShape.o ../lib/RooUserPoly.o ../lib/roofit_Dict.o

# =============================== 
ana: ../src/analysis_photon.cc 
# ------------------------------- 
	$(CXX) $(CXXFLAGS) -c ../src/analysis_photon.cc -o ../lib/analysis_photon.o 
	$(CXX) $(CXXFLAGS) -c ../src/analysis_ele.cc -o ../lib/analysis_ele.o 
	$(CXX) $(CXXFLAGS) -c ../src/analysis_muon.cc -o ../lib/analysis_muon.o 
	$(CXX) $(CXXFLAGS) -c ../src/analysis_rawData.cc -o ../lib/analysis_rawData.o
	$(CXX) $(CXXFLAGS) -c ../src/analysis_scalefactor.cc -o ../lib/analysis_scalefactor.o
	$(CXX) $(CXXFLAGS) -c ../src/RooDCBShape.cc -o ../lib/RooDCBShape.o
	$(CXX) $(CXXFLAGS) -c ../src/RooUserPoly.cc -o ../lib/RooUserPoly.o
	$(ROOTSYS)/bin/rootcint -v -f analysis_photonDict.cc -c analysis_rawData.h analysis_photon.h analysis_ele.h analysis_muon.h analysis_scalefactor.h
	$(ROOTSYS)/bin/rootcint -v -f roofit_Dict.cc -c RooDCBShape.h RooUserPoly.h
	$(CXX) $(CXXFLAGS) -c  analysis_photonDict.cc -o ../lib/analysis_photonDict.o 	
	$(CXX) $(CXXFLAGS) -c  roofit_Dict.cc -o ../lib/roofit_Dict.o
	$(CXX) $(SOFLAGS) $(ANACLASSES) -o ../lib/libAnaClasses.so $(ROOTGLIBS) 
	$(CXX) $(SOFLAGS) $(ROOFITCLASSES) -o ../lib/libRooFitClasses.so $(ROOTGLIBS)
	cp *.pcm ../lib/.
# =============================== 
clean: 
	rm -f ana.o anaDict.o anaDict.cc anaDict.h libAnaClasses.so  
# =============================== 
