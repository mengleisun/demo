// Do NOT change. Changes will be lost next time file is generated

#define R__DICTIONARY_FILENAME roofit_Dict

/*******************************************************************/
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <assert.h>
#define G__DICTIONARY
#include "RConfig.h"
#include "TClass.h"
#include "TDictAttributeMap.h"
#include "TInterpreter.h"
#include "TROOT.h"
#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TInterpreter.h"
#include "TVirtualMutex.h"
#include "TError.h"

#ifndef G__ROOT
#define G__ROOT
#endif

#include "RtypesImp.h"
#include "TIsAProxy.h"
#include "TFileMergeInfo.h"
#include <algorithm>
#include "TCollectionProxyInfo.h"
/*******************************************************************/

#include "TDataMember.h"

// Since CINT ignores the std namespace, we need to do so in this file.
namespace std {} using namespace std;

// Header files passed as explicit arguments
#include "RooDCBShape.h"
#include "RooUserPoly.h"

// Header files passed via #pragma extra_include

namespace ROOT {
   static void *new_RooDCBShape(void *p = 0);
   static void *newArray_RooDCBShape(Long_t size, void *p);
   static void delete_RooDCBShape(void *p);
   static void deleteArray_RooDCBShape(void *p);
   static void destruct_RooDCBShape(void *p);
   static void streamer_RooDCBShape(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::RooDCBShape*)
   {
      ::RooDCBShape *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::RooDCBShape >(0);
      static ::ROOT::TGenericClassInfo 
         instance("RooDCBShape", ::RooDCBShape::Class_Version(), "RooDCBShape.h", 24,
                  typeid(::RooDCBShape), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::RooDCBShape::Dictionary, isa_proxy, 16,
                  sizeof(::RooDCBShape) );
      instance.SetNew(&new_RooDCBShape);
      instance.SetNewArray(&newArray_RooDCBShape);
      instance.SetDelete(&delete_RooDCBShape);
      instance.SetDeleteArray(&deleteArray_RooDCBShape);
      instance.SetDestructor(&destruct_RooDCBShape);
      instance.SetStreamerFunc(&streamer_RooDCBShape);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::RooDCBShape*)
   {
      return GenerateInitInstanceLocal((::RooDCBShape*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::RooDCBShape*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

namespace ROOT {
   static void *new_RooUserPoly(void *p = 0);
   static void *newArray_RooUserPoly(Long_t size, void *p);
   static void delete_RooUserPoly(void *p);
   static void deleteArray_RooUserPoly(void *p);
   static void destruct_RooUserPoly(void *p);
   static void streamer_RooUserPoly(TBuffer &buf, void *obj);

   // Function generating the singleton type initializer
   static TGenericClassInfo *GenerateInitInstanceLocal(const ::RooUserPoly*)
   {
      ::RooUserPoly *ptr = 0;
      static ::TVirtualIsAProxy* isa_proxy = new ::TInstrumentedIsAProxy< ::RooUserPoly >(0);
      static ::ROOT::TGenericClassInfo 
         instance("RooUserPoly", ::RooUserPoly::Class_Version(), "RooUserPoly.h", 24,
                  typeid(::RooUserPoly), ::ROOT::Internal::DefineBehavior(ptr, ptr),
                  &::RooUserPoly::Dictionary, isa_proxy, 16,
                  sizeof(::RooUserPoly) );
      instance.SetNew(&new_RooUserPoly);
      instance.SetNewArray(&newArray_RooUserPoly);
      instance.SetDelete(&delete_RooUserPoly);
      instance.SetDeleteArray(&deleteArray_RooUserPoly);
      instance.SetDestructor(&destruct_RooUserPoly);
      instance.SetStreamerFunc(&streamer_RooUserPoly);
      return &instance;
   }
   TGenericClassInfo *GenerateInitInstance(const ::RooUserPoly*)
   {
      return GenerateInitInstanceLocal((::RooUserPoly*)0);
   }
   // Static variable to force the class initialization
   static ::ROOT::TGenericClassInfo *_R__UNIQUE_(Init) = GenerateInitInstanceLocal((const ::RooUserPoly*)0x0); R__UseDummy(_R__UNIQUE_(Init));
} // end of namespace ROOT

//______________________________________________________________________________
atomic_TClass_ptr RooDCBShape::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *RooDCBShape::Class_Name()
{
   return "RooDCBShape";
}

//______________________________________________________________________________
const char *RooDCBShape::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::RooDCBShape*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int RooDCBShape::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::RooDCBShape*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *RooDCBShape::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::RooDCBShape*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *RooDCBShape::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::RooDCBShape*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
atomic_TClass_ptr RooUserPoly::fgIsA(0);  // static to hold class pointer

//______________________________________________________________________________
const char *RooUserPoly::Class_Name()
{
   return "RooUserPoly";
}

//______________________________________________________________________________
const char *RooUserPoly::ImplFileName()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::RooUserPoly*)0x0)->GetImplFileName();
}

//______________________________________________________________________________
int RooUserPoly::ImplFileLine()
{
   return ::ROOT::GenerateInitInstanceLocal((const ::RooUserPoly*)0x0)->GetImplFileLine();
}

//______________________________________________________________________________
TClass *RooUserPoly::Dictionary()
{
   fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::RooUserPoly*)0x0)->GetClass();
   return fgIsA;
}

//______________________________________________________________________________
TClass *RooUserPoly::Class()
{
   if (!fgIsA.load()) { R__LOCKGUARD2(gInterpreterMutex); fgIsA = ::ROOT::GenerateInitInstanceLocal((const ::RooUserPoly*)0x0)->GetClass(); }
   return fgIsA;
}

//______________________________________________________________________________
void RooDCBShape::Streamer(TBuffer &R__b)
{
   // Stream an object of class RooDCBShape.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      RooAbsPdf::Streamer(R__b);
      m.Streamer(R__b);
      m0.Streamer(R__b);
      sigma.Streamer(R__b);
      alpha.Streamer(R__b);
      n.Streamer(R__b);
      alpha2.Streamer(R__b);
      n2.Streamer(R__b);
      R__b.CheckByteCount(R__s, R__c, RooDCBShape::IsA());
   } else {
      R__c = R__b.WriteVersion(RooDCBShape::IsA(), kTRUE);
      RooAbsPdf::Streamer(R__b);
      m.Streamer(R__b);
      m0.Streamer(R__b);
      sigma.Streamer(R__b);
      alpha.Streamer(R__b);
      n.Streamer(R__b);
      alpha2.Streamer(R__b);
      n2.Streamer(R__b);
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_RooDCBShape(void *p) {
      return  p ? new(p) ::RooDCBShape : new ::RooDCBShape;
   }
   static void *newArray_RooDCBShape(Long_t nElements, void *p) {
      return p ? new(p) ::RooDCBShape[nElements] : new ::RooDCBShape[nElements];
   }
   // Wrapper around operator delete
   static void delete_RooDCBShape(void *p) {
      delete ((::RooDCBShape*)p);
   }
   static void deleteArray_RooDCBShape(void *p) {
      delete [] ((::RooDCBShape*)p);
   }
   static void destruct_RooDCBShape(void *p) {
      typedef ::RooDCBShape current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_RooDCBShape(TBuffer &buf, void *obj) {
      ((::RooDCBShape*)obj)->::RooDCBShape::Streamer(buf);
   }
} // end of namespace ROOT for class ::RooDCBShape

//______________________________________________________________________________
void RooUserPoly::Streamer(TBuffer &R__b)
{
   // Stream an object of class RooUserPoly.

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      RooAbsPdf::Streamer(R__b);
      m.Streamer(R__b);
      slope.Streamer(R__b);
      R__b.CheckByteCount(R__s, R__c, RooUserPoly::IsA());
   } else {
      R__c = R__b.WriteVersion(RooUserPoly::IsA(), kTRUE);
      RooAbsPdf::Streamer(R__b);
      m.Streamer(R__b);
      slope.Streamer(R__b);
      R__b.SetByteCount(R__c, kTRUE);
   }
}

namespace ROOT {
   // Wrappers around operator new
   static void *new_RooUserPoly(void *p) {
      return  p ? new(p) ::RooUserPoly : new ::RooUserPoly;
   }
   static void *newArray_RooUserPoly(Long_t nElements, void *p) {
      return p ? new(p) ::RooUserPoly[nElements] : new ::RooUserPoly[nElements];
   }
   // Wrapper around operator delete
   static void delete_RooUserPoly(void *p) {
      delete ((::RooUserPoly*)p);
   }
   static void deleteArray_RooUserPoly(void *p) {
      delete [] ((::RooUserPoly*)p);
   }
   static void destruct_RooUserPoly(void *p) {
      typedef ::RooUserPoly current_t;
      ((current_t*)p)->~current_t();
   }
   // Wrapper around a custom streamer member function.
   static void streamer_RooUserPoly(TBuffer &buf, void *obj) {
      ((::RooUserPoly*)obj)->::RooUserPoly::Streamer(buf);
   }
} // end of namespace ROOT for class ::RooUserPoly

namespace {
  void TriggerDictionaryInitialization_roofit_Dict_Impl() {
    static const char* headers[] = {
"RooDCBShape.h",
"RooUserPoly.h",
0
    };
    static const char* includePaths[] = {
"/cvmfs/cms.cern.ch/slc6_amd64_gcc530/lcg/root/6.06.00-ikhhed6/include",
"/uscms/homes/m/mengleis/work/SUSY2016/SUSYAnalysis/include/",
0
    };
    static const char* fwdDeclCode = R"DICTFWDDCLS(
#line 1 "roofit_Dict dictionary forward declarations' payload"
#pragma clang diagnostic ignored "-Wkeyword-compat"
#pragma clang diagnostic ignored "-Wignored-attributes"
#pragma clang diagnostic ignored "-Wreturn-type-c-linkage"
extern int __Cling_Autoloading_Map;
class __attribute__((annotate(R"ATTRDUMP(Crystal Ball lineshape PDF)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$RooDCBShape.h")))  RooDCBShape;
class __attribute__((annotate(R"ATTRDUMP(Crystal Ball lineshape PDF)ATTRDUMP"))) __attribute__((annotate("$clingAutoload$RooUserPoly.h")))  RooUserPoly;
)DICTFWDDCLS";
    static const char* payloadCode = R"DICTPAYLOAD(
#line 1 "roofit_Dict dictionary payload"

#ifndef G__VECTOR_HAS_CLASS_ITERATOR
  #define G__VECTOR_HAS_CLASS_ITERATOR 1
#endif

#define _BACKWARD_BACKWARD_WARNING_H
#include "RooDCBShape.h"
#include "RooUserPoly.h"

#undef  _BACKWARD_BACKWARD_WARNING_H
)DICTPAYLOAD";
    static const char* classesHeaders[]={
"RooDCBShape", payloadCode, "@",
"RooUserPoly", payloadCode, "@",
nullptr};

    static bool isInitialized = false;
    if (!isInitialized) {
      TROOT::RegisterModule("roofit_Dict",
        headers, includePaths, payloadCode, fwdDeclCode,
        TriggerDictionaryInitialization_roofit_Dict_Impl, {}, classesHeaders);
      isInitialized = true;
    }
  }
  static struct DictInit {
    DictInit() {
      TriggerDictionaryInitialization_roofit_Dict_Impl();
    }
  } __TheDictionaryInitializer;
}
void TriggerDictionaryInitialization_roofit_Dict() {
  TriggerDictionaryInitialization_roofit_Dict_Impl();
}
