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
#include "MOOSAlgebra.h"
#include "RelationAlgebra.h"
#include "PairIntDouble.h"
#include "GInterval.h"

using namespace std;
using namespace mappings;
using namespace sgraph;

extern NestedList* nl;
extern QueryProcessor* qp;

namespace sgraph{


/*
1.1 ~gint~

Describes a graph interval this means two identifiers that indicate source and target of a bus.

*/

TypeConstructor gintTC(
  GInterval::BasicType(),
  GInterval::Property,
  GInterval::Out, JRouteInterval::In,
  0, 0,
  GInterval::Create, GInterval::Delete,
  OpenAttribute<GInterval>,
  SaveAttribute<GInterval>,
  GInterval::Close, GInterval::Clone,
  GInterval::Cast,
  GInterval::SizeOf,
  GInterval::KindCheck);

}





/*
1.1.1 ~creategint~

Creates an ~grint~ from two ~int~ (source identifier, target identifier).

*/

const string maps_creategint[1][5] =
{
  {CcInt::BasicType(), CcInt::BasicType(), GInterval::BasicType()}
};

ListExpr creategintTM (ListExpr args)
{
  return SimpleMaps<1,3>(maps_creategint, args);
}

int creategintSelect(ListExpr args)
{
  return SimpleSelect<1,3>(maps_creategint, args);
}

int creategintVM( Word* args, Word& result, int message, Word& local,
                  Supplier s)
{
  result = qp->ResultStorage(s);
  GInterval* res = static_cast<GInterval*> (result.addr);

  CcInt* tint = (CcInt*) args[0].addr;
  CcReal* tspos = (CcReal*) args[1].addr;
  CcReal* tepos = (CcReal*) args[2].addr;
  Direction* tdir = (Direction*) args[3].addr;

  if (tint != 0 && tint->IsDefined() &&
    tspos != 0 && tspos->IsDefined() &&
          tepos != 0 && tepos->IsDefined() &&
                tdir != 0 && tdir->IsDefined())
  {
    JRouteInterval* t = new JRouteInterval(tint->GetIntval(),
                                           tspos->GetRealval(),
                                           tepos->GetRealval(),
                                           (Direction)tdir->GetDirection());
    *res = *t;
    t->DeleteIfAllowed();
  }
  else
    res->SetDefined(false);

  return 0;
}

ValueMapping createrintMap[] =
{
  createrintVM
};

const string createrintSpec =
  "( ( \"Signature\" \"Syntax\" \"Meaning\" \"Example\" ) "
  "(<text>" + CcInt::BasicType() + " x " + CcReal::BasicType() + " x " +
  CcReal::BasicType() + " x " +  Direction::BasicType() + " -> " +
  JRouteInterval::BasicType() + "</text--->"
  "<text>createrint( <rid> , <startpos> , <endpos> , <side>) </text--->"
  "<text>Creates a " + JRouteInterval::BasicType() + " from the route id, " +
  "the distances of the start and the end point from the start of the route "+
  "and the side value.</text--->"
  "<text>query createrint(rid, spos, epos, side)</text--->))";

Operator createrintJNet( "createrint", createrintSpec, 1, createrintMap,
                         createrintSelect, createrintTM);
//end of namespace sgraph

/*
1 Implementation of ~class MOOSAlgebra~

1.1 Constructor

*/
MOOSAlgebra::MOOSAlgebra():Algebra()
{

}