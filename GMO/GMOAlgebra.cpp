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
#include "QueryProcessor.h"   // needed for implementing value mappings
#include "AlgebraManager.h"   // e.g., check for certain kind
#include "../Network/NetworkAlgebra.h"
#include "Point.h"
#include <typeinfo>
#include <fstream>
#include <vector>
#include <iterator>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <fstream> 
#include <boost/algorithm/string.hpp>
#include <boost/unordered_map.hpp>


using namespace std;
using namespace mappings;
using namespace gmo;
using namespace network;

extern NestedList* nl;
extern QueryProcessor* qp;

namespace gmo{



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
    //cout<<"line :"<<line<<endl;
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

bool
CheckMTPoint( ListExpr type, ListExpr& errorInfo )
{
  return (nl->IsEqual( type, MTPoint::BasicType() ));
}


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


/*
1.1.1 ~creategint~

Creates an ~gint~ from two ~int~ (source identifier, target identifier).

*/

const string maps_creategint[2][3] =
{
  {CcString::BasicType(), Point::BasicType(),GenericPoint::BasicType()},
  {CcString::BasicType(), GPoint::BasicType(),GenericPoint::BasicType()}
  
};

ListExpr creategpointTM (ListExpr args)
{ 
  
   return SimpleMaps<2,3>(maps_creategint, args);

}

int creategpointSelect(ListExpr args)
{ 
  

  return SimpleSelect<2,3>(maps_creategint, args);
}

int creategpointVM( Word* args, Word& result, int message, Word& local,
                  Supplier s)
{
  
  result = qp->ResultStorage(s);
  GenericPoint* res = static_cast<GenericPoint*> (result.addr);

  CcString* domain = (CcString*) args[0].addr;
  Point* point = (Point*) args[1].addr;
  //res->Clear();
  if (! domain->IsDefined() || ! point->IsDefined()){
    res->SetDefined(false);
    return 0;
  }
  Domain dDomain ;
  string sDomain =(const char*)(domain->GetStringval());
  if(sDomain == "FreeSpace")
      dDomain = FreeSpace;
  else if (sDomain =="Network")
      dDomain = Network;
  else  //default value
    dDomain = FreeSpace;

  cout<<dDomain<<endl;
  res->SetDefined(true);

  GenericPoint* t = new GenericPoint(*point);
  *res = *t;
  t->DeleteIfAllowed();
  
  return 0;
}


int creategnpointVM( Word* args, Word& result, int message, Word& local,
                  Supplier s)
{ 
  result = qp->ResultStorage(s);
  GenericPoint* res = static_cast<GenericPoint*> (result.addr);

  CcString* domain = (CcString*) args[0].addr;
  network::GPoint* point = (network::GPoint*) args[1].addr;

  if (! domain->IsDefined() || ! point->IsDefined()){
    res->SetDefined(false);
    return 0;
  }

  // Set domain (enum)

  Domain dDomain ;
  string sDomain =(const char*)(domain->GetStringval());

  
  if(sDomain == "FreeSpace"){
      res->SetDefined(false);
      return 0;
    }
  else if(sDomain =="Network")
      dDomain = Network;
  
  cout<<dDomain<<endl; 
 

  GenericPoint* t = new GenericPoint(
                                           *point);
  

  
  *res = *t;

  
  
  //t->DeleteIfAllowed();
  
  return 0;
}

ValueMapping creategpointMap[] =
{
  creategpointVM,
  creategnpointVM
};

const string creategpointSpec =
  "( ( \"Signature\" \"Syntax\" \"Meaning\" \"Example\" ) "
  "(<text><text>query creategint(sourceid,targetid)</text--->))";

Operator creategpointGMO( "creategpoint", creategpointSpec,2 , creategpointMap,
                         creategpointSelect, creategpointTM);


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

  std::transform(map_function_str.begin(), map_function_str.end(), map_function_str.begin(), ::tolower);

  
  if(point->GetDefMTPoint()){
    if(map_function_str == "identity"){
      res->SetDefined(true);
      *res = *point;
      point->DeleteIfAllowed();
      return 0;
    }else{

      std::ifstream test(map_function_str.c_str()); 
      if (!test)
      {
          std::cout << "The file for mapping doesn't exist" << std::endl;
          return 0;
      }

      CSVReader reader(map_function_str,",");
     
      // Get the data from CSV File
      std::vector<std::vector<std::string> > dataList = reader.getData();

      if (dataList.size()==0){ //Case when path of file is incorrect or stops file is empty
        res->SetDefined(false);
        return 0;
      }


      boost::unordered_map<std::string,double> map_latitude;
      boost::unordered_map<std::string,double> map_longitude;
      
      for (unsigned int i = 0; i < dataList.size(); ++i)
      {
        char* aux;
        char* aux_2;
        
        map_latitude[dataList[i][0]]= std::strtod(dataList[i][3].c_str(),&aux); //acording to GTFS stops file
        map_longitude[dataList[i][0]]= std::strtod(dataList[i][4].c_str(),&aux_2);
      }

      MPoint newpoint(point->GetMTPoint().GetNoComponents());

      if(point->GetDefMTPoint()){

        for(int i=0; i<point->GetMTPoint().GetNoComponents(); i++)
        {
          UTPoint unit;
          point->GetMTPoint().Get( i , unit );
          Point p0(true,map_latitude[unit.GetStart().GetStop()],map_longitude[unit.GetStart().GetStop()]);
          Point p1(true,map_latitude[unit.GetEnd().GetStop()],map_longitude[unit.GetEnd().GetStop()]);
          UPoint upoint(unit.getTimeInterval(),p0,p1);
          newpoint.Add(upoint);
          
        }
      }


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

    MGPoint mgpoint(point->GetMGPoint());
  
    MPoint* temp = new MPoint(true);

    mgpoint.Mgpoint2mpoint(temp);

    GenericMPoint* outpoint =  new GenericMPoint(temp);

    *res = *outpoint;
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


AddOperator(&creategpointGMO);
AddOperator(&map_functionGMO);
AddOperator(&map2fs_functionGMO);
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