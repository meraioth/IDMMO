#include <iostream>
#include <string>
#include "../Tools/Flob/DbArray.h"
#include "Attribute.h"
#include "StandardTypes.h"
#include "DateTime.h"
#include "Domain.h"
#include "Thematic.h"
// #include "../Spatial/SpatialAlgebra.h"
// #include "../Temporal/TemporalAlgebra.h"
#include "SpatialAlgebra.h"
#include "TemporalAlgebra.h"

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
ThematicPath::~ThematicPath(){}
void ThematicPath::Append(const ThematicUnit& unit )
{

units.Append( unit );
}

void ThematicPath::Clear(){

  units.clean();
  this->del.isDefined = true;
}

void ThematicPath::Destroy()
{
  canDestroy = true;
}

bool ThematicPath::Save( SmiRecord& valueRecord,
             size_t& offset,
             const ListExpr typeInfo,
             Word& value )
{
  ThematicPath *p = (ThematicPath *)value.addr;
  Attribute::Save( valueRecord, offset, typeInfo, p );
  return true;
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



size_t ThematicPath::Sizeof()const{
  assert( IsDefined() );
  return units.Size();
}

// TO DO
int ThematicPath::Compare(const Attribute*) const{
  return 0;
}
// TO DO
bool ThematicPath::Adjacent(const Attribute*) const{
  return true;
}

// TO DO
size_t ThematicPath::HashValue() const{

  return Sizeof();
}

ostream& ThematicPath::Print(ostream& os) const
{
  os << "ThematicPath: ";
  if (IsDefined())
  {
   
 
       
  }
  else
    os << Symbol::UNDEFINED() << endl;
  return os;
}

const string ThematicPath::Example()
{
  return "2 3";
}

bool ThematicPath::IsEmpty()const{
    return (units.Size() == 0);

}

ListExpr ThematicPath::Property()
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


ListExpr ThematicPath::Out( ListExpr typeInfo, Word value )
{
  ThematicPath* m = (ThematicPath*)(value.addr);
  if(! m->IsDefined()){
    return nl->SymbolAtom(Symbol::UNDEFINED());
  } else
  if( m->IsEmpty() )
    return (nl->TheEmptyList());
  else
  {
    //assert( m->IsOrdered() );
    ListExpr l = nl->TheEmptyList();
    ListExpr lastElem = nl->TheEmptyList();
    ListExpr unitList;

    for( int i = 0; i < m->GetNoComponents(); i++ )
    {
      ThematicUnit unit;
      m->Get( i, unit );
      unitList = ThematicUnit::Out( nl->TheEmptyList(), SetWord(&unit) );
      if( l == nl->TheEmptyList() )
      {
        l = nl->Cons( unitList, nl->TheEmptyList() );
        lastElem = l;
      }
      else
        lastElem = nl->Append( lastElem, unitList );
    }
    return l;
  }
}




Word ThematicPath::In( const ListExpr typeInfo, const ListExpr instance,
                const int errorPos, ListExpr& errorInfo, bool& correct )
{
  int numUnits = 0;
  if(nl->AtomType(instance)==NoAtom){
    numUnits = nl->ListLength(instance);
  }
  ThematicPath* m = new ThematicPath( numUnits );
  correct = true;
  int unitcounter = 0;
  std::string errmsg;

  // m->StartBulkLoad();

  ListExpr rest = instance;
  if (nl->AtomType( rest ) != NoAtom)
  { if(listutils::isSymbolUndefined(rest)){
       // m->EndBulkLoad();
       m->SetDefined(false);
       return SetWord( Address( m ) );
    } else {
      correct = false;
      m->Destroy();
      delete m;
      return SetWord( Address( 0 ) );
    }
  }
  else while( !nl->IsEmpty( rest ) )
  {
    ListExpr first = nl->First( rest );
    rest = nl->Rest( rest );

    ThematicUnit *unit = (ThematicUnit*)ThematicUnit::In( nl->TheEmptyList(), first,
                                errorPos, errorInfo, correct ).addr;

    if ( !correct )
    {
      errmsg = "InMapping(): Representation of Unit "
          + int2string(unitcounter) + " is wrong.";
      errorInfo = nl->Append(errorInfo, nl->StringAtom(errmsg));
      m->Destroy();
      delete m;
      return SetWord( Address(0) );
    }
    if( /* correct && (...)*/ !unit->IsDefined() )
    {
      errmsg = "InMapping(): Unit " + int2string(unitcounter) + " is undef.";
      errorInfo = nl->Append(errorInfo, nl->StringAtom(errmsg));
      correct = false;
      delete unit;
      m->Destroy();
      delete m;
      return SetWord( Address(0) );
    }
    m->Add( *unit );
    unitcounter++;
    delete unit;
  }

  return SetWord( m );
}


void ThematicPath::Delete( const ListExpr typeInfo, Word& w )
{
  ((ThematicPath *)w.addr)->Destroy();
  delete (ThematicPath *)w.addr;
  w.addr = 0;
}


void ThematicPath::Close( const ListExpr typeInfo, Word& w )
{
  ((ThematicPath *)w.addr)->DeleteIfAllowed();
  w.addr = 0;
}


Word ThematicPath::Clone( const ListExpr typeInfo, const Word& w )
{
   ThematicPath* m = (ThematicPath*)(w.addr);
   ThematicPath *result;

  if( !m->IsDefined() ){
    result = new ThematicPath( 0 );
    result->SetDefined( false );
    return result;
  }
  result = new ThematicPath( m->GetNoComponents() );
  result->SetDefined( true );

  if(m->GetNoComponents()>0){
     result->units.resize(m->GetNoComponents());
  }

  ThematicUnit unit;
  for( int i = 0; i < m->GetNoComponents(); i++ )
  {
    m->Get( i, unit );
    result->Add( unit );
  }

  return SetWord( result );
}

void* ThematicPath::Cast(void* addr)
{
  return new (addr) ThematicPath;
}


bool ThematicPath::KindCheck( ListExpr type, ListExpr& errorInfo )
{
  return (nl->IsEqual( type, ThematicPath::BasicType() ));
}


int ThematicPath::SizeOf()
{
  return sizeof(ThematicPath);
}

Word ThematicPath::Create( const ListExpr typeInfo )
{
  return (SetWord( new ThematicPath( 0 ) ));
}

int ThematicPath::NumOfFLOBs() const
{
  return 1;
}

/*
2.3.4 GetFLOB


*/
Flob *ThematicPath::GetFLOB(const int i)
{
  assert( i >= 0 && i < NumOfFLOBs() );
  return &units;
}



TPath::TPath( const int n) :
  Attribute(true),
  units( n )
{
  SetDefined(true);
}

/*
2.3.2 Copy Constructor

*/
TPath::TPath(const TPath& src):
  Attribute(src.IsDefined()),
  units(src.units.Size()){
  units.copyFrom(src.units);
}

/*

2.3.2 Destructor.

*/
TPath::~TPath()
{
}

TPath& TPath::operator=(const TPath& src){
  units.copyFrom(src.units);
  return *this;
}



/*
2.3.3 NumOfFLOBs.


*/
int TPath::NumOfFLOBs() const
{
  return 1;
}

/*
2.3.4 GetFLOB


*/
Flob *TPath::GetFLOB(const int i)
{
  assert( i >= 0 && i < NumOfFLOBs() );
  return &units;
}

/*
2.3.5 Compare

Not yet implemented. Needed to be a tuple attribute.

*/
int TPath::Compare(const Attribute*) const
{
  return 0;
}

/*
2.3.6 HashValue

Because Compare returns alway 0, we can only return a constant hash value.

*/
size_t TPath::HashValue() const{
  return  1;
}


/*
2.3.5 Adjacent

Not yet implemented. Needed to be a tuple attribute.

*/
bool TPath::Adjacent(const Attribute*) const
{
  return 0;
}

/*
2.3.7 Clone

Returns a new created TPath (clone) which is a
copy of ~this~.

*/
TPath *TPath::Clone() const
{
  TPath *p = new TPath( *this );
  return p;
}

void TPath::CopyFrom(const Attribute* right){
  *this = *( (TPath*) right);
}

/*
2.3.8 Sizeof

*/
size_t TPath::Sizeof() const
{
  return sizeof( *this );
}

std::ostream& operator<<(std::ostream& os, const TPath& p)
{
  
  for(int i = 0; i < p.GetNoUnits(); i++)
    p.GetUnit( i ).Print(os); 
    os << " ";

  os << ">";

  return os;
}

/*
2.3.8 Print

*/
std::ostream& TPath::Print( std::ostream& os ) const
{
  return (os );
}

/*
2.3.9 Append

Appends a ThematicUnit ~v~ at the end of the TPath.

*Precondition* ~state == partial~.

*/
void TPath::Append( const ThematicUnit& v )
{
  units.Append( v );
}


/*
2.3.13 Destroy

Turns the TPath into the ~closed~ state destroying the
units array.

*Precondition* ~state == complete~.

*/
void TPath::Destroy()
{
  
  //units.Destroy();
}



/*
2.3.15 GetThematicUnit

Returns a ThematicUnit indexed by ~i~.

*Precondition* ~state == complete \&\& 0 <= i < nounits~.

*/
ThematicUnit TPath::GetUnit( int i ) const
{
  assert( 0 <= i && i < units.Size() );

  ThematicUnit v;
  units.Get( i, &v );
  return v;
}

/*
2.3.14 NoEdges

Returns the number of edges of the TPath.

*Precondition* ~state == complete~.

*/
int TPath::GetNoUnits() const
{
  return units.Size();
}

/*
2.3.18 IsEmpty

Returns if the TPath is empty or not.

*/
const bool TPath::IsEmpty() const
{
  return units.Size() == 0;
}

/*
3 TPath Algebra.

3.1 List Representation

The list representation of a TPath is

----    ( (<recordId>) (x1 y1) (x2 y2) ... (xn yn) )
----

3.2 ~In~ and ~Out~ Functions

*/

ListExpr
TPath::Out( ListExpr typeInfo, Word value )
{
  TPath* tpath = (TPath*)(value.addr);

  if( !tpath->IsDefined() ){
    return nl->SymbolAtom(Symbol::UNDEFINED());
  }
  if( tpath->IsEmpty() )
  {
    return (nl->TheEmptyList());
  }
  else
  {
    ListExpr l = nl->TheEmptyList();
    ListExpr lastElem = nl->TheEmptyList();
    ListExpr unitList;

    for( int i = 0; i < tpath->GetNoUnits(); i++ )
    {
      ThematicUnit unit;
      unit = tpath->GetUnit( i);
      unitList = ThematicUnit::Out( nl->TheEmptyList(), SetWord(&unit) );
      if( l == nl->TheEmptyList() )
      {
        l = nl->Cons( unitList, nl->TheEmptyList() );
        lastElem = l;
      }
      else
        lastElem = nl->Append( lastElem, unitList );
    }
    return l;
  }
}

Word
TPath::In( const ListExpr typeInfo, const ListExpr instance,
           const int errorPos, ListExpr& errorInfo, bool& correct )
{
  int numUnits = 0;
  if(nl->AtomType(instance)==NoAtom){
    numUnits = nl->ListLength(instance);
  }
  ThematicPath* m = new ThematicPath( numUnits );
  m->SetDefined(true);
  correct = true;
  int unitcounter = 0;
  std::string errmsg;

  // m->StartBulkLoad();

  ListExpr rest = instance;
  if (nl->AtomType( rest ) != NoAtom)
  { if(listutils::isSymbolUndefined(rest)){
       // m->EndBulkLoad();
       m->SetDefined(false);
       return SetWord( Address( m ) );
    } else {
      correct = false;
      m->Destroy();
      delete m;
      return SetWord( Address( 0 ) );
    }
  }
  else while( !nl->IsEmpty( rest ) )
  {
    ListExpr first = nl->First( rest );
    rest = nl->Rest( rest );

    ThematicUnit *unit = (ThematicUnit*)ThematicUnit::In( nl->TheEmptyList(), first,
                                errorPos, errorInfo, correct ).addr;

    if ( !correct )
    {
      errmsg = "InMapping(): Representation of Unit "
          + int2string(unitcounter) + " is wrong.";
      errorInfo = nl->Append(errorInfo, nl->StringAtom(errmsg));
      m->Destroy();
      delete m;
      return SetWord( Address(0) );
    }
    if( /* correct && (...)*/ !unit->IsDefined() )
    {
      errmsg = "InMapping(): Unit " + int2string(unitcounter) + " is undef.";
      errorInfo = nl->Append(errorInfo, nl->StringAtom(errmsg));
      correct = false;
      delete unit;
      m->Destroy();
      delete m;
      return SetWord( Address(0) );
    }
    m->Add( *unit );
    unitcounter++;
    delete unit;
  }

  return SetWord( m );
}

/*
3.3 Function Describing the Signature of the Type Constructor

*/

ListExpr
TPath::Property()
{
  return (nl->TwoElemList(
         nl->FiveElemList(nl->StringAtom("Signature"),
                          nl->StringAtom("Example Type List"),
                          nl->StringAtom("List Rep"),
                          nl->StringAtom("Example List"),
                          nl->StringAtom("Remarks")),
         nl->FiveElemList(nl->StringAtom("->" + Kind::DATA() ),
                          nl->StringAtom(TPath::BasicType()),
                          nl->StringAtom("(<point>*) where <point> is "
                          "(<x> <y>)"),
                          nl->StringAtom("( (3 4) (10 10) (8 2) (6 4) "
                          "(3 4) )"),
                          nl->StringAtom("x- and y-coordinates must be of "
                          "type int."))));
}

/*
3.4 Kind Checking Function

This function checks whether the type constructor is applied correctly. Since
type constructor ~TPath~ does not have arguments, this is trivial.

*/
bool
TPath::KindCheck( ListExpr type, ListExpr& errorInfo )
{
  return (nl->IsEqual( type, TPath::BasicType() ));
}

/*

3.5 ~Create~-function

*/
Word TPath::Create(const ListExpr typeInfo)
{
  TPath* tpath = new TPath( 0 );
  return ( SetWord(tpath) );
}

/*
3.6 ~Delete~-function

*/
void TPath::Delete(const ListExpr typeInfo, Word& w)
{
  TPath* tpath = (TPath*)w.addr;

  tpath->Destroy();
  delete tpath;
  w.addr= 0;
}

/*
3.6 ~Open~-function

*/
bool
TPath::Open( SmiRecord& valueRecord,
             size_t& offset,
             const ListExpr typeInfo,
             Word& value )
{
  TPath *p = (TPath*)Attribute::Open( valueRecord, offset, typeInfo );
  value.setAddr( p );
  return true;
}

/*
3.7 ~Save~-function

*/
bool
TPath::Save( SmiRecord& valueRecord,
             size_t& offset,
             const ListExpr typeInfo,
             Word& value )
{
  TPath *p = (TPath *)value.addr;
  Attribute::Save( valueRecord, offset, typeInfo, p );
  return true;
}

/*
3.8 ~Close~-function

*/
void TPath::Close(const ListExpr typeInfo, Word& w)
{
  TPath* tpath = (TPath*)w.addr;
  delete tpath;
  w.addr=0;
}

/*
3.9 ~Clone~-function

*/
Word TPath::Clone(const ListExpr typeInfo, const Word& w)
{
  return SetWord( ((TPath*)w.addr)->Clone() );
}

/*
3.9 ~SizeOf~-function

*/
int TPath::SizeOfObj()
{
  return sizeof(TPath);
}

/*
3.10 ~Cast~-function

*/
void* TPath::Cast(void* addr)
{
  return (new (addr) TPath);
}


/*
2.3.18 Print functions

*/
std::ostream& operator<<(std::ostream& os, const Unit& v)
{
  os << "(" << v.unit << ")";
  return os;
}


std::ostream& operator<<(std::ostream& os, const TemPath& p)
{
  

  for(int i = 0; i < p.GetNoUnits(); i++)
    os << p.GetUnit( i ) << " ";

  os << ">";

  return os;
}

/*
2.3.1 Constructors.

This first constructor creates a new polygon.

*/
TemPath::TemPath( const int n, const char unit_[10]) :
  Attribute(true),
  units( n )
{
  SetDefined(true);
  
}

/*
2.3.2 Copy Constructor

*/
TemPath::TemPath(const TemPath& src):
  Attribute(src.IsDefined()),
  units(src.units.Size()){
  units.copyFrom(src.units);
}

/*

2.3.2 Destructor.

*/
TemPath::~TemPath()
{
}

TemPath& TemPath::operator=(const TemPath& src){
  units.copyFrom(src.units);
  return *this;
}



/*
2.3.3 NumOfFLOBs.


*/
int TemPath::NumOfFLOBs() const
{
  return 1;
}

/*
2.3.4 GetFLOB


*/
Flob *TemPath::GetFLOB(const int i)
{
  assert( i >= 0 && i < NumOfFLOBs() );
  return &units;
}

/*
2.3.5 Compare

Not yet implemented. Needed to be a tuple attribute.

*/
int TemPath::Compare(const Attribute*) const
{
  return 0;
}

/*
2.3.6 HashValue

Because Compare returns alway 0, we can only return a constant hash value.

*/
size_t TemPath::HashValue() const{
  return  1;
}


/*
2.3.5 Adjacent

Not yet implemented. Needed to be a tuple attribute.

*/
bool TemPath::Adjacent(const Attribute*) const
{
  return 0;
}

/*
2.3.7 Clone

Returns a new created polygon (clone) which is a
copy of ~this~.

*/
TemPath *TemPath::Clone() const
{
  TemPath *p = new TemPath( *this );
  return p;
}

void TemPath::CopyFrom(const Attribute* right){
  *this = *( (TemPath*) right);
}

/*
2.3.8 Sizeof

*/
size_t TemPath::Sizeof() const
{
  return sizeof( *this );
}

/*
2.3.8 Print

*/
std::ostream& TemPath::Print( std::ostream& os ) const
{
  return (os);// << *this);
}

/*
2.3.9 Append

Appends a vertex ~v~ at the end of the polygon.

*Precondition* ~state == partial~.

*/
void TemPath::Append( const Unit& v )
{
  units.Append( v );
}

/*
2.3.13 Destroy

Turns the polygon into the ~closed~ state destroying the
vertices array.

*Precondition* ~state == complete~.

*/
void TemPath::Destroy()
{
  units.destroy();
}

/*
2.3.14 NoEdges

Returns the number of edges of the polygon.

*Precondition* ~state == complete~.

*/
int TemPath::GetNoUnits() const
{
  return units.Size();
}

/*
2.3.15 GetUnit

Returns a vertex indexed by ~i~.

*Precondition* ~state == complete \&\& 0 <= i < noVertices~.

*/
Unit TemPath::GetUnit( int i ) const
{
  assert( 0 <= i && i < GetNoUnits() );

  Unit v;
  units.Get( i, &v );
  return v;
}


/*
2.3.18 IsEmpty

Returns if the polygon is empty or not.

*/
const bool TemPath::IsEmpty() const
{
  return GetNoUnits() == 0;
}

/*
3 TemPath Algebra.

3.1 List Representation

The list representation of a polygon is

----    ( (<recordId>) (x1 y1) (x2 y2) ... (xn yn) )
----

3.2 ~In~ and ~Out~ Functions

*/

ListExpr
TemPath::Out( ListExpr typeInfo, Word value )
{
  TemPath* polygon = (TemPath*)(value.addr);

  if( !polygon->IsDefined() ){
    return nl->SymbolAtom(Symbol::UNDEFINED());
  }
  if( polygon->IsEmpty() )
  {
    return (nl->TheEmptyList());
  }
  else
  {
    ListExpr result =
      nl->OneElemList(
        nl->OneElemList(
          nl->StringAtom( polygon->GetUnit(0).unit ) ) );
    ListExpr last = result;

    for( int i = 1; i < polygon->GetNoUnits(); i++ )
    {
      last = nl->Append( last,
                         nl->OneElemList(
                           nl->StringAtom( polygon->GetUnit(i).unit )) );
    }
    return result;
  }
}

Word
TemPath::In( const ListExpr typeInfo, const ListExpr instance,
           const int errorPos, ListExpr& errorInfo, bool& correct )
{
  TemPath* polygon = new TemPath( 0 );
  polygon->SetDefined(true);

  if(listutils::isSymbolUndefined( instance )){
    polygon->SetDefined(false);
    correct = true;
    return SetWord( polygon );
  }
  ListExpr first = nl->Empty();
  ListExpr rest = instance;
  while( !nl->IsEmpty( rest ) )
  {
    first = nl->First( rest );
    rest = nl->Rest( rest );

    if( nl->ListLength( first ) == 1 &&
        nl->IsAtom( nl->First( first ) ) &&
        nl->AtomType( nl->First( first ) ) == StringType )
    {
      Unit v(  const_cast<char*>( nl->StringValue( nl->First( first ) ).c_str())   );
      polygon->Append( v );
    }
    else
    {
      correct = false;
      delete polygon;
      return SetWord( Address(0) );
    }
  }
  correct = true;
  return SetWord( polygon );
}

/*
3.3 Function Describing the Signature of the Type Constructor

*/

ListExpr
TemPath::Property()
{
  return (nl->TwoElemList(
         nl->FiveElemList(nl->StringAtom("Signature"),
                          nl->StringAtom("Example Type List"),
                          nl->StringAtom("List Rep"),
                          nl->StringAtom("Example List"),
                          nl->StringAtom("Remarks")),
         nl->FiveElemList(nl->StringAtom("->" + Kind::DATA() ),
                          nl->StringAtom(TemPath::BasicType()),
                          nl->StringAtom("(<point>*) where <point> is "
                          "(<x> <y>)"),
                          nl->StringAtom("( (3 4) (10 10) (8 2) (6 4) "
                          "(3 4) )"),
                          nl->StringAtom("x- and y-coordinates must be of "
                          "type int."))));
}

/*
3.4 Kind Checking Function

This function checks whether the type constructor is applied correctly. Since
type constructor ~polygon~ does not have arguments, this is trivial.

*/
bool
TemPath::KindCheck( ListExpr type, ListExpr& errorInfo )
{
  return (nl->IsEqual( type, TemPath::BasicType() ));
}

/*

3.5 ~Create~-function

*/
Word TemPath::Create(const ListExpr typeInfo)
{
  TemPath* polygon = new TemPath( 0 );
  return ( SetWord(polygon) );
}

/*
3.6 ~Delete~-function

*/
void TemPath::Delete(const ListExpr typeInfo, Word& w)
{
  TemPath* polygon = (TemPath*)w.addr;

  polygon->Destroy();
  delete polygon;
}

/*
3.6 ~Open~-function

*/
bool
TemPath::Open( SmiRecord& valueRecord,
             size_t& offset,
             const ListExpr typeInfo,
             Word& value )
{
  TemPath *p = (TemPath*)Attribute::Open( valueRecord, offset, typeInfo );
  value.setAddr( p );
  return true;
}

/*
3.7 ~Save~-function

*/
bool
TemPath::Save( SmiRecord& valueRecord,
             size_t& offset,
             const ListExpr typeInfo,
             Word& value )
{
  TemPath *p = (TemPath *)value.addr;
  Attribute::Save( valueRecord, offset, typeInfo, p );
  return true;
}

/*
3.8 ~Close~-function

*/
void TemPath::Close(const ListExpr typeInfo, Word& w)
{
  TemPath* polygon = (TemPath*)w.addr;
  delete polygon;
}

/*
3.9 ~Clone~-function

*/
Word TemPath::Clone(const ListExpr typeInfo, const Word& w)
{
  return SetWord( ((TemPath*)w.addr)->Clone() );
}

/*
3.9 ~SizeOf~-function

*/
int TemPath::SizeOfObj()
{
  return sizeof(TemPath);
}

/*
3.10 ~Cast~-function

*/
void* TemPath::Cast(void* addr)
{
  return (new (addr) TemPath);
}