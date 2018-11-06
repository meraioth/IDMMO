#ifndef IGenericPoint_H
#define IGenericPoint_H

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
using namespace temporalalgebra;



typedef Intime<GPoint> IGPoint;
typedef Intime<Point> IPoint;


namespace gmo{

	class IGenericPoint : public Attribute{
		public  :
			IGenericPoint();
			explicit IGenericPoint(const bool defined);
			IGenericPoint(const Instant& _instant , const Point _point);
			IGenericPoint(const Instant& _instant , const GPoint _gpoint);
			IGenericPoint(const IPoint& _ipoint);
			IGenericPoint(const IGPoint& _ipoint);
			IGenericPoint(const IGenericPoint& gP);
			~IGenericPoint();

			/*
			1.1 Getters and setters

			*/
			Domain GetDomain() const;
			std::string GetStrDomain() const;
			IPoint GetIPoint() const;
			IGPoint GetIGPoint() const;
			bool GetDefIPoint() const;
			bool GetDefIGPoint() const;
			//Spatial_Structure GetSpatialStructure() const;

			void CopyFrom(const Attribute* right);
			StorageType GetStorageType() const;
			size_t HashValue() const;
			IGenericPoint* Clone() const;
			bool Adjacent(const Attribute* attrib) const;
			bool Adjacent(const IGenericPoint& attrib) const;
			static int Compare(const void* ls, const void* rs);
			int Compare(const Attribute* rhs) const;
			int Compare(const IGenericPoint& rhs) const;
			size_t Sizeof() const;
			std::ostream& Print(std::ostream& os) const;
			static const std::string BasicType();
			static const bool checkType(const ListExpr type);


			/*
			1.1.1 Standard Operators

			*/

			IGenericPoint& operator=(const IGenericPoint& other);

			bool operator==(const IGenericPoint& other) const;
			bool operator!=(const IGenericPoint& other) const;
			bool operator<(const IGenericPoint& other) const;
			bool operator<=(const IGenericPoint& other) const;
			bool operator>(const IGenericPoint& other) const;
			bool operator>=(const IGenericPoint& other) const;

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
			1.1.1 Helpful Operators

			1.1.1.1 ~Example~

			Provides example string for list representation can be used by external
			property definitions for part of ~JRouteInterval~ representation.

			*/

			static std::string Example();

		private :
			IPoint ipoint;
			IGPoint igpoint;
			bool def_ipoint,def_igpoint;
			Domain domain;
	};


}

std::ostream& operator<< (std::ostream& os, const gmo::IGenericPoint& gp);


#endif