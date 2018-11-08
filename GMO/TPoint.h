#ifndef GENERICPOINT_H
#define GENERICPOINT_H

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


namespace gmo{



  class TPoint : public Attribute
  {
    public:

  /*
  3.4.1 Constructors and Destructor

  */

      TPoint():Attribute()
      {}

      TPoint(const bool def)
        : Attribute(def)
      {}

      // TPoint(  ):
     
      // {
      //   SetDefined(in_bDefined);
      // }

      TPoint( const TPoint& in_xOther ):
      Attribute(in_xOther.IsDefined()),
      {
        SetDefined(in_xOther.IsDefined());
      }

      ~TPoint(){};

  /*
  3.4.2 Methods of class ~gpoint~

  */

      TPoint& operator=( const TPoint& in_xOther )
      {
        SetDefined(in_xOther.IsDefined());
        if( IsDefined())
        {
          
        }
        return *this;
      }

  /*
  Get Methods of ~tpoint~

  */

      inline void GetNetworkId() const
      {
        
      }



  /*
  Set Methods of ~tpoint~

  */

      inline void SetRouteId( const int in_rid )
      {
        
      }

     
      size_t Sizeof() const
      {
        return sizeof(TPoint);
      }

      size_t HashValue() const
      {
        int a = 10;
        return (size_t) a;
      }

      void CopyFrom( const Attribute* right )
      {
        const TPoint* gp = (const TPoint*)right;
        *this = *gp;
      }

      int Compare( const Attribute* arg ) const
      {
        const Toint *p = (const TPoint*) arg;
        return Compare(*p);
      }

      int Compare(const Toint p) const
      {
        
        return -1; //should never been reached
      }

      bool Adjacent( const Attribute *arg ) const
      {
        return false;
      }

      TPoint *Clone() const
      {
        return new TPoint( *this );
      }

      std::ostream& Print( std::ostream& os ) const
      {
        if(IsDefined())
        {
        os << "" << m_iNetworkId<< endl;
        }
        else
          os << "not defined." << endl;
        return os;
      }

  /*
  Functions for Secondo integration.

  */
      static ListExpr OutTPoint( ListExpr typeInfo, Word value );

      static Word InTPoint( const ListExpr typeInfo, const ListExpr instance,
                 const int errorPos, ListExpr& errorInfo, bool& correct );

      static Word CreateTPoint( const ListExpr typeInfo );

      static void DeleteTPoint( const ListExpr typeInfo, Word& w );

      static void CloseTPoint( const ListExpr typeInfo, Word& w );

      static Word CloneTPoint( const ListExpr typeInfo, const Word& w );

      static void* CastTPoint( void* addr );

      static int SizeOfTPoint();

      static bool CheckTPoint( ListExpr type, ListExpr& errorInfo );

      inline static const std::string BasicType() { return "tpoint"; }
      static const bool checkType(const ListExpr type){
        return listutils::isSymbol(type, BasicType());
      }




  /*
  Computes the euclidean distance of 2 glines.

  */
      double Distance (const TPoint* toTPoint) const ;

  /*
  Returns true if the gpoint is inside the gline false elsewhere.

  */

      //bool Inside( const GLine *gl, const double tolerance) const;

  /*
  Returns true if two gpoint are identical.

  */

      bool operator== (const GPoint& p) const;

      /*
      Returns false if two gpoint are identical.

      */

      bool operator!= (const GPoint&p) const;

      /*
      Translates a ~gpoint~ into a ~point~ in the 2D plane.

      */

      void ToPoint(Point *& res) const;

      Point* ToPoint() const;

      //Point* ToPoint() const;

  /*
  Returns the spatial Bounding Box of the point which is a rectangle degenerated
  to a single point.

  */

      inline const Rectangle<2> BoundingBox(){
        if (IsDefined()) {
          Point *p = ToPoint();
          Rectangle<2> result = p->BoundingBox();
          p->DeleteIfAllowed();
          return result;
        } else return Rectangle<2> (false, 0.0, 0.0, 0.0, 0.0);
      };



  /*
  Returns a gline representing the shortest path between two GPoint.
  Using DijkstrasAlgorithm

  */

    bool ShortestPath(const GPoint *ziel, GLine *result,
                      DbArray<TupleId>* touchedSects = 0) const;
    bool ShortestPath(const GPoint* ziel, GLine *result,
                      const Network* pNetwork,
                      DbArray<TupleId>* touchedSects = 0) const;




    private:

      string stop;

  };

}

std::ostream& operator<< (std::ostream& os, const gmo::GenericPoint& gp);


#endif
