#ifndef TPOINT_H
#define TPOINT_H

#include <iostream>
#include <string>
#include "../Tools/Flob/DbArray.h"
#include "Attribute.h"
#include "StandardTypes.h"
#include "../Spatial/SpatialAlgebra.h"

using namespace std; 

namespace gmo{



  class TPoint : public Attribute
  {
    public:

  /*
  3.4.1 Constructors and Destructor

  */
      TPoint();
      TPoint( STRING_T stop_id);
      TPoint(const string stop_id);
      explicit TPoint(const bool def);
      TPoint( const TPoint& in_xOther);
      ~TPoint();

      TPoint& operator=( const TPoint& in_xOther ) const;
    
      std::string GetStop() const;
      // STRING_T GetStop() const;
      void SetStop(std::string stop);
      void SetStop(STRING_T stop);
 

     
      void CopyFrom(const Attribute* right);
      StorageType GetStorageType() const;
      size_t HashValue() const;
      TPoint* Clone() const;
      bool Adjacent(const Attribute* attrib) const;
      bool Adjacent(const TPoint& attrib) const;
      static int Compare(const void* ls, const void* rs);
      int Compare(const Attribute* rhs) const;
      int Compare(const TPoint& rhs) const;
      size_t Sizeof() const;
      std::ostream& Print(std::ostream& os) const;
      static const std::string BasicType();
      static const bool checkType(const ListExpr type);
      static const std::string Example();


      /*
      1.1.1 Standard Operators

      */

      TPoint& operator=(const TPoint& other);

      bool operator==(const TPoint& other) const;
      bool operator!=(const TPoint& other) const;
      bool operator<(const TPoint& other) const;
      bool operator<=(const TPoint& other) const;
      bool operator>(const TPoint& other) const;
      bool operator>=(const TPoint& other) const;

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


  /*
  Computes the euclidean distance of 2 glines.

  */
    double Distance (const TPoint* toTPoint) const ;
    void ToPoint(Point *& res) const;

    Point* ToPoint() const;

      //Point* ToPoint() const;

  /*
  Returns the spatial Bounding Box of the point which is a rectangle degenerated
  to a single point.

  */

      // inline const Rectangle<2> BoundingBox(){
      //   if (IsDefined()) {
      //     Point *p = ToPoint();
      //     Rectangle<2> result = p->BoundingBox();
      //     p->DeleteIfAllowed();
      //     return result;
      //   } else return Rectangle<2> (false, 0.0, 0.0, 0.0, 0.0);
      // };



  /*
  Returns a gline representing the shortest path between two GPoint.
  Using DijkstrasAlgorithm

  */

    // bool ShortestPath(const GPoint *ziel, GLine *result,
    //                   DbArray<TupleId>* touchedSects = 0) const;
    




    private:

      STRING_T stop_id;

  };

}

std::ostream& operator<< (std::ostream& os, const gmo::TPoint& gp);


#endif
