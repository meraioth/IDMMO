#include <ostream>
#include <string>
#include "../Tools/Flob/DbArray.h"
#include "Attribute.h"
#include "StandardTypes.h"
#include "DateTime.h"
#include "TemporalAlgebra.h"
#include "UJPoint.h"
#include "JNetwork.h"
#include "JLine.h"



using namespace std;


namespace sgraph{


class MO : public Attribute
{
	public:

		explicit MO(const bool defined);

		explicit MO(const MO& other);
		~MO();


/* TODO
1.1.1 Getter and Setter for private Attributes

*/

		/*
1.1.1 Override Methods from Attribute

*/

  void CopyFrom(const Attribute* right);
  StorageType GetStorageType() const;
  size_t HashValue() const;
  MO* Clone() const;
  bool Adjacent(const Attribute* attrib) const;
  static int Compare(const void* ls, const void* rs);
  int Compare(const Attribute* rhs) const;
  int Compare(const MO& rhs) const;
  size_t Sizeof() const;
  int NumOfFLOBs() const;
  Flob* GetFLOB(const int i);
  void Destroy();
  void Clear();
  std::ostream& Print(std::ostream& os) const;
  static const std::string BasicType();
  static const bool checkType(const ListExpr type);

/*
1.1.1 Standard Operators

*/

  MO& operator=(const MO& other);

  bool operator==(const MO& other) const;
  bool operator!=(const MO& other) const;
  bool operator<(const MO& other) const;
  bool operator<=(const MO& other) const;
  bool operator>(const MO& other) const;
  bool operator>=(const MO& other) const;

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

		
	private:
		DbArray<SUnit> units;

		/*TODO add attributes*/



};
} 

std::ostream& operator<< (std::ostream& os, const sgraph::MO& mo);

#endif
