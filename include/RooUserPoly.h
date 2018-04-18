 /*****************************************************************************
  * Project: RooFit                                                           *
  * Package: RooFitModels                                                     *
  *    File: $Id: RooUserPoly.h,v 1.11 2007/07/12 20:30:49 wouter Exp $
  * Authors:                                                                  *
  *   WV, Wouter Verkerke, UC Santa Barbara, verkerke@slac.stanford.edu       *
  *   DK, David Kirkby,    UC Irvine,         dkirkby@uci.edu                 *
  *                                                                           *
  * Copyright (c) 2000-2005, Regents of the University of California          *
  *                          and Stanford University. All rights reserved.    *
  *                                                                           *
  * Redistribution and use in source and binary forms,                        *
  * with or without modification, are permitted according to the terms        *
  * listed in LICENSE (http://roofit.sourceforge.net/license.txt)             *
  *****************************************************************************/
 #ifndef ROO_Poly_SHAPE
 #define ROO_Poly_SHAPE
 
 #include "RooAbsPdf.h"
 #include "RooRealProxy.h"
 
 class RooRealVar;
 
 class RooUserPoly : public RooAbsPdf {
 public:
   RooUserPoly() {} ;
   RooUserPoly(const char *name, const char *title, RooAbsReal& _m, RooAbsReal& _slope);
 
   RooUserPoly(const RooUserPoly& other, const char* name = 0);
   virtual TObject* clone(const char* newname) const { return new RooUserPoly(*this,newname); }
 
   inline virtual ~RooUserPoly() { }
 
 protected:
 
   RooRealProxy m;
   RooRealProxy slope;
 
   Double_t evaluate() const;
 
 private:
 
   ClassDef(RooUserPoly,1) // Crystal Ball lineshape PDF
 };
 
 #endif

