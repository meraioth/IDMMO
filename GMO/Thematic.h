#ifndef THEMATIC_H
#define THEMATIC_H

#include <iostream>
#include <string>
#include "../Tools/Flob/DbArray.h"
#include "Attribute.h"
#include "StandardTypes.h"
#include "DateTime.h"
#include "Domain.h"
#include "TemporalAlgebra.h"
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
    static bool KindCheck ( const ListExpr type, ListExpr& errorInfo );
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
    ThematicPath();
/*
The simple constructor. This constructor should not be used.

*/

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

  int GetNoComponents() const;
  void Clear();
  void Add( const ThematicUnit& unit );
  void Append(const ThematicUnit& unit );
  // void MergeAdd(const ThematicUnit& unit);
  std::ostream& Print( std::ostream &os ) const;
  bool operator==( const ThematicPath& r ) const;

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



  static const std::string BasicType(){ return "thematicpath"; }
  static const bool checkType(const ListExpr type){
    return listutils::isSymbol(type, BasicType());
  }

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
    DbArray< ThematicUnit > units;
/*
The database array of temporal units.*/

}; //end class

}//end namespace

// std::ostream& operator<< (std::ostream& os, const gmo::ThematicPath& up);


#endif