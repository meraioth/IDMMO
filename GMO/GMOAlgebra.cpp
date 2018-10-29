#include <string>
#include <iostream>
#include "SecondoCatalog.h"
#include "QueryProcessor.h"
#include "Attribute.h"
#include "AlgebraTypes.h"
#include "Operator.h"
#include "ConstructorTemplates.h"
//#include "GenericTC.h"
#include "TypeMapUtils.h"
#include "ListUtils.h"
#include "Symbols.h"
#include "StandardTypes.h"
#include "GMOAlgebra.h"
#include "PairIntDouble.h"
#include "GenericPoint.h"
#include "QueryProcessor.h"   // needed for implementing value mappings
#include "AlgebraManager.h"   // e.g., check for certain kind
#include "../Network/NetworkAlgebra.h"
#include "Point.h"



using namespace std;
using namespace mappings;
using namespace gmo;

extern NestedList* nl;
extern QueryProcessor* qp;

namespace gmo{
  
se vListExpr creategpTM (ListExpr args)
{
  string err = "string x {point,gpoint} expected";
  if(!nl->HasLength(args,2)){
      return listutils::typeError(err);
  }
  if(!CcString::checkType(nl->First(args))){
      return listutils::typeError(err);
  }

  if(!Point::checkType(nl->Second(args)) && !network::GPoint::checkType(nl->Second(args))){
      return listutils::typeError(err);
  }
  if( Point::checkType(nl->Second(args)) ){
    cout<<"paso comprobacion TM Point"<<endl;
    return listutils::basicSymbol<GenericPoint<Point> >();
  }
  else{
    return listutils::basicSymbol<GenericPoint<network::GPoint> >();
  }
  
}


int creategpVMpoint( Word* args, Word& result, int message, Word& local,
                  Supplier s)
{ 
  cout<<"entro a VMpoint"<<endl;
  result = qp->ResultStorage(s);
  GenericPoint<Point>* res = static_cast<GenericPoint<Point>*> (result.addr);

  CcString* domain = (CcString*) args[0].addr;
  Point* point = (Point*) args[1].addr;

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

  res->SetDefined(true);

  GenericPoint<Point>* t = new GenericPoint<Point>(*point,dDomain);
  *res = *t;
  t->DeleteIfAllowed();
  
  return 0;
}

int creategpVMgpoint( Word* args, Word& result, int message, Word& local,
                  Supplier s)
{ cout<<"entro a VMgpoint"<<endl;
  result = qp->ResultStorage(s);
  GenericPoint<network::GPoint>* res = static_cast<GenericPoint<network::GPoint>*> (result.addr);

  CcString* domain = (CcString*) args[0].addr;
  network::GPoint* point = (network::GPoint*) args[1].addr;

  if (! domain->IsDefined() || ! point->IsDefined()){
    res->SetDefined(false);
    return 0;
  }

  // Set domain (enum)

  Domain dDomain ;
  string sDomain =(const char*)(domain->GetStringval());
  if(sDomain == "FreeSpace")
      dDomain = FreeSpace;
  else if (sDomain =="Network")
      dDomain = Network;
  else  //default value
    dDomain = FreeSpace;
  
  res->SetDefined(true);

  GenericPoint<network::GPoint>* t = new GenericPoint<network::GPoint>(
                                           *point,dDomain);
  *res = *t;
  t->DeleteIfAllowed();
  
  return 0;
}



ValueMapping creategpVM[] =
{
  creategpVMpoint,
  creategpVMgpoint,0
};

int creategpSelect(ListExpr args){
  

  if(Point::checkType(nl->Second(args))){
    cout<<"es punto"<<endl;
    return 0;
  }
  if(network::GPoint::checkType(nl->Second(args))){
    return 1;
  }

  return 0;

}

const string creategpSpec =
  "( ( \"Signature\" \"Syntax\" \"Meaning\" \"Example\" ) "
  "(<text>" + CcString::BasicType() + " x " + " -> " +
   "</text--->"+
  "<text>creategint( <sourceid> , <targetid> ) </text--->"+
  "<text>Creates a " + ".</text--->"+
  "<text>query creategp(domain,{point,mpoint})</text--->))";

Operator creategpOp( "creategp", creategpSpec, 2, creategpVM,
                         creategpSelect, creategpTM);


}//end of namespace sgraph

/*
1 Implementation of ~class GMOAlgebra~

1.1 Constructor

*/
GMOAlgebra::GMOAlgebra():Algebra()
{

// AddTypeConstructor(&gpTC);
// gpTC.AssociateKind(Kind::DATA());


 AddOperator(&creategpOp);
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