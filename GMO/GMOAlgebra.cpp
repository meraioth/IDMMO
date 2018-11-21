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
#include "MTPoint.h"
#include "TPoint.h"
#include "UTPoint.h"
#include "Thematic.h"
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

ListExpr
MTPointProperty()
{
  return (nl->TwoElemList(
            nl->FourElemList(nl->StringAtom("Signature"),
                             nl->StringAtom("Example Type List"),
                             nl->StringAtom("List Rep"),
                             nl->StringAtom("Example List")),
            nl->FourElemList(nl->StringAtom("-> MAPPING"),
                             nl->StringAtom("(mtpoint) "),
                             nl->StringAtom("( u1 ... un ) "),
        nl->StringAtom("(((i1 i2 TRUE FALSE) (\"PF13\" \"PF14\")) ...)"))));
};

bool
CheckMTPoint( ListExpr type, ListExpr& errorInfo )
{
  return (nl->IsEqual( type, MTPoint::BasicType() ));
}


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

TypeConstructor tpointTC(
  TPoint::BasicType(),
  TPoint::Property,
  TPoint::Out, TPoint::In,
  0, 0,
  TPoint::Create, TPoint::Delete,
  OpenAttribute<TPoint >,
  SaveAttribute<TPoint >,
  TPoint::Close, TPoint::Clone,
  TPoint::Cast,
  TPoint::SizeOf,
  TPoint::KindCheck);

TypeConstructor utpointTC(
  UTPoint::BasicType(),
  UTPoint::Property,
  UTPoint::Out, UTPoint::In,
  0, 0,
  UTPoint::Create, UTPoint::Delete,
  OpenAttribute<UTPoint >,
  SaveAttribute<UTPoint >,
  UTPoint::Close, UTPoint::Clone,
  UTPoint::Cast,
  UTPoint::SizeOf,
  UTPoint::KindCheck);

TypeConstructor mtpointTC(

        MTPoint::BasicType(),   //name
        MTPointProperty,        //property function describing signature
        OutMapping<MTPoint, UTPoint, UTPoint::Out>,
        InMapping<MTPoint, UTPoint, UTPoint::In>,//Out and In functions
        0,
        0,                 //SaveToList and RestoreFromList functions
        CreateMapping<MTPoint>,
        DeleteMapping<MTPoint>,     //object creation and deletion
        OpenAttribute<MTPoint>,
        SaveAttribute<MTPoint>,      // object open and save
        CloseMapping<MTPoint>,
        CloneMapping<MTPoint>, //object close and clone
        CastMapping<MTPoint>,    //cast function
        SizeOfMapping<MTPoint>, //sizeof function
        CheckMTPoint );  //kind checking function


TypeConstructor thematicunitTC(
  ThematicUnit::BasicType(),
  ThematicUnit::Property,
  ThematicUnit::Out, ThematicUnit::In,
  0, 0,
  ThematicUnit::Create, ThematicUnit::Delete,
  OpenAttribute<ThematicUnit >,
  SaveAttribute<ThematicUnit >,
  ThematicUnit::Close, ThematicUnit::Clone,
  ThematicUnit::Cast,
  ThematicUnit::SizeOf,
  ThematicUnit::KindCheck);

// TypeConstructor thematicpathTC(
//   ThematicUnit::BasicType(),
//   ThematicUnit::Property,
//   ThematicUnit::Out, ThematicUnit::In,
//   0, 0,
//   ThematicUnit::Create, ThematicUnit::Delete,
//   OpenAttribute<ThematicUnit >,
//   SaveAttribute<ThematicUnit >,
//   ThematicUnit::Close, ThematicUnit::Clone,
//   ThematicUnit::Cast,
//   ThematicUnit::SizeOf,
//   ThematicUnit::KindCheck);


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
AddTypeConstructor(&tpointTC);
tpointTC.AssociateKind(Kind::DATA());
AddTypeConstructor(&utpointTC);
utpointTC.AssociateKind(Kind::DATA());
AddTypeConstructor(&mtpointTC);
mtpointTC.AssociateKind(Kind::DATA());


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