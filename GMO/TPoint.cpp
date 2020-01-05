/*
1 Includes*/
#include "TPoint.h"
#include "ListUtils.h"
#include "NestedList.h"
#include "NList.h"
#include "Symbols.h"
#include "StandardTypes.h"
#include "NetworkAlgebra.h"
#include "SpatialAlgebra.h"
// #include "../Network/NetworkAlgebra.h"
// #include "../Spatial/SpatialAlgebra.h"
#include "DateTime.h"
#include <typeinfo>




using namespace gmo;
using namespace std;


/*
1 Implementation of ~class TPoint~

1.1 Constructors and Deconstructor

The default constructor should never been used, except in the Cast-Function.

*/
TPoint::TPoint():Attribute()
 {}


  
TPoint::TPoint( STRING_T stop):Attribute(true){
  sprintf(stop_id, "%s", stop);
}

TPoint::TPoint( string stop):Attribute(true){
  sprintf(stop_id, "%s", stop.c_str());
}

TPoint::TPoint(const bool def):Attribute(def){}

TPoint::TPoint( const TPoint& in_xOther):Attribute(in_xOther.IsDefined()){
	if(in_xOther.IsDefined()){
    sprintf(stop_id, "%s", in_xOther.GetStop().c_str());
		
	}
}
TPoint::~TPoint(){};


string TPoint::GetStop()const{
  string out(stop_id);
	return out;
}



void TPoint::SetStop( string stop){
  sprintf(stop_id, "%s", stop.c_str());
}

void TPoint::SetStop( STRING_T stop){
  sprintf(stop_id, "%s", stop);
}
/*
1.1 Overwrite Methods from Attribute

*/


void TPoint::CopyFrom(const Attribute* right)
{
  *this = *((TPoint*)right);
}


Attribute::StorageType TPoint::GetStorageType() const
{
  return Default;
}


size_t TPoint::HashValue() const
{
  return (size_t) sizeof(STRING_T);
}


TPoint* TPoint::Clone() const
{
  return new TPoint(*this);
}

//TODO : define what means adjacent

bool TPoint::Adjacent(const TPoint& other) const
{
  // if (IsDefined() && other.IsDefined())
  //   return (sourceid == other.GetTargetId() || targetid == other.GetSourceId());
  // else
    return false;
}


bool TPoint::Adjacent(const Attribute* attrib) const
{
  return Adjacent(*((TPoint*) attrib));
}


int TPoint::Compare(const Attribute* rhs) const
{
  return Compare(*((TPoint*)rhs));
}


int TPoint::Compare(const void* ls, const void* rs)
{
  TPoint lhs( *(TPoint*) ls);
  TPoint rhs( *(TPoint*) rs);
  return lhs.Compare(rhs);
}

/*TODO : check critera for compare two generic point*/

int TPoint::Compare(const TPoint& in) const
{
  if (!IsDefined() && !in.IsDefined()) return 0;
  if (!IsDefined() && in.IsDefined()) return -1;
  if (IsDefined() && !in.IsDefined()) return -1;
  // if (stop_id == in.GetStop()) return 1;

 
 	return 0;
}


size_t TPoint::Sizeof() const
{
  return sizeof(TPoint);
}



ostream& TPoint::Print(ostream& os) const
{
  os << "TPoint: ";
  if (IsDefined())
  {
    os << "stop : " << GetStop();
 
       
  }
  else
    os << Symbol::UNDEFINED() << endl;
  return os;
}

const string TPoint::BasicType(){ 
	return "tpoint"; 
}

const bool TPoint::checkType(const ListExpr type){
	return listutils::isSymbol(type, BasicType());
}



/*
1.4 Standard Operators

*/



TPoint& TPoint::operator=( const TPoint& in_xOther )
{
	SetDefined(in_xOther.IsDefined());
	if( IsDefined())
	{

    sprintf(stop_id, "%s", in_xOther.GetStop().c_str());
	}
	return *this;
}


bool TPoint::operator==(const TPoint& other) const
{
  return (Compare(&other) == 1);
}


bool TPoint::operator!=(const TPoint& other) const
{
  return (Compare(&other) != 0);
}


bool TPoint::operator<(const TPoint& other) const
{
  return (Compare(&other) < 0);
}


bool TPoint::operator<=(const TPoint& other) const
{
  return (Compare(&other) < 1);
}


bool TPoint::operator>(const TPoint& other) const
{
  return (Compare(&other) > 0);
}


bool TPoint::operator>=(const TPoint& other) const
{
  return (Compare(&other) > -1);
}



/*
1.5 Operators for Secondo Integration
*/

ListExpr TPoint::Out(ListExpr typeInfo, Word value)
{ 
  cout<<"TPoint::Out"<<value.addr<<endl;
  TPoint* actValue = (TPoint*) value.addr;
  
  if (!actValue->IsDefined())
    return nl->SymbolAtom(Symbol::UNDEFINED());
  else
  {
      return nl->OneElemList(nl->StringAtom(actValue->GetStop()));
    
  }
}


Word TPoint::In(const ListExpr typeInfo, const ListExpr instance,
               const int errorPos, ListExpr& errorInfo, bool& correct)
{  


  NList in_list(instance);
  if (in_list.length() == 0)
  { 
    correct = false;
    cmsg.inFunError("List length should be one");
    return SetWord(Address(0));
      
  }
  else
  {	
  	
  	if(in_list.first().isString() && in_list.first().str()!= ""){
  		
      TPoint* tpoint = new TPoint(in_list.first().str());
 	  correct=true;
    cout<<"TPoint:in "<<tpoint<<endl;
      return SetWord(tpoint);
     }else{
     	correct = false;
    	cmsg.inFunError("List should have a string value");
    	return SetWord(Address(0));
     }
        
  }

  
}



Word TPoint::Create(const ListExpr typeInfo)
{
  
  return SetWord(new TPoint(true));
}




void TPoint::Delete( const ListExpr typeInfo, Word& w )
{
  ((TPoint*) w.addr)->DeleteIfAllowed();
  w.addr = 0;
}


void TPoint::Close( const ListExpr typeInfo, Word& w )
{
  ((TPoint*) w.addr)->DeleteIfAllowed();
  w.addr = 0;
}


Word TPoint::Clone( const ListExpr typeInfo, const Word& w )
{
  return SetWord(new TPoint(*(TPoint*) w.addr));
}


void* TPoint::Cast( void* addr )
{
  return (new (addr) TPoint);
}


bool TPoint::KindCheck ( ListExpr type, ListExpr& errorInfo )
{
  return checkType(type);
}


int TPoint::SizeOf()
{
  return sizeof(TPoint);
}




ListExpr TPoint::Property()
{
  return nl->TwoElemList(
    nl->FourElemList(
      nl->StringAtom("Signature"),
      nl->StringAtom("Example Type List"),
      nl->StringAtom("List Rep"),
      nl->StringAtom("Example List")),
    nl->FourElemList(
      nl->StringAtom("-> " + Kind::DATA()),
      nl->StringAtom(BasicType()),
      nl->TextAtom("(" + BasicType() + ")"),
      nl->StringAtom("("+ Example() +")")));
}


const string TPoint::Example()
{
  return "PT102";
}














/*
Computes the euclidean distance of 2 glines.

*/
double TPoint::Distance (const TPoint* toTPoint)const {
	return 0.0f;
} 



void TPoint::ToPoint(Point *& res)const{}

Point* TPoint::ToPoint()const{
	return new Point(false);
}




/*
Returns a gline representing the shortest path between two GPoint.
Using DijkstrasAlgorithm

*/

// bool ShortestPath(const GPoint *ziel, GLine *result,
// DbArray<TupleId>* touchedSects = 0) const;
// bool ShortestPath(const GPoint* ziel, GLine *result,
// const Network* pNetwork,
// DbArray<TupleId>* touchedSects = 0) const;




