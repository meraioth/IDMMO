#ifndef THEMATIC_H
#define THEMATIC_H

#include <iostream>
#include <string>
#include "../Tools/Flob/DbArray.h"
#include "Attribute.h"
#include "StandardTypes.h"
#include "DateTime.h"
#include "Domain.h"
#include "../Temporal/TemporalAlgebra.h"
#include "../Network/NetworkAlgebra.h"
#include "../Spatial/SpatialAlgebra.h"

using namespace network;
using namespace std;
using namespace gmo;

namespace gmo{


  class ThematicUnit : public Attribute{

  public:

    ThematicUnit();
    ThematicUnit( STRING_T unit);
    ThematicUnit(const string unit);
    explicit ThematicUnit(const bool def);
    ThematicUnit( const ThematicUnit& other);
    ~ThematicUnit();

    ThematicUnit& operator=( const ThematicUnit& other ) const;
  
    std::string GetUnit() const;

    
    void CopyFrom(const Attribute* right);
    StorageType GetStorageType() const;
    size_t HashValue() const;
    ThematicUnit* Clone() const;
    bool Adjacent(const Attribute* attrib) const;
    bool Adjacent(const ThematicUnit& attrib) const;
    static int Compare(const void* ls, const void* rs);
    int Compare(const Attribute* rhs) const;
    int Compare(const ThematicUnit& rhs) const;
    size_t Sizeof() const;
    std::ostream& Print(std::ostream& os) const;
    static const std::string BasicType();
    static const bool checkType(const ListExpr type);
    static const std::string Example();


    /*
    1.1.1 Standard Operators

    */

    ThematicUnit& operator=(const ThematicUnit& other);

    bool operator==(const ThematicUnit& other) const;
    bool operator!=(const ThematicUnit& other) const;
    bool operator<(const ThematicUnit& other) const;
    bool operator<=(const ThematicUnit& other) const;
    bool operator>(const ThematicUnit& other) const;
    bool operator>=(const ThematicUnit& other) const;

    /*
    1.1.1 Operators for Secondo Integration

    */

    static ListExpr Out(ListExpr typeInfo, Word value);
    static Word In(const ListExpr typeInfo, const ListExpr instance,
                 const int errorPos, ListExpr& errorInfo, bool& correct);
    static Word Create(const ListExpr typeInfo);
    static void Delete( const ListExpr typeInfo, Word& w );
    static void Close( const ListExpr typeInfo, Word& w );
    static Word Clone( const ListExpr typeInfo, const Word& w );
    static void* Cast( void* addr );
    static bool KindCheck ( const ListExpr type, ListExpr& derrorInfo );
    static int SizeOf();
    static ListExpr Property();


  private:
    STRING_T unit;

  };

	class ThematicPath:public Attribute {
  public:

/*
3.12.1 Constructors and Destructor

*/
    //ThematicPath();
/*
The simple constructor. This constructor should not be used.

*/  ~ThematicPath();

    ThematicPath( const int n );
/*
The constructor. Initializes space for ~n~ elements.

*/

/*
3.12.2 Modifications of Inherited Functions

Overwrites the function defined in Mapping, mostly in order to
maintain the object's bounding box. Also, some methods can be improved
using a check on bbox.

*/
  size_t Sizeof() const;
  int Compare(const Attribute*) const;
  bool Adjacent(const Attribute*) const;
  size_t HashValue() const;
  int NumOfFLOBs() const;
  Flob *GetFLOB(const int i);
  int GetNoComponents() const;
  void Clear();
  void Add( const ThematicUnit& unit );
  void Append(const ThematicUnit& unit );
  // void MergeAdd(const ThematicUnit& unit);
  std::ostream& Print( std::ostream &os ) const;
  bool operator==( const ThematicPath& r ) const;
  bool Save( SmiRecord& valueRecord,
             size_t& offset,
             const ListExpr typeInfo,
             Word& value );
  void Get( const int i, ThematicUnit &unit ) const;


  virtual Attribute* Clone() const
  {
		// assert( IsOrdered() );
		ThematicPath *result;
		if( !this->IsDefined() ){
		  result = new ThematicPath( 0 );
		} else {
		  result = new ThematicPath( GetNoComponents() );
		  if(GetNoComponents()>0){
		    result->units.resize(GetNoComponents());
		  }
		  // result->StartBulkLoad();
		  ThematicUnit unit;
		  for( int i = 0; i < GetNoComponents(); i++ ){
		    Get( i, unit );
		    result->Add( unit );
		  }
		  // result->EndBulkLoad( false );
		}
		result->SetDefined(this->IsDefined());
		return (Attribute*) result;
  }

  void CopyFrom( const Attribute* right )
  {
		const ThematicPath *r = (const ThematicPath*)right;
		// assert( r->IsOrdered() );
		Clear();
		this->SetDefined(r->IsDefined());
		if( !this->IsDefined() ) {
		  return;
		}
		// StartBulkLoad();
		ThematicUnit unit;
		for( int i = 0; i < r->GetNoComponents(); i++ ){
		  r->Get( i, unit );
		  Add( unit );
		}
		// EndBulkLoad( false );
  }



  static const std::string BasicType(){ return "thempath"; }
  static const bool checkType(const ListExpr type){
    return listutils::isSymbol(type, BasicType());
  }
  void Destroy();
  bool IsEmpty() const;
  static ListExpr Out(ListExpr typeInfo, Word value);
  static Word In(const ListExpr typeInfo, const ListExpr instance,
               const int errorPos, ListExpr& errorInfo, bool& correct);
  static Word Create(const ListExpr typeInfo);
  static void Delete( const ListExpr typeInfo, Word& w );
  static void Close( const ListExpr typeInfo, Word& w );
  static Word Clone( const ListExpr typeInfo, const Word& w );
  static void* Cast( void* addr );
  static bool KindCheck ( const ListExpr type, ListExpr& errorInfo );
  static int SizeOf();
  static ListExpr Property();
  static const string Example();
  private:

    bool canDestroy;
/*
A flag indicating if the destructor should destroy also the persistent
array of intervals.

*/

    bool ordered;
/*
A flag indicating whether the unit set is ordered or not.

*/

  protected:
    ThematicPath();
    DbArray< ThematicUnit > units;
/*
The database array of temporal units.*/

}; //end class

class TPath : public Attribute
{

  public:
    TPath( const int n);
    ~TPath();

    TPath(const TPath& src);
    TPath& operator=(const TPath& src);

    int NumOfFLOBs() const;
    int GetNoUnits() const;
    Flob *GetFLOB(const int i);
    int Compare(const Attribute*) const;
    bool Adjacent(const Attribute*) const;
    TPath *Clone() const;
    size_t Sizeof() const;
    std::ostream& Print( std::ostream& os ) const;

    void Append( const ThematicUnit &v );
    void Destroy();
    ThematicUnit GetUnit( int i ) const;
    const bool IsEmpty() const;
    void CopyFrom(const Attribute* right);
    size_t HashValue() const;

    friend std::ostream& operator <<( std::ostream& os, const TPath& p );

    static Word     In( const ListExpr typeInfo, const ListExpr instance,
                        const int errorPos, ListExpr& errorInfo,
                        bool& correct );

    static ListExpr Out( ListExpr typeInfo, Word value );

    static Word     Create( const ListExpr typeInfo );

    static void     Delete( const ListExpr typeInfo, Word& w );

    static void     Close( const ListExpr typeInfo, Word& w );

    static bool     Save( SmiRecord& valueRecord, size_t& offset,
                          const ListExpr typeInfo, Word& value    );

    static bool     Open( SmiRecord& valueRecord, size_t& offset,
                          const ListExpr typeInfo, Word& value    );

    static Word     Clone( const ListExpr typeInfo, const Word& w );


    static bool     KindCheck( ListExpr type, ListExpr& errorInfo );

    static int      SizeOfObj();

    static ListExpr Property();

    static void* Cast(void* addr);

    static const std::string BasicType() { return "thpath"; }
    static const bool checkType(const ListExpr type){
      return listutils::isSymbol(type, BasicType());
    }

  private:
    TPath() {} // this constructor is reserved for the cast function.
    DbArray<ThematicUnit> units;
    
};


struct Unit
{
  Unit() {}
/*
Do not use this constructor.

*/

  Unit( char _unit[20])
    {
      sprintf(unit, "%s", _unit);
    }

  Unit(const Unit& v){
    sprintf(unit, "%s", v.unit);
  }

  Unit& operator=(const Unit& v){
    sprintf(unit, "%s", v.unit); 
    return *this;
  }

  ~Unit(){}

  char unit[20];
};


/*

2.3 Class TemPath

*/
class TemPath : public Attribute
{

  public:
    TemPath( const int n, const char unit_[20] = 0);
    ~TemPath();

    TemPath(const TemPath& src);
    TemPath& operator=(const TemPath& src);

    int NumOfFLOBs() const;
    Flob *GetFLOB(const int i);
    int Compare(const Attribute*) const;
    bool Adjacent(const Attribute*) const;
    TemPath *Clone() const;
    size_t Sizeof() const;
    std::ostream& Print( std::ostream& os ) const;

    void Append( const Unit &v );
    void Complete();
    bool Correct();
    void Destroy();
    int GetNoUnits() const;
    Unit GetUnit( int i ) const;
    const bool IsEmpty() const;
    void CopyFrom(const Attribute* right);
    size_t HashValue() const;

    friend std::ostream& operator <<( std::ostream& os, const TemPath& p );

    static Word     In( const ListExpr typeInfo, const ListExpr instance,
                        const int errorPos, ListExpr& errorInfo,
                        bool& correct );

    static ListExpr Out( ListExpr typeInfo, Word value );

    static Word     Create( const ListExpr typeInfo );

    static void     Delete( const ListExpr typeInfo, Word& w );

    static void     Close( const ListExpr typeInfo, Word& w );

    static bool     Save( SmiRecord& valueRecord, size_t& offset,
                          const ListExpr typeInfo, Word& value    );

    static bool     Open( SmiRecord& valueRecord, size_t& offset,
                          const ListExpr typeInfo, Word& value    );

    static Word     Clone( const ListExpr typeInfo, const Word& w );


    static bool     KindCheck( ListExpr type, ListExpr& errorInfo );

    static int      SizeOfObj();

    static ListExpr Property();

    static void* Cast(void* addr);

    static const std::string BasicType() { return "thematicpath"; }
    static const bool checkType(const ListExpr type){
      return listutils::isSymbol(type, BasicType());
    }

  private:
    TemPath() {} // this constructor is reserved for the cast function.
    DbArray<Unit> units;
};

}//end namespace

// std::ostream& operator<< (std::ostream& os, const gmo::ThematicPath& up);


#endif