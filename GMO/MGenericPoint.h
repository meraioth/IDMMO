#ifndef GENERICPOINT_H
#define GENERICPOINT_H

#include <ostream>
#include <string>
#include "../Tools/Flob/DbArray.h"
#include "Attribute.h"
#include "StandardTypes.h"
#include "DateTime.h"
#include "TemporalAlgebra.h"


namespace gmo{



	template <class Spatio_Temporal>
	class GenericMPoint : public Attribute{
		public:
			GenericMPoint();
		private:
			Spatio_Temporal ST;


		/*
		1.1 Public Declarations
		1.1.1 Constructors and Deconstructors
		*/

		explicit GenericMPoint(const bool def);
		explicit GenericMPoint(const Spatio_Temporal& other);
		

		~GenericMPoint();

		/*
		1.1.1 Getter and Setter for private Attributes

		*/

		
		const DbArray<Spatio_Temporal>& GetUnits() const;
		double Length() const;

		

		Rectangle<3> BoundingBox() const;


		/*
		1.1.1 Override Methods from Attribute

		*/

		void CopyFrom(const Attribute* right);
		StorageType GetStorageType() const;
		size_t HashValue() const;
		GenericMPoint* Clone() const;
		bool Adjacent(const Attribute* attrib) const;
		static int Compare(const void* ls, const void* rs);
		int Compare(const Attribute* rhs) const;
		int Compare(const GenericMPoint& rhs) const;
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

		GenericMPoint& operator=(const GenericMPoint& other);

		bool operator==(const GenericMPoint& other) const;
		bool operator!=(const GenericMPoint& other) const;
		bool operator<(const GenericMPoint& other) const;
		bool operator<=(const GenericMPoint& other) const;
		bool operator>(const GenericMPoint& other) const;
		bool operator>=(const GenericMPoint& other) const;

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
		1.1.1 Manage Bullkload

		*/

		void StartBulkload();
		void EndBulkload(const bool simplify = true,
		const bool buildtrajectory = true);
		//GenericMPoint& Add(const Spatio_Temporal& up);

		/*
		1.1.1 Append

		Appends the units of the in MJPoint to the current mjpoint.

		*/

		void Append(const GenericMPoint* in);

		/*
		1.1.1 Other Operations

		1.1.1.1 Example

		Returns an example mjpoint listrepresentation as string.

		*/

		static std::string Example();

		/*
		1.1.1.1 GetNoComponents

		Returns the number of ~junit~s of the ~mjpoint~.

		*/

		int GetNoComponents() const;

		/*
		1.1.1.1 IsEmpty

		Returns false if the ~mjpoint~ has at least one defined junit, false elsewhere.

		*/

		bool IsEmpty() const;

		/*
		1.1.1.1 Get

		Returns the ~GenericPoint~ at the given position.

		*/

		// void Get(const int i, JUnit& up) const;
		// void Get(const int i, JUnit* up) const;
		// void Get(const int i, UJPoint& up) const;

		/*
		1.1.1.1 FromSpatial

		Returns the corresponding ~mjpoint~ in ~jnet~ for the given ~mpoint~.

		*/
		//void FromSpatial(JNetwork* jnet, const temporalalgebra::MPoint* in);

		/*
		1.1.1.1 ToSpatial

		Returns the corresponding ~mpoint~ for this ~mjpoint~.

		*/

		//void ToSpatial(temporalalgebra::MPoint& result) const;

		/*
		1.1.1.1 Union

		Computes the union of two ~mjpoint~ if possible otherwise the resulting
		~mjpoint~ is undefined.

		*/

		//void Union(const MJPoint* other, MJPoint* result) const;

		/*
		1.1.1.1 ~AtInstant~

		Returns an ijpoint telling the position of the mjpoint at the given time
		instant.

		*/

		//IJPoint AtInstant(const Instant* time) const;

		/*
		1.1.1.1 ~Present~

		Returns true if the ~mjpoint~ is defined at least once in the given ~periods~.

		*/

		//bool Present(const temporalalgebra::Periods* per) const;
		//bool Present(const Instant* inst) const;

		/*
		1.1.1.1 ~AtPeriods~

		The result value restricts the mjpoint to the given periods of time.

		*/

		//void AtPeriods(const temporalalgebra::Periods* times, 
		//MJPoint& result) const;


		/*
		1.1.1.1 ~Initial~

		Returns an ~ijpoint~ with the start position and time of the given ~mjpoint~.

		*/

		// IJPoint Initial() const;

		/*
		1.1.1.1 ~Final~

		Returns an ~ijpoint~ with the end position and time of the given ~mjpoint~.

		*/
		// IJPoint Final() const;

		/*
		1.1.1.1 ~Passes~

		Returns true if the mjpoint passes at least once the given place.

		*/

		// bool Passes(const JPoint* jp) const;
		// bool Passes(const JLine* jl) const;

		/*
		1.1.1.1 ~At~

		Returns an ~mjpoint~ restricted to the times the original was at the given
		places.

		*/

		// void At(const JPoint* jp, MJPoint& result) const;
		// void At(const JLine* jl, MJPoint& result) const;

		/*
		1.1.1.1 ~Intersects~

		Returns true if the ~mjpoint~ are at least once at the same place at the same
		time, false elsewhere.

		*/

		// bool Intersects(const MJPoint* other) const;

		private:

		/*
		1.1 Private Declarations

		1.1.1 Attributes

		*/

		// STRING_T nid;         //network identifier
		// DbArray<JUnit> units; //set of JUnit describing the way of the mjpoint
		// DbArray<JRouteInterval> trajectory; //sorted list of all jrint passed
		//           //by the mjpoint
		bool activBulkload;   //only true while bulkload of ujpoints runs
		double lenth;         //total driven distance

		/*
		1.1.1 Standard Constructor

		The standard constructor should only be used in cast-Function and is therefore
		declared to be private.

		*/

		GenericPoint();

		/*
		1.1.1 Methods

		1.1.1.1 ~CheckSorted~

		Checks if the units are well sorted. Used if complete Arrays are inserted.

		*/

		bool CheckSorted() const;

		/*
		1.1.1.1 ~Simplify~

		Checks if the units are sorted and compresses the units as far as possible.

		*/

		bool Simplify();

		/*
		1.1.1.1 Starttime

		Returns the time instant the MJPoint starts, if the mjpoint is defined and
		not empty. 0 elsewhere.

		*/

		// Instant* Starttime() const;

		/*
		1.1.1.1 Endtime

		Returns the time instant the MJPoint ends, if the mjpoint is defined and
		not empty. 0 elsewhere.

		*/

		// Instant* Endtime() const;

		/*
		1.1.1.1 GetUnitPosForTime

		Searches binary for the position of the JUnit including the start of the
		given time interval.

		*/

		// int GetUnitPosForTime(const temporalalgebra::Interval<Instant>& time,
		// const int spos, const int epos) const;
		// int GetUnitPosForTime(const Instant& time,
		// const int spos, const int epos) const;

		/*
		1.1.1.1 ~findFirstUnit~

		Searches for first unit of mjp with biggest time Interval before start of
		actTimeInterval.

		*/

		// void FindFirstUnit(const temporalalgebra::Interval<Instant>& actTimeInterval,
		// int& unitIndex, JUnit& actUnit) const;

		/*
		1.1.1.1 Refinement

		Converts the two given ~mjpoint~ into two ~mjpoint~ with identic time intervals.
		The result might be empty if the two in ~mjpoint~ does not have any common
		deftime.

		*/

		// void Refinement(const MJPoint* in2, MJPoint* out1, MJPoint* out2) const;

		/*
		1.1.1.1 Append

		Appends the junit to the current mjpoint. Declared as private because it should
		only be used if it is ensured that ju is part of the jnetwork of mjpoint. Used
		by private Method Refinement.

		*/

		// void Append(const JUnit ju);

};
	}






}


#endif