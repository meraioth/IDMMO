/*
1 Includes*/
#include <GInterval.h>
#include "ListUtils.h"
#include "NestedList.h"
#include "NList.h"
#include "Symbols.h"
#include "StandardTypes.h"

using namespace sgraph;
using namespace std;


/*
1 Implementation of ~class GInterval~

1.1 Constructors and Deconstructor

The default constructor should never been used, except in the Cast-Function.

*/

GInterval::GInterval() :
	Attribute()
{}


GInterval::GInterval(const GInterval& other) :
	Attribute(other.IsDefined())
{
	if(other.IsDefined()){
		sourceid = other.GetSourceId();
		targetid = other.GetTargetId();
	}else{
		sourceid = 0;
		targetid = 0;
	}
}

GInterval::GInterval(const int sourceid , const int targetid) :
	Attribute(true) , sourceid(sourceid) , targetid(targetid)
{
	if(!(sourceid>0 && targetid>0))
		SetDefined(false)
}

GInterval::GInterval(const bool defined) :
	Attribute(defined), sourceid(0),targetid(0)
{}

GInterval::~GInterval(){}

/*
1.1 Getter and Setter for private Attributes
*/

int GInterval::GetTargetId(){
	return targetid;
}

int GInterval::GetSourceId(){
	return sourceid;
}

/*
1.1 Overwrite Methods from Attribute

*/

void GInterval::CopyFrom(const Attribute* right)
{
  *this = *((GInterval*)right);
}

Attribute::StorageType GInterval::GetStorageType() const
{
  return Default;
}

size_t GInterval::HashValue() const
{
  return (size_t) sourceid + (size_t) targetid;
}

GInterval* GInterval::Clone() const
{
  return new GInterval(*this);
}

bool GInterval::Adjacent(const GInterval& other) const
{
  if (IsDefined() && other.IsDefined())
    return (sourceid == other.GetTargetId() || targetid == other.GetSourceId());
  else
    return false;
}

bool GInterval::Adjacent(const Attribute* attrib) const
{
  return Adjacent(*((GInterval*) attrib));
}

int GInterval::Compare(const Attribute* rhs) const
{
  return Compare(*((GInterval*)rhs));
}

int GInterval::Compare(const void* ls, const void* rs)
{
  GInterval lhs( *(GInterval*) ls);
  GInterval rhs( *(GInterval*) rs);
  return lhs.Compare(rhs);
}

/*TODO : check critera for compare two intervals*/
int GInterval::Compare(const GInterval& in) const
{
  if (!IsDefined() && !in.IsDefined()) return 0;
  if (!IsDefined() && in.IsDefined()) return -1;
  if (IsDefined() && !in.IsDefined()) return 1;
  if (sourceid < in.GetSourceId()) return -1;
  if (sourceid > in.GetSourceId()) return 1;
 
 	return 0;
}


size_t GInterval::Sizeof() const
{
  return sizeof(GInterval);
}

ostream& GInterval::Print(ostream& os) const
{
  os << "GInterval: ";
  if (IsDefined())
  {
    os << "sourceid: " << sourceid
       << ", targetid: " << targetid;
       
  }
  else
    os << Symbol::UNDEFINED() << endl;
  return os;
}

const string GInterval::BasicType()
{
  return "gint";
}

const bool GInterval::checkType(const ListExpr type)
{
  return listutils::isSymbol(type, BasicType());
}

/*
1.4 Standard Operators

*/

GInterval& GInterval::operator=(const GInterval& other)
{
  SetDefined(other.IsDefined());
  if (other.IsDefined())
  {
    sourceid = other.GetSourceId();
    targetid = other.GetTargetId();

  }
  return *this;
}

bool GInterval::operator==(const GInterval& other) const
{
  return (Compare(&other) == 0);
}

bool GInterval::operator!=(const GInterval& other) const
{
  return (Compare(&other) != 0);
}

bool GInterval::operator<(const GInterval& other) const
{
  return (Compare(&other) < 0);
}

bool GInterval::operator<=(const GInterval& other) const
{
  return (Compare(&other) < 1);
}

bool GInterval::operator>(const GInterval& other) const
{
  return (Compare(&other) > 0);
}

bool GInterval::operator>=(const GInterval& other) const
{
  return (Compare(&other) > -1);
}


/*
1.5 Operators for Secondo Integration
*/

ListExpr GInterval::Out(ListExpr typeInfo, Word value)
{
  GInterval* actValue = (GInterval*) value.addr;
  if (!actValue->IsDefined())
    return nl->SymbolAtom(Symbol::UNDEFINED());
  else
  {
    
    return nl->TwoElemList(nl->IntAtom(actValue->GetSourceId()),
                            nl->IntAtom(actValue->GetTargetId()));
  }
}

Word GInterval::In(const ListExpr typeInfo, const ListExpr instance,
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
        return (new GInterval (false) );
      }
    }
  }
  else
  {
    if (in_list.length() == 2)
    {
      NList sourceIdList(in_list.first());
      NList targetIdList(in_list.second());
      
      int sourceId = 0;
      int targetId = 0;
      
      if (sourceIdList.isInt() && sourceIdList.intval() >= 0)
          sourceId = sourceIdList.intval();
      else
      {
        correct = false;
        cmsg.inFunError("1.Element should be " + CcInt::BasicType() + " >= 0.");
        return SetWord(Address(0));
      }
      if (targetIdList.isInt() && targetIdList.intval() >= 0)
      {
        targetId = targetIdList.intval();
      }
      else
      {
        correct = false;
        cmsg.inFunError("2.Element should be " + CcInt::BasicType() +" >= 0.");
        return SetWord(Address(0));
      }
      
      GInterval* res = new GInterval(sourceId, targetId);
    
      return SetWord(res);
    }
  }
  correct = false;
  cmsg.inFunError("List length should be one or two");
  return SetWord(Address(0));
}


Word GInterval::Create(const ListExpr typeInfo)
{
  return SetWord(new GInterval(true));
}
