#include <iostream>
#include <string>
#include "../Tools/Flob/DbArray.h"
#include "Attribute.h"
#include "StandardTypes.h"
#include "DateTime.h"
#include "Domain.h"
#include "TemporalAlgebra.h"
#include "UTPoint.h"
#include "../Network/NetworkAlgebra.h"
#include "../Spatial/SpatialAlgebra.h"

using namespace network;
using namespace datetime;
using namespace temporalalgebra;
using namespace gmo;

UTPoint::UTPoint():SpatialTemporalUnit<gmo::TPoint,3>() {}
UTPoint::UTPoint(const bool defined):SpatialTemporalUnit<gmo::TPoint, 3>(defined){}
UTPoint::UTPoint(const temporalalgebra::Interval<Instant>& interval, TPoint& tp_start, TPoint& tp_end):
temporalalgebra::SpatialTemporalUnit<gmo::TPoint, 3>( interval )
{	tp_start=tp_start;
	tp_end=tp_end;
}

UTPoint::UTPoint(const temporalalgebra::Interval<Instant>& interval, string tp_start, string tp_end):
temporalalgebra::SpatialTemporalUnit<gmo::TPoint, 3>( interval ),tp_start(TPoint(tp_start)),tp_end(TPoint(tp_end))
{	
  cout<<"Entrada :"<<tp_start<<" "<<tp_end<<endl;
  GetStart();
  GetEnd();
}
// UTPoint::UTPoint(const UTPoint& gP):temporalalgebra::SpatialTemporalUnit<gmo::TPoint, 3>( gP.IsDefined() ){
// 	if(gP.IsDefined()){
// 		tp_start = gP.GetStart();
// 		tp_end = gP.GetEnd();

// 	}
// }


UTPoint::UTPoint(const UTPoint& gP):
temporalalgebra::SpatialTemporalUnit<gmo::TPoint, 3>( gP.IsDefined() ){
	*((TemporalUnit<TPoint>*)this)=
                    *((TemporalUnit<TPoint>*)&gP);
    tp_start =gP.GetStart();
    tp_end =gP.GetEnd();
    SetDefined(gP.IsDefined());
}
  
UTPoint::~UTPoint(){};

/*
1.1 Getters and setters

*/

TPoint UTPoint::GetStart() const{
    cout<<"Start"<<tp_start.GetStop()<<endl;

	return tp_start;
}

TPoint UTPoint::GetEnd()const{
  cout<<"End"<<tp_end.GetStop()<<endl;
	return tp_end;
}

//Spatial_Structure GetSpatialStructure() const;

void UTPoint::CopyFrom(const Attribute* right){
  *this = *((UTPoint*)right);
}

Attribute::StorageType UTPoint::GetStorageType() const 
{
  return Default;
}

size_t UTPoint::HashValue() const
{
  return (size_t) tp_start.SizeOf()+ (size_t) tp_end.SizeOf();
}


bool UTPoint::Adjacent(const Attribute* attrib) const{
 
    return false;
}
bool UTPoint::Adjacent(const UTPoint& attrib) const{

    return false;
}
int UTPoint::Compare(const void* ls, const void* rs){
  UTPoint lhs( *(UTPoint*) ls);
  UTPoint rhs( *(UTPoint*) rs);
  return lhs.Compare(rhs);
}

int UTPoint::Compare(const Attribute* rhs) const{
  return Compare(*((UTPoint*)rhs));
}
int UTPoint::Compare(const UTPoint& in) const{
  if (!IsDefined() && !in.IsDefined()) return 0;
  if (!IsDefined() && in.IsDefined()) return -1;
  if (IsDefined() && !in.IsDefined()) return -1;
  //if (stop_id == in.GetStop()) return 1;

 
 	return 0;
}

size_t UTPoint::Sizeof() const{
	  return sizeof(TPoint);

}
std::ostream& UTPoint::Print(std::ostream& os) const{
  os<<tp_start.GetStop()<<" "<<tp_end.GetStop()<<endl;
	return os;
}
const std::string UTPoint::BasicType(){
	return "utpoint"; 
}
const bool UTPoint::checkType(const ListExpr type){
	return listutils::isSymbol(type, BasicType());
}

/*
1.1.1 Standard Operators

*/

UTPoint& UTPoint::operator=(const UTPoint& other){
	SetDefined(other.IsDefined());
	if( IsDefined())
	{
		tp_start = other.GetStart();
		tp_end = other.GetEnd();
		timeInterval = other.timeInterval;
	}
	return *this;
}


bool UTPoint::operator==(const UTPoint& other) const
{
  return (Compare(&other) == 1);
}


bool UTPoint::operator!=(const UTPoint& other) const
{
  return (Compare(&other) != 0);
}


bool UTPoint::operator<(const UTPoint& other) const
{
  return (Compare(&other) < 0);
}


bool UTPoint::operator<=(const UTPoint& other) const
{
  return (Compare(&other) < 1);
}


bool UTPoint::operator>(const UTPoint& other) const
{
  return (Compare(&other) > 0);
}


bool UTPoint::operator>=(const UTPoint& other) const
{
  return (Compare(&other) > -1);
}

/*
1.1.1 Operators for Secondo Integration

*/

 ListExpr UTPoint::Out(ListExpr typeInfo, Word value){
	UTPoint* utpoint = (UTPoint*)(value.addr);
  utpoint->Print(cout);
  if( !(((UTPoint*)value.addr)->IsDefined()) )
  {
    return (nl->SymbolAtom("undefined"));
  }
  else
  {
    ListExpr timeintervalList =
        nl->FourElemList(OutDateTime( nl->TheEmptyList(),
                                      SetWord(&utpoint->timeInterval.start) ),
                         OutDateTime( nl->TheEmptyList(),
                                      SetWord(&utpoint->timeInterval.end) ),
                         nl->BoolAtom( utpoint->timeInterval.lc ),
                         nl->BoolAtom( utpoint->timeInterval.rc));

    ListExpr pointsList =
         nl->TwoElemList(nl->StringAtom( utpoint->GetStart().GetStop() ),
                          nl->StringAtom( utpoint->GetEnd().GetStop() )
                          );
      return nl->TwoElemList( timeintervalList, pointsList );
  }  
}

 Word UTPoint::In(const ListExpr typeInfo, const ListExpr instance,
             const int errorPos, ListExpr& errorInfo, bool& correct){
	string errmsg;
  if ( nl->ListLength( instance ) == 2 )
  {
    ListExpr first = nl->First( instance );

    if( nl->ListLength( first ) == 4 &&
        nl->IsAtom( nl->Third( first ) ) &&
        nl->AtomType( nl->Third( first ) ) == BoolType &&
        nl->IsAtom( nl->Fourth( first ) ) &&
        nl->AtomType( nl->Fourth( first ) ) == BoolType )
    {
      correct = true;
      Instant *start = (Instant *)InInstant( nl->TheEmptyList(),
       nl->First( first ),
        errorPos, errorInfo, correct ).addr;

      if( !correct )
      {
        errmsg = "InUGPoint(): Error in first instant.";
        errorInfo = nl->Append(errorInfo, nl->StringAtom(errmsg));
        start->DeleteIfAllowed();
        return SetWord( Address(0) );
      }

      Instant *end = (Instant *)InInstant( nl->TheEmptyList(),
       nl->Second( first ),
       errorPos, errorInfo, correct ).addr;

      if( !correct )
      {
        errmsg = "InUTPoint(): Error in second instant.";
        errorInfo = nl->Append(errorInfo, nl->StringAtom(errmsg));
        start->DeleteIfAllowed();
        end->DeleteIfAllowed();
        return SetWord( Address(0) );
      }

      Interval<Instant> tinterval( *start, *end,
                                   nl->BoolValue( nl->Third( first ) ),
                                   nl->BoolValue( nl->Fourth( first ) ) );
      start->DeleteIfAllowed();
      end->DeleteIfAllowed();

      correct = tinterval.IsValid();
      if (!correct)
        {
          errmsg = "InUTPoint(): Non valid time interval.";
          errorInfo = nl->Append(errorInfo, nl->StringAtom(errmsg));
          return SetWord( Address(0) );
        }

      ListExpr second = nl->Second( instance );
      if( nl->ListLength( second ) == 2 &&
          nl->IsAtom( nl->First( second ) ) &&
          nl->AtomType( nl->First( second ) ) == StringType &&
          nl->IsAtom( nl->Second( second ) ) &&
          nl->AtomType( nl->Second( second ) ) == StringType )
      {
        NList in_list(second);
        cout<<"String values :"<<endl<<nl->StringValue( nl->First( second ) )<<" "<<nl->StringValue( nl->Second( second ) )<<endl;
        cout<<"String values :"<<endl<<in_list.first().str()<<" "<<in_list.second().str()<<endl;
        UTPoint *utpoint = new UTPoint(tinterval,
                                     nl->StringValue( nl->First( second ) ),
                                     nl->StringValue( nl->Second( second ) ));

        correct = utpoint->IsValid();
        if( correct )
          return SetWord( utpoint );

        errmsg = "InUTPoint(): Error in start/end point.";
        errorInfo = nl->Append(errorInfo, nl->StringAtom(errmsg));
        utpoint->DeleteIfAllowed();
      }
    }
  }
  
  errmsg = "InUTPoint(): Error in representation.";
  errorInfo = nl->Append(errorInfo, nl->StringAtom(errmsg));
  correct = false;
  return SetWord( Address(0) );
}
 Word UTPoint::Create(const ListExpr typeInfo){
  
  return SetWord(new UTPoint(true));
}



 void UTPoint::Delete( const ListExpr typeInfo, Word& w ){
  ((UTPoint*) w.addr)->DeleteIfAllowed();
  w.addr = 0;
}
void UTPoint::Close( const ListExpr typeInfo, Word& w )
{
  ((UTPoint*) w.addr)->DeleteIfAllowed();
  w.addr = 0;
}


Word UTPoint::Clone( const ListExpr typeInfo, const Word& w )
{
  return SetWord(new UTPoint(*(UTPoint*) w.addr));
}


void* UTPoint::Cast( void* addr )
{
  return (new (addr) UTPoint);
}


bool UTPoint::KindCheck ( ListExpr type, ListExpr& errorInfo )
{
  return checkType(type);
}


int UTPoint::SizeOf()
{
  return sizeof(UTPoint);
}

 ListExpr UTPoint::Property(){
	return nl->TwoElemList(
	    nl->FourElemList(
	      nl->StringAtom("Signature"),
	      nl->StringAtom("Example Type List"),
	      nl->StringAtom("List Rep"),
	      nl->StringAtom("Example List")),
	    nl->FourElemList(
	      nl->StringAtom("-> " + Kind::DATA()),
	      nl->StringAtom(BasicType()),
	      nl->TextAtom("(" + CcInt::BasicType() + " "+CcInt::BasicType() + ")"),
	      nl->StringAtom("("+ Example() +")")));

}

/*
1.1.1 SpatioTemporal Operators*/



			const Rectangle<3> UTPoint::BoundingBox(const Geoid* geoid /*=0*/)const { return Rectangle<3>(false, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);}


			 double UTPoint::Distance(const Rectangle<3>& rect,const Geoid* geoid) const{return 0.0f;}

 


 			 void UTPoint::TemporalFunction( const Instant& t,
                               TPoint& result,
                               bool ignoreLimits ) const
        {
          TemporalFunction(t, result, 0, ignoreLimits);
        }

			   void UTPoint::TemporalFunction( const Instant& t,
			                                 TPoint& result,const Geoid* geoid,
			                                 bool ignoreLimits) const{ 
          if( !IsDefined() ||
            !t.IsDefined() ||
            (geoid && !geoid->IsDefined()) ||
            (!timeInterval.Contains( t ) && !ignoreLimits) ){
            result.SetDefined(false);
          } else if( t == timeInterval.start ){
            result = tp_start;
            result.SetDefined(true);
          } else if( t == timeInterval.end ){
            result = tp_end;
            result.SetDefined(true);
          } 
          // else if(geoid){ // spherical geometry case
          //   Instant t0 = timeInterval.start;
          //   Instant t1 = timeInterval.end;
          //   Coord f = ((t-t0)/(t1-t0));
          //   result = p0.MidpointTo(p1, f, geoid);
          // } 
          else {// euclidean geometry cases
            Instant t0 = timeInterval.start;
            Instant t1 = timeInterval.end;
            if(t-t0 >= t1-t){
              result=tp_end;

            }else{
              result = tp_start;
            }
            
            result.SetDefined(true);
          }
        }

			  /*
			  Returns true if the ~ugpoint~ passes a given ~gpoint~ false elsewhere.

			  */
			   bool UTPoint::Passes( const TPoint& val )const{
			   	return true;
			   }

			  /*
			  Returns the ~igpoint~ the ~ugpoint~ was at a given network position.

			  */
			   bool UTPoint::At( const TPoint& val, 
			                   TemporalUnit<TPoint>& result ) const{
			   	return true;
			   }

			   

			  


/*

1.1.1.1 ~Example~

Provides example string for list representation can be used by external
property definitions for part of ~JRouteInterval~ representation.

*/

 std::string UTPoint::Example(){
	return "(())";
}

