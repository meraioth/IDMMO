#ifndef UTPOINT_H
#define UTPOINT_H

#include <iostream>
#include <string>
#include "../Tools/Flob/DbArray.h"
#include "Attribute.h"
#include "StandardTypes.h"
#include "DateTime.h"
#include "Domain.h"
#include "TemporalAlgebra.h"
#include "TPoint.h"
#include "../Network/NetworkAlgebra.h"
#include "../Spatial/SpatialAlgebra.h"

using namespace network;

namespace gmo{

	class UTPoint :  public temporalalgebra::SpatialTemporalUnit<gmo::TPoint, 3>{
		public  :
			UTPoint();
			explicit UTPoint(const bool defined);
			UTPoint(const temporalalgebra::Interval<Instant>& interval, TPoint& tp_begin, TPoint& tp_end);
			UTPoint(const temporalalgebra::Interval<Instant>& interval, string tp_start, string tp_end);
			UTPoint(const UTPoint& gP);
			~UTPoint();

			/*
			1.1 Getters and setters

			*/

			TPoint GetStart() const;
			TPoint GetEnd() const;

			//Spatial_Structure GetSpatialStructure() const;

			void CopyFrom(const Attribute* right);
			Attribute::StorageType GetStorageType() const;
			size_t HashValue() const;
			inline virtual UTPoint* Clone() const
			  {
			    return new UTPoint(*this);
			  }

			bool Adjacent(const Attribute* attrib) const;
			bool Adjacent(const UTPoint& attrib) const;
			static int Compare(const void* ls, const void* rs);
			int Compare(const Attribute* rhs) const;
			int Compare(const UTPoint& rhs) const;
			size_t Sizeof() const;
			std::ostream& Print(std::ostream& os) const;
			static const std::string BasicType();
			static const bool checkType(const ListExpr type);


			/*
			1.1.1 Standard Operators

			*/

			UTPoint& operator=(const UTPoint& other);

			bool operator==(const UTPoint& other) const;
			bool operator!=(const UTPoint& other) const;
			bool operator<(const UTPoint& other) const;
			bool operator<=(const UTPoint& other) const;
			bool operator>(const UTPoint& other) const;
			bool operator>=(const UTPoint& other) const;

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
			1.1.1 SpatialTemporal Operators*/

			virtual const Rectangle<3> BoundingBox(const Geoid* geoid = 0) const;

 		
 			virtual double Distance(const Rectangle<3>& rect,const Geoid* geoid = 0) const;

 			virtual bool IsEmpty() const{ return IsDefined(); }


 			 /*
			  Computes the network position of the ~ugpoint~ at a given time instant ~t~.

			  */
			  virtual void TemporalFunction( const Instant& t,
			                                 TPoint& result,
			                                 bool ignoreLimits = false ) const;

			  /*
			  Returns true if the ~ugpoint~ passes a given ~gpoint~ false elsewhere.

			  */
			  virtual bool Passes( const TPoint& val )const ;

			  /*
			  Returns the ~igpoint~ the ~ugpoint~ was at a given network position.

			  */
			  virtual bool At( const TPoint& val, 
			                   TemporalUnit<TPoint>& result ) const;

			  /*
			  Returns the length of the route passed within the ~ugpoint~

			  */
			  inline double Length() const
			  {
			    return 0.0f;	
			  };


			/*1.1.1.1 ~Example~

			Provides example string for list representation can be used by external
			property definitions for part of ~JRouteInterval~ representation.

			*/

			static std::string Example();

		private :
			TPoint tp_start,tp_end;
	};


}

std::ostream& operator<< (std::ostream& os, const gmo::UTPoint& up);


#endif