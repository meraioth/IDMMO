/*
1 Includes*/
#include "Temp.h"
#include "ListUtils.h"
#include "NestedList.h"
#include "NList.h"
#include "Symbols.h"
#include "StandardTypes.h"
// #include "../Network/NetworkAlgebra.h"
// #include "../Spatial/SpatialAlgebra.h"
#include "NetworkAlgebra.h"
#include "SpatialAlgebra.h"
#include "DateTime.h"
#include <typeinfo>




using namespace gmo;
using namespace std;
using namespace network;
using namespace temporalnet;


/*
1 Implementation of ~class Temp~

1.1 Constructors and Deconstructor

The default constructor should never been used, except in the Cast-Function.

*/

Temp::Temp() :
	Attribute()
{}



Temp::Temp(const Temp& other) :
	Attribute(other.IsDefined())
{
	if(other.IsDefined()){
    if(other.GetDefMPoint()){
      //cout<<"construct mpoint mtpoint case"<<endl;
      mtpoint = new MPoint(other.GetMPoint());
      def_mtpoint = true;
      domain = FreeSpace;
    }
	}else{
		
		
	}
}


Temp::Temp(const MPoint& _mtpoint) :
  Attribute(true) 
{ 
 if(_mtpoint.IsDefined() ){
    domain = FreeSpace;
    
    def_mtpoint = true ;
    mtpoint = new MPoint(_mtpoint);

  }else{
    SetDefined(false);

  }
}


Temp::Temp(const bool defined) :
	Attribute(defined)
{}



Temp::~Temp(){}

/*
1.1 Getter and Setter for private Attributes
*/


bool Temp::GetDefMPoint()const{
  return def_mtpoint;
}

MPoint Temp::GetMPoint()const{
  MPoint other(*mtpoint);
  return other;
}


Domain Temp::GetDomain() const{
	return domain;
}



string Temp::GetStrDomain() const{
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






/*Spatial_Structure Temp::GetSpatialStructure() const{
	return sS;
}*/

/*
1.1 Overwrite Methods from Attribute

*/


void Temp::CopyFrom(const Attribute* right)
{
	cout<<"calling Temp::CopyFrom"<<endl;
  *this = *((Temp*)right);
}


Attribute::StorageType Temp::GetStorageType() const
{
	cout<<"calling Temp::GetStorageType"<<endl;
  return Default;
}


size_t Temp::HashValue() const
{
  return (size_t) mtpoint->Sizeof() + (size_t) def_mtpoint;
}


Temp* Temp::Clone() const
{
	cout<<"calling Temp::Clone"<<endl;
  return new Temp(*this);
}

//TODO : define what means adjacent

bool Temp::Adjacent(const Temp& other) const
{
  // if (IsDefined() && other.IsDefined())
  //   return (sourceid == other.GetTargetId() || targetid == other.GetSourceId());
  // else
    return false;
}


bool Temp::Adjacent(const Attribute* attrib) const
{
  return Adjacent(*((Temp*) attrib));
}


int Temp::Compare(const Attribute* rhs) const
{
	cout<<"calling Temp::Compare 1"<<endl;
  return Compare(*((Temp*)rhs));
}


int Temp::Compare(const void* ls, const void* rs)
{
	cout<<"calling Temp::Compare 2"<<endl;
  Temp lhs( *(Temp*) ls);
  Temp rhs( *(Temp*) rs);
  return lhs.Compare(rhs);
}

/*TODO : check critera for compare two generic point*/

int Temp::Compare(const Temp& in) const
{
	cout<<"calling Temp::Compare 3"<<endl;
  if (!IsDefined() && !in.IsDefined()) return 0;
  if (!IsDefined() && in.IsDefined()) return -1;
  if (IsDefined() && !in.IsDefined()) return 1;
  // if (sourceid < in.GetSourceId()) return -1;
  // if (sourceid > in.GetSourceId()) return 1;
 
 	return 0;
}


size_t Temp::Sizeof() const
{
  return sizeof(Temp);
}



ostream& Temp::Print(ostream& os) const
{
  os << "Temp: ";
  if (IsDefined())
  {
    //os << "domain: " << GetStrDomain();
    //sS.Print(os);
       
  }
  else
    os << Symbol::UNDEFINED() << endl;
  return os;
}


const string Temp::BasicType()
{ 
  return "temp";
}


const bool Temp::checkType(const ListExpr type)
{
	cout<<"calling Temp::checkType"<<endl;
  return listutils::isSymbol(type, BasicType());
}


/*
1.4 Standard Operators

*/


Temp& Temp::operator=(const Temp& other)
{
	cout<<"calling Temp::operator="<<endl;
  SetDefined(other.IsDefined());
  if (other.IsDefined())
  {
    if(other.GetDefMPoint()){
      
      def_mtpoint=true;
      mtpoint = new MPoint( other.GetMPoint());
      domain = FreeSpace;
    }
    // targetid = other.GetTargetId();

  }
  return *this;
}


bool Temp::operator==(const Temp& other) const
{
 cout<<"calling Temp::operator=="<<endl;
  return (Compare(&other) == 0);
}


bool Temp::operator!=(const Temp& other) const
{
  return (Compare(&other) != 0);
}


bool Temp::operator<(const Temp& other) const
{
  return (Compare(&other) < 0);
}


bool Temp::operator<=(const Temp& other) const
{
  return (Compare(&other) < 1);
}


bool Temp::operator>(const Temp& other) const
{
  return (Compare(&other) > 0);
}


bool Temp::operator>=(const Temp& other) const
{
  return (Compare(&other) > -1);
}



/*
1.5 Operators for Secondo Integration
*/

ListExpr Temp::Out(ListExpr typeInfo, Word value)
{ 
  cout<<"calling Temp::oUT"<<endl;
  Temp* actValue = (Temp*) value.addr;
  cout<<"Pointer out"<<value.addr<<endl;
  
  if (!actValue->IsDefined())
    return nl->SymbolAtom(Symbol::UNDEFINED());
  else
  {
    if( actValue->GetDefMPoint()){

      MPoint mgpoint = actValue->GetMPoint();
      return nl->TwoElemList(nl->StringAtom(actValue->GetStrDomain()),
      OutMapping<MPoint, UPoint, OutUPoint>(nl->TheEmptyList(),SetWord( &mgpoint)));

    }else //this case should never occur
    return nl->SymbolAtom(Symbol::UNDEFINED());
    
  }
}


Word Temp::In(const ListExpr typeInfo, const ListExpr instance,
               const int errorPos, ListExpr& errorInfo, bool& correct)
{  


  cout<<"calling Temp::In"<<endl;
  NList in_list(instance);
  if (in_list.length() == 1)
  { 
    NList first = in_list.first();
    if (first.hasStringValue())
    {
      if (first.str() == Symbol::UNDEFINED())
      {
      	cout<<"first str"<<first.str()<<endl;
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
          //cout<<"Domain : "<<domain<<endl;
          
      }
      else
      {
       
        correct = false;
        // cmsg.inFunError("1.Element should be " + CcInt::BasicType() + " >= 0.");
        return SetWord(Address(0));
      }

      	cout<<"Domain "<<domain<<endl;

      if(domain == "FreeSpace"){
        ListExpr mp = nl->Second(instance);
        NList mp_(nl->Second(instance));
        
        //Check if mp contains a string or integer as value
        NList first_elem(nl->First(mp));
        NList space(first_elem.second());
        
          MPoint *value = (MPoint *)InMapping<MPoint, UPoint, InUPoint>( nl->TheEmptyList(),
                                       mp,
                                       errorPos, errorInfo, correct ).addr;
          cout<<"Corrrect infunc" << correct<<endl;
          if( correct  ){
            Temp* mpt = new Temp(*value);
            cout<<"pointer mpoint func in"<<value<<endl;
            delete value;
            cout<<"pointer func in"<<mpt<<endl;
            return SetWord( mpt );
          }
          if(value){
             delete value;
          }
        
      }
    }
  }
  cout<<"In List length"<<in_list.length()<<endl;
    correct = false;
    cmsg.inFunError("List length should be two");
    return SetWord(Address(0));
  
}







Word Temp::Create(const ListExpr typeInfo)
{
  cout<<"calling Temp::Create"<<endl;
  return SetWord(new Temp(true));
}




void Temp::Delete( const ListExpr typeInfo, Word& w )
{
	cout<<"calling Temp::Delete"<<endl;
  ((Temp*) w.addr)->DeleteIfAllowed();
  w.addr = 0;
}


void Temp::Close( const ListExpr typeInfo, Word& w )
{
  cout<<"calling Temp::Close"<<endl;
  ((Temp*) w.addr)->DeleteIfAllowed();
  w.addr = 0;
}


Word Temp::Clone( const ListExpr typeInfo, const Word& w )
{
	cout<<"calling Temp::Clone"<<endl;
  return SetWord(new Temp(*(Temp*) w.addr));
}


void* Temp::Cast( void* addr )
{
	cout<<"calling Temp::Cast"<<endl;
  return (new (addr) Temp);
}


bool Temp::KindCheck ( ListExpr type, ListExpr& errorInfo )
{
	cout<<"calling Temp::KindCheck"<<endl;
	cout<< NList(type).str()<<endl;;
  return checkType(type);
}


int Temp::SizeOf()
{
	cout<<"calling Temp::SizeOf"<<endl;
  return sizeof(Temp);
}




ListExpr Temp::Property()
{
	cout<<"calling Temp::Property"<<endl;
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
string Temp::Example()
{
  return "2 3";
}
