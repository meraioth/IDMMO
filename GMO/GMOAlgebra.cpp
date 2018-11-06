#include <string>
#include "SecondoCatalog.h"
#include "QueryProcessor.h"
#include "Attribute.h"
#include "AlgebraTypes.h"
#include "Operator.h"
#include "ConstructorTemplates.h"
#include "GenericTC.h"
#include "TypeMapUtils.h"
#include "ListUtils.h"
#include "Symbols.h"
#include "StandardTypes.h"
#include "GMOAlgebra.h"
#include "GenericPoint.h"
#include "IGenericPoint.h"
#include "GenericMPoint.h"
#include "QueryProcessor.h"   // needed for implementing value mappings
#include "AlgebraManager.h"   // e.g., check for certain kind
#include "../Network/NetworkAlgebra.h"
#include "Point.h"
#include <typeinfo>


using namespace std;
using namespace mappings;
using namespace gmo;
using namespace network;

extern NestedList* nl;
extern QueryProcessor* qp;

namespace gmo{



TypeConstructor gpointTC(
  GenericPoint::BasicType(),
  GenericPoint::Property,
  GenericPoint::Out, GenericPoint::In,
  0, 0,
  GenericPoint::Create, GenericPoint::Delete,
  OpenAttribute<GenericPoint >,
  SaveAttribute<GenericPoint >,
  GenericPoint::Close, GenericPoint::Clone,
  GenericPoint::Cast,
  GenericPoint::SizeOf,
  GenericPoint::KindCheck);


TypeConstructor igpointTC(
  IGenericPoint::BasicType(),
  IGenericPoint::Property,
  IGenericPoint::Out, IGenericPoint::In,
  0, 0,
  IGenericPoint::Create, IGenericPoint::Delete,
  OpenAttribute<IGenericPoint >,
  SaveAttribute<IGenericPoint >,
  IGenericPoint::Close, IGenericPoint::Clone,
  IGenericPoint::Cast,
  IGenericPoint::SizeOf,
  IGenericPoint::KindCheck);

TypeConstructor gmpointTC(
  GenericMPoint::BasicType(),
  GenericMPoint::Property,
  GenericMPoint::Out, GenericMPoint::In,
  0, 0,
  GenericMPoint::Create, GenericMPoint::Delete,
  OpenAttribute<GenericMPoint >,
  SaveAttribute<GenericMPoint >,
  GenericMPoint::Close, GenericMPoint::Clone,
  GenericMPoint::Cast,
  GenericMPoint::SizeOf,
  GenericMPoint::KindCheck);



/*
1.1.1 ~creategint~

Creates an ~gint~ from two ~int~ (source identifier, target identifier).

*/

const string maps_creategint[2][3] =
{
  {CcString::BasicType(), Point::BasicType(),GenericPoint::BasicType()},
  {CcString::BasicType(), GPoint::BasicType(),GenericPoint::BasicType()}
  
};

ListExpr creategpointTM (ListExpr args)
{ 
  
   return SimpleMaps<2,3>(maps_creategint, args);

}

int creategpointSelect(ListExpr args)
{ 
  

  return SimpleSelect<2,3>(maps_creategint, args);
}

int creategpointVM( Word* args, Word& result, int message, Word& local,
                  Supplier s)
{
  
  result = qp->ResultStorage(s);
  GenericPoint* res = static_cast<GenericPoint*> (result.addr);

  CcString* domain = (CcString*) args[0].addr;
  Point* point = (Point*) args[1].addr;
  //res->Clear();
  if (! domain->IsDefined() || ! point->IsDefined()){
    res->SetDefined(false);
    return 0;
  }
  Domain dDomain ;
  string sDomain =(const char*)(domain->GetStringval());
  if(sDomain == "FreeSpace")
      dDomain = FreeSpace;
  else if (sDomain =="Network")
      dDomain = Network;
  else  //default value
    dDomain = FreeSpace;

  cout<<dDomain<<endl;
  res->SetDefined(true);

  GenericPoint* t = new GenericPoint(*point);
  *res = *t;
  t->DeleteIfAllowed();
  
  return 0;
}


int creategnpointVM( Word* args, Word& result, int message, Word& local,
                  Supplier s)
{ 
  result = qp->ResultStorage(s);
  GenericPoint* res = static_cast<GenericPoint*> (result.addr);

  CcString* domain = (CcString*) args[0].addr;
  network::GPoint* point = (network::GPoint*) args[1].addr;

  if (! domain->IsDefined() || ! point->IsDefined()){
    res->SetDefined(false);
    return 0;
  }

  // Set domain (enum)

  Domain dDomain ;
  string sDomain =(const char*)(domain->GetStringval());

  
  if(sDomain == "FreeSpace"){
      res->SetDefined(false);
      return 0;
    }
  else if(sDomain =="Network")
      dDomain = Network;
  
  cout<<dDomain<<endl; 
 

  GenericPoint* t = new GenericPoint(
                                           *point);
  

  
  *res = *t;

  
  
  //t->DeleteIfAllowed();
  
  return 0;
}

ValueMapping creategpointMap[] =
{
  creategpointVM,
  creategnpointVM
};

const string creategpointSpec =
  "( ( \"Signature\" \"Syntax\" \"Meaning\" \"Example\" ) "
  "(<text><text>query creategint(sourceid,targetid)</text--->))";

Operator creategpointGMO( "creategpoint", creategpointSpec,2 , creategpointMap,
                         creategpointSelect, creategpointTM);


}//end of namespace sgraph

/*
1 Implementation of ~class MOOSAlgebra~

1.1 Constructor

*/
GMOAlgebra::GMOAlgebra():Algebra()
{


AddTypeConstructor(&gpointTC);
gpointTC.AssociateKind(Kind::DATA());
AddTypeConstructor(&igpointTC);
igpointTC.AssociateKind(Kind::DATA());
AddTypeConstructor(&gmpointTC);
gmpointTC.AssociateKind(Kind::DATA());


AddOperator(&creategpointGMO);
}

GMOAlgebra::~GMOAlgebra(){}


extern "C"
Algebra* InitializeGMOAlgebra ( NestedList* nlRef,
                                 QueryProcessor* qpRef )
{
  nl = nlRef;
  qp = qpRef;
  return ( new GMOAlgebra() );
}