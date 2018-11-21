#ifndef MTPOINT_H
#define MTPOINT_H

#include <iostream>
#include <string>
#include "../Tools/Flob/DbArray.h"
#include "Attribute.h"
#include "StandardTypes.h"
#include "DateTime.h"
#include "Domain.h"
#include "TemporalAlgebra.h"
#include "UTPoint.h"
#include "TPoint.h"
#include "../Network/NetworkAlgebra.h"
#include "../Spatial/SpatialAlgebra.h"

using namespace network;
using namespace temporalalgebra;
using namespace gmo;

namespace gmo{

	class MTPoint : public Mapping< UTPoint, TPoint >{
  public:

/*
3.12.1 Constructors and Destructor

*/
    MTPoint() {};
/*
The simple constructor. This constructor should not be used.

*/

    MTPoint( const int n ):
      Mapping< UTPoint, TPoint >( n )
      {
        del.refs=1;
        del.SetDelete();
        del.isDefined = true;
        // bbox = Rectangle<3>(false, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
      };
/*
The constructor. Initializes space for ~n~ elements.

*/

/*
3.12.2 Modifications of Inherited Functions

Overwrites the function defined in Mapping, mostly in order to
maintain the object's bounding box. Also, some methods can be improved
using a check on bbox.

*/

  void Clear();
  void Add( const UTPoint& unit );
  void MergeAdd(const UTPoint& unit);
  bool EndBulkLoad( const bool sort = true, const bool checkvalid = false );
  void Restrict( const std::vector< std::pair<int, int> >& intervals );
  std::ostream& Print( std::ostream &os ) const;
  bool operator==( const MTPoint& r ) const;
  bool Present( const Instant& t ) const;
  bool Present( const Periods& t ) const;
  // void AtInstant( const Instant& t, Intime<TPoint>& result ) const;
  // void AtPeriods( const Periods& p, MTPoint& result ) const;
  // void AtRect(const Rectangle<2>& rect, MTPoint& result) const;


  virtual Attribute* Clone() const
  {
		assert( IsOrdered() );
		MTPoint *result;
		if( !this->IsDefined() ){
		  result = new MTPoint( 0 );
		} else {
		  result = new MTPoint( GetNoComponents() );
		  if(GetNoComponents()>0){
		    result->units.resize(GetNoComponents());
		  }
		  result->StartBulkLoad();
		  UTPoint unit;
		  for( int i = 0; i < GetNoComponents(); i++ ){
		    Get( i, unit );
		    result->Add( unit );
		  }
		  result->EndBulkLoad( false );
		}
		result->SetDefined(this->IsDefined());
		return (Attribute*) result;
  }

  void CopyFrom( const Attribute* right )
  {
		const MTPoint *r = (const MTPoint*)right;
		assert( r->IsOrdered() );
		Clear();
		this->SetDefined(r->IsDefined());
		if( !this->IsDefined() ) {
		  return;
		}
		StartBulkLoad();
		UTPoint unit;
		for( int i = 0; i < r->GetNoComponents(); i++ ){
		  r->Get( i, unit );
		  Add( unit );
		}
		EndBulkLoad( false );
  }

/*
3.10.5.3 Operation ~trajectory~

*Precondition:* ~X.IsOrdered()~

*Semantics:*

*Complexity:* $O( n )$, where ~n~ is the number of units of this ~MPoint~

*/
  // void Trajectory( Line& line ) const;

  // The scalar velocity as a temporal function
  // If geoid = 0, metric (X.Y)-coordinates are used within the MPoint.
  // If geoid points to a valid Geoid object, geografic coordinates (LON,LAT)
  // are used within the MPoint (speed over ground).
  // void MSpeed(  MReal& result, const Geoid* geoid = 0 ) const;

  // The vectorial velocity --- (X,Y)-components --- as temporal function
  // void MVelocity( MTPoint& result ) const;

/*
3.10.5.3 Operation ~distance~

If ~geoid~ is NULL, euclidean geometry is used, spherical geometry otherwise.
If invalid geographic coordinates are found, the result is UNDEFINED.

*Precondition:* ~X.IsOrdered()~

*Semantics:*

*Complexity:* $O( n )$, where ~n~ is the number of units of this ~MPoint~

*/
  // void Distance( const TPoint& p, MReal& result,
  //                const Geoid* geoid=0 ) const;
  // void SquaredDistance( const TPoint& p, MReal& result,
  //                       const Geoid* geoid=0 ) const;
  // void SquaredDistance( const MTPoint& p, MReal& result,
  //                       const Geoid* geoid=0 ) const;

/*
3.10.5.4 Operation ~Simplify~

*Precondition*: ~X.IsOrdered()~

*Semantics*: Simplifies the tracjectory this moving point.

*Complexity*: worst case: O(n * n), in average O(n * log (n))

*/

   // void Simplify(const double epsilon, MTPoint& result,
   //               const bool checkBreakPoints,
   //               const datetime::DateTime& duration) const;

/*
3.10.5.5 Operation ~BreakPoints~

*Precondition*: ~X.IsOrdered()~
*Semantics*: Computes all points where this mpoints stays longer than the given
             time.

*/
    // void BreakPoints(Points& result, const datetime::DateTime& dur) const;
    // void BreakPoints(Points& result, const datetime::DateTime& dur,
    //                  const CcReal& epsilon,
    //                  const Geoid* geoid=0) const;

/*
3.10.5.6 Operatiopn ~Breaks~

This function computes the timeIntervalls for Breaks

*/

    // void Breaks(Periods& result, const datetime::DateTime& dur,
    //             const CcReal& epsilon,
    //             const Geoid* geoid=0) const;


/*
3.10.5.5 Operation ~Vertices~


This operations stores the ends of the units into a ~points~ value.

*/

   // void Vertices(Points& result) const;


/*
3.10.5.5 Operation ~gk~

This method performs a gauss krueger projection to that mpoint.
If the coordinates are not in geo range (-180-180, -90-90), the result
will be undefined. The same holds for zone < 0 and zone > 119.

*/
   // void gk(const int &zone, MPoint& result) const;


/*
3.10.5.6 Operation ~TranslateAppend~

Appends the mpoint argument to this moving point. The point will stay on its
last position for an interval of length dur. If dur is smaller or equals to
zero, the new movement starts directly after the definition time of the
original object. The movement is continued at the last position of this mpoint.

*/

  // void TranslateAppend(const MTPoint& mp, const datetime::DateTime& dur);


/*
3.10.5.7 Reverse

Store the reverse of the movement of this instance of a mpoint into result.

*/
  // void Reverse(MTPoint& result);


/*
3.10.5.7 Direction

Compute the ~direction~ (~useHeading~ is ~false~) resp. ~heading~ (~true~) of a
moving point.
If ~geoid~ is not NULL, spherical geometry is applied. In this case, each unit
may produce several result units, since the true course changes along the
orthodrome. Parameter ~epsilon~ determines how exact the approximation will be.

*/
  // void Direction( MReal* result,
  //                 const bool useHeading = false,
  //                 const Geoid* geoid    = 0,
  //                 const double epsilon  = 0.0000001) const;

/*
3.10.5.8 ~Sample~

This operator creates a new mpoint from the original one.
All units will have the length of the duration given as
parameter. The starting time is the same as for the original
mpoint. If gaps longer than the given duration exist,
the next unit will also have the starting time of the unit
directly after the gap.
If __KeepEndPoint__ is set to __true__, the last position will be
part of the resuolt even if at the corresponding time no
sample point exist. If the parameter __exactPath__ is set to
__true__, additional sample points are inserted to be sure that the
path of the object is the same as in the original, i.e. no shortcuts
for corners.

*/
  // void Sample(const datetime::DateTime& duration, MTPoint& result,
  //             const bool KeepEndPoint = false,
  //             const bool exactPath = false  )const;

/*
3.10.5.8 ~Append~

The ~Append~ function appends all units of the argument to this
MPoint. If this mpoint or the argument is undefined or if the
argument starts before this mpoint ends, this mpoint will be set
to be undefined. The return value is the defined state of this
mpoint after the operation (indicating the success).

*/
   // bool Append(const MTPoint& p, const bool autoresize = true);

/*
3.10.5.9 ~Disturb~

The ~disturb~ operation changes the position of the moving point
using a random generator.

*/
   // void Disturb(MPoint& result,
   //              const double maxDerivation,
   //              double maxDerivationPerStep);


/*
3.10.5.10 ~length~

Determines the drive distance of this moving point (odometer).
Coordinates are interpreted as metric (X,Y) coordinates.
Will return a value smaller than zero if this mpoint is not defined

*/
  //double Length() const;

/*
Determines the drive distance of this moving point (odometer).
Coordinates are interpreted as geographic (LON,LAT) coordinates.

If an invalid geographic coordinate is encountered, ~valid~ is set to false,
otherwise the result is calculated and ~valid~ is set to true.

The same happens, if the Mpoint is undefined. In addition to setting ~valid~ to
false, the return value will be negative.

*/
  //double Length(const Geoid& g, bool& valid) const;

/*
3.10.5.11 ~BoundingBox~

Returns the MPoint's minimum bounding rectangle. If geoid is NULL, euclidean
geometry is used, otherwise spherical geometry is applied.

*/
  // return the stored bbox
  //Rectangle<3> BoundingBox(const Geoid* geoid = 0) const;

  static unsigned GetDim(){ return 3; }

  // return the spatial bounding box (2D: X/Y)
  //const Rectangle<2> BoundingBoxSpatial(const Geoid* geoid = 0) const;

  // recompute bbox, if necessary
  //void RestoreBoundingBox(const bool force = false);


  // void EqualizeUnitsSpatial(const double epsilon,
  //                           MPoint& result,
  //                           const bool skipSplit = false) const;

  static const std::string BasicType(){ return "mtpoint"; }
  static const bool checkType(const ListExpr type){
    return listutils::isSymbol(type, BasicType());
  }


}; //end class
}//end namespace

std::ostream& operator<< (std::ostream& os, const gmo::MTPoint& up);


#endif