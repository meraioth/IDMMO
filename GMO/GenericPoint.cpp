/*
1 Includes*/
#include "GenericPoint.h"
#include "ListUtils.h"
#include "NestedList.h"
#include "NList.h"
#include "Symbols.h"
#include "StandardTypes.h"
#include "SpatialAlgebra.h"
#include "../Network/NetworkAlgebra.h"
#include "../Spatial/SpatialAlgebra.h"




using namespace gmo;
using namespace std;
using namespace network;


/*
1 Implementation of ~class GenericPoint~

1.1 Constructors and Deconstructor

The default constructor should never been used, except in the Cast-Function.

*/

template class GenericPoint<Point>;
template class GenericPoint<GPoint>;

template<class Spatial_Structure>
GenericPoint<Spatial_Structure>::GenericPoint() :
	Attribute()
{}


template<class Spatial_Structure>
GenericPoint<Spatial_Structure>::GenericPoint(const GenericPoint<Spatial_Structure>& other) :
	Attribute(other.IsDefined())
{
	if(other.IsDefined()){
		sS = other.GetSpatialStructure();
		domain = other.GetDomain();
	}else{
		
		
	}
}

template<class Spatial_Structure>
GenericPoint<Spatial_Structure>::GenericPoint(const Spatial_Structure& sS ,const Domain& domain) :
	Attribute(true) , sS(sS) , domain(domain)
{
	if(domain == Stop) //TODO: decide condition to set the variable defined as false
		SetDefined(false);
}

template<class Spatial_Structure>
GenericPoint<Spatial_Structure>::GenericPoint(const bool defined) :
	Attribute(defined), domain(FreeSpace)
{}


template<class Spatial_Structure>
GenericPoint<Spatial_Structure>::~GenericPoint(){}

/*
1.1 Getter and Setter for private Attributes
*/

template<class Spatial_Structure>
Domain GenericPoint<Spatial_Structure>::GetDomain() const{
	return domain;
}

template<class Spatial_Structure>
string GenericPoint<Spatial_Structure>::GetStrDomain() const{
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
      case Stop :
      {
        return "Stop";
        break; 
      }
    }
    return "None";
}





template<class Spatial_Structure>
Spatial_Structure GenericPoint<Spatial_Structure>::GetSpatialStructure() const{
	return sS;
}

/*
1.1 Overwrite Methods from Attribute

*/

template<class Spatial_Structure>
void GenericPoint<Spatial_Structure>::CopyFrom(const Attribute* right)
{
  *this = *((GenericPoint*)right);
}

template<class Spatial_Structure>
Attribute::StorageType GenericPoint<Spatial_Structure>::GetStorageType() const
{
  return Default;
}

template<class Spatial_Structure>
size_t GenericPoint<Spatial_Structure>::HashValue() const
{
  return (size_t) sS.Sizeof() + (size_t) domain;
}

template<class Spatial_Structure>
GenericPoint<Spatial_Structure>* GenericPoint<Spatial_Structure>::Clone() const
{
  return new GenericPoint<Spatial_Structure>(*this);
}

//TODO : define what means adjacent
template<class Spatial_Structure>
bool GenericPoint<Spatial_Structure>::Adjacent(const GenericPoint& other) const
{
  // if (IsDefined() && other.IsDefined())
  //   return (sourceid == other.GetTargetId() || targetid == other.GetSourceId());
  // else
    return false;
}

template<class Spatial_Structure>
bool GenericPoint<Spatial_Structure>::Adjacent(const Attribute* attrib) const
{
  return Adjacent(*((GenericPoint*) attrib));
}

template<class Spatial_Structure>
int GenericPoint<Spatial_Structure>::Compare(const Attribute* rhs) const
{
  return Compare(*((GenericPoint*)rhs));
}

template<class Spatial_Structure>
int GenericPoint<Spatial_Structure>::Compare(const void* ls, const void* rs)
{
  GenericPoint lhs( *(GenericPoint*) ls);
  GenericPoint rhs( *(GenericPoint*) rs);
  return lhs.Compare(rhs);
}

/*TODO : check critera for compare two generic point*/
template<class Spatial_Structure>
int GenericPoint<Spatial_Structure>::Compare(const GenericPoint& in) const
{
  if (!IsDefined() && !in.IsDefined()) return 0;
  if (!IsDefined() && in.IsDefined()) return -1;
  if (IsDefined() && !in.IsDefined()) return 1;
  // if (sourceid < in.GetSourceId()) return -1;
  // if (sourceid > in.GetSourceId()) return 1;
 
 	return 0;
}

template<class Spatial_Structure>
size_t GenericPoint<Spatial_Structure>::Sizeof() const
{
  return sizeof(GenericPoint);
}


template<class Spatial_Structure>
ostream& GenericPoint<Spatial_Structure>::Print(ostream& os) const
{
  os << "GenericPoint: ";
  if (IsDefined())
  {
    // os << "sourceid: " << sourceid
    //    << ", targetid: " << targetid;
       
  }
  else
    os << Symbol::UNDEFINED() << endl;
  return os;
}

template<class Spatial_Structure>
const string GenericPoint<Spatial_Structure>::BasicType()
{
  return "genericpoint";
}

template<class Spatial_Structure>
const bool GenericPoint<Spatial_Structure>::checkType(const ListExpr type)
{
  return listutils::isSymbol(type, BasicType());
}


/*
1.4 Standard Operators

*/

template<class Spatial_Structure>
GenericPoint<Spatial_Structure>& GenericPoint<Spatial_Structure>::operator=(const GenericPoint<Spatial_Structure>& other)
{
  SetDefined(other.IsDefined());
  if (other.IsDefined())
  {
    // sourceid = other.GetSourceId();
    // targetid = other.GetTargetId();

  }
  return *this;
}

template<class Spatial_Structure>
bool GenericPoint<Spatial_Structure>::operator==(const GenericPoint& other) const
{
  return (Compare(&other) == 0);
}

template<class Spatial_Structure>
bool GenericPoint<Spatial_Structure>::operator!=(const GenericPoint& other) const
{
  return (Compare(&other) != 0);
}

template<class Spatial_Structure>
bool GenericPoint<Spatial_Structure>::operator<(const GenericPoint& other) const
{
  return (Compare(&other) < 0);
}

template<class Spatial_Structure>
bool GenericPoint<Spatial_Structure>::operator<=(const GenericPoint& other) const
{
  return (Compare(&other) < 1);
}

template<class Spatial_Structure>
bool GenericPoint<Spatial_Structure>::operator>(const GenericPoint& other) const
{
  return (Compare(&other) > 0);
}

template<class Spatial_Structure>
bool GenericPoint<Spatial_Structure>::operator>=(const GenericPoint& other) const
{
  return (Compare(&other) > -1);
}



/*
1.5 Operators for Secondo Integration
*/
template<class Spatial_Structure>
ListExpr GenericPoint<Spatial_Structure>::Out(ListExpr typeInfo, Word value)
{
  GenericPoint* actValue = (GenericPoint*) value.addr;
  if (!actValue->IsDefined())
    return nl->SymbolAtom(Symbol::UNDEFINED());
  else
  {

    // ListExpr sS_lE = nl->TheEmptyList();
    Spatial_Structure sS(actValue->GetSpatialStructure());

    if(actValue->GetStrDomain() == "FreeSpace")
      return nl->TwoElemList(nl->StringAtom(actValue->GetStrDomain()),
      OutPoint(nl->TheEmptyList(),SetWord((void*) &sS)));
    else 
      return nl->TwoElemList(nl->StringAtom(actValue->GetStrDomain()),
      network::GPoint::OutGPoint(nl->TheEmptyList(),SetWord((void*) &sS)));
  }
}

template<class Spatial_Structure>
Word GenericPoint<Spatial_Structure>::In(const ListExpr typeInfo, const ListExpr instance,
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
        correct = true;
        return (new GenericPoint (false) );
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
          domain = domainList.str();
      else
      {
        correct = false;
        // cmsg.inFunError("1.Element should be " + CcInt::BasicType() + " >= 0.");
        return SetWord(Address(0));
      }

      if(domain == "FreeSpace"){
        correct = true;
        Domain dom = FreeSpace;
        Point* sS = (Point*) InPoint(nl->TheEmptyList(),nl->Second(instance), errorPos, errorInfo , correct).addr;
        if(correct){
          GenericPoint<Point>* out = new GenericPoint<Point>(*sS,dom);
          return SetWord(out);
        }
      }




      // if (spatial_structureList.isInt() && spatial_structureList)
      // {
      //   spatial_structureList = spatial_structureList.intval();
      // }
      // else
      // {
      //   correct = false;
      //   cmsg.inFunError("2.Element should be " + CcInt::BasicType() +" >= 0.");
      //   return SetWord(Address(0));
      // }
      
    }
  }
  correct = false;
  cmsg.inFunError("List length should be two");
  return SetWord(Address(0));
}






template<class Spatial_Structure>
Word GenericPoint<Spatial_Structure>::Create(const ListExpr typeInfo)
{
  return SetWord(new GenericPoint(true));
}



template<class Spatial_Structure>
void GenericPoint<Spatial_Structure>::Delete( const ListExpr typeInfo, Word& w )
{
  ((GenericPoint*) w.addr)->DeleteIfAllowed();
  w.addr = 0;
}

template<class Spatial_Structure>
void GenericPoint<Spatial_Structure>::Close( const ListExpr typeInfo, Word& w )
{
  ((GenericPoint*) w.addr)->DeleteIfAllowed();
  w.addr = 0;
}

template<class Spatial_Structure>
Word GenericPoint<Spatial_Structure>::Clone( const ListExpr typeInfo, const Word& w )
{
  return SetWord(new GenericPoint(*(GenericPoint*) w.addr));
}

template<class Spatial_Structure>
void* GenericPoint<Spatial_Structure>::Cast( void* addr )
{
  return (new (addr) GenericPoint);
}

template<class Spatial_Structure>
bool GenericPoint<Spatial_Structure>::KindCheck ( ListExpr type, ListExpr& errorInfo )
{
  return checkType(type);
}

template<class Spatial_Structure>
int GenericPoint<Spatial_Structure>::SizeOf()
{
  return sizeof(GenericPoint);
}



template<class Spatial_Structure>
ListExpr GenericPoint<Spatial_Structure>::Property()
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

template<class Spatial_Structure>
string GenericPoint<Spatial_Structure>::Example()
{
  return "2 3";
}
