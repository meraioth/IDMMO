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

#ifndef SUNIT_H
#define SUNIT_H

#include <ostream>
#include <string>
#include "Attribute.h"
#include "StandardTypes.h"
#include "DateTime.h"
#include "TemporalAlgebra.h"
//#include "JRouteInterval.h"
//#include "RouteLocation.h"
//#include "IJPoint.h"

namespace sgraph{

class SGraph;

/*
Un ~SUnit~ consiste en un ~GraphInterval~ y en un intervalo de tiempo. 
Representa las posiciones de un MO en el grafo de paraderos en este intervalo de tiempo
*/
class SUnit : public Attribute
{

/*
1.1 Public Declarations
*/
public:
/*
1.1.1 Constructos and Deconstructors
*/
	SUnit();
	explicit SUnit(const bool def);
	SUnit(const SUnit& other);
	SUnit(const temporalalgebra::Interval<Instant>& inst, const GInterval& gint);

	~SUnit();


	/* TODO:
1.1.1 Getter and Setter for private Attributes


*/

	/*
1.1.1 Override Methods from Attribute

*/

  void CopyFrom(const Attribute* right);
  StorageType GetStorageType() const;
  size_t HashValue() const;
  SUnit* Clone() const;
  bool Adjacent(const Attribute* attrib) const;
  static int Compare(const void* ls, const void* rs);
  int Compare(const Attribute* rhs) const;
  int Compare(const SUnit& rhs) const;
  size_t Sizeof() const;
  std::ostream& Print(std::ostream& os) const;
  static const std::string BasicType();
  static const bool checkType(const ListExpr type);

/*
1.1.1 Standard Operators

*/

  JUnit& operator=(const SUnit& other);

  bool operator==(const SUnit& other) const;
  bool operator!=(const SUnit& other) const;
  bool operator<(const SUnit& other) const;
  bool operator<=(const SUnit& other) const;
  bool operator>(const SUnit& other) const;
  bool operator>=(const SUnit& other) const;

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
1.1.1 Other Operations

*/
  static std::string Example();

private: 
	temporalalgera::Interval<Instant> timeInterval;

	GInterval gInterval;

};


std::ostream& operator<< (std::ostream& os, const sgraph::SUnit& su);

#endif // SUNIT_H
