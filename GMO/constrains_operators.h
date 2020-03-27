#include <string>
#include "SecondoCatalog.h"
#include "QueryProcessor.h"
#include "Attribute.h"
#include "AlgebraTypes.h"
#include "Operator.h"
#include "ConstructorTemplates.h"
#include "GenericTC.h"
#include "TypeMapUtils.h"
#include "ListUtils.h"
#include "Symbols.h"
#include "StandardTypes.h"
#include "GMOAlgebra.h"
#include "GenericPoint.h"
#include "IGenericPoint.h"
#include "GenericMPoint.h"
#include "MTPoint.h"
#include "TPoint.h"
#include "UTPoint.h"
#include "Thematic.h"
#include "Temp.h"
#include "QueryProcessor.h"   // needed for implementing value mappings
#include "AlgebraManager.h"   // e.g., check for certain kind
#include "../Network/NetworkAlgebra.h"
//#include "NetworkAlgebra.h"
#include "../MapMatching/MapMatchingMHT.h"
#include "../Network/NetworkManager.h"
#include "../MapMatching/NetworkAdapter.h"
#include "../MapMatching/MapMatchingMHTMGPointCreator.h"
#include <typeinfo>
#include <fstream>
#include <vector>
#include <iterator>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <boost/algorithm/string.hpp>
#include <boost/unordered_map.hpp>


using namespace std;
using namespace mappings;
using namespace gmo;
using namespace network;
using namespace temporalalgebra;
using namespace mapmatch;

extern NestedList* nl;
extern QueryProcessor* qp;

namespace gmo{


/*------------------------------------------------------------------------------

1.1.1 ~gmo_simplification~

*/

bool similar_time_interval(temporalalgebra::UPoint& a, temporalalgebra::UPoint& b, int delta_time=300*1000){
  Interval<Instant> a_interval = a.getTimeInterval();
  Interval<Instant> b_interval = b.getTimeInterval();
  return (abs(a_interval.start.millisecondsToNull() - b_interval.start.millisecondsToNull()) < delta_time) && (abs(a_interval.end.millisecondsToNull() - b_interval.end.millisecondsToNull()) < delta_time);
}

bool similar_space_interval(temporalalgebra::UPoint& a, temporalalgebra::UPoint& b, double delta_space=0.0001){
    Line a_line(true), b_line(true);
    a.UTrajectory(a_line);
    b.UTrajectory(b_line);
    CcBool result(false);
    a_line.DistanceSmallerThan(b_line, delta_space, true, result);
    return result.GetBoolval();
}

bool similar_unit(temporalalgebra::UPoint& a, temporalalgebra::UPoint& b){
  return similar_time_interval(a,b) && similar_space_interval(a,b);
}

bool intersection(temporalalgebra::MPoint* T, temporalalgebra::MPoint* T_prime, double delta_space=0.001, int delta_time=300*1000){
  for(int i=0; i<T->GetNoComponents();i++){ 
    temporalalgebra::UPoint unit_t(true);
    T->Get(i, unit_t);    
    for(int j=0;j<T_prime->GetNoComponents();j++){
      temporalalgebra::UPoint unit_t_prime(true);
      T_prime->Get(j, unit_t_prime);
      if(similar_unit(unit_t, unit_t_prime)){
        return true;
      }
    }
  }
  return false;

}

bool simplification(temporalalgebra::MPoint* T, temporalalgebra::MPoint* T_prime, double delta_space=0.001, int delta_time=300*1000){
  bool found;
  for(int i=0; i<T->GetNoComponents();i++){
    found=false; 
    temporalalgebra::UPoint unit_t(true);
    T->Get(i, unit_t);    
    for(int j=0;j<T_prime->GetNoComponents();j++){
      temporalalgebra::UPoint unit_t_prime(true);
      T_prime->Get(j, unit_t_prime);
      if(similar_unit(unit_t, unit_t_prime)){
        found = true;
      }

    }
    if(!found){
      return false;
    }
  }
  return true;
  
}

bool group(temporalalgebra::MPoint* T, temporalalgebra::MPoint* T_prime, double delta_space=0.001, int delta_time=300*1000){
  //vector<int> grouped;
  bool found;
  for(int i=0; i<T_prime->GetNoComponents();i++){
    temporalalgebra::UPoint unit_t_prime(true);
    T_prime->Get(i, unit_t_prime);  
    found = false;  
    //for(int j=grouped.size();j<T->GetNoComponents();j++){
    for(int j=0;j<T->GetNoComponents();j++){  
      temporalalgebra::UPoint unit_t(true);
      T->Get(j, unit_t);
      if(similar_unit(unit_t_prime, unit_t)){
        found = true;
      }

    }
    if(!found){
      return false;
    }
  }
  return true;
  //     if(similar_unit(unit_t_prime, unit_t)){
  //       grouped.push_back(true);
  //     }else{
  //       //means for T_prime there is not subset of T that is near 
  //       return false;
  //     }
  //   }
  // }
  //return grouped.size() == (unsigned)T->GetNoComponents();
}


bool partition(temporalalgebra::MPoint* T, temporalalgebra::MPoint* T_prime, double delta_space=0.001, int delta_time=300*1000){
  return simplification(T,T_prime) && group(T,T_prime);
}

bool similarity(temporalalgebra::MPoint* T, temporalalgebra::MPoint* T_prime, double delta_space=0.001, int delta_time=300*1000){
  return simplification(T,T_prime) && group(T,T_prime) && simplification(T_prime, T) && group(T_prime, T);
}


const string maps_simplification[1][3] =
{
  {GenericMPoint::BasicType(),GenericMPoint::BasicType(),CcBool::BasicType()},
  
};

ListExpr simplificationTM (ListExpr args)
{ 
  
   return SimpleMaps<1,3>(maps_simplification, args);

}

int simplificationSelect(ListExpr args)
{ 

   return SimpleSelect<1,3>(maps_simplification, args);
}

int simplificationVM( Word* args, Word& result, int message, Word& local,
                  Supplier s)
{
  
  result = qp->ResultStorage(s);
  //CcBool* res = static_cast<CcBool*> (result.addr);

  GenericMPoint* mpoint0 = (GenericMPoint*) args[0].addr;

  GenericMPoint* mpoint1 = (GenericMPoint*) args[1].addr;

  if(!mpoint0->IsDefined() || !mpoint1->IsDefined() || mpoint0->GetDefMTPoint() || mpoint0->GetDefMTPoint()){
    ((CcBool *)result.addr)->Set( false, false );
    return 0;    
  }

  temporalalgebra::MPoint * mp0 = new temporalalgebra::MPoint(true);
  temporalalgebra::MPoint * mp1 = new temporalalgebra::MPoint(true);
  if(mpoint0->GetDomain() == FreeSpace){
    
    *mp0 = mpoint0->GetMPoint();
  }else if(mpoint0->GetDomain() == Network){

    mpoint0->GetMGPoint2()->Mgpoint2mpoint(mp0);


  }


  if(mpoint1->GetDomain() == FreeSpace){
    
    *mp1 = mpoint1->GetMPoint();
  }else if(mpoint1->GetDomain() == Network){

    mpoint1->GetMGPoint2()->Mgpoint2mpoint(mp1);
  }

  ((CcBool *)result.addr)->Set(true, simplification(mp0, mp1)); 

  delete mp0;
  delete mp1;
  return 0;
}



ValueMapping simplificationMap[] =
{
  simplificationVM
  
};

const string simplificationSpec =
  "( ( \"Signature\" \"Syntax\" \"Meaning\" \"Example\" ) "
  "(<text><text>query creategint(sourceid,targetid)</text--->))";

Operator simplificationGMO( "gmo_simplification", simplificationSpec,1 , simplificationMap,
                         simplificationSelect, simplificationTM);




/*
1.1.1 ~gmo_group~

*/



const string maps_group[1][3] =
{
  {GenericMPoint::BasicType(),GenericMPoint::BasicType(),CcBool::BasicType()},
  
};

ListExpr groupTM (ListExpr args)
{ 
  
   return SimpleMaps<1,3>(maps_group, args);

}

int groupSelect(ListExpr args)
{ 

   return SimpleSelect<1,3>(maps_group, args);
}

int groupVM( Word* args, Word& result, int message, Word& local,
                  Supplier s)
{
  
  result = qp->ResultStorage(s);
  //CcBool* res = static_cast<CcBool*> (result.addr);

  GenericMPoint* mpoint0 = (GenericMPoint*) args[0].addr;

  GenericMPoint* mpoint1 = (GenericMPoint*) args[1].addr;

  if(!mpoint0->IsDefined() || !mpoint1->IsDefined() || mpoint0->GetDefMTPoint() || mpoint0->GetDefMTPoint()){
    ((CcBool *)result.addr)->Set( false, false );
    return 0;    
  }

  temporalalgebra::MPoint * mp0 = new temporalalgebra::MPoint(true);
  temporalalgebra::MPoint * mp1 = new temporalalgebra::MPoint(true);
  if(mpoint0->GetDomain() == FreeSpace){
    
    *mp0 = mpoint0->GetMPoint();
  }else if(mpoint0->GetDomain() == Network){

    mpoint0->GetMGPoint2()->Mgpoint2mpoint(mp0);


  }


  if(mpoint1->GetDomain() == FreeSpace){
    
    *mp1 = mpoint1->GetMPoint();
  }else if(mpoint1->GetDomain() == Network){

    mpoint1->GetMGPoint2()->Mgpoint2mpoint(mp1);
  }


  ((CcBool *)result.addr)->Set(true, group(mp0, mp1)); 

  delete mp0;
  delete mp1;
  return 0;
}



ValueMapping groupMap[] =
{
  groupVM
  
};

const string groupSpec =
  "( ( \"Signature\" \"Syntax\" \"Meaning\" \"Example\" ) "
  "(<text><text>query creategint(sourceid,targetid)</text--->))";

Operator groupGMO( "gmo_group", groupSpec,1 , groupMap,
                         groupSelect, groupTM);


/*
1.1.1 ~gmo_partition~

*/



const string maps_partition[1][3] =
{
  {GenericMPoint::BasicType(),GenericMPoint::BasicType(),CcBool::BasicType()},
  
};

ListExpr partitionTM (ListExpr args)
{ 
  
   return SimpleMaps<1,3>(maps_partition, args);

}

int partitionSelect(ListExpr args)
{ 

   return SimpleSelect<1,3>(maps_partition, args);
}

int partitionVM( Word* args, Word& result, int message, Word& local,
                  Supplier s)
{
  
  result = qp->ResultStorage(s);
  //CcBool* res = static_cast<CcBool*> (result.addr);

  GenericMPoint* mpoint0 = (GenericMPoint*) args[0].addr;

  GenericMPoint* mpoint1 = (GenericMPoint*) args[1].addr;

  if(!mpoint0->IsDefined() || !mpoint1->IsDefined() || mpoint0->GetDefMTPoint() || mpoint0->GetDefMTPoint()){
    ((CcBool *)result.addr)->Set( false, false );
    return 0;    
  }

  temporalalgebra::MPoint * mp0 = new temporalalgebra::MPoint(true);
  temporalalgebra::MPoint * mp1 = new temporalalgebra::MPoint(true);
  if(mpoint0->GetDomain() == FreeSpace){
    
    *mp0 = mpoint0->GetMPoint();
  }else if(mpoint0->GetDomain() == Network){

    mpoint0->GetMGPoint2()->Mgpoint2mpoint(mp0);


  }


  if(mpoint1->GetDomain() == FreeSpace){
    
    *mp1 = mpoint1->GetMPoint();
  }else if(mpoint1->GetDomain() == Network){

    mpoint1->GetMGPoint2()->Mgpoint2mpoint(mp1);
  }
  ((CcBool *)result.addr)->Set(true, partition(mp0, mp1)); 

  delete mp0;
  delete mp1;
  return 0;
}



ValueMapping partitionMap[] =
{
  partitionVM
  
};

const string partitionSpec =
  "( ( \"Signature\" \"Syntax\" \"Meaning\" \"Example\" ) "
  "(<text><text>query creategint(sourceid,targetid)</text--->))";

Operator partitionGMO( "gmo_partition", partitionSpec,1 , partitionMap,
                         partitionSelect, partitionTM);



/*
1.1.1 ~gmo_similarity~

*/



const string maps_similarity[1][3] =
{
  {GenericMPoint::BasicType(),GenericMPoint::BasicType(),CcBool::BasicType()},
  
};

ListExpr similarityTM (ListExpr args)
{ 
  
   return SimpleMaps<1,3>(maps_similarity, args);

}

int similaritySelect(ListExpr args)
{ 

   return SimpleSelect<1,3>(maps_similarity, args);
}

int similarityVM( Word* args, Word& result, int message, Word& local,
                  Supplier s)
{
  
  result = qp->ResultStorage(s);
  //CcBool* res = static_cast<CcBool*> (result.addr);

  GenericMPoint* mpoint0 = (GenericMPoint*) args[0].addr;

  GenericMPoint* mpoint1 = (GenericMPoint*) args[1].addr;

  if(!mpoint0->IsDefined() || !mpoint1->IsDefined() || mpoint0->GetDefMTPoint() || mpoint0->GetDefMTPoint()){
    ((CcBool *)result.addr)->Set( false, false );
    return 0;    
  }

  temporalalgebra::MPoint * mp0 = new temporalalgebra::MPoint(true);
  temporalalgebra::MPoint * mp1 = new temporalalgebra::MPoint(true);
  if(mpoint0->GetDomain() == FreeSpace){
    
    *mp0 = mpoint0->GetMPoint();
  }else if(mpoint0->GetDomain() == Network){

    mpoint0->GetMGPoint2()->Mgpoint2mpoint(mp0);


  }


  if(mpoint1->GetDomain() == FreeSpace){
    
    *mp1 = mpoint1->GetMPoint();
  }else if(mpoint1->GetDomain() == Network){

    mpoint1->GetMGPoint2()->Mgpoint2mpoint(mp1);
  }

  ((CcBool *)result.addr)->Set(true, similarity(mp0, mp1)); 
  delete mp0;
  delete mp1;
  return 0;
}



ValueMapping similarityMap[] =
{
  similarityVM
  
};

const string similaritySpec =
  "( ( \"Signature\" \"Syntax\" \"Meaning\" \"Example\" ) "
  "(<text><text>query creategint(sourceid,targetid)</text--->))";

Operator similarityGMO( "gmo_similarity", similaritySpec,1 , similarityMap,
                         similaritySelect, similarityTM);








/*
1.1.1 ~gmo_intersects~

*/



const string maps_intersects[1][3] =
{
  {GenericMPoint::BasicType(),GenericMPoint::BasicType(),CcBool::BasicType()},
  
};

ListExpr intersectsTM (ListExpr args)
{ 
  
   return SimpleMaps<1,3>(maps_intersects, args);

}

int intersectsSelect(ListExpr args)
{ 

   return SimpleSelect<1,3>(maps_intersects, args);
}

int intersectsVM( Word* args, Word& result, int message, Word& local,
                  Supplier s)
{
  
  result = qp->ResultStorage(s);
  //CcBool* res = static_cast<CcBool*> (result.addr);

  GenericMPoint* mpoint0 = (GenericMPoint*) args[0].addr;

  GenericMPoint* mpoint1 = (GenericMPoint*) args[1].addr;

  if(!mpoint0->IsDefined() || !mpoint1->IsDefined() || mpoint0->GetDefMTPoint() || mpoint0->GetDefMTPoint()){
    ((CcBool *)result.addr)->Set( false, false );
    return 0;    
  }

  temporalalgebra::MPoint * mp0 = new temporalalgebra::MPoint(true);
  temporalalgebra::MPoint * mp1 = new temporalalgebra::MPoint(true);
  if(mpoint0->GetDomain() == FreeSpace){
    
    *mp0 = mpoint0->GetMPoint();
  }else if(mpoint0->GetDomain() == Network){

    mpoint0->GetMGPoint2()->Mgpoint2mpoint(mp0);


  }


  if(mpoint1->GetDomain() == FreeSpace){
    
    *mp1 = mpoint1->GetMPoint();
  }else if(mpoint1->GetDomain() == Network){

    mpoint1->GetMGPoint2()->Mgpoint2mpoint(mp1);
  }




  ((CcBool *)result.addr)->Set(true, intersection(mp0, mp1)); 
  delete mp0;
  delete mp1;
  return 0;
  
}



ValueMapping intersectsMap[] =
{
  intersectsVM
  
};

const string intersectsSpec =
  "( ( \"Signature\" \"Syntax\" \"Meaning\" \"Example\" ) "
  "(<text><text>query creategint(sourceid,targetid)</text--->))";

Operator intersectsGMO( "gmo_intersects", intersectsSpec,1 , intersectsMap,
                         intersectsSelect, intersectsTM);


/*
1.1.1 ~gmo_similarity~

*/

// bool Similarity(Line& PLine, Line& ALine){

//   double delta = 0.000001;
//   double minDist;
//   int aux = 0;

//   assert(PLine.IsDefined()); 
//   assert(ALine.IsDefined());
//   if(PLine.IsEmpty() || ALine.IsEmpty())
//     return false;

//   assert(PLine.IsOrdered());
//   assert(ALine.IsOrdered());
  
//   Point p1, p2;
//   Points *avert = new Points(true);
//   Points *pvert = new Points(true);
//   vector<Point> S;

//   ALine.Vertices(avert);
//   PLine.Vertices(pvert);

//   avert->Print(cout);
//   pvert->Print(cout);
//   for(int i = 0; i < avert->Size(); i++){
//     avert->Get(i, p1);
//     for(int j = aux; j < pvert->Size(); j++){
//       pvert->Get(j, p2);
//       minDist = p1.Distance(p2); //distancia de punto a punto
//       cout<<"min Dist ";
//       p1.Print(cout);
//       cout<<"-";
//       p2.Print(cout);
//       cout<<" "<<minDist<<endl;
//       if(minDist >= 0.0){
//         if(minDist <= delta){
//           S.push_back(p2);
//         }else{
//           if(j == 0){
//             return false;
//           }else{
//             aux = j; 
//             break;
//           }
//         }
//       }
//     }
//   }
//   return false;
// } 


// const string maps_similarity[1][3] =
// {
//   {GenericMPoint::BasicType(),GenericMPoint::BasicType(),CcBool::BasicType()},
  
// };

// ListExpr similarityTM (ListExpr args)
// { 
  
//    return SimpleMaps<1,3>(maps_similarity, args);

// }

// int similaritySelect(ListExpr args)
// { 

//    return SimpleSelect<1,3>(maps_similarity, args);
// }

// int similarityVM( Word* args, Word& result, int message, Word& local,
//                   Supplier s)
// {
  
//   result = qp->ResultStorage(s);
//   //CcBool* res = static_cast<CcBool*> (result.addr);

//   GenericMPoint* mpoint0 = (GenericMPoint*) args[0].addr;

//   GenericMPoint* mpoint1 = (GenericMPoint*) args[1].addr;

  
//   if(!mpoint0->IsDefined() || !mpoint1->IsDefined() || mpoint0->GetDefMTPoint() || mpoint0->GetDefMTPoint()){
//     ((CcBool *)result.addr)->Set( false, false );
//     return 0;    
//   }

//   temporalalgebra::MPoint * mp0 = new temporalalgebra::MPoint(true);
//   temporalalgebra::MPoint * mp1 = new temporalalgebra::MPoint(true);
//   if(mpoint0->GetDomain() == FreeSpace){
    
//     *mp0 = mpoint0->GetMPoint();
//     mp0->Print(cout);
//     cout<<endl;
//   }else if(mpoint0->GetDomain() == Network){

//     mpoint0->GetMGPoint2()->Mgpoint2mpoint(mp0);


//   }


//   if(mpoint1->GetDomain() == FreeSpace){
    
//     *mp1 = mpoint1->GetMPoint();
//     mp1->Print(cout);
//     cout<<endl;
//   }else if(mpoint1->GetDomain() == Network){

//     mpoint1->GetMGPoint2()->Mgpoint2mpoint(mp1);
//   }

//   Line traj0(0);
//   Line traj1(0);

//   mp0->Trajectory(traj0);
//   mp1->Trajectory(traj1);

//   //traj0.Print(cout);
//   //traj1.Print(cout);

//   Line line(0) ;
//   mpoint0->GetMPoint().Trajectory(line);
//   line.Print(cout);
//   cout<<endl;

//   cout<<traj0.NoComponents()<<' '<<traj1.NoComponents()<<endl;
//   cout<<"Similarity"<<Similarity(traj0, traj1)<<endl;
//   ((CcBool *)result.addr)->Set(true, Similarity(traj0, traj1)); 

//   return 0;
// }



// ValueMapping similarityMap[] =
// {
//   similarityVM
  
// };

// const string similaritySpec =
//   "( ( \"Signature\" \"Syntax\" \"Meaning\" \"Example\" ) "
//   "(<text><text>query creategint(sourceid,targetid)</text--->))";

// Operator similarityGMO( "gmo_similarity", similaritySpec,1 , similarityMap,
//                          similaritySelect, similarityTM);



}