/*
1 Includes*/
#include "IGenericPoint.h"
#include "ListUtils.h"
#include "NestedList.h"
#include "NList.h"
#include "Symbols.h"
#include "StandardTypes.h"
#include "SpatialAlgebra.h"
#include "../Network/NetworkAlgebra.h"
#include "../Spatial/SpatialAlgebra.h"
#include "DateTime.h"
#include <typeinfo>




using namespace gmo;
using namespace std;
using namespace network;


/*
1 Implementation of ~class IGenericPoint~

1.1 Constructors and Deconstructor

The default constructor should never been used, except in the Cast-Function.

*/

IGenericPoint::IGenericPoint() :
	Attribute()
{}



IGenericPoint::IGenericPoint(const IGenericPoint& other) :
	Attribute(other.IsDefined())
{
	if(other.IsDefined()){
    if(other.GetDefIPoint() == true){
		  ipoint = other.GetIPoint();
      igpoint = IGPoint(false);
      def_ipoint = true;
      def_igpoint = false;
    }else if (other.GetDefIGPoint() == true ){
      igpoint = other.GetIGPoint();
      ipoint = IPoint(false);
      def_igpoint = true;
      def_ipoint = false;
    }
	}else{
		
		
	}
}


IGenericPoint::IGenericPoint(const Instant& _instant ,const  Point _point) :
	Attribute(true) 
{ 
  if(_instant.IsDefined() && _point.IsDefined()){
    domain = FreeSpace;
    def_ipoint = true;
    def_igpoint = false;
    ipoint = IPoint(_instant,_point);
    igpoint = IGPoint(false);

  }else{
    SetDefined(false);

  }
}

IGenericPoint::IGenericPoint(const Instant& _instant , const GPoint _gpoint) :
  Attribute(true) 
{ 
  if(_instant.IsDefined() && _gpoint.IsDefined()){
    domain = Network;
    def_ipoint = false;
    def_igpoint = true;
    ipoint = IPoint(false);
    igpoint = IGPoint(_instant,_gpoint);

  }else{
    SetDefined(false);

  }
}

IGenericPoint::IGenericPoint(const IPoint& _ipoint) :
  Attribute(true) 
{ 
  if(_ipoint.IsDefined() ){
    domain = FreeSpace;
    def_ipoint = true;
    def_igpoint = false;
    ipoint = _ipoint;
    igpoint = IGPoint(false);

  }else{
    SetDefined(false);

  }
}

IGenericPoint::IGenericPoint(const IGPoint& _igpoint) :
  Attribute(true) 
{ 
 if(_igpoint.IsDefined() ){
    domain = Network;
    def_ipoint = false;
    def_igpoint = true;
    ipoint = IPoint(false);
    igpoint = _igpoint;

  }else{
    SetDefined(false);

  }
}


IGenericPoint::IGenericPoint(const bool defined) :
	Attribute(defined)
{}



IGenericPoint::~IGenericPoint(){}

/*
1.1 Getter and Setter for private Attributes
*/

bool IGenericPoint::GetDefIGPoint()const{
  return def_igpoint;
}

bool IGenericPoint::GetDefIPoint()const{
  return def_ipoint;
}

IPoint IGenericPoint::GetIPoint()const{
  return ipoint;
}

IGPoint IGenericPoint::GetIGPoint()const{
  return igpoint;
}


Domain IGenericPoint::GetDomain() const{
	return domain;
}



string IGenericPoint::GetStrDomain() const{
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






/*Spatial_Structure IGenericPoint::GetSpatialStructure() const{
	return sS;
}*/

/*
1.1 Overwrite Methods from Attribute

*/


void IGenericPoint::CopyFrom(const Attribute* right)
{
  *this = *((IGenericPoint*)right);
}


Attribute::StorageType IGenericPoint::GetStorageType() const
{
  return Default;
}


size_t IGenericPoint::HashValue() const
{
  return (size_t) ipoint.Sizeof() + (size_t) def_ipoint;
}


IGenericPoint* IGenericPoint::Clone() const
{
  return new IGenericPoint(*this);
}

//TODO : define what means adjacent

bool IGenericPoint::Adjacent(const IGenericPoint& other) const
{
  // if (IsDefined() && other.IsDefined())
  //   return (sourceid == other.GetTargetId() || targetid == other.GetSourceId());
  // else
    return false;
}


bool IGenericPoint::Adjacent(const Attribute* attrib) const
{
  return Adjacent(*((IGenericPoint*) attrib));
}


int IGenericPoint::Compare(const Attribute* rhs) const
{
  return Compare(*((IGenericPoint*)rhs));
}


int IGenericPoint::Compare(const void* ls, const void* rs)
{
  IGenericPoint lhs( *(IGenericPoint*) ls);
  IGenericPoint rhs( *(IGenericPoint*) rs);
  return lhs.Compare(rhs);
}

/*TODO : check critera for compare two generic point*/

int IGenericPoint::Compare(const IGenericPoint& in) const
{
  if (!IsDefined() && !in.IsDefined()) return 0;
  if (!IsDefined() && in.IsDefined()) return -1;
  if (IsDefined() && !in.IsDefined()) return 1;
  // if (sourceid < in.GetSourceId()) return -1;
  // if (sourceid > in.GetSourceId()) return 1;
 
 	return 0;
}


size_t IGenericPoint::Sizeof() const
{
  return sizeof(IGenericPoint);
}



ostream& IGenericPoint::Print(ostream& os) const
{
  os << "IGenericPoint: ";
  if (IsDefined())
  {
    //os << "domain: " << GetStrDomain();
    //sS.Print(os);
       
  }
  else
    os << Symbol::UNDEFINED() << endl;
  return os;
}


const string IGenericPoint::BasicType()
{ 
  return "igenericpoint";
}


const bool IGenericPoint::checkType(const ListExpr type)
{
  return listutils::isSymbol(type, BasicType());
}


/*
1.4 Standard Operators

*/


IGenericPoint& IGenericPoint::operator=(const IGenericPoint& other)
{
  SetDefined(other.IsDefined());
  if (other.IsDefined())
  {
    if(other.GetDefIPoint()){
      def_ipoint=true;
      def_igpoint = false;
      ipoint = other.GetIPoint();
      igpoint = IGPoint(false);
      domain = FreeSpace;
    }else if(other.GetDefIGPoint()){
      
      def_ipoint=false;
      def_igpoint = true;
      ipoint = IPoint(false);
      igpoint = other.GetIGPoint();
      domain = Network;
    }
    // targetid = other.GetTargetId();

  }
  return *this;
}


bool IGenericPoint::operator==(const IGenericPoint& other) const
{
  return (Compare(&other) == 0);
}


bool IGenericPoint::operator!=(const IGenericPoint& other) const
{
  return (Compare(&other) != 0);
}


bool IGenericPoint::operator<(const IGenericPoint& other) const
{
  return (Compare(&other) < 0);
}


bool IGenericPoint::operator<=(const IGenericPoint& other) const
{
  return (Compare(&other) < 1);
}


bool IGenericPoint::operator>(const IGenericPoint& other) const
{
  return (Compare(&other) > 0);
}


bool IGenericPoint::operator>=(const IGenericPoint& other) const
{
  return (Compare(&other) > -1);
}



/*
1.5 Operators for Secondo Integration
*/

ListExpr IGenericPoint::Out(ListExpr typeInfo, Word value)
{ 
  IGenericPoint* actValue = (IGenericPoint*) value.addr;
  
  if (!actValue->IsDefined())
    return nl->SymbolAtom(Symbol::UNDEFINED());
  else
  {
    if(actValue->GetDefIPoint()){
      IPoint ipoint = actValue->GetIPoint();
      return nl->TwoElemList(nl->StringAtom(actValue->GetStrDomain()),
      OutIntime<Point, OutPoint>(nl->TheEmptyList(),SetWord( &ipoint)));


    }else if( actValue->GetDefIGPoint()){

      IGPoint igpoint = actValue->GetIGPoint();
      return nl->TwoElemList(nl->StringAtom(actValue->GetStrDomain()),
      OutIntime<GPoint, GPoint::OutGPoint>(nl->TheEmptyList(),SetWord( &igpoint)));

    }else //this case should never occur
    return nl->SymbolAtom(Symbol::UNDEFINED());
    
  }
}


Word IGenericPoint::In(const ListExpr typeInfo, const ListExpr instance,
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
      else{

      // Intime<Alpha> *intime = new Intime<Alpha>(false);
      // intime->SetDefined( false );
      // correct = true;
      // return SetWord( intime );
      }
    }
  }
  else
  {
    if (in_list.length() == 2)
    {
      
      NList domainList(in_list.first());
      NList intimeList(in_list.second());
      
      
      std::string domain = "FreeSpace";
      
      if (domainList.isString() && domainList.str()!= "")
        {   
          domain = domainList.str();
          cout<<"Domain : "<<domain<<endl;
          
      }
      else
      {
       
        correct = false;
        // cmsg.inFunError("1.Element should be " + CcInt::BasicType() + " >= 0.");
        return SetWord(Address(0));
      }

      

      if(domain == "FreeSpace"){
        ListExpr intime = nl->Second(instance);
        Instant *instant = (Instant *)datetime::InInstant( nl->TheEmptyList(),
                                             nl->First( intime ),
                                                         errorPos,
                                                         errorInfo,
                                                         correct ).addr;
        string output;
        nl->WriteToString(output,intime);
        cout<<"ListExpr intime : "<<output<<endl;
        cout<<"corret :"<<correct<<endl;
        if(!correct)
        {
          if(instant){
             delete instant;
          }
          return SetWord( Address(0) );
        }

        Point *value = (Point *)InPoint( nl->TheEmptyList(),
                                       nl->Second( intime ),
                                       errorPos, errorInfo, correct ).addr;

        ostream& o = cout;

        value->Print(o);
        cout<<endl;


        cout<<"corret after point :"<<correct<<endl;
        if( correct  )
        {
          Intime<Point> *intime = new Intime<Point>( *instant, *value );
          delete instant;
          delete value;
          IGenericPoint* igp = new IGenericPoint(*intime);
          intime->Print(o);
          cout<<endl;
          delete intime;
          return SetWord( igp );
        }
        delete instant;
        if(value){
           delete value;
        }
      }

      else if(domain == "Network"){
        ListExpr intime = nl->Second(instance);
        Instant *instant = (Instant *)datetime::InInstant( nl->TheEmptyList(),
                                             nl->First( intime ),
                                                         errorPos,
                                                         errorInfo,
                                                         correct ).addr;

        string output;
        nl->WriteToString(output,intime);
        cout<<"ListExpr intime : "<<output<<endl;
        cout<<"corret :"<<correct<<endl;

        if(!correct)
        {
          if(instant){
             delete instant;
          }
          return SetWord( Address(0) );
        }

        GPoint *value = (GPoint *)GPoint::InGPoint( nl->TheEmptyList(),
                                       nl->Second( intime ),
                                       errorPos, errorInfo, correct ).addr;

        ostream& o = cout;

        value->Print(o);
        cout<<endl;


        cout<<"corret after gpoint :"<<correct<<endl;
        if( correct  )
        {
          Intime<GPoint> *intime = new Intime<GPoint>( *instant, *value );
          delete instant;
          delete value;
          IGenericPoint* igp = new IGenericPoint(*intime);
          return SetWord( igp );
        }
        delete instant;
        if(value){
           delete value;
        }
      }
    }
  }
  correct = false;
  cmsg.inFunError("List length should be two");
  return SetWord(Address(0));
}







Word IGenericPoint::Create(const ListExpr typeInfo)
{
  
  return SetWord(new IGenericPoint(true));
}




void IGenericPoint::Delete( const ListExpr typeInfo, Word& w )
{
  ((IGenericPoint*) w.addr)->DeleteIfAllowed();
  w.addr = 0;
}


void IGenericPoint::Close( const ListExpr typeInfo, Word& w )
{
  ((IGenericPoint*) w.addr)->DeleteIfAllowed();
  w.addr = 0;
}


Word IGenericPoint::Clone( const ListExpr typeInfo, const Word& w )
{
  return SetWord(new IGenericPoint(*(IGenericPoint*) w.addr));
}


void* IGenericPoint::Cast( void* addr )
{
  return (new (addr) IGenericPoint);
}


bool IGenericPoint::KindCheck ( ListExpr type, ListExpr& errorInfo )
{
  return checkType(type);
}


int IGenericPoint::SizeOf()
{
  return sizeof(IGenericPoint);
}




ListExpr IGenericPoint::Property()
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
      nl->TextAtom("()"),
      nl->StringAtom("("+ Example() +")")));
}

//TODO: Change Example
string IGenericPoint::Example()
{
  return "2 3";
}
