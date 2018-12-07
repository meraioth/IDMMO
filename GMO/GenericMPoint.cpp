/*
1 Includes*/
#include "GenericMPoint.h"
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
using namespace temporalnet;


/*
1 Implementation of ~class GenericMPoint~

1.1 Constructors and Deconstructor

The default constructor should never been used, except in the Cast-Function.

*/

GenericMPoint::GenericMPoint() :
	Attribute()
{}



GenericMPoint::GenericMPoint(const GenericMPoint& other) :
	Attribute(other.IsDefined())
{
	if(other.IsDefined()){
    if(other.GetDefMPoint()){
		  mpoint = new MPoint(other.GetMPoint());
      mtpoint = new MTPoint(false);
      mgpoint = new MGPoint(false);
      def_mpoint = true;
      def_mgpoint = false;
      def_mtpoint = false;
      domain = FreeSpace;
    }else if (other.GetDefMGPoint()){
      mgpoint = new MGPoint(other.GetMGPoint());
      mtpoint = new MTPoint(false);
      mpoint = new MPoint(false);
      def_mgpoint = true;
      def_mpoint = false;
      def_mtpoint = false;
      domain = Network;

    }else if(other.GetDefMTPoint()){
      mtpoint = new MTPoint(other.GetMTPoint());
      mpoint = new MPoint(false);
      mgpoint = new MGPoint(false);
      def_mgpoint = true;
      def_mpoint = false;
      def_mtpoint = false;
      domain = FreeSpace;
    }
	}else{
		
		
	}
}


GenericMPoint::GenericMPoint(const MPoint& _mpoint) :
  Attribute(true) 
{ 
  if(_mpoint.IsDefined() ){
    domain = FreeSpace;
    def_mpoint = true;
    def_mgpoint = false;
    mpoint = new MPoint(_mpoint);
    mgpoint = new MGPoint(false);

  }else{
    SetDefined(false);

  }
}

GenericMPoint::GenericMPoint(const MGPoint& _mgpoint) :
  Attribute(true) 
{ 
 if(_mgpoint.IsDefined() ){
    domain = Network;
    def_mpoint = false;
    def_mgpoint = true;

    mpoint = new MPoint(false);
    mgpoint = new MGPoint(_mgpoint);

  }else{
    SetDefined(false);

  }
}


GenericMPoint::GenericMPoint(const MTPoint& _mtpoint) :
  Attribute(true) 
{ 
 if(_mtpoint.IsDefined() ){
    domain = FreeSpace;
    def_mpoint = false;
    def_mgpoint = false;
    def_mtpoint = true ;

    mpoint = new MPoint(false);
    mgpoint = new MGPoint(false);
    mtpoint = new MTPoint(_mtpoint);

  }else{
    SetDefined(false);

  }
}


GenericMPoint::GenericMPoint(const bool defined) :
	Attribute(defined)
{}



GenericMPoint::~GenericMPoint(){}

/*
1.1 Getter and Setter for private Attributes
*/

bool GenericMPoint::GetDefMGPoint()const{
  return def_mgpoint;
}

bool GenericMPoint::GetDefMPoint()const{
  return def_mpoint;
}
bool GenericMPoint::GetDefMTPoint()const{
  return def_mtpoint;
}

MPoint GenericMPoint::GetMPoint()const{
  MPoint other(*mpoint);
  return other;
}

MTPoint GenericMPoint::GetMTPoint()const{
  MTPoint other(*mtpoint);
  return other;
}

MGPoint GenericMPoint::GetMGPoint()const{
  MGPoint other(*mgpoint);
  return other;
}


Domain GenericMPoint::GetDomain() const{
	return domain;
}



string GenericMPoint::GetStrDomain() const{
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






/*Spatial_Structure GenericMPoint::GetSpatialStructure() const{
	return sS;
}*/

/*
1.1 Overwrite Methods from Attribute

*/


void GenericMPoint::CopyFrom(const Attribute* right)
{
  *this = *((GenericMPoint*)right);
}


Attribute::StorageType GenericMPoint::GetStorageType() const
{
  return Default;
}


size_t GenericMPoint::HashValue() const
{
  return (size_t) mpoint->Sizeof() + (size_t) def_mpoint;
}


GenericMPoint* GenericMPoint::Clone() const
{
  return new GenericMPoint(*this);
}

//TODO : define what means adjacent

bool GenericMPoint::Adjacent(const GenericMPoint& other) const
{
  // if (IsDefined() && other.IsDefined())
  //   return (sourceid == other.GetTargetId() || targetid == other.GetSourceId());
  // else
    return false;
}


bool GenericMPoint::Adjacent(const Attribute* attrib) const
{
  return Adjacent(*((GenericMPoint*) attrib));
}


int GenericMPoint::Compare(const Attribute* rhs) const
{
  return Compare(*((GenericMPoint*)rhs));
}


int GenericMPoint::Compare(const void* ls, const void* rs)
{
  GenericMPoint lhs( *(GenericMPoint*) ls);
  GenericMPoint rhs( *(GenericMPoint*) rs);
  return lhs.Compare(rhs);
}

/*TODO : check critera for compare two generic point*/

int GenericMPoint::Compare(const GenericMPoint& in) const
{
  if (!IsDefined() && !in.IsDefined()) return 0;
  if (!IsDefined() && in.IsDefined()) return -1;
  if (IsDefined() && !in.IsDefined()) return 1;
  // if (sourceid < in.GetSourceId()) return -1;
  // if (sourceid > in.GetSourceId()) return 1;
 
 	return 0;
}


size_t GenericMPoint::Sizeof() const
{
  return sizeof(GenericMPoint);
}



ostream& GenericMPoint::Print(ostream& os) const
{
  os << "GenericMPoint: ";
  if (IsDefined())
  {
    //os << "domain: " << GetStrDomain();
    //sS.Print(os);
       
  }
  else
    os << Symbol::UNDEFINED() << endl;
  return os;
}


const string GenericMPoint::BasicType()
{ 
  return "genericmpoint";
}


const bool GenericMPoint::checkType(const ListExpr type)
{
  return listutils::isSymbol(type, BasicType());
}


/*
1.4 Standard Operators

*/


GenericMPoint& GenericMPoint::operator=(const GenericMPoint& other)
{
  SetDefined(other.IsDefined());
  if (other.IsDefined())
  {
    if(other.GetDefMPoint()){
      def_mpoint=true;
      def_mgpoint = false;
      def_mtpoint=false;
      mtpoint = new MTPoint(false);
      mpoint = new MPoint(other.GetMPoint());
      mgpoint =new  MGPoint(false);
      domain = FreeSpace;
    }else if(other.GetDefMGPoint()){
      
      def_mpoint=false;
      def_mgpoint = true;
      def_mtpoint=false;
      mpoint = new MPoint(false);
      mtpoint = new MTPoint(false);
      mgpoint = new MGPoint( other.GetMGPoint());
      domain = Network;
    }
    else if(other.GetDefMTPoint()){
      
      def_mpoint=false;
      def_mtpoint=true;
      def_mgpoint = false;
      mpoint = new MPoint(false);
      mgpoint = new MGPoint(false);
      mtpoint = new MTPoint( other.GetMTPoint());
      domain = FreeSpace;
    }
    // targetid = other.GetTargetId();

  }
  return *this;
}


bool GenericMPoint::operator==(const GenericMPoint& other) const
{
  return (Compare(&other) == 0);
}


bool GenericMPoint::operator!=(const GenericMPoint& other) const
{
  return (Compare(&other) != 0);
}


bool GenericMPoint::operator<(const GenericMPoint& other) const
{
  return (Compare(&other) < 0);
}


bool GenericMPoint::operator<=(const GenericMPoint& other) const
{
  return (Compare(&other) < 1);
}


bool GenericMPoint::operator>(const GenericMPoint& other) const
{
  return (Compare(&other) > 0);
}


bool GenericMPoint::operator>=(const GenericMPoint& other) const
{
  return (Compare(&other) > -1);
}



/*
1.5 Operators for Secondo Integration
*/

ListExpr GenericMPoint::Out(ListExpr typeInfo, Word value)
{ 
  GenericMPoint* actValue = (GenericMPoint*) value.addr;
  
  if (!actValue->IsDefined())
    return nl->SymbolAtom(Symbol::UNDEFINED());
  else
  {
    if(actValue->GetDefMPoint()){
      MPoint mpoint = actValue->GetMPoint();
      return nl->TwoElemList(nl->StringAtom(actValue->GetStrDomain()),
      OutMapping<MPoint, UPoint, OutUPoint>(nl->TheEmptyList(),SetWord( &mpoint)));


    }else if( actValue->GetDefMGPoint()){

      MGPoint mgpoint = actValue->GetMGPoint();
      return nl->TwoElemList(nl->StringAtom(actValue->GetStrDomain()),
      OutMapping<MGPoint, UGPoint, UGPoint::Out>(nl->TheEmptyList(),SetWord( &mgpoint)));

    }else if( actValue->GetDefMTPoint()){

      MTPoint mgpoint = actValue->GetMTPoint();
      return nl->TwoElemList(nl->StringAtom(actValue->GetStrDomain()),
      OutMapping<MTPoint, UTPoint, UTPoint::Out>(nl->TheEmptyList(),SetWord( &mgpoint)));

    }else //this case should never occur
    return nl->SymbolAtom(Symbol::UNDEFINED());
    
  }
}


Word GenericMPoint::In(const ListExpr typeInfo, const ListExpr instance,
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
        ListExpr mp = nl->Second(instance);
        NList mp_(nl->Second(instance));
        
        //Check if mp contains a string or integer as value
        NList first_elem(nl->First(mp));
        NList space(first_elem.second());
        
        if(space.length()==2 && space.first().isString() && space.second().isString()){
          MTPoint *value = (MTPoint *)InMapping<MTPoint, UTPoint, UTPoint::In>( nl->TheEmptyList(),
                                       mp,
                                       errorPos, errorInfo, correct ).addr;
          if( correct  ){
            GenericMPoint* mpt = new GenericMPoint(*value);
            
            delete value;
            return SetWord( mpt );
          }
          if(value){
             delete value;
          }
        }else{

          MPoint *value = (MPoint *)InMapping<MPoint, UPoint, InUPoint>( nl->TheEmptyList(),
                                       mp,
                                       errorPos, errorInfo, correct ).addr;


          cout<<"corret after point :"<<correct<<endl;
          if( correct  )
          {
            
            GenericMPoint* mpt = new GenericMPoint(*value);

            delete value;
            return SetWord( mpt );
          }
          if(value){
             delete value;
          }
        }
      }else if(domain == "Network"){
        ListExpr mp = nl->Second(instance);
    

        MGPoint *value = (MGPoint *)InMapping<MGPoint, UGPoint, UGPoint::In>( nl->TheEmptyList(),
                                       mp ,
                                       errorPos, errorInfo, correct ).addr;

        ostream& o = cout;

        value->Print(o);
        cout<<endl;


        cout<<"corret after point :"<<correct<<endl;
        if( correct  )
        {
          
          GenericMPoint* mpt = new GenericMPoint(*value);
          mpt->Print(o);
          cout<<endl;
          delete value;
          return SetWord( mpt );
        }
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







Word GenericMPoint::Create(const ListExpr typeInfo)
{
  
  return SetWord(new GenericMPoint(true));
}




void GenericMPoint::Delete( const ListExpr typeInfo, Word& w )
{
  ((GenericMPoint*) w.addr)->DeleteIfAllowed();
  w.addr = 0;
}


void GenericMPoint::Close( const ListExpr typeInfo, Word& w )
{
  ((GenericMPoint*) w.addr)->DeleteIfAllowed();
  w.addr = 0;
}


Word GenericMPoint::Clone( const ListExpr typeInfo, const Word& w )
{
  return SetWord(new GenericMPoint(*(GenericMPoint*) w.addr));
}


void* GenericMPoint::Cast( void* addr )
{
  return (new (addr) GenericMPoint);
}


bool GenericMPoint::KindCheck ( ListExpr type, ListExpr& errorInfo )
{
  return checkType(type);
}


int GenericMPoint::SizeOf()
{
  return sizeof(GenericMPoint);
}




ListExpr GenericMPoint::Property()
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
string GenericMPoint::Example()
{
  return "2 3";
}
