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
#include "TPoint.h"
#include "../Network/NetworkAlgebra.h"
#include "../Spatial/SpatialAlgebra.h"

using namespace network;

namespace gmo{

	class GenericPoint : public Attribute{
		public  :
			GenericPoint();
			explicit GenericPoint(const bool defined);
			GenericPoint(const Point& point);
			GenericPoint(const GPoint& gpoint);
			GenericPoint(const TPoint& tpoint);
			GenericPoint(const GenericPoint& gP);
			~GenericPoint();

			/*
			1.1 Getters and setters

			*/
			Domain GetDomain() const;
			std::string GetStrDomain() const;
			Point GetPoint() const;
			GPoint GetGPoint() const;
			TPoint GetTPoint() const;
			bool GetDefPoint() const;
			bool GetDefGPoint() const;
			bool GetDefTPoint() const;
			//Spatial_Structure GetSpatialStructure() const;

			void CopyFrom(const Attribute* right);
			StorageType GetStorageType() const;
			size_t HashValue() const;
			GenericPoint* Clone() const;
			bool Adjacent(const Attribute* attrib) const;
			bool Adjacent(const GenericPoint& attrib) const;
			static int Compare(const void* ls, const void* rs);
			int Compare(const Attribute* rhs) const;
			int Compare(const GenericPoint& rhs) const;
			size_t Sizeof() const;
			std::ostream& Print(std::ostream& os) const;
			static const std::string BasicType();
			static const bool checkType(const ListExpr type);


			/*
			1.1.1 Standard Operators

			*/

			GenericPoint& operator=(const GenericPoint& other);

			bool operator==(const GenericPoint& other) const;
			bool operator!=(const GenericPoint& other) const;
			bool operator<(const GenericPoint& other) const;
			bool operator<=(const GenericPoint& other) const;
			bool operator>(const GenericPoint& other) const;
			bool operator>=(const GenericPoint& other) const;

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
			Point point;
			GPoint gpoint;
			TPoint tpoint;
			bool def_point,def_gpoint,def_tpoint;
			Domain domain;
	};


}

std::ostream& operator<< (std::ostream& os, const gmo::GenericPoint& gp);


#endif