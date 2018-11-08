/*
1 Includes*/
#include "TPoint.h"
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


/*
1 Implementation of ~class TPoint~

1.1 Constructors and Deconstructor

The default constructor should never been used, except in the Cast-Function.

*/
TPoint::TPoint():Attribute()
 {}


  
TPoint::TPoint(const string stop_id):Attribute(true),stop_id(stop_id){}
TPoint::TPoint(const bool def):Attribute(def){};
TPoint( const TPoint& in_xOther):Attribute(in_xOther.IsDefined()){
	if(in_xOther.IsDefined()){
		stop_id= in_xOther.GetStop();
	}
}
~TPoint();


string TPoint::GetStop(){
	return stop_id;
}

/*
3.4.2 Methods of class ~tpoint~

*/

TPoint& TPoint::operator=( const TPoint& in_xOther )
{
	SetDefined(in_xOther.IsDefined());
	if( IsDefined())
	{
		stop_id = in_xOther.GetStop();
	}
	return *this;
}




bool TPoint::CheckTPoint( ListExpr type, ListExpr& errorInfo );

string TPoint::BasicType(){ 
	return "tpoint"; 
}

bool TPoint::checkType(const ListExpr type){
	return listutils::isSymbol(type, BasicType());
}


/*
Computes the euclidean distance of 2 glines.

*/
double TPoint::Distance (const TPoint* toTPoint){
	return 0.0f;
} 



void TPoint::ToPoint(Point *& res){}

Point* TPoint::ToPoint(){
	return new Point(false);
}




/*
Returns a gline representing the shortest path between two GPoint.
Using DijkstrasAlgorithm

*/

bool ShortestPath(const GPoint *ziel, GLine *result,
DbArray<TupleId>* touchedSects = 0) const;
bool ShortestPath(const GPoint* ziel, GLine *result,
const Network* pNetwork,
DbArray<TupleId>* touchedSects = 0) const;




