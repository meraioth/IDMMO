/*
This file is part of SECONDO.

Copyright (C) 2011, University in Hagen, Department of Computer Science,
Database Systems for New Applications.

SECONDO is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

SECONDO is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with SECONDO; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

2018, August Meraioth Ulloa

1 Defines and Includes

*/

#ifndef GINTERVAL_H
#define GINTERVAL_H

#include <ostream>
#include <string>
#include "Attribute.h"
#include "StandardTypes.h"


namespace sgraph{

class SGraph;

/*Un ~GInterval~ es como un arco del grafo de paraderos, contiene dos identificadores de paraderos, 
uno de salida, y uno de llegada, ordenados por el sentido del viaje */

class GInterval: public Attribute
{
	public:
		GInterval();
		explicit GInterval(const bool defined);
		GInterval(const GInterval& other);
		GInterval(const int sourceid,const int targetid);
		~GInterval();

		/*TODO: rellenar con getters y setters*/
		/*1.1.1 Getter and Setter for private Attributes
		*/
		int GetSourceId() const;

		int GetTargetId() const;





	/*
	1.1.1 Override Methods from Attribute

	*/

	  void CopyFrom(const Attribute* right);
	  StorageType GetStorageType() const;
	  size_t HashValue() const;
	  GInterval* Clone() const;
	  bool Adjacent(const Attribute* attrib) const;
	  static int Compare(const void* ls, const void* rs);
	  int Compare(const Attribute* rhs) const;
	  int Compare(const GInterval& rhs) const;
	  size_t Sizeof() const;
	  std::ostream& Print(std::ostream& os) const;
	  static const std::string BasicType();
	  static const bool checkType(const ListExpr type);


	  /*
	1.1.1 Standard Operators

	*/

	  JUnit& operator=(const JUnit& other);

	  bool operator==(const JUnit& other) const;
	  bool operator!=(const JUnit& other) const;
	  bool operator<(const JUnit& other) const;
	  bool operator<=(const JUnit& other) const;
	  bool operator>(const JUnit& other) const;
	  bool operator>=(const JUnit& other) const;

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
	  static bool KindCheck ( ListExpr type, ListExpr& errorInfo );
	  static int SizeOf();
	  static ListExpr Property();

	  /*
		1.1.1 Helpful Operators

		1.1.1.1 ~Example~

		Provides example string for list representation can be used by external
		property definitions for part of ~JRouteInterval~ representation.

		*/

		static std::string Example();

	private:
		int source,target;
};


}

 // end of namespace sgraph

/*
1 Overwrite output operator

*/

std::ostream& operator<< (std::ostream& os, const sgraph::GInterval& gi);

#endif // GINTERVAL_H