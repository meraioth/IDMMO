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


namespace gmo{

	template <class Spatial_Structure>
	class GenericPoint : public Attribute{
		public  :
			GenericPoint();
			explicit GenericPoint(const bool defined);
			GenericPoint(const Spatial_Structure& sS,const  Domain& domain);
			GenericPoint(const GenericPoint<Spatial_Structure>& gP);
			~GenericPoint();

			/*
			1.1 Getters and setters

			*/
			Domain GetDomain() const;
			std::string GetStrDomain() const;
			Spatial_Structure GetSpatialStructure() const;

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
			Spatial_Structure sS;
			Domain domain;
	};


}


#endif