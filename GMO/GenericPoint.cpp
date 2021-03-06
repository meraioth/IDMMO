 /*
1 Includes*/
#include "GenericPoint.h"
#include "ListUtils.h"
#include "NestedList.h"
#include "NList.h"
#include "Symbols.h"
#include "StandardTypes.h"
// #include "../Network/NetworkAlgebra.h"
// #include "../Spatial/SpatialAlgebra.h"
#include "NetworkAlgebra.h"
#include "SpatialAlgebra.h"
#include <typeinfo>




using namespace gmo;
using namespace std;
using namespace network;


/*
1 Implementation of ~class GenericPoint~

1.1 Constructors and Deconstructor

The default constructor should never been used, except in the Cast-Function.

*/

GenericPoint::GenericPoint() :
	Attribute()
{}



GenericPoint::GenericPoint(const GenericPoint& other) :
	Attribute(other.IsDefined())
{
	if(other.IsDefined()){
    if(other.GetDefPoint() == true){
		  point = other.GetPoint();
      tpoint = TPoint(false);
      gpoint =  GPoint(false);
      def_point = true;
      def_gpoint = false;
      def_tpoint = false;
      

    }else if (other.GetDefGPoint() == true ){
      gpoint = other.GetGPoint();
      tpoint = TPoint(false);
      point =  Point(false);
      def_gpoint = true;
      def_point = false;
      def_tpoint = false;
    }
	}else if (other.GetDefTPoint() == true ){
      tpoint = other.GetTPoint();
      point =  Point(false);
      gpoint =  GPoint(false);
      def_gpoint = false;
      def_point = false;
      def_tpoint = true;
    }
}


GenericPoint::GenericPoint(const Point& inpoint) :
	Attribute(true) 
{ 
  domain= FreeSpace; 
  def_point = true;
  def_tpoint = false;
  def_gpoint =false;
  point=inpoint;
  gpoint =  GPoint(false);
  tpoint = TPoint(false);
}
GenericPoint::GenericPoint(const GPoint& ingpoint) :
  Attribute(true) 
{ 
  domain = Network;
  def_point = false;
  def_tpoint = false;
  def_gpoint =true;
  gpoint=ingpoint;
  tpoint = TPoint(false);
  point =  Point(false);
}

GenericPoint::GenericPoint(const TPoint& intpoint) :
  Attribute(true) 
{ 
  domain = FreeSpace;
  def_point = false;
  def_tpoint = true;
  def_gpoint =false;
  TPoint aux(intpoint);
  tpoint = aux;
  gpoint=GPoint(false);
  point =  Point(false);
}


GenericPoint::GenericPoint(const bool defined) :
	Attribute(defined)
{}



GenericPoint::~GenericPoint(){}

/*
1.1 Getter and Setter for private Attributes
*/

bool GenericPoint::GetDefGPoint()const{
  return def_gpoint;
}

bool GenericPoint::GetDefPoint()const{
  return def_point;
}

bool GenericPoint::GetDefTPoint()const{
  return def_tpoint;
}

Point GenericPoint::GetPoint()const{
  return point;
}

GPoint GenericPoint::GetGPoint()const{
  return gpoint;
}

TPoint GenericPoint::GetTPoint()const{
  return tpoint;
}

Domain GenericPoint::GetDomain() const{
	return domain;
}



string GenericPoint::GetStrDomain() const{
  switch(GetDomain()){
      case FreeSpace:
      {
        return "FreeSpace";
        break;
      }

      case Network:
      {
        return "Network";
        break;
      }
      
    }
    return "None";
}






/*Spatial_Structure GenericPoint::GetSpatialStructure() const{
	return sS;
}*/

/*
1.1 Overwrite Methods from Attribute

*/


void GenericPoint::CopyFrom(const Attribute* right)
{
  *this = *((GenericPoint*)right);
}


Attribute::StorageType GenericPoint::GetStorageType() const
{
  return Default;
}


size_t GenericPoint::HashValue() const
{
  return (size_t) point.Sizeof() + (size_t) def_point;
}


GenericPoint* GenericPoint::Clone() const
{
  return new GenericPoint(*this);
}

//TODO : define what means adjacent

bool GenericPoint::Adjacent(const GenericPoint& other) const
{
  // if (IsDefined() && other.IsDefined())
  //   return (sourceid == other.GetTargetId() || targetid == other.GetSourceId());
  // else
    return false;
}


bool GenericPoint::Adjacent(const Attribute* attrib) const
{
  return Adjacent(*((GenericPoint*) attrib));
}


int GenericPoint::Compare(const Attribute* rhs) const
{
  return Compare(*((GenericPoint*)rhs));
}


int GenericPoint::Compare(const void* ls, const void* rs)
{
  GenericPoint lhs( *(GenericPoint*) ls);
  GenericPoint rhs( *(GenericPoint*) rs);
  return lhs.Compare(rhs);
}

/*TODO : check critera for compare two generic point*/

int GenericPoint::Compare(const GenericPoint& in) const
{
  if (!IsDefined() && !in.IsDefined()) return 0;
  if (!IsDefined() && in.IsDefined()) return -1;
  if (IsDefined() && !in.IsDefined()) return 1;
  // if (sourceid < in.GetSourceId()) return -1;
  // if (sourceid > in.GetSourceId()) return 1;
 
 	return 0;
}


size_t GenericPoint::Sizeof() const
{
  return sizeof(GenericPoint);
}



ostream& GenericPoint::Print(ostream& os) const
{
  os << "GenericPoint: ";
  if (IsDefined())
  {
    //os << "domain: " << GetStrDomain();
    //sS.Print(os);
       
  }
  else
    os << Symbol::UNDEFINED() << endl;
  return os;
}


const string GenericPoint::BasicType()
{ 
  return "genericpoint";
}


const bool GenericPoint::checkType(const ListExpr type)
{
  return listutils::isSymbol(type, BasicType());
}


/*
1.4 Standard Operators

*/


GenericPoint& GenericPoint::operator=(const GenericPoint& other)
{
  SetDefined(other.IsDefined());

  if (other.IsDefined())
  {
    if(other.GetDefPoint()){
      def_point=true;
      def_tpoint=false;
      def_gpoint = false;
      point = other.GetPoint();
      gpoint = GPoint(false);
      tpoint = TPoint(false);
      domain = FreeSpace;
    }else if(other.GetDefGPoint()){
      def_point=false;
      def_tpoint=false;
      def_gpoint = true;
      point = Point(false);
      tpoint = TPoint(false);
      gpoint = other.GetGPoint();
      domain = Network;
    }else if(other.GetDefTPoint()){
      cout<<endl;
      def_point=false;
      def_tpoint=false;
      def_gpoint = false;
      point = Point(false);
      tpoint = other.GetTPoint();
      gpoint = GPoint(false);
      domain = FreeSpace;
    }
    // targetid = other.GetTargetId();

  }
  return *this;
}


bool GenericPoint::operator==(const GenericPoint& other) const
{
  return (Compare(&other) == 0);
}


bool GenericPoint::operator!=(const GenericPoint& other) const
{
  return (Compare(&other) != 0);
}


bool GenericPoint::operator<(const GenericPoint& other) const
{
  return (Compare(&other) < 0);
}


bool GenericPoint::operator<=(const GenericPoint& other) const
{
  return (Compare(&other) < 1);
}


bool GenericPoint::operator>(const GenericPoint& other) const
{
  return (Compare(&other) > 0);
}


bool GenericPoint::operator>=(const GenericPoint& other) const
{
  return (Compare(&other) > -1);
}



/*
1.5 Operators for Secondo Integration
*/

ListExpr GenericPoint::Out(ListExpr typeInfo, Word value)
{ 
  GenericPoint* actValue = (GenericPoint*) value.addr;
  if (!actValue->IsDefined())
    return nl->SymbolAtom(Symbol::UNDEFINED());
  else
  {
    if(actValue->GetDefPoint()){
      Point point = actValue->GetPoint();
      return nl->TwoElemList(nl->StringAtom(actValue->GetStrDomain()),
      OutPoint(nl->TheEmptyList(),SetWord( &point)));


    }else if( actValue->GetDefGPoint()){

      GPoint gpoint = actValue->GetGPoint();
      return nl->TwoElemList(nl->StringAtom(actValue->GetStrDomain()),
      GPoint::OutGPoint(nl->TheEmptyList(),SetWord( &gpoint)));

    }else if( actValue->GetDefTPoint()){

      TPoint tpoint = actValue->GetTPoint();
      return nl->TwoElemList(nl->StringAtom(actValue->GetStrDomain()),
      TPoint::Out(nl->TheEmptyList(),SetWord( &tpoint)));

    }
    else //this case should never occur
    return nl->SymbolAtom(Symbol::UNDEFINED());
    
  }
}


Word GenericPoint::In(const ListExpr typeInfo, const ListExpr instance,
               const int errorPos, ListExpr& errorInfo, bool& correct)
{  

  NList in_list(instance);
  if (in_list.length() == 1)
  { 
    NList first = in_list.first();
    if (first.hasStringValue())
    {
      if (first.str() == Symbol::UNDEFINED())
      {
        correct = false;
        cmsg.inFunError("List length should be two");
        return SetWord(Address(0));
      }
    }
  }
  else
  {   

    if (in_list.length() == 2)
    {
      NList domainList(in_list.first());
      NList spatial_structureList(in_list.second());
      
      
      std::string domain = "FreeSpace";
      
      if (domainList.isString() && domainList.str()!= "")
        {   
          domain = domainList.str();
          
      }
      else
      {
        correct = false;
        // cmsg.inFunError("1.Element should be " + CcInt::BasicType() + " >= 0.");
        return SetWord(Address(0));
      }

      

      if(domain == "FreeSpace"){
        if(spatial_structureList.hasLength(2) && (spatial_structureList.first().isReal() || spatial_structureList.first().isInt()) 
        && (spatial_structureList.second().isReal() || spatial_structureList.second().isInt())) //Length 2 means spatial structure is given by a point using coordinates, Length 1 means spatial structure is given by a stop
        {
          Point* sS = (Point*) InPoint(nl->TheEmptyList(),nl->Second(instance), errorPos, errorInfo , correct).addr;
          cout<<"correct : "<<correct<<endl;
          if(correct){
            GenericPoint* out = new GenericPoint(*sS);
            return SetWord(out);
          }
        }
        // else if (spatial_structureList.isString() && spatial_structureList.str()!= "" ){
        //   cout<<"Entro a TPoint"<<endl;
        //   TPoint* sS = (TPoint*) TPoint::In(nl->TheEmptyList(),spatial_structureList.listExpr(), errorPos, errorInfo , correct).addr;
        //   cout<<"Correct : "<<correct<<endl;
        //   if(correct){
        //     GenericPoint* out = new GenericPoint(*sS);
        //     return SetWord(out);
        //   }
        // }
        else if(spatial_structureList.hasLength(1) && spatial_structureList.first().isString() && spatial_structureList.first().str()!= ""){
           // ListExpr stop = nl->Second(instance);
           TPoint* sS = (TPoint*) TPoint::In(nl->TheEmptyList(),nl->Second(instance), errorPos, errorInfo , correct).addr;
          if(correct){
            GenericPoint* out = new GenericPoint(*sS);
            return SetWord(out);
          }
        }
      }else if(domain == "Network"){
        correct = true;
        //Domain dom = Network;
        GPoint* sS = (GPoint*) GPoint::InGPoint(nl->TheEmptyList(),nl->Second(instance), errorPos, errorInfo , correct).addr;
        if(correct){
          GenericPoint* out = new GenericPoint(*sS);
          return SetWord(out);
        }
      }
    }
  }
  correct = false;
  cmsg.inFunError("List length should be two");
  return SetWord(Address(0));
}







Word GenericPoint::Create(const ListExpr typeInfo)
{
  
  return SetWord(new GenericPoint(true));
}




void GenericPoint::Delete( const ListExpr typeInfo, Word& w )
{
  ((GenericPoint*) w.addr)->DeleteIfAllowed();
  w.addr = 0;
}


void GenericPoint::Close( const ListExpr typeInfo, Word& w )
{
  ((GenericPoint*) w.addr)->DeleteIfAllowed();
  w.addr = 0;
}


Word GenericPoint::Clone( const ListExpr typeInfo, const Word& w )
{
  return SetWord(new GenericPoint(*(GenericPoint*) w.addr));
}


void* GenericPoint::Cast( void* addr )
{
  return (new (addr) GenericPoint);
}


bool GenericPoint::KindCheck ( ListExpr type, ListExpr& errorInfo )
{
  return checkType(type);
}


int GenericPoint::SizeOf()
{
  return sizeof(GenericPoint);
}




ListExpr GenericPoint::Property()
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
      nl->TextAtom("(" + CcInt::BasicType() + " "+CcInt::BasicType() + ")"),
      nl->StringAtom("("+ Example() +")")));
}


string GenericPoint::Example()
{
  return "2 3";
}
