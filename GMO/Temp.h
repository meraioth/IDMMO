#ifndef Temp_H
#define Temp_H

#include <iostream>
#include <string>
#include "../Tools/Flob/DbArray.h"
#include "Attribute.h"
#include "StandardTypes.h"
#include "DateTime.h"
#include "Domain.h"
#include "MTPoint.h"
#include "../Network/NetworkAlgebra.h"
#include "../TemporalNet/TemporalNetAlgebra.h"
#include "../Spatial/SpatialAlgebra.h"

using namespace network;
using namespace temporalalgebra;
using namespace temporalnet;




namespace gmo{

	class Temp : public Attribute{
		public  :
			Temp();
			explicit Temp(const bool defined);
			Temp(const MPoint& _mpoint);
			Temp(const Temp& gP);
			~Temp();

			/*
			1.1 Getters and setters

			*/
			Domain GetDomain() const;
			std::string GetStrDomain() const;
			MPoint GetMPoint() const;
			bool GetDefMPoint() const;

			void CopyFrom(const Attribute* right);
			StorageType GetStorageType() const;
			size_t HashValue() const;
			Temp* Clone() const;
			bool Adjacent(const Attribute* attrib) const;
			bool Adjacent(const Temp& attrib) const;
			static int Compare(const void* ls, const void* rs);
			int Compare(const Attribute* rhs) const;
			int Compare(const Temp& rhs) const;
			size_t Sizeof() const;
			std::ostream& Print(std::ostream& os) const;
			static const std::string BasicType();
			static const bool checkType(const ListExpr type);


			/*
			1.1.1 Standard Operators

			*/

			Temp& operator=(const Temp& other);

			bool operator==(const Temp& other) const;
			bool operator!=(const Temp& other) const;
			bool operator<(const Temp& other) const;
			bool operator<=(const Temp& other) const;
			bool operator>(const Temp& other) const;
			bool operator>=(const Temp& other) const;

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


			*/

			static std::string Example();

		private :
			MPoint mtpoint;
			bool def_mtpoint;
			Domain domain;
	};


}

std::ostream& operator<< (std::ostream& os, const gmo::Temp& gp);


#endif