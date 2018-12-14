#include <iostream>
#include <string>
#include "../Tools/Flob/DbArray.h"
#include "Attribute.h"
#include "StandardTypes.h"
#include "DateTime.h"
#include "Domain.h"
#include "../Temporal/TemporalAlgebra.h"
#include "MTPoint.h"
#include "TPoint.h"
#include "UTPoint.h"
#include "../Spatial/SpatialAlgebra.h"

using namespace datetime;
using namespace temporalalgebra;
using namespace gmo;



// Utilities for MTPoint

//TODO
bool AlmostEqual(TPoint& one, TPoint& two){ return false;}





void MTPoint::Clear()
{
  Mapping<UTPoint, TPoint>::Clear(); // call super
  // bbox.SetDefined(false);          // invalidate bbox
}

void MTPoint::Add( const UTPoint& unit )
{
  cout << "CALLED: MTPoint::Add" << endl;
  assert( unit.IsDefined() );
  assert( unit.IsValid() );
  cout<<"Print utpoint:"<<endl;
  unit.Print(cout);
  if(!IsDefined() || !unit.IsDefined()){
    SetDefined( false );
    return;
  }
  units.Append( unit );
  if(units.Size() == 1)
  {
//     cout << "        MTPoint::Add FIRST ADD" << endl;
//     cout << "\t Old bbox = "; bbox.Print(cout);
    // bbox.SetDefined( true );
    // bbox = unit.BoundingBox();
//     cout << "\n\t New bbox = "; bbox.Print(cout);
  } else {
//     cout << "\t Old bbox = "; bbox.Print(cout);
    // bbox = bbox.Union(unit.BoundingBox());
//     cout << "\n\t New bbox = "; bbox.Print(cout);
  }
  // RestoreBoundingBox(false);
}


void MTPoint::MergeAdd(const UTPoint& unit){
  assert( IsDefined() );
  assert( unit.IsDefined() );
  assert( unit.IsValid() );

  int size = GetNoComponents();
  if(size==0){ // the first unit
    Add(unit); // Add() unit as first unit to empty mapping; bbox is updated.
    return;
  }
  UTPoint last;
  Get(size-1,last);

  assert(last.timeInterval.end <= unit.timeInterval.start);

  if(last.timeInterval.end!=unit.timeInterval.start ||
     !( (last.timeInterval.rc )  ^ (unit.timeInterval.lc))){
     // intervals are not connected
    Add(unit); // also adopts bbox
    return;
  }
  TPoint last_end = last.GetEnd();
  TPoint unit_start = unit.GetStart();

  if( last_end.GetStop() != unit_start.GetStop()){
    // jump in spatial dimension
    Add(unit);  // also adopts bbox
    return;
  }
  Interval<Instant> complete(last.timeInterval.start,
                             unit.timeInterval.end,
                             last.timeInterval.lc,
                             unit.timeInterval.rc);
  TPoint last_start = last.GetStart();
  TPoint unit_end = unit.GetEnd();

  UTPoint upoint(complete, last_end, unit_end);
  TPoint p;
  upoint.TemporalFunction(last.timeInterval.end, p, true);
  if( p != last_start){
    Add(unit); // also adopts bbox
    return;
  }
  assert( upoint.IsValid() );
  assert( upoint.IsDefined() );
  // bbox = bbox.Union(upoint.BoundingBox()); // update bbox
  units.Put(size-1,upoint); // overwrite the last unit by a connected one
}

bool MTPoint::EndBulkLoad(const bool sort, const bool checkvalid)
{
  bool res = Mapping<UTPoint, TPoint>::EndBulkLoad( sort, checkvalid ); 
  if(res){
      // RestoreBoundingBox(); 
  }
  return res;
}


void MTPoint::Restrict( const vector< pair<int, int> >& intervals )
{
  if(!IsDefined()){
    Clear();
    // bbox.SetDefined(false);
    SetDefined( false );
    return;
  }
  units.Restrict( intervals, units ); // call super
  // bbox.SetDefined(false);      // invalidate bbox
  // RestoreBoundingBox();        // recalculate it
}

ostream& MTPoint::Print( ostream &os ) const
{
  if( !IsDefined() )
  {
    return os << "(MTPoint: undefined)";
  }
  // os << "(MTPoint: defined, MBR = ";
  // bbox.Print(os);
  os << ", contains " << GetNoComponents() << " units: ";
  for(int i=0; i<GetNoComponents(); i++)
  {
    UTPoint unit;
    Get( i , unit );
    os << "\n\t";
    unit.Print(os);
  }
  os << "\n)" << endl;
  return os;
}


bool MTPoint::operator==( const MTPoint& r ) const
{
  if(!IsDefined()){
     return !r.IsDefined();
  }
  if(!r.IsDefined()){
     return false;
  }
  assert( IsOrdered() && r.IsOrdered() );
  if(IsEmpty()){
    return r.IsEmpty();
  }
  if(r.IsEmpty()){
    return false;
  }

  // if(bbox != r.bbox)
  //   return false;
  return Mapping<UTPoint, TPoint>::operator==(r);
}

bool MTPoint::Present( const Instant& t ) const
{
  assert( IsDefined() );
  assert( IsOrdered() );
  assert( t.IsDefined() );

//   if(bbox.IsDefined())
//   { // do MBR-check
//     double instd = t.ToDouble();
//     double mint = bbox.MinD(2);
//     double maxt = bbox.MaxD(2);
//     if( (instd < mint && !AlmostEqual(instd,mint)) ||
//         (instd > maxt && !AlmostEqual(instd,maxt))
//       )
//     {
// //       cout << __PRETTY_FUNCTION__<< "(" << __FILE__ << __LINE__
// //         << "): Bounding box check failed:" << endl;
// //       cout << "\tInstant : "; t.Print(cout); cout << endl;
// //       cout << "\tinstd   : " << instd << endl;
// //       cout << "\tmint/maxt :" << mint << "\t/\t" << maxt << endl;
// //       cout << "\tBBox = "; bbox.Print(cout); cout << endl;
//       return false;
//     }
//   }
  int pos = Position(t);
  if( pos == -1 )         //not contained in any unit
    return false;
  return true;
}

bool MTPoint::Present( const Periods& t ) const
{
  assert( IsDefined() );
  assert( IsOrdered() );
  assert( t.IsDefined() );
  assert( t.IsOrdered() );

//   if(bbox.IsDefined())
//   { // do MBR-check
//     double MeMin = bbox.MinD(2);
//     double MeMax = bbox.MaxD(2);
//     Instant tmin; t.Minimum(tmin);
//     Instant tmax; t.Maximum(tmax);
//     double pmin = tmin.ToDouble();
//     double pmax = tmax.ToDouble();
//     if( (pmax < MeMin && !AlmostEqual(pmax,MeMin)) ||
//         (pmin > MeMax && !AlmostEqual(pmin,MeMax))
//       )
//     {
// //       cout << __PRETTY_FUNCTION__<< "(" << __FILE__ << __LINE__
// //            << "): Bounding box check failed:" << endl;
// //       cout << "\tPeriod : "; t.Print(cout); cout << endl;
// //       cout << "\tpmin/pmax : " << pmin  << "\t/\t" << pmax << endl;
// //       cout << "\ttmin/tmax :" << tmin << "\t/\t" << tmax << endl;
// //       cout << "\tMTPoint : " << MeMin << "\t---\t" << MeMax << endl;
// //       cout << "\tBBox = "; bbox.Print(cout); cout << endl;
//       return false;
//     }
//   }
  Periods defTime( 0 );
  DefTime( defTime );
  return t.Intersects( defTime );
}

// void MTPoint::AtInstant( const Instant& t, Intime<Point>& result ) const
// {
//   if( IsDefined() && t.IsDefined() )
//   {
//     if( !bbox.IsDefined() )
//     { // result is undefined
//       result.SetDefined(false);
//     } else if( IsEmpty() )
//     { // result is undefined
//       result.SetDefined(false);
//     } else
//     { // compute result
//       double instd = t.ToDouble();
//       double mind = bbox.MinD(2);
//       double maxd = bbox.MaxD(2);
//       if( (mind > instd && !AlmostEqual(mind,instd)) ||
//            (maxd < instd && !AlmostEqual(maxd,instd))
//         )
//       {
// //         cout << __PRETTY_FUNCTION__<< "(" << __FILE__ << __LINE__
// //           << "): Bounding box check failed:" << endl;
// //         cout << "\tInstant : "; t.Print(cout); cout << endl;
// //         cout << "\tinstd   : " << instd << endl;
// //         cout << "\tmind/maxd :" << mind << "\t/\t" << maxd << endl;
// //         cout << "\tBBox = "; bbox.Print(cout); cout << endl;
//         result.SetDefined(false);
//       } else
//       {
//         assert( IsOrdered() );
//         int pos = Position( t );
//         if( pos == -1 )  // not contained in any unit
//           result.SetDefined( false );
//         else
//         {
//           UTPoint posUnit;
//           units.Get( pos, &posUnit );
//           result.SetDefined( true );
//           posUnit.TemporalFunction( t, result.value );
//           result.instant.CopyFrom( &t );
//         }
//       }
//     }
//   } else
//   {
//     result.SetDefined(false);
//   }
// }

// void MTPoint::AtPeriods( const Periods& p, MTPoint& result ) const
// {
//   result.Clear();
//   result.SetDefined(true);
//   if( IsDefined() && p.IsDefined() )
//   {
//     if( !bbox.IsDefined())
//     { // result is undefined
//       result.SetDefined(false);
//     } else if( IsEmpty() || p.IsEmpty())
//     { // result is defined but empty
//       result.SetDefined(true);
//     } else if( IsMaximumPeriods(p) )
//     { // p is [begin of time, end of time]. Copy the input into result.
//       result.CopyFrom(this);
//     }
//     else
//     { // compute result
//       assert( IsOrdered() );
//       assert( p.IsOrdered() );
//       Instant perMinInst; p.Minimum(perMinInst);
//       Instant perMaxInst; p.Maximum(perMaxInst);
//       double permind = perMinInst.ToDouble();
//       double permaxd = perMaxInst.ToDouble();
//       double mind = bbox.MinD(2);
//       double maxd = bbox.MaxD(2);
//       if( (mind > permaxd && !AlmostEqual(mind,permaxd)) ||
//           (maxd < permind && !AlmostEqual(maxd,permind)))
//       {
// //         cout << __PRETTY_FUNCTION__<< "(" << __FILE__ << __LINE__
// //           << "): Bounding box check failed:" << endl;
// //         cout << "\tPeriod : "; p.Print(cout); cout << endl;
// //         cout << "\tperMinInst : "; perMinInst.Print(cout); cout << endl;
// //         cout << "\tperMaxInst : "; perMaxInst.Print(cout); cout << endl;
// //         cout << "\tpermind/permaxd : " << permind  << "\t/\t"
// //              << permaxd << endl;
// //         cout << "\tmind/maxd :" << mind << "\t/\t" << maxd << endl;
// //         cout << "\tBBox = "; bbox.Print(cout); cout << endl;
//         result.SetDefined(true);
//       } else
//       {
//         result.StartBulkLoad();
//         UTPoint unit;
//         Interval<Instant> interval;
//         int i = 0, j = 0;
//         Get( i, unit );
//         p.Get( j, interval );

//         while( 1 )
//         {
//           if( unit.timeInterval.Before( interval ) )
//           {
//             if( ++i == GetNoComponents() )
//               break;
//             Get( i, unit );
//           }
//           else if( interval.Before( unit.timeInterval ) )
//           {
//             if( ++j == p.GetNoComponents() )
//               break;
//             p.Get( j, interval );
//           }
//           else
//           { // we have overlapping intervals, now
//             UTPoint r(1);
//             unit.AtInterval( interval, r );
//             assert( r.IsDefined() );
//             assert( r.IsValid()   );
//             result.Add( r );
// //          cout << "\n\tunit = "; unit.Print(cout); cout << endl;
// //          cout << "\tinterval =       "; interval.Print(cout); cout << endl;
// //          cout << "\tr    = "; r.Print(cout); cout << endl;

//             if( interval.end == unit.timeInterval.end )
//             { // same ending instant
//               if( interval.rc == unit.timeInterval.rc )
//               { // same ending instant and rightclosedness: Advance both
//                 if( ++i == GetNoComponents() )
//                   break;
//                 Get( i, unit );
//                 if( ++j == p.GetNoComponents() )
//                   break;
//                 p.Get( j, interval );
//               }
//               else if( interval.rc == true )
//               { // Advanve in mapping
//                 if( ++i == GetNoComponents() )
//                   break;
//                 Get( i, unit );
//               }
//               else
//               { // Advance in periods
//                 assert( unit.timeInterval.rc == true );
//                 if( ++j == p.GetNoComponents() )
//                   break;
//                 p.Get( j, interval );
//               }
//             }
//             else if( interval.end > unit.timeInterval.end )
//             { // Advance in mpoint
//               if( ++i == GetNoComponents() )
//                 break;
//               Get( i, unit );
//             }
//             else
//             { // Advance in periods
//               assert( interval.end < unit.timeInterval.end );
//               if( ++j == p.GetNoComponents() )
//                 break;
//               p.Get( j, interval );
//             }
//           }
//         }
//         result.EndBulkLoad();
//       }
//     }
//   } else
//   {
//     result.SetDefined(false);
//   }
// }

// /*

// RestoreBoundingBox() checks, whether the MTPoint's MBR ~bbox~ is ~undefined~
// and thus may needs to be recalculated and if, does so.

// */

// void MTPoint::RestoreBoundingBox(const bool force)
// {
//   if(!IsDefined() || GetNoComponents() == 0)
//   { // invalidate bbox
//     bbox.SetDefined(false);
//   }
//   else if(force || !bbox.IsDefined())
//   { // construct bbox
//     UTPoint unit;
//     int size = GetNoComponents();
//     Get( 0, unit ); // safe, since (this) contains at least 1 unit
//     bbox = unit.BoundingBox();
//     for( int i = 1; i < size; i++ ){
//       Get( i, unit );
//       bbox = bbox.Union(unit.BoundingBox());
//     }
//   } // else: bbox unchanged and still correct
// }

// // Class functions
// Rectangle<3u> MTPoint::BoundingBox(const Geoid* geoid /*=0*/) const
// {
//   if(geoid){ // spherical geometry case:
//     if(!IsDefined() || (GetNoComponents()<=0) ){
//       return Rectangle<3>(false);
//     }
//     UTPoint u;
//     Rectangle<3> bbx(false);
//     for(int i=0; i<GetNoComponents(); i++){
//       Get(i,u);
//       assert( u.IsDefined() );
//       if(bbx.IsDefined()){
//         bbx.Union(u.BoundingBox(geoid));
//       } else {
//         bbx = u.BoundingBox(geoid);
//       }
//     }
//     return bbx;
//   } // else: euclidean case
//   return bbox;
// }

// // return the spatial bounding box (2D: X/Y)
// const Rectangle<2> MTPoint::BoundingBoxSpatial(const Geoid* geoid) const {
//   Rectangle<2u> result(false,0.0,0.0,0.0,0.0);
//   if(!IsDefined() || (GetNoComponents()<=0) ){
//     return result;
//   } else {
//     Rectangle<3> bbx = this->BoundingBox(geoid);
//     result = bbx.Project2D(0,1); // project to X/Y
//     return result;
//   }
// };

// void MTPoint::Trajectory( Line& line ) const
// {
//   line.Clear();
//   if(!IsDefined()){
//     line.SetDefined( false );
//     return;
//   }
//   line.SetDefined( true );
//   line.StartBulkLoad();

//   HalfSegment hs;
//   UTPoint unit;
//   int edgeno = 0;

//   int size = GetNoComponents();
//   if (size>0)
//     line.Resize(size);

//   Point p0(false);      // starting point
//   Point p1(false);      // end point of the first unit
//   Point p_last(false);  // last point of the connected segment

//   for( int i = 0; i < size; i++ ) {
//     Get( i, unit );

//     if( !AlmostEqual( unit.p0, unit.p1 ) )    {
//       if(!p0.IsDefined()){ // first unit
//         p0 = unit.p0;
//         p1 = unit.p1;
//         p_last = unit.p1;
//       } else { // segment already exists
//         if(p_last!=unit.p0){ // spatial jump
//            hs.Set(true,p0,p_last);
//            hs.attr.edgeno = ++edgeno;
//            line += hs;
//            hs.SetLeftDomPoint(!hs.IsLeftDomPoint());
//            line += hs;
//            p0 = unit.p0;
//            p1 = unit.p1;
//            p_last = unit.p1;
//         } else { // an extension, check direction
//            if(!AlmostEqual(p0,unit.p1)){
//              HalfSegment tmp(true,p0,unit.p1);
//              double dist = tmp.Distance(p1);
//              double dist2 = tmp.Distance(p_last);
//              if(AlmostEqual(dist,0.0) && AlmostEqual(dist2,0.0)){
//                p_last = unit.p1;
//              } else {
//                hs.Set(true,p0,p_last);
//                hs.attr.edgeno = ++edgeno;
//                line += hs;
//                hs.SetLeftDomPoint(!hs.IsLeftDomPoint());
//                line += hs;
//                p0 = unit.p0;
//                p1 = unit.p1;
//                p_last = unit.p1;
//              }
//           }
//         }
//       }
//     }
//   }
//   if(p0.IsDefined() && p_last.IsDefined() && !AlmostEqual(p0,p_last)){
//     hs.Set(true,p0,p_last);
//     hs.attr.edgeno = ++edgeno;
//     line += hs;
//     hs.SetLeftDomPoint(!hs.IsLeftDomPoint());
//     line += hs;
//   }
//   line.EndBulkLoad();
// }

// void MTPoint::Distance( const Point& p, MReal& result, const Geoid* geoid ) const
// {
//   result.Clear();
//   if( !IsDefined() || !p.IsDefined() || (geoid && !geoid->IsDefined()) ){
//     result.SetDefined( false );
//     return;
//   }
//   result.SetDefined( true );
//   UTPoint uPoint;
//   result.Resize(GetNoComponents());
//   result.StartBulkLoad();
//   for( int i = 0; i < GetNoComponents(); i++ ){
//     Get( i, uPoint );
//     vector<UReal> resvec;
//     uPoint.Distance( p, resvec, geoid );
//     for(vector<UReal>::iterator it=resvec.begin(); it!=resvec.end(); it++ ){
//       if(it->IsDefined()){
//         result.MergeAdd( *it );
//       }
//     }
//   }
//   result.EndBulkLoad( false, false );
// }

// void MTPoint::SquaredDistance( const Point& p, MReal& result,
//                               const Geoid* geoid ) const
// {
//   result.Clear();
//   if( !IsDefined() || !p.IsDefined() || (geoid && !geoid->IsDefined()) ){
//     result.SetDefined( false );
//     return;
//   }
//   result.SetDefined( true );
//   UTPoint uPoint;
//   result.Resize(GetNoComponents());
//   result.StartBulkLoad();
//   for( int i = 0; i < GetNoComponents(); i++ ){
//     Get( i, uPoint );
//     vector<UReal> resvec;
//     uPoint.Distance( p, resvec, geoid );
//     for(vector<UReal>::iterator it(resvec.begin()); it!=resvec.end(); it++ ){
//       if(it->IsDefined()){
//         UReal resunit(*it);
//         if( resunit.r ) {
//            resunit.r = false;
//         } else {
//            assert(resunit.a==0);
//            assert(resunit.b==0);
//            resunit.c = resunit.c * resunit.c;
//         }
//         result.MergeAdd( resunit );
//       }
//     }
//   }
//   result.EndBulkLoad( false, false );
// }

// void MTPoint::SquaredDistance( const MTPoint& p, MReal& result,
//                               const Geoid* geoid ) const
// {
//   result.Clear();
//   if( !IsDefined() || !p.IsDefined() || (geoid && !geoid->IsDefined()) ){
//     result.SetDefined( false );
//     return;
//   }
//   result.SetDefined( true );
//   UReal uReal(true);

//   RefinementPartition<MTPoint, MTPoint, UTPoint, UTPoint> rp(*this, p);

//   result.Resize(rp.Size());
//   result.StartBulkLoad();
//   for( unsigned int i = 0; i < rp.Size(); i++ )
//   {
//     Interval<Instant> iv;
//     int u1Pos, u2Pos;
//     UTPoint u1;
//     UTPoint u2;

//     rp.Get(i, iv, u1Pos, u2Pos);

//     if (u1Pos == -1 || u2Pos == -1)
//       continue;
//     else {
//       Get(u1Pos, u1);
//       p.Get(u2Pos, u2);
//     }
//     if(u1.IsDefined() && u2.IsDefined())
//     { // do not need to test for overlapping deftimes anymore...
//       u1.Distance( u2, uReal, geoid );
//       if(!uReal.IsDefined()){
//         cerr << __PRETTY_FUNCTION__
//              << "Invalid geographic coord found!" << endl;
//         result.EndBulkLoad( false, false );
//         result.Clear();
//         result.SetDefined(false);
//         return;
//       }
//       uReal.r= false;
//       result.MergeAdd( uReal );
//     }
//   }
//   result.EndBulkLoad();
// }



// /*
// This function checks whether the end point of the first unit is equal
// to the start point of the second unit and if the time difference is
// at most a single instant

// */
// static bool connected(const UTPoint* u1, const UTPoint* u2){
//    if(u1->p1 != u2->p0){ // spatial connected
//        return false;
//    }
//    // temporal connection
//    if(! ((u1->timeInterval.end) == (u2->timeInterval.start))){
//        return false;
//    }
//    return true;
// }

// static bool IsBreakPoint(const UTPoint* u,const DateTime& duration){
//    if(u->p0 != u->p1){ // equal points required
//      return false;
//    }
//    DateTime dur = u->timeInterval.end -  u->timeInterval.start;
//    return (dur > duration);
// }



// /**
// ~Simplify~

// This function removes some sampling points from a moving point
// to get simpler data. It's implemented using an algorithm based
// on the Douglas Peucker algorithm for line simplification.

// **/

// void MTPoint::Simplify(const double epsilon, MTPoint& result,
//                       const bool checkBreakPoints,
//                       const DateTime& dur) const{
//    result.Clear();

//    // check for defined state
//    if( !IsDefined() || !dur.IsDefined() ){
//      result.SetDefined(false);
//      return;
//    }
//    result.SetDefined(true);

//    unsigned int size = GetNoComponents();
//    // no simplification possible if epsilon < 0
//    // or if at most one unit present
//    if(epsilon<0 || size < 2){
//       result.CopyFrom(this);
//       return;
//    }

//    // create an boolean array which represents all sample points
//    // contained in the result
//    bool useleft[size];
//    bool useright[size];
//    // at start, no sampling point is used
//    for(unsigned int i=0;i<size;i++){
//        useleft[i] = false;
//        useright[i] =false;
//    }

//    unsigned int first=0;
//    unsigned int last=1;
//    UTPoint u1(false);
//    UTPoint u2(false);
//    while(last<size){
//       // check whether last and last -1 are connected
//       Get(last-1,u1);
//       Get(last,u2);

//       if( checkBreakPoints && IsBreakPoint(&u1,dur)){
//          if(last-1 > first){
//             Simplify(first,last-2,useleft,useright,epsilon);
//          }
//          Simplify(last-1, last-1, useleft, useright, epsilon);
//          first = last;
//          last++;
//       } else if( checkBreakPoints && IsBreakPoint(&u2,dur)){
//          Simplify(first,last-1,useleft,useright,epsilon);
//          last++;
//          Simplify(last-1, last-1,useleft,useright,epsilon);
//          first = last;
//          last++;
//       } else if(connected(&u1,&u2)){ // enlarge the sequence
//          last++;
//       } else {
//           Simplify(first,last-1,useleft, useright, epsilon);
//           first=last;
//           last++;
//       }
//    }
//    // process the last recognized sequence
//    Simplify(first,last-1,useleft, useright,epsilon);


//    // count the number of units
//    int count = 1; // count the most right sample point
//    for( unsigned int i=0;i<size;i++){
//       if( useleft[i]){
//          count++;
//       }
//    }

//    result.Resize(count); // prepare enough memory

//    result.StartBulkLoad();
//    Instant start;
//    Point p0;
//    bool closeLeft = false;
//    bool leftDefined = false;
//    for(unsigned int i=0; i< size; i++){
//      UTPoint upoint(false);

//      Get(i,upoint);
//      if(useleft[i]){
//         // debug
//         if(leftDefined){
//            cerr << " error in mpoint simplification,"
//                 << " overwrite an existing leftPoint "  << endl;
//         }
//         // end of debug
//         p0 = upoint.p0;
//         closeLeft = upoint.timeInterval.lc;
//         start = upoint.timeInterval.start;
//         leftDefined=true;
//      }
//      if(useright[i]){
//         // debug
//         if(!leftDefined){
//            cerr << " error in mpoint simplification,"
//                 << " rightdefined before leftdefined "  << endl;

//         }
//         Interval<Instant> interval(start,upoint.timeInterval.end,closeLeft,
//                                    upoint.timeInterval.rc);

//         UTPoint newUnit(interval,p0,upoint.p1);
//         result.Add(newUnit);
//         leftDefined=false;
//      }
//    }
//    result.EndBulkLoad(false,false);
// }


// /**
// ~Simplify~

// Recursive implementation of simplifying movements.

// **/

// void MTPoint::Simplify(const int min,
//                  const int max,
//                  bool* useleft,
//                  bool* useright,
//                  const double epsilon) const {

//   // the endpoints are used in each case
//   useleft[min] = true;
//   useright[max] = true;

//   if(min==max){ // no intermediate sampling points -> nothing to simplify
//      return;
//   }

//   UTPoint u1;
//   UTPoint u2;
//   // build a UTPoint from the endpoints
//   Get(min,u1);
//   Get(max,u2);

//   UTPoint upoint(Interval<Instant>(u1.timeInterval.start,
//                 u2.timeInterval.end,true,true),
//                 u1.p0,
//                 u2.p1);

//   // search for the point with the highest distance to its simplified position
//   double maxDist = 0;
//   int maxIndex=0;
//   Point p_orig;
//   Point p_simple;
//   UTPoint u;
//   double distance;
//   for(int i=min+1;i<=max;i++){
//      Get(i,u);
//      upoint.TemporalFunction(u.timeInterval.start,p_simple, true);
//      distance  = p_simple.Distance(u.p0);
//      if(distance>maxDist){ // new maximum found
//         maxDist = distance;
//         maxIndex = i;
//      }
//   }

//   if(maxIndex==0){  // no difference found
//       return;
//   }
//   if(maxDist<=epsilon){  // differnce is in allowed range
//       return;
//   }

//   // split at the left point of maxIndex
//   Simplify(min,maxIndex-1,useleft,useright,epsilon);
//   Simplify(maxIndex,max,useleft,useright,epsilon);
// }



// void MTPoint::BreakPoints(Points& result, const DateTime& dur) const{
//     result.Clear();
//     if( !IsDefined() || !dur.IsDefined() ){
//       result.SetDefined(false);
//       return;
//     }
//     result.SetDefined(true);
//     int size = GetNoComponents();
//     result.StartBulkLoad();
//     UTPoint unit;
//     for(int i=0;i<size;i++){
//         Get(i,unit);
//         if(IsBreakPoint(&unit,dur)){
//            result += (unit.p0);
//         }
//     }
//     result.EndBulkLoad();
// }

// void MTPoint::BreakPoints(Points& result, const DateTime& dur, 
//                          const CcReal& epsilon, 
//                          const Geoid* geoid /*=0*/ ) const{

//     result.Clear();
//     if(!IsDefined() || !dur.IsDefined() || !epsilon.IsDefined()){
//        result.SetDefined(false);
//        return;
//     }

//     double eps = epsilon.GetValue();
//     if(eps<0){
//       return; // we cannot find distances smaller than zero 
//     }

//     result.SetDefined(true);
//     int size = GetNoComponents();
//     result.StartBulkLoad();
//     UTPoint unit;
//     UTPoint firstUnit;
//     Point firstPoint;
//     int firstIndex=0;
//     int index = 0;
//     DateTime currentDur(datetime::durationtype);

//     while(firstIndex < size){
//       if(index == firstIndex){ 
//          Get(firstIndex,firstUnit);
//          if(firstUnit.p0.Distance(firstUnit.p1,geoid) > eps){
//             // this units overcomes the maximum epsilon value
//             index++;
//             firstIndex++;
//          } else { // try to find more points for this break
//            firstPoint = firstUnit.p0;
//            currentDur =  firstUnit.timeInterval.end - 
//                          firstUnit.timeInterval.start;
//            index++;
//            if(index>=size){
//              if(currentDur >= dur){
//                result += firstPoint;
//              }
//              firstIndex = index;

//            }
//          }
//       } else {
//         assert(index > firstIndex);
//         UTPoint lastUnit;
//         Get(index-1, lastUnit);
//         Get(index, unit);
//         if(!lastUnit.Adjacent(&unit)){ // gap found, close chain
//            if(currentDur >= dur){
//               result += firstPoint;
//            }
//            firstIndex = index; // start a new try
//         } else {
//           Point rp = unit.p1;
//           if(firstPoint.Distance(rp,geoid) > eps){
//              // next unit does not contribute to break
//              if(currentDur >= dur){
//                 result += firstPoint;
//                 firstIndex = index;
//              } else {
//                 // start a new try
//                 firstIndex++;
//                 index = firstIndex; 
//             }
//           } else {
//             // extend the possible break   
//             currentDur += (unit.timeInterval.end - unit.timeInterval.start);
//             index++;
//             if(index >= size){
//                firstIndex = index;
//                if(currentDur >= dur){
//                   result += firstPoint;
//                }
//             }
//           } 
//         }
//       }
//     }
//     result.EndBulkLoad();
// }


// void MTPoint::Breaks(Periods& result, const DateTime& dur, 
//                          const CcReal& epsilon, 
//                          const Geoid* geoid /*=0*/ ) const{

//     result.Clear();
//     if(!IsDefined() || !dur.IsDefined() || !epsilon.IsDefined()){
//        result.SetDefined(false);
//        return;
//     }

//     double eps = epsilon.GetValue();
//     if(eps<0){
//       return; // we cannot find distances smaller than zero 
//     }

//     result.SetDefined(true);
//     Periods tmp(0);
//     int size = GetNoComponents();
//     //result.StartBulkLoad();
//     UTPoint unit;
//     UTPoint firstUnit;
//     Point firstPoint;
//     DateTime firstTime;
//     int firstIndex=0;
//     int index = 0;
//     DateTime currentDur(datetime::durationtype);

//     while(firstIndex < size){
//       if(index == firstIndex){ 
//          Get(firstIndex,firstUnit);
//          if(firstUnit.p0.Distance(firstUnit.p1,geoid) > eps){
//             // this units overcomes the maximum epsilon value
//             index++;
//             firstIndex++;
//          } else { // try to find more points for this break
//            firstPoint = firstUnit.p0;
//            firstTime = firstUnit.timeInterval.start;
//            currentDur =  firstUnit.timeInterval.end - 
//                          firstUnit.timeInterval.start;
//            index++;
//            if(index>=size){
//              if(currentDur >= dur){
//                Interval<Instant> iv(firstTime, firstTime+currentDur,true,true);
//                result.Union(iv, tmp);
//                result.CopyFrom(&tmp);
//              }
//              firstIndex = index;

//            }
//          }
//       } else {
//         assert(index > firstIndex);
//         UTPoint lastUnit;
//         Get(index-1, lastUnit);
//         Get(index, unit);
//         if(!lastUnit.Adjacent(&unit)){ // gap found, close chain
//            if(currentDur >= dur){
//               Interval<Instant> iv(firstTime,firstTime + currentDur, 
//                                    true, true); 
//               result.Union(iv, tmp);
//               result.CopyFrom(&tmp);
//            }
//            firstIndex = index; // start a new try
//         } else {
//           Point rp = unit.p1;
//           if(firstPoint.Distance(rp,geoid) > eps){
//              // next unit does not contribute to break
//              if(currentDur >= dur){
//                 Interval<Instant> iv(firstTime,firstTime + currentDur, 
//                                      true, true); 
//                 result.Union(iv, tmp);
//                 result.CopyFrom(&tmp);
//                 firstIndex = index;
//              } else {
//                 // start a new try
//                 firstIndex++;
//                 index = firstIndex;
//             }
//           } else {
//             // extend the possible break   
//             currentDur += (unit.timeInterval.end - unit.timeInterval.start);
//             index++;
//             if(index >= size){
//                firstIndex = index;
//                if(currentDur >= dur){
//                 Interval<Instant> iv(firstTime,firstTime + currentDur,
//                                      true, true);
//                 result.Union(iv, tmp);
//                 result.CopyFrom(&tmp);
//                }
//             }
//           } 
//         }
//       }
//     }
//     //result.EndBulkLoad();
// }


// void MTPoint::TranslateAppend(const MTPoint& mp, const DateTime& dur){
//    if( !IsDefined() || !mp.IsDefined() || !dur.IsDefined() ){
//        Clear();
//        SetDefined(false);
//        return;
//    }
//    if(mp.GetNoComponents()==0){ // nothing to do (already defined)
//        return;
//    }
//    if(GetNoComponents()==0){
//        this->CopyFrom(&mp);
//        return;
//    }

//    int newSize = GetNoComponents()+mp.GetNoComponents();
//    Resize(newSize);
//    UTPoint lastUnit;

//    StartBulkLoad();

//    UTPoint firstUnit;
//    mp.Get(0,firstUnit);

//    // add a staying unit
//    if(!dur.IsZero() && !dur.LessThanZero()){
//      Get(GetNoComponents()-1,lastUnit);
//      Interval<Instant> interval = lastUnit.timeInterval;
//      Point lastPoint = lastUnit.p1;
//      // append a unit of staying
//      Interval<Instant> gapInterval(interval.end,interval.end +dur,
//                                    !interval.rc,!firstUnit.timeInterval.lc);
//      UTPoint gap(gapInterval,lastPoint,lastPoint);
//      Add(gap);
//    }

//    Get(GetNoComponents()-1,lastUnit);
//    Instant end = lastUnit.timeInterval.end;
//    DateTime timediff = end - firstUnit.timeInterval.start;
//    double xdiff  = lastUnit.p1.GetX() - firstUnit.p0.GetX();
//    double ydiff  = lastUnit.p1.GetY() - firstUnit.p0.GetY();

//    UTPoint Punit;
//    mp.Get(0,Punit);
//    UTPoint unit = Punit;
//    unit.Translate(xdiff,ydiff,timediff);
//    if(!(lastUnit.timeInterval.rc)){
//        unit.timeInterval.lc = true;
//    } else {
//        unit.timeInterval.lc = false;
//    }
//    Add(unit);

//    for(int i=1; i< mp.GetNoComponents(); i++){
//       mp.Get(i,Punit);
//       unit = Punit;
//       unit.Translate(xdiff,ydiff,timediff);
//       Add(unit);
//    }
//    EndBulkLoad(false);
// }


// void MTPoint::Reverse(MTPoint& result){
//     result.Clear();
//     if(!IsDefined()){
//        result.SetDefined(false);
//        return;
//     }
//     result.SetDefined(true);
//     int size = GetNoComponents();
//     if(size==0){
//        return;
//     }

//     UTPoint unit;
//     Get(size-1,unit);
//     Instant end = unit.timeInterval.end;
//     Get(0,unit);
//     Instant start = unit.timeInterval.start;

//     result.StartBulkLoad();

//     for(int i=size-1; i>=0; i--){
//        Get(i,unit);
//        Instant newEnd = (end - unit.timeInterval.start) + start;
//        Instant newStart = (end - unit.timeInterval.end) + start;
//        Interval<Instant> interval(newStart,newEnd,
//                                   unit.timeInterval.rc,
//                                   unit.timeInterval.lc);
//        UTPoint newUnit(interval,unit.p1,unit.p0);

//        result.Add(newUnit);
//     }
//     result.EndBulkLoad(false);
// }

// void MTPoint::Direction( MReal* result,
//                         const bool useHeading /*=false*/,
//                         const Geoid* geoid    /*=0*/,
//                         const double epsilon  /*=0.0000001*/ ) const
// {
//   if( !IsDefined() || ( geoid && (!geoid->IsDefined() || (epsilon<=0.0)) ) ) {
//     result->SetDefined(false);
//     return;
//   }
//   vector<UReal> resvector;
//   UTPoint unitin;
//   for(int i=0;i<GetNoComponents();i++) {
//     Get(i, unitin);
//     unitin.Direction( resvector, useHeading, geoid, epsilon );
//   }
//   result->Clear();
//   result->SetDefined(true);
//   result->StartBulkLoad();
//   for(vector<UReal>::iterator iter = resvector.begin();
//   iter != resvector.end(); iter++){
//     if( iter->IsDefined() && iter->IsValid() ){
//       result->MergeAdd(*iter);
//     }
//   }
//   result->EndBulkLoad( false );
//   return;
// }
