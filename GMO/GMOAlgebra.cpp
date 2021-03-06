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
#include "constrains_operators.h"


using namespace std;
using namespace mappings;
using namespace gmo;
using namespace network;
using namespace temporalalgebra;
using namespace mapmatch;

extern NestedList* nl;
extern QueryProcessor* qp;

namespace gmo{

// static map<int,string> *netList;


/*
 * A class to read data from a csv file.
 */
class CSVReader
{
  std::string fileName;
  std::string delimeter;
  bool skip_first;
  public:
    CSVReader(std::string filename, std::string delm = ",", bool skip_first_ = true) :
        fileName(filename), delimeter(delm), skip_first(skip_first_)
    { }
   
    // Function to fetch data from a CSV File
    std::vector<std::vector<std::string> > getData();
};
 
/*
* Parses through csv file line by line and returns the data
* in vector of vector of strings.
*/
std::vector<std::vector<std::string> > CSVReader::getData()
{
  std::ifstream file(fileName.c_str());
 
  std::vector<std::vector<std::string> > dataList;
 
  std::string line = "";

  if(!skip_first){ // Case when firt row has values
    getline(file, line);
    std::vector<std::string> vec;
    boost::algorithm::split(vec, line, boost::is_any_of(delimeter));
    dataList.push_back(vec);  
  }else{
    getline(file, line);
  }
  // Iterate through each line and split the content using delimeter
  while (getline(file, line))
  {
    std::vector<std::string> vec;
    boost::algorithm::split(vec, line, boost::is_any_of(delimeter));
    dataList.push_back(vec);
  }
  // Close the File
  file.close();
 
  return dataList;
}

ListExpr
MTPointProperty()
{
  return (nl->TwoElemList(
            nl->FourElemList(nl->StringAtom("Signature"),
                             nl->StringAtom("Example Type List"),
                             nl->StringAtom("List Rep"),
                             nl->StringAtom("Example List")),
            nl->FourElemList(nl->StringAtom("-> MAPPING"),
                             nl->StringAtom("(mtpoint) "),
                             nl->StringAtom("( u1 ... un ) "),
        nl->StringAtom("(((i1 i2 TRUE FALSE) (\"PF13\" \"PF14\")) ...)"))));
};




MGPoint* MPoint2MGPoint(GenericMPoint* point, network::Network * pNetwork){

    MGPoint* res = new MGPoint(true);
    res->Clear();
    
    temporalalgebra::MPoint *pMPoint = new temporalalgebra::MPoint(point->GetMPoint());
    CcReal *pNetworkScale = new CcReal(1.0);

    // Do Map Matching

    mapmatch::NetworkAdapter Network(pNetwork, pNetworkScale->GetValue());
    mapmatch::MapMatchingMHT MapMatching(&Network, pMPoint);

    mapmatch::MGPointCreator Creator(&Network, res);

    if (!MapMatching.DoMatch(&Creator))
    {
        // Error
    }
    return res;
    
}

temporalalgebra::MPoint MTPoint2MPoint(GenericMPoint* point,string map_function_str){
  CSVReader reader(map_function_str,",");
     
  // Get the data from CSV File
  std::vector<std::vector<std::string> > dataList = reader.getData();

  /*if (dataList.size()==0){ //Case when path of file is incorrect or stops file is empty
    res->SetDefined(false);
    return 0;
  }*/


  boost::unordered_map<std::string,double> map_latitude;
  boost::unordered_map<std::string,double> map_longitude;
  
  for (unsigned int i = 0; i < dataList.size(); ++i)
  {
    char* aux;
    char* aux_2;
    
    map_latitude[dataList[i][0]]= std::strtod(dataList[i][3].c_str(),&aux); //acording to GTFS stops file
    map_longitude[dataList[i][0]]= std::strtod(dataList[i][4].c_str(),&aux_2);
  }

  temporalalgebra::MPoint newpoint(point->GetMTPoint().GetNoComponents());

  if(point->GetDefMTPoint()){

    for(int i=0; i<point->GetMTPoint().GetNoComponents(); i++)
    {
      UTPoint unit;
      point->GetMTPoint().Get( i , unit );
      Point p0(true,map_longitude[unit.GetStart().GetStop()],map_latitude[unit.GetStart().GetStop()]);
      Point p1(true,map_longitude[unit.GetEnd().GetStop()],map_latitude[unit.GetEnd().GetStop()]);
      UPoint upoint(unit.getTimeInterval(),p0,p1);
      newpoint.Add(upoint);
      
    }
  }
  return newpoint;
}


double DistancePoint(GenericPoint* point0, GenericPoint* point1){

  switch(point0->GetDomain()){

    case FreeSpace:{
      Geoid* geoid = new Geoid(Geoid::WGS1984);
      double dist = -1.0;
      bool valid = true;
      Point  p1 =  point1->GetPoint();
      dist = point0->GetPoint().DistanceOrthodrome(p1,*geoid,valid);
      return dist;
      break;
    }
    case Network:{

      GPoint* arg1_= new GPoint(point0->GetGPoint());
      GPoint* arg2_= new GPoint(point1->GetGPoint());
      double dist_ = -1.0;
      
       dist_ = arg1_->NetdistanceNew(arg2_);
       return dist_;
      
     break;
   }
    default:
      return -1.0;
    }


   return -1.0;
}

bool
CheckMTPoint( ListExpr type, ListExpr& errorInfo )
{
  return (nl->IsEqual( type, MTPoint::BasicType() ));
}

TypeConstructor tpath(
        TPath::BasicType(),               //name
        TPath::Property,                  //property function
        TPath::Out,   TPath::In,        //Out and In functions
        0,              0,                  //SaveTo and RestoreFrom functions
        TPath::Create,  TPath::Delete,  //object creation and deletion
        TPath::Open,    TPath::Save,    //object open and save
        TPath::Close,   TPath::Clone,   //object close and clone
        TPath::Cast,                      //cast function
        TPath::SizeOfObj,                 //sizeof function
        TPath::KindCheck );               //kind checking function


TypeConstructor tempoTC(
        Temp::BasicType(),               //name
        Temp::Property,                  //property function
        Temp::Out,   Temp::In,        //Out and In functions
        0,              0,                  //SaveTo and RestoreFrom functions
        Temp::Create,  Temp::Delete,  //object creation and deletion
        OpenAttribute<Temp >,
  SaveAttribute<Temp >,
        Temp::Close,   Temp::Clone,   //object close and clone
        Temp::Cast,                      //cast function
        Temp::SizeOf,                 //sizeof function
        Temp::KindCheck);               //kind checking function

TypeConstructor gpointTC(
  GenericPoint::BasicType(),
  GenericPoint::Property,
  GenericPoint::Out, GenericPoint::In,
  0, 0,
  GenericPoint::Create, GenericPoint::Delete,
  OpenAttribute<GenericPoint >,
  SaveAttribute<GenericPoint >,
  GenericPoint::Close, GenericPoint::Clone,
  GenericPoint::Cast,
  GenericPoint::SizeOf,
  GenericPoint::KindCheck);
TypeConstructor igpointTC(
  IGenericPoint::BasicType(),
  IGenericPoint::Property,
  IGenericPoint::Out, IGenericPoint::In,
  0, 0,
  IGenericPoint::Create, IGenericPoint::Delete,
  OpenAttribute<IGenericPoint >,
  SaveAttribute<IGenericPoint >,
  IGenericPoint::Close, IGenericPoint::Clone,
  IGenericPoint::Cast,
  IGenericPoint::SizeOf,
  IGenericPoint::KindCheck);
TypeConstructor gmpointTC(
  GenericMPoint::BasicType(),
  GenericMPoint::Property,
  GenericMPoint::Out, GenericMPoint::In,
  0, 0,
  GenericMPoint::Create, GenericMPoint::Delete,
  OpenAttribute<GenericMPoint >,
  SaveAttribute<GenericMPoint >,
  GenericMPoint::Close, GenericMPoint::Clone,
  GenericMPoint::Cast,
  GenericMPoint::SizeOf,
  GenericMPoint::KindCheck);
TypeConstructor tpointTC(
  TPoint::BasicType(),
  TPoint::Property,
  TPoint::Out, TPoint::In,
  0, 0,
  TPoint::Create, TPoint::Delete,
  OpenAttribute<TPoint >,
  SaveAttribute<TPoint >,
  TPoint::Close, TPoint::Clone,
  TPoint::Cast,
  TPoint::SizeOf,
  TPoint::KindCheck);
TypeConstructor utpointTC(
  UTPoint::BasicType(),
  UTPoint::Property,
  UTPoint::Out, UTPoint::In,
  0, 0,
  UTPoint::Create, UTPoint::Delete,
  OpenAttribute<UTPoint >,
  SaveAttribute<UTPoint >,
  UTPoint::Close, UTPoint::Clone,
  UTPoint::Cast,
  UTPoint::SizeOf,
  UTPoint::KindCheck);
TypeConstructor mtpointTC(
  MTPoint::BasicType(),   //name
  MTPointProperty,        //property function describing signature
  OutMapping<MTPoint, UTPoint, UTPoint::Out>,
  InMapping<MTPoint, UTPoint, UTPoint::In>,//Out and In functions
  0,
  0,                 //SaveToList and RestoreFromList functions
  CreateMapping<MTPoint>,
  DeleteMapping<MTPoint>,     //object creation and deletion
  OpenAttribute<MTPoint>,
  SaveAttribute<MTPoint>,      // object open and save
  CloseMapping<MTPoint>,
  CloneMapping<MTPoint>, //object close and clone
  CastMapping<MTPoint>,    //cast function
  SizeOfMapping<MTPoint>, //sizeof function
  CheckMTPoint );  //kind checking function
TypeConstructor thematicunitTC(
  ThematicUnit::BasicType(),
  ThematicUnit::Property,
  ThematicUnit::Out, ThematicUnit::In,
  0, 0,
  ThematicUnit::Create, ThematicUnit::Delete,
  OpenAttribute<ThematicUnit >,
  SaveAttribute<ThematicUnit >,
  ThematicUnit::Close, ThematicUnit::Clone,
  ThematicUnit::Cast,
  ThematicUnit::SizeOf,
  ThematicUnit::KindCheck);

TypeConstructor thematicpathTC(
  ThematicPath::BasicType(),
  ThematicPath::Property,
  ThematicPath::Out, ThematicPath::In,
  0, 0,
  ThematicPath::Create, ThematicPath::Delete,
  OpenAttribute<ThematicPath >,
  SaveAttribute<ThematicPath >,
  ThematicPath::Close, ThematicPath::Clone,
  ThematicPath::Cast,
  ThematicPath::SizeOf,
  ThematicPath::KindCheck);

TypeConstructor tempathTC(
  TemPath::BasicType(),
  TemPath::Property,
  TemPath::Out, TemPath::In,
  0, 0,
  TemPath::Create, TemPath::Delete,
  OpenAttribute<TemPath >,
  SaveAttribute<TemPath >,
  TemPath::Close, TemPath::Clone,
  TemPath::Cast,
  TemPath::SizeOfObj,
  TemPath::KindCheck);


/*
1.1.1 ~creategenericpoint~

Creates an ~gint~ from two ~int~ (source identifier, target identifier).

*/

const string maps_creategenericpoint[3][2] =
{

  {TPoint::BasicType(), GenericPoint::BasicType()},
  {GPoint::BasicType(), GenericPoint::BasicType()},
  {Point::BasicType(),GenericPoint::BasicType()}
  
};

ListExpr creategenericpointTM (ListExpr args)
{ 
  
   return SimpleMaps<3,2>(maps_creategenericpoint, args);

}

int creategenericpointSelect(ListExpr args)
{ 
  

  return SimpleSelect<3,2>(maps_creategenericpoint, args);
}

int creategenericpoint_pointVM( Word* args, Word& result, int message, Word& local,
                  Supplier s)
{
  
  result = qp->ResultStorage(s);
  GenericPoint* res = static_cast<GenericPoint*> (result.addr);

  Point* point = (Point*) args[0].addr;
  
  if ( ! point->IsDefined()){
    res->SetDefined(false);
    return 0;
  }
  
  res->SetDefined(true);

  GenericPoint* t = new GenericPoint(*point);
  *res = *t;
  t->DeleteIfAllowed();
  
  return 0;
}


int creategenericpoint_gpointVM( Word* args, Word& result, int message, Word& local,
                  Supplier s)
{
  
  
  result = qp->ResultStorage(s);
  GenericPoint* res = static_cast<GenericPoint*> (result.addr);

  GPoint* point = (GPoint*) args[0].addr;
  //res->Clear();
  if ( ! point->IsDefined()){
    res->SetDefined(false);
    return 0;
  }
  
  res->SetDefined(true);

  GenericPoint* t = new GenericPoint(*point);
  *res = *t;
  t->DeleteIfAllowed();
  
  return 0;
}


int creategenericpoint_tpointVM( Word* args, Word& result, int message, Word& local,
                  Supplier s)
{
  
  //cout<<"creategenericpoint_tpointVM"<<endl;
  result = qp->ResultStorage(s);
  GenericPoint* res = static_cast<GenericPoint*> (result.addr);

  TPoint* point = (TPoint*) args[0].addr;
  //res->Clear();
  point->Print(cout);
  if ( ! point->IsDefined()){
    cout<<"Point is not defined"<<endl;
    res->SetDefined(false);
    return 0;
  }
  
  //res->SetDefined(true);

  //GenericPoint* t = new GenericPoint(*point);
  //*res = *t;
  res=new GenericPoint(*point);
  //res->GetTPoint().Print(cout);
  //cout<<endl;
  //t->DeleteIfAllowed();
  
  return 0;
}

ValueMapping creategenericpointMap[] =
{ creategenericpoint_tpointVM,
  creategenericpoint_gpointVM,
  creategenericpoint_pointVM,

};

const string creategenericpointSpec =
  "( ( \"Signature\" \"Syntax\" \"Meaning\" \"Example\" ) "
  "(<text><text>query creategint(sourceid,targetid)</text--->))";

Operator creategenericpointGMO( "creategenericpoint", creategenericpointSpec, 3 , creategenericpointMap,
                         creategenericpointSelect, creategenericpointTM);






/*
1.1.1 ~creategenericmpoint~

Creates an ~gint~ from two ~int~ (source identifier, target identifier).

*/

const string maps_creategenericmpoint[3][2] =
{

  {MTPoint::BasicType(), GenericMPoint::BasicType()},
  {MGPoint::BasicType(), GenericMPoint::BasicType()},
  {temporalalgebra::MPoint::BasicType(),GenericMPoint::BasicType()}
  
};

ListExpr creategenericmpointTM (ListExpr args)
{ 
  
   return SimpleMaps<3,2>(maps_creategenericmpoint, args);

}

int creategenericmpointSelect(ListExpr args)
{ 
  

  return SimpleSelect<3,2>(maps_creategenericmpoint, args);
}

int creategenericmpoint_mpointVM( Word* args, Word& result, int message, Word& local,
                  Supplier s)
{
  
  result = qp->ResultStorage(s);
  GenericMPoint* res = static_cast<GenericMPoint*> (result.addr);

  temporalalgebra::MPoint* point = (temporalalgebra::MPoint*) args[0].addr;
  //res->Clear();
  if ( ! point->IsDefined()){
    res->SetDefined(false);
    return 0;
  }
  
  res->SetDefined(true);

  GenericMPoint* t = new GenericMPoint(*point);
  *res = *t;
  t->DeleteIfAllowed();
  
  return 0;
}


int creategenericmpoint_mgpointVM( Word* args, Word& result, int message, Word& local,
                  Supplier s)
{
  
  
  result = qp->ResultStorage(s);
  GenericMPoint* res = static_cast<GenericMPoint*> (result.addr);

  MGPoint* point = (MGPoint*) args[0].addr;
  //res->Clear();
  if ( ! point->IsDefined()){
    res->SetDefined(false);
    return 0;
  }
  
  res->SetDefined(true);

  GenericMPoint* t = new GenericMPoint(*point);
  *res = *t;
  t->DeleteIfAllowed();
  
  return 0;
}


int creategenericmpoint_mtpointVM( Word* args, Word& result, int message, Word& local,
                  Supplier s)
{
  
  
  result = qp->ResultStorage(s);
  GenericMPoint* res = static_cast<GenericMPoint*> (result.addr);

  MTPoint* point = (MTPoint*) args[0].addr;
  //cout<<"Entro a VM genericmpoint mtpoint"<<endl;
  //point->Print(cout);
  //res->Clear();
  if ( ! point->IsDefined()){
    res->SetDefined(false);
    return 0;
  }
  
  res->SetDefined(true);

  GenericMPoint* t = new GenericMPoint(*point);
  *res = *t;
  t->DeleteIfAllowed();
  
  return 0;
}

ValueMapping creategenericmpointMap[] =
{ creategenericmpoint_mtpointVM,
  creategenericmpoint_mgpointVM,
  creategenericmpoint_mpointVM,

};

const string creategenericmpointSpec =
  "( ( \"Signature\" \"Syntax\" \"Meaning\" \"Example\" ) "
  "(<text><text>query creategint(sourceid,targetid)</text--->))";

Operator creategenericmpointGMO( "creategenericmpoint", creategenericmpointSpec, 3 , creategenericmpointMap,
                         creategenericmpointSelect, creategenericmpointTM);







/*
1.1.1 ~distance~

Gives distance between 2 genericmpoint in km. In order to use this operator , genericmpoint should contains same domain and representation

*/

const string maps_distance[1][3] =
{
  {GenericPoint::BasicType(),GenericPoint::BasicType(),CcReal::BasicType()},
  
};

ListExpr distanceTM (ListExpr args)
{ 
  
   return SimpleMaps<1,3>(maps_distance, args);

}

int distanceSelect(ListExpr args)
{ 
  

  return SimpleSelect<1,3>(maps_distance, args);
}

int distanceVM( Word* args, Word& result, int message, Word& local,
                  Supplier s)
{
  
  result = qp->ResultStorage(s);
  CcReal* res = static_cast<CcReal*> (result.addr);

  GenericPoint* point0 = (GenericPoint*) args[0].addr;

  GenericPoint* point1 = (GenericPoint*) args[1].addr;


  
  if ( ! point0->IsDefined()|| ! point1->IsDefined() || (point0->GetDomain() != point1->GetDomain()) || point0->GetDefTPoint()){
    cout<<"genericmpoint should have same representation to calculate distance"<<endl;
    res->SetDefined(false);
    return 0;
  }
  
  res->SetDefined(true);



  double aux = DistancePoint(point0,point1);

  

  CcReal* t = new CcReal(aux);
  *res = *t;
  t->DeleteIfAllowed();
  
  return 0;
}

ValueMapping distanceMap[] =
{
  distanceVM
};

const string distanceSpec =
  "( ( \"Signature\" \"Syntax\" \"Meaning\" \"Example\" ) "
  "(<text><text>query creategint(sourceid,targetid)</text--->))";

Operator distanceGMO( "distance_point", distanceSpec,1 , distanceMap,
                         distanceSelect, distanceTM);



/*
1.1.1 ~map~

Creates an ~mpoint~ from ~mtpoint~ .

*/

const string maps_map_function[1][3] =
{
  {CcString::BasicType(), GenericMPoint::BasicType(),GenericMPoint::BasicType()}
};

ListExpr map_functionTM (ListExpr args)
{ 
  
   return SimpleMaps<1,3>(maps_map_function, args);

}

int map_functionSelect(ListExpr args)
{ 
  

  return SimpleSelect<1,3>(maps_map_function, args);
}

int map_functionVM( Word* args, Word& result, int message, Word& local,
                  Supplier s)
{
  
  result = qp->ResultStorage(s);
  GenericMPoint* res = static_cast<GenericMPoint*> (result.addr);

  CcString* map_function_name = (CcString*) args[0].addr;

  // CcString* domain = (CcString*) args[2].addr;
  
  GenericMPoint* point = (GenericMPoint*) args[1].addr;

  if (! map_function_name->IsDefined() || ! point->IsDefined()){
    res->SetDefined(false);
    return 0;
  }

  string map_function_str =(const char*)(map_function_name->GetStringval());

  string map_function_str_copy = map_function_str;
  std::transform(map_function_str_copy.begin(), map_function_str_copy.end(), map_function_str_copy.begin(), ::tolower);

  
  if(point->GetDefMTPoint()){
    if(map_function_str_copy == "identity"){
      res->SetDefined(true);
      *res = *point;
      point->DeleteIfAllowed();
      return 0;
    }else{

      temporalalgebra::MPoint newpoint = MTPoint2MPoint(point,map_function_str);
      
      res->SetDefined(true);

      GenericMPoint* t = new GenericMPoint(newpoint);
      *res = *t;
      t->DeleteIfAllowed();
      return 0;
    }

  }else if(point->GetDefMGPoint()){
    if(map_function_str == "identity"){
      res->SetDefined(true);
      *res = *point;
      point->DeleteIfAllowed();
      return 0;
    }else{ // Other case not existing yet, which takes some identifiers and needs to map in network 
      res->SetDefined(false);

      return 0;
    }

  }else if(point->GetDefMPoint()){ // Means that origin GenericMPoint has a MPoint and the domain target is freespace. Return same GenericMPoint

    res->SetDefined(true);
    *res = *point;
    point->DeleteIfAllowed();
    return 0;

  }
  
  return 0;
}

ValueMapping map_functionMap[] =
{
  map_functionVM
};

const string map_functionSpec =
  "( ( \"Signature\" \"Syntax\" \"Meaning\" \"Example\" ) "
  "(<text><text>query creategint(sourceid,targetid)</text--->))";

Operator map_functionGMO( "mapping", map_functionSpec,1 , map_functionMap,
                         map_functionSelect, map_functionTM);



/*
1.1.1 ~map2fs~

Creates an ~mpoint~ from ~mgpoint~ as GenericMPoint .

*/

const string map2fs_function[1][2] =
{
  { GenericMPoint::BasicType(),GenericMPoint::BasicType()}
};

ListExpr map2fs_functionTM (ListExpr args)
{ 
  
   return SimpleMaps<1,2>(map2fs_function, args);

}

int map2fs_functionSelect(ListExpr args)
{ 
  

  return SimpleSelect<1,2>(map2fs_function, args);
}

int map2fs_functionVM( Word* args, Word& result, int message, Word& local,
                  Supplier s)
{
  
  result = qp->ResultStorage(s);
  GenericMPoint* res = static_cast<GenericMPoint*> (result.addr);
  
  GenericMPoint* point = (GenericMPoint*) args[0].addr;


  if (! point->IsDefined()){
    res->SetDefined(false);
    return 0;
  }

  
  
  if(point->GetDefMGPoint()){ 

    MGPoint mgpoint(false);
  
    temporalalgebra::MPoint* temp = new temporalalgebra::MPoint(true);

    point->GetMGPoint2()->Mgpoint2mpoint(temp);

    GenericMPoint* outpoint =  new GenericMPoint(*temp);

    //outpoint->GetMPoint().Print(cout);


    *res = *outpoint;
    delete(outpoint);
    return 0;

     

  }else{
    cout<<"This operator needs GenericMPoint contains a MGPoint"<<endl;
    return 0;
  }
  
  return 0;
}

ValueMapping map2fs_functionMap[] =
{
  map2fs_functionVM
};

const string map2fs_functionSpec =
  "( ( \"Signature\" \"Syntax\" \"Meaning\" \"Example\" ) "
  "(<text><text>query creategint(sourceid,targetid)</text--->))";

Operator map2fs_functionGMO( "map2fs", map2fs_functionSpec,1 , map2fs_functionMap,
                         map2fs_functionSelect, map2fs_functionTM);



/*
1.1.1 ~map2network~

Creates an ~mgpoint~ from ~mpoint~ as GenericMPoint .

*/

const string map2network_function[1][3] =
{
  { GenericMPoint::BasicType(),network::Network::BasicType(),GenericMPoint::BasicType()}
};

ListExpr map2network_functionTM (ListExpr args)
{ 
  
   return SimpleMaps<1,3>(map2network_function, args);

}

int map2network_functionSelect(ListExpr args)
{ 
  

  return SimpleSelect<1,3>(map2network_function, args);
}

int map2network_functionVM( Word* args, Word& result, int message, Word& local,
                  Supplier s)
{
  
  result = qp->ResultStorage(s);
  GenericMPoint* res_final = static_cast<GenericMPoint*> (result.addr);
  
  GenericMPoint* point = (GenericMPoint*) args[0].addr;
  network::Network *pNetwork = (network::Network*) args[1].addr;

  if (! point->IsDefined()){
    res_final->SetDefined(false);
    return 0;
  }
  if (pNetwork == 0 || !pNetwork->IsDefined())
    {
      res_final->SetDefined(false);
      return 0;
    }

  
  
  if(point->GetDefMPoint()){ 

    //MGPoint* res = static_cast<MGPoint*>(result.addr);
    
    MGPoint* res = MPoint2MGPoint(point,pNetwork);
    //res->Print(cout);
    GenericMPoint * final = new GenericMPoint(*res);
    *res_final = *final;
    return 0;

     

  }else{
    cout<<"This operator needs GenericMPoint contains a MPoint"<<endl;
    return 0;
  }
  
  return 0;
}

ValueMapping map2network_functionMap[] =
{
  map2network_functionVM
};

const string map2network_functionSpec =
  "( ( \"Signature\" \"Syntax\" \"Meaning\" \"Example\" ) "
  "(<text><text>query creategint(sourceid,targetid)</text--->))";

Operator map2network_functionGMO( "map2network", map2network_functionSpec,1 , map2network_functionMap,
                         map2network_functionSelect, map2network_functionTM);

/*
1.1.1 ~present~

Return true if genericmpoint is alive in the Period or Instant
*/


const string maps_present[2][3] =
{
  {GenericMPoint::BasicType(),Instant::BasicType(),CcBool::BasicType()},
  {GenericMPoint::BasicType(),Periods::BasicType(),CcBool::BasicType()}
  
};

ListExpr presentTM (ListExpr args)
{ 
  //cout<<"presentTM"<<endl;
  
   return SimpleMaps<2,3>(maps_present, args);

}

int presentSelect(ListExpr args)
{ 
  
  //cout<<"presentSelect"<<endl;
   return SimpleSelect<2,3>(maps_present, args);
}

int presentIVM( Word* args, Word& result, int message, Word& local,
                  Supplier s)
{
  //cout<<"Entro a Instant"<<endl; 
  result = qp->ResultStorage(s);
  CcBool* res = static_cast<CcBool*> (result.addr);

  GenericMPoint* point0 = (GenericMPoint*) args[0].addr;

  Instant* instant = (Instant*) args[1].addr;

  if ( ! point0->IsDefined()|| ! instant->IsDefined() ){
    ((CcBool *)result.addr)->Set( false, false );
    return 0;
  }
  if(point0->GetDefMPoint()){
    res->Set( true, point0->GetMPoint().Present( *instant ) );
  }else if(point0->GetDefMGPoint()){
    res->Set( true, point0->GetMGPoint().Present( instant ) );
  }else if(point0->GetDefMTPoint()){
    res->Set( true, point0->GetMTPoint().Present( *instant ) );
  }
  
  
  return 0;
}


int presentPVM( Word* args, Word& result, int message, Word& local,
                  Supplier s)
{
  
  result = qp->ResultStorage(s);
  CcBool* res = static_cast<CcBool*> (result.addr);

  GenericMPoint* point0 = (GenericMPoint*) args[0].addr;

  Periods* periods = (Periods*) args[1].addr;


  
  if ( ! point0->IsDefined()|| ! periods->IsDefined()  || periods->IsEmpty() ){
    ((CcBool *)result.addr)->Set( false, false );
    return 0;
  }
  
  if(point0->GetDefMPoint()){
    res->Set( true, point0->GetMPoint().Present( *periods ) );
  }else if(point0->GetDefMGPoint()){
    res->Set( true, point0->GetMGPoint().Present( periods ) );
  }else if(point0->GetDefMTPoint()){
    res->Set( true, point0->GetMTPoint().Present( *periods ) );
  }
  
  return 0;
}

ValueMapping presentMap[] =
{
  presentIVM,
  presentPVM
};

const string presentSpec =
  "( ( \"Signature\" \"Syntax\" \"Meaning\" \"Example\" ) "
  "(<text><text>query creategint(sourceid,targetid)</text--->))";

Operator presentGMO( "gmo_present", presentSpec,2 , presentMap,
                         presentSelect, presentTM);


/*
1.1.1 ~mt_present~

Return true if mtpoint is alive in the Period or Instant
*/


const string maps_mt_present[2][3] =
{
  {MTPoint::BasicType(),Instant::BasicType(),CcBool::BasicType()},
  {MTPoint::BasicType(),Periods::BasicType(),CcBool::BasicType()}
  
};

ListExpr mt_presentTM (ListExpr args)
{ 
  //cout<<"mt_presentTM"<<endl;
  
   return SimpleMaps<2,3>(maps_mt_present, args);

}

int mt_presentSelect(ListExpr args)
{ 
  
  //cout<<"mt_presentSelect"<<endl;
   return SimpleSelect<2,3>(maps_mt_present, args);
}

int mt_presentIVM( Word* args, Word& result, int message, Word& local,
                  Supplier s)
{
  //cout<<"Entro a Instant"<<endl; 
  result = qp->ResultStorage(s);
  CcBool* res = static_cast<CcBool*> (result.addr);

  MTPoint* point0 = (MTPoint*) args[0].addr;

  Instant* instant = (Instant*) args[1].addr;

  if ( ! point0->IsDefined()|| ! instant->IsDefined() ){
    ((CcBool *)result.addr)->Set( false, false );
    return 0;
  }
  res->Set( true, point0->Present( *instant ) );
  
  
  return 0;
}


int mt_presentPVM( Word* args, Word& result, int message, Word& local,
                  Supplier s)
{
  
  result = qp->ResultStorage(s);
  CcBool* res = static_cast<CcBool*> (result.addr);

  MTPoint* point0 = (MTPoint*) args[0].addr;

  Periods* periods = (Periods*) args[1].addr;


  
  if ( ! point0->IsDefined()|| ! periods->IsDefined()  || periods->IsEmpty() ){
    ((CcBool *)result.addr)->Set( false, false );
    return 0;
  }
  res->Set( true, point0->Present( *periods ) );
  
  return 0;
}

ValueMapping mt_presentMap[] =
{
  mt_presentIVM,
  mt_presentPVM
};

const string mt_presentSpec =
  "( ( \"Signature\" \"Syntax\" \"Meaning\" \"Example\" ) "
  "(<text><text>query creategint(sourceid,targetid)</text--->))";

Operator mt_presentGMO( "mt_present", mt_presentSpec,2 , mt_presentMap,
                         mt_presentSelect, mt_presentTM);

/*
1.1.1 ~gmo_between~

Return true if genericmpoint is between two identifiers
*/


const string maps_between[1][4] =
{
  {GenericMPoint::BasicType(),GenericPoint::BasicType(),GenericPoint::BasicType(),CcBool::BasicType()},
  
};

ListExpr betweenTM (ListExpr args)
{ 
  
   return SimpleMaps<1,4>(maps_between, args);

}

int betweenSelect(ListExpr args)
{ 

   return SimpleSelect<1,4>(maps_between, args);
}

int betweenVM( Word* args, Word& result, int message, Word& local,
                  Supplier s)
{
  
  result = qp->ResultStorage(s);
  //CcBool* res = static_cast<CcBool*> (result.addr);

  GenericMPoint* mpoint = (GenericMPoint*) args[0].addr;

  GenericPoint* point0 = (GenericPoint*) args[1].addr;
  GenericPoint* point1 = (GenericPoint*) args[2].addr;

  if ( ! mpoint->IsDefined()|| ! point0->IsDefined() || ! point1->IsDefined() ){
    ((CcBool *)result.addr)->Set( false, false );
    return 0;
  }
  if(mpoint->GetDefMPoint()){

    if(point0->GetDefPoint() && point1->GetDefPoint()){
      double bbox[] = {point0->GetPoint().GetX(),point0->GetPoint().GetY(),point1->GetPoint().GetX(),point1->GetPoint().GetY()}; 
      Rectangle<2> *accubbox = new Rectangle<2>(true, bbox);
      /*Rectangle<2> *accubbox = new Rectangle<2>(true, std::min(point0->GetPoint().GetX(), point1->GetPoint().GetX()),
                                  std::max(point0->GetPoint().GetX(), point1->GetPoint().GetX()),
                                  std::min(point0->GetPoint().GetY(), point1->GetPoint().GetY()),
                                  std::max(point0->GetPoint().GetY(), point1->GetPoint().GetY()));*/
      cout<<"Rectangle"<<endl;
      accubbox->Print(cout);
      Line line(0) ;
      mpoint->GetMPoint().Trajectory(line);
      //accubbox->Print(cout);
      if( accubbox->IsDefined() && line.IsDefined() )
      {
        Region *reg = new Region( *accubbox );
        reg->Print(cout);
        ((CcBool *)result.addr)->Set( true, line.Intersects(*reg));
      }
    }else{
      ((CcBool *)result.addr)->Set( false, false );
      return 0;
    }

  }else if(mpoint->GetDefMGPoint()){
    if(point0->GetDefGPoint() && point1->GetDefGPoint()){

  
      MGPoint mgpoint = mpoint->GetMGPoint();
      if( !mgpoint.IsDefined() ||
         mgpoint.GetNoComponents() < 1 ) {
        ((CcBool *)result.addr)->Set(false, false);
        return 0;
      }
      GPoint gp0 = point0->GetGPoint();
      if( !gp0.IsDefined()) {
        ((CcBool *)result.addr)->Set(false, false);
        return 0;
      }
      GPoint gp1 = point1->GetGPoint();
      if( !gp1.IsDefined()) {
        ((CcBool *)result.addr)->Set(false, false);
        return 0;
      }
      

        GLine* pGLine = new GLine(0);
        GLine * traj = new GLine(0);

        mgpoint.Trajectory(traj);
        // DbArray<RouteInterval>* GetRouteIntervals()
        
        network::Network * pNetwork = NetworkManager::GetNetwork(gp0.GetNetworkId());


        pGLine->SetSorted ( false );
        pGLine->SetDefined ( gp0.ShortestPath ( &gp1, pGLine, pNetwork,
                                                         0 ) );
        RouteInterval ri0,ri1; 




        for(int i = 0 ; i< pGLine->Size() ; i++){
          pGLine->Get(i,ri0);
          for(int j = 0 ; j < traj->Size() ; j++){
            traj->Get(j,ri1);
            if(ri0.Intersects(&ri0,1.0)){
              ((CcBool *)result.addr)->Set(true, true);
              NetworkManager::CloseNetwork(pNetwork);
              //cout<<"Intersectó en ShortestPath"<<endl;
              return 0;
            }
          }
        }
        NetworkManager::CloseNetwork(pNetwork);


      ((CcBool *)result.addr)->Set(true, mgpoint.Passes(&gp0) &&  mgpoint.Passes(&gp1));
      return 0;


    }else{
      ((CcBool *)result.addr)->Set( false, false );
      return 0;
    }
  }else if(mpoint->GetDefMTPoint()){
    //bool first,second;
    int f=-1,s=-1;
    if(point0->GetDefTPoint() && point1->GetDefTPoint()){
        MTPoint mtpoint = mpoint->GetMTPoint();
        if( !mtpoint.IsDefined() ||
         mtpoint.GetNoComponents() < 1 ) {
        ((CcBool *)result.addr)->Set(false, false);
        return 0;
      }
      TPoint gp0 = point0->GetTPoint();
      if( !gp0.IsDefined()) {
        ((CcBool *)result.addr)->Set(false, false);
        return 0;
      } 
      TPoint gp1 = point1->GetTPoint();
      if( !gp1.IsDefined()) {
        ((CcBool *)result.addr)->Set(false, false);
        return 0;
      } 
      for (int i = 0; i < mtpoint.GetNoComponents(); ++i)
       {
         UTPoint utp;
         mtpoint.Get(i, utp);
         if(gp0.GetStop() == utp.GetStart().GetStop())
            f=i;
          if(gp1.GetStop() == utp.GetStart().GetStop())
            s=i;
       } 

      if(f !=-1 || s!=-1 ){
        ((CcBool *)result.addr)->Set(true, true);
        return 0;
      }else{
        ((CcBool *)result.addr)->Set( true, false );
        return 0;
      }
    } 
  }
  
  
  return 0;
}



ValueMapping betweenMap[] =
{
  betweenVM
  
};

const string betweenSpec =
  "( ( \"Signature\" \"Syntax\" \"Meaning\" \"Example\" ) "
  "(<text><text>query creategint(sourceid,targetid)</text--->))";

Operator betweenGMO( "gmo_between", betweenSpec,1 , betweenMap,
                         betweenSelect, betweenTM);

/*
1.1.1 ~gmo_net_between~

Returns periods where genericmpoint was alive
*/


const string maps_net_between[1][4] =
{
  {MGPoint::BasicType(),GPoint::BasicType(),GPoint::BasicType(),CcBool::BasicType()
  },
  
};

ListExpr net_betweenTM (ListExpr args)
{ 
  
   return SimpleMaps<1,4>(maps_net_between, args);

}

int net_betweenSelect(ListExpr args)
{ 

   return SimpleSelect<1,4>(maps_net_between, args);
}

int net_betweenVM( Word* args, Word& result, int message, Word& local,
                  Supplier s)
{
  
  result = qp->ResultStorage(s);
  //CcBool* res = static_cast<CcBool*> (result.addr);

  MGPoint* mpoint = (MGPoint*) args[0].addr;

  GPoint* point0 = (GPoint*) args[1].addr;
  GPoint* point1 = (GPoint*) args[2].addr;

  if ( ! mpoint->IsDefined()|| ! point0->IsDefined() || ! point1->IsDefined() ){
    ((CcBool *)result.addr)->Set( false, false );
    return 0;
  }
  
  
      if( !mpoint->IsDefined() ||
         mpoint->GetNoComponents() < 1 ) {
        ((CcBool *)result.addr)->Set(false, false);
        return 0;
      }
      
      

        GLine* pGLine = new GLine(0);
        GLine * traj = new GLine(0);

        mpoint->Trajectory(traj);
        // DbArray<RouteInterval>* GetRouteIntervals()
        
        network::Network * pNetwork = NetworkManager::GetNetwork(point0->GetNetworkId());


        pGLine->SetSorted ( false );
        pGLine->SetDefined ( point0->ShortestPath ( point1, pGLine, pNetwork,
                                                         0 ) );
        RouteInterval ri0,ri1; 

        for(int i = 0 ; i< pGLine->Size() ; i++){
          pGLine->Get(i,ri0);
          for(int j = 0 ; j < traj->Size() ; j++){
            traj->Get(j,ri1);
            if(ri0.Intersects(&ri0,1.0)){
              ((CcBool *)result.addr)->Set(true, true);
              //cout<<"Intersectó en ShortestPath"<<endl;
              return 0;
            }
          }
        }
        NetworkManager::CloseNetwork(pNetwork);


      ((CcBool *)result.addr)->Set(true, mpoint->Passes(point0) &&  mpoint->Passes(point1));
      return 0;
  
}



ValueMapping net_betweenMap[] =
{
  net_betweenVM
  
};

const string net_betweenSpec =
  "( ( \"Signature\" \"Syntax\" \"Meaning\" \"Example\" ) "
  "(<text><text>query creategint(sourceid,targetid)</text--->))";

Operator net_betweenGMO( "net_between", net_betweenSpec,1 , net_betweenMap,
                         net_betweenSelect, net_betweenTM);




/*
1.1.1 ~gmo_mt_between~

Returns periods where genericmpoint was alive
*/


const string maps_mt_between[1][4] =
{
  {MTPoint::BasicType(),TPoint::BasicType(),TPoint::BasicType(),CcBool::BasicType()
  },
  
};

ListExpr mt_betweenTM (ListExpr args)
{ 
  
   return SimpleMaps<1,4>(maps_mt_between, args);

}

int mt_betweenSelect(ListExpr args)
{ 

   return SimpleSelect<1,4>(maps_mt_between, args);
}

int mt_betweenVM( Word* args, Word& result, int message, Word& local,
                  Supplier sp)
{
  
  result = qp->ResultStorage(sp);
  //CcBool* res = static_cast<CcBool*> (result.addr);

  MTPoint* mpoint = (MTPoint*) args[0].addr;

  TPoint* point0 = (TPoint*) args[1].addr;
  TPoint* point1 = (TPoint*) args[2].addr;

  if ( ! mpoint->IsDefined()|| ! point0->IsDefined() || ! point1->IsDefined() ){
    ((CcBool *)result.addr)->Set( false, false );
    return 0;
  }
  int f=-1,s=-1;
    for (int i = 0; i < mpoint->GetNoComponents(); ++i)
     {
       UTPoint utp;
       mpoint->Get(i, utp);
       if(point0->GetStop() == utp.GetStart().GetStop())
          f=i;
        if(point1->GetStop() == utp.GetStart().GetStop())
          s=i;
     } 

    if(f !=-1 || s!=-1 ){
      ((CcBool *)result.addr)->Set(true, true);
      return 0;
    }else{
      ((CcBool *)result.addr)->Set( true, false );
      return 0;
    }
   
  
  
  return 0;
}



ValueMapping mt_betweenMap[] =
{
  mt_betweenVM
  
};

const string mt_betweenSpec =
  "( ( \"Signature\" \"Syntax\" \"Meaning\" \"Example\" ) "
  "(<text><text>query creategint(sourceid,targetid)</text--->))";

Operator mt_betweenGMO( "mt_between", mt_betweenSpec,1 , mt_betweenMap,
                         mt_betweenSelect, mt_betweenTM);




/*
1.1.1 ~gmo_duration~

Returns periods where genericmpoint was alive
*/


const string maps_duration[1][2] =
{
  {GenericMPoint::BasicType(),Periods::BasicType()},
  
};

ListExpr durationTM (ListExpr args)
{ 
  
   return SimpleMaps<1,2>(maps_duration, args);

}

int durationSelect(ListExpr args)
{ 

   return SimpleSelect<1,2>(maps_duration, args);
}

int durationVM( Word* args, Word& result, int message, Word& local,
                  Supplier s)
{
  
  result = qp->ResultStorage(s);
  Periods* res = static_cast<Periods*> (result.addr);

  GenericMPoint* mpoint = (GenericMPoint*) args[0].addr;

  if ( ! mpoint->IsDefined()   ){
    ((CcBool *)result.addr)->Set( false, false );
    return 0;
  }
  Periods defTime( 0 );
  if(mpoint->GetDefMPoint()){
    
    mpoint->GetMPoint().DefTime( defTime );
    *res = defTime;
   
  }else if(mpoint->GetDefMGPoint()){
    mpoint->GetMGPoint().DefTime( defTime );
    *res = defTime; 
    
  }
  
  
  return 0;
}



ValueMapping durationMap[] =
{
  durationVM
  
};

const string durationSpec =
  "( ( \"Signature\" \"Syntax\" \"Meaning\" \"Example\" ) "
  "(<text><text>query creategint(sourceid,targetid)</text--->))";

Operator durationGMO( "gmo_duration", durationSpec,1 , durationMap,
                         durationSelect, durationTM);


/*
1.1.1 ~map~

Creates an ~mpoint~ from ~mtpoint~ .

*/

const string maps_gmo_in_thematic[1][3] =
{
  {CcString::BasicType(), TemPath::BasicType(), CcBool::BasicType()}
};

ListExpr gmo_in_thematicTM (ListExpr args)
{ 
  
   return SimpleMaps<1,3>(maps_gmo_in_thematic, args);

}

int gmo_in_thematicSelect(ListExpr args)
{ 
  

  return SimpleSelect<1,3>(maps_gmo_in_thematic, args);
}

int gmo_in_thematicVM( Word* args, Word& result, int message, Word& local,
                  Supplier s)
{
  
  result = qp->ResultStorage(s);
  CcBool* res = static_cast<CcBool*> (result.addr);

  CcString* unit = (CcString*) args[0].addr;

  TemPath* path = (TemPath*) args[1].addr;
  
  
  if ( ! unit->IsDefined() || ! path->IsDefined()){
    res->SetDefined(false);
    return 0;
  }

  for (int i = 0; i < path->GetNoUnits(); ++i)
  {
    ThematicUnit thematic_unit(path->GetUnit(i).unit);
    
    if( thematic_unit.GetUnit() == (const char*)(unit->GetStringval())){
      ((CcBool *)result.addr)->Set( true, true );
      return 0;
    }
  }
  
  res->SetDefined(true);

  ((CcBool *)result.addr)->Set( true, false );

  
  return 0;
}

ValueMapping gmo_in_thematicMap[] =
{
  gmo_in_thematicVM
};

const string gmo_in_thematicSpec =
  "( ( \"Signature\" \"Syntax\" \"Meaning\" \"Example\" ) "
  "(<text><text>query creategint(sourceid,targetid)</text--->))";

Operator gmo_in_thematicGMO( "in_thematic", gmo_in_thematicSpec,1 , gmo_in_thematicMap,
                         gmo_in_thematicSelect, gmo_in_thematicTM);




}//end of namespace sgraph

/*
1 Implementation of ~class MOOSAlgebra~

1.1 Constructor

*/
GMOAlgebra::GMOAlgebra():Algebra()
{


AddTypeConstructor(&gpointTC);
gpointTC.AssociateKind(Kind::DATA());
AddTypeConstructor(&igpointTC);
igpointTC.AssociateKind(Kind::DATA());
AddTypeConstructor(&gmpointTC);
gmpointTC.AssociateKind(Kind::DATA());
AddTypeConstructor(&tpointTC);
tpointTC.AssociateKind(Kind::DATA());
AddTypeConstructor(&utpointTC);
utpointTC.AssociateKind(Kind::DATA());
AddTypeConstructor(&mtpointTC);
mtpointTC.AssociateKind(Kind::DATA());
AddTypeConstructor(&thematicunitTC);
thematicunitTC.AssociateKind(Kind::DATA());
AddTypeConstructor(&thematicpathTC);
thematicpathTC.AssociateKind(Kind::DATA());

AddTypeConstructor(&tpath);
tpath.AssociateKind(Kind::DATA());
AddTypeConstructor(&tempathTC);
tempathTC.AssociateKind(Kind::DATA());

AddTypeConstructor(&tempoTC);
tempoTC.AssociateKind(Kind::DATA());



AddOperator(&creategenericmpointGMO);
AddOperator(&creategenericpointGMO);
AddOperator(&distanceGMO);
AddOperator(&map_functionGMO);
AddOperator(&map2fs_functionGMO);
AddOperator(&map2network_functionGMO);
AddOperator(&presentGMO);
AddOperator(&mt_presentGMO);
AddOperator(&betweenGMO);
AddOperator(&durationGMO);
AddOperator(&simplificationGMO);
AddOperator(&groupGMO);
AddOperator(&partitionGMO);
AddOperator(&intersectsGMO);
AddOperator(&similarityGMO);
AddOperator(&gmo_in_thematicGMO);
AddOperator(&mt_betweenGMO);
AddOperator(&net_betweenGMO);
}

GMOAlgebra::~GMOAlgebra(){}


extern "C"
Algebra* InitializeGMOAlgebra ( NestedList* nlRef,
                                 QueryProcessor* qpRef )
{
  nl = nlRef;
  qp = qpRef;
  return ( new GMOAlgebra() );
}