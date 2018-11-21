#include <iostream>
#include <string>
#include "../Tools/Flob/DbArray.h"
#include "Attribute.h"
#include "StandardTypes.h"
#include "DateTime.h"
#include "Domain.h"
#include "TemporalAlgebra.h"
#include "Thematic.h"
#include "../Spatial/SpatialAlgebra.h"

using namespace datetime;
using namespace temporalalgebra;
using namespace gmo;




// Begin of Unit implementation

ThematicUnit::ThematicUnit():Attribute(){} //Never used;

ThematicUnit::ThematicUnit(STRING_T _unit):Attribute(true){
  sprintf(unit, "%s", _unit);

}

ThematicUnit::ThematicUnit(string _unit):Attribute(true){
  sprintf(unit, "%s", _unit.c_str());

}

ThematicUnit::~ThematicUnit(){};

ThematicUnit::ThematicUnit(const bool def):Attribute(def){}

ThematicUnit::ThematicUnit(const ThematicUnit& other):Attribute(other.IsDefined()){
	
	if(other.IsDefined()){
		string other_s = other.GetUnit();
		sprintf(unit, "%s", other_s.c_str());
	}
}



string ThematicUnit::GetUnit()const{
  string out(unit);
	return out;
}


void ThematicUnit::CopyFrom(const Attribute* right)
{
  *this = *((ThematicUnit*)right);
}


Attribute::StorageType ThematicUnit::GetStorageType() const
{
  return Default;
}


size_t ThematicUnit::HashValue() const
{
  return (size_t) sizeof(STRING_T);
}


ThematicUnit* ThematicUnit::Clone() const
{
  return new ThematicUnit(*this);
}

//TODO : define what means adjacent

bool ThematicUnit::Adjacent(const ThematicUnit& other) const
{
  // if (IsDefined() && other.IsDefined())
  //   return (sourceid == other.GetTargetId() || targetid == other.GetSourceId());
  // else
    return false;
}


bool ThematicUnit::Adjacent(const Attribute* attrib) const
{
  return Adjacent(*((ThematicUnit*) attrib));
}


int ThematicUnit::Compare(const Attribute* rhs) const
{
  return Compare(*((ThematicUnit*)rhs));
}


int ThematicUnit::Compare(const void* ls, const void* rs)
{
  ThematicUnit lhs( *(ThematicUnit*) ls);
  ThematicUnit rhs( *(ThematicUnit*) rs);
  return lhs.Compare(rhs);
}

/*TODO : check critera for compare two generic point*/

int ThematicUnit::Compare(const ThematicUnit& in) const
{
  if (!IsDefined() && !in.IsDefined()) return 0;
  if (!IsDefined() && in.IsDefined()) return -1;
  if (IsDefined() && !in.IsDefined()) return -1;
  // if (stop_id == in.GetStop()) return 1;

 
 	return 0;
}


size_t ThematicUnit::Sizeof() const
{
  return sizeof(ThematicUnit);
}



ostream& ThematicUnit::Print(ostream& os) const
{
  os << "ThematicUnit: ";
  if (IsDefined())
  {
    os << "unit : " << GetUnit();
 
       
  }
  else
    os << Symbol::UNDEFINED() << endl;
  return os;
}

const string ThematicUnit::BasicType(){ 
	return "thematicunit"; 
}

const bool ThematicUnit::checkType(const ListExpr type){
	return listutils::isSymbol(type, BasicType());
}



/*
1.4 Standard Operators

*/



ThematicUnit& ThematicUnit::operator=( const ThematicUnit& other )
{
	SetDefined(other.IsDefined());
	if( IsDefined())
	{

    sprintf(unit, "%s", other.GetUnit().c_str());
	}
	return *this;
}


bool ThematicUnit::operator==(const ThematicUnit& other) const
{
  return (Compare(&other) == 1);
}


bool ThematicUnit::operator!=(const ThematicUnit& other) const
{
  return (Compare(&other) != 0);
}


bool ThematicUnit::operator<(const ThematicUnit& other) const
{
  return (Compare(&other) < 0);
}


bool ThematicUnit::operator<=(const ThematicUnit& other) const
{
  return (Compare(&other) < 1);
}


bool ThematicUnit::operator>(const ThematicUnit& other) const
{
  return (Compare(&other) > 0);
}


bool ThematicUnit::operator>=(const ThematicUnit& other) const
{
  return (Compare(&other) > -1);
}



/*
1.5 Operators for Secondo Integration
*/

ListExpr ThematicUnit::Out(ListExpr typeInfo, Word value)
{ 
  ThematicUnit* actValue = (ThematicUnit*) value.addr;
  
  if (!actValue->IsDefined())
    return nl->SymbolAtom(Symbol::UNDEFINED());
  else
  {
      return nl->OneElemList(nl->StringAtom(actValue->GetUnit()));
    
  }
}


Word ThematicUnit::In(const ListExpr typeInfo, const ListExpr instance,
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
  		
      ThematicUnit* tpoint = new ThematicUnit(in_list.first().str());
 	  correct=true;
      return SetWord(tpoint);
     }else{
     	correct = false;
    	cmsg.inFunError("List should have a string value");
    	return SetWord(Address(0));
     }
        
  }

  
}



Word ThematicUnit::Create(const ListExpr typeInfo)
{
  
  return SetWord(new ThematicUnit(true));
}




void ThematicUnit::Delete( const ListExpr typeInfo, Word& w )
{
  ((ThematicUnit*) w.addr)->DeleteIfAllowed();
  w.addr = 0;
}


void ThematicUnit::Close( const ListExpr typeInfo, Word& w )
{
  ((ThematicUnit*) w.addr)->DeleteIfAllowed();
  w.addr = 0;
}


Word ThematicUnit::Clone( const ListExpr typeInfo, const Word& w )
{
  return SetWord(new ThematicUnit(*(ThematicUnit*) w.addr));
}


void* ThematicUnit::Cast( void* addr )
{
  return (new (addr) ThematicUnit);
}


bool ThematicUnit::KindCheck ( ListExpr type, ListExpr& errorInfo )
{
  return checkType(type);
}


int ThematicUnit::SizeOf()
{
  return sizeof(ThematicUnit);
}




ListExpr ThematicUnit::Property()
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


const string ThematicUnit::Example()
{
  return "F14";
}



// End of Unit implementation

// Start of Path implementation


ThematicPath::ThematicPath():Attribute(false),units(0){} //Never used
ThematicPath::ThematicPath(const int n ):Attribute(true),units(n){}

void ThematicPath::Append(const ThematicUnit& unit )
{

units.Append( unit );
}

void ThematicPath::Clear(){

  units.clean();
  this->del.isDefined = true;
}

void ThematicPath::Add(const ThematicUnit& unit){

  assert( unit.IsDefined() );
  // assert( unit.IsValid() );
  if(!IsDefined() || !unit.IsDefined()){
    SetDefined( false );
    return;
  }
  units.Append( unit );
}

int ThematicPath::GetNoComponents()const{
	assert( IsDefined() );
  return units.Size();
}

void ThematicPath::Get( const int i, ThematicUnit &unit ) const
{
  assert( IsDefined() );
  assert(i>=0);
  assert(i<units.Size());
  bool ok = units.Get( i, unit );
  if(!ok){
    cout << "Problem in getting data from " << units << std::endl;
    assert(ok);
  }
  // if ( !unit.IsValid() )
  // {
  //   cout << __FILE__ << "," << __LINE__ << ":" << __PRETTY_FUNCTION__
  //     << " Get(" << i << ", Unit): Unit is invalid:";
  //   unit.Print(cout); cout << std::endl;
  //   assert( unit.IsValid());
  // }
}





