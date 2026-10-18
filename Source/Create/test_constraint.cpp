//////////////////////////////////////////////////////////////////////////////////////////
//  Методы для задания геометрических ограничений и сопряжений
//  Methods to set the geometric constraints and mates
//////////////////////////////////////////////////////////////////////////////////////////

#include <test.h>
#include <test_manager.h>
#include <test_service.h>
#include <space_instance.h>
#include <constraint.h>
#include <plane_instance.h>
#include <cur_arc.h>
#include <gc_api.h>
#include <mb_class_traits.h>
#include <alg_draw.h>
#include <last.h>

using namespace c3d;


//----------------------------------------------------------------------------------------
// Query geometric arguments of a constraint from GUI editor
//---
static 
bool GetConstraintArgs(  int & mateType
                       , MbItem *& obj1, SPtr<const MbRefItem> & gItem1
                       , MbItem *& obj2, SPtr<const MbRefItem> & gItem2 )
{
  bool res = (mateType != GCM_UNKNOWN) || GetInt( IDS_SELECT_DEPENDENCE, mateType ); // Ввод целого числа (Укажите зависимость)

  if ( res ) 
  {
    MbRefItem * subObj1 = nullptr;
    MbRefItem * subObj2 = nullptr;
    PathMatrix pathFrom1, pathFrom2;

    // Укажите геометрический объект
    if ( GetRigidObj( IDS_SELECT_GEOM_OBJECT, obj1, subObj1, pathFrom1 ) ) 
    {
      if ( GetRigidObj( IDS_SELECT_GEOM_OBJECT, obj2, subObj2, pathFrom2 ) ) 
      {
        TestVariables::drawImp->DrawItem( subObj1, 200, 55, 0 );      
        TestVariables::drawImp->DrawItem( subObj2, 200, 55, 0 );
        gItem1 = subObj1;
        gItem2 = subObj2;
        return true;
      }
    }
  }

  return false;
}

//----------------------------------------------------------------------------------------
// Query a mating 3D-constraint arguments from GUI editor
//---
/*
static bool GetMateConstraintArgs(  int & mateType,
                                    MbItem *& obj1, SPtr<MbTopItem> & gItem1,
                                    MbItem *& obj2, SPtr<MbTopItem> & gItem2,
                                    double & realPar )
{
  bool res = (mateType != GCM_UNKNOWN) || GetInt( IDS_SELECT_DEPENDENCE, mateType ); // Ввод целого числа (Укажите зависимость)

  if ( res ) 
  {
    // Укажите геометрический объект
    MbTopItem * tItem1 = nullptr;
    MbTopItem * tItem2 = nullptr;
    res = GetTObj( IDS_SELECT_GEOM_OBJECT, st_Undefined, tt_TopItem, &obj1, &tItem1 );

    if ( res ) 
    {
      drawImp->DrawItem( tItem1, 200, 55, 0 );
      res = GetTObj( IDS_SELECT_GEOM_OBJECT, st_Undefined, tt_TopItem, &obj2, &tItem2 );
      if ( res && obj1 != nullptr && obj2 != nullptr )
      {
        drawImp->DrawItem( tItem2, 200, 55, 0 );
        gItem1 = tItem1;
        gItem2 = tItem2;

        if ( gItem1 != nullptr && gItem1 != nullptr )
        {
          if ( realPar != MB_MAXDOUBLE ) // MB_MAXDOUBLE - означает, что вещественный параметр ненужен
          {
            res = GetDouble( IDS_ENTER_PARAMETER, realPar ); // Ввод вещественного числа
          }
        }
        else
          res = false;
      }
    }
  }

  return res;
}
*/

//----------------------------------------------------------------------------------------
// The assembly selected as "current".
// Сборка, выбранная в качестве "текущей".
// ---
static MbModel * _CurrentModel()
{
  if ( WorkWindow * wind = TestVariables::viewManager->GetActiveWindow() ) 
  {
    return &wind->SetGeomModel();
  }
  return nullptr;
}

//----------------------------------------------------------------------------------------
// Query or create a new assembly
// Запросить или создать новую сборку
// ---
static MbAssembly * _PrepareAssembly()
{
  if ( MbModel * model = _CurrentModel() ) 
  {
    for ( MbModel::ItemIterator iter = model->Begin(), last = model->End(); iter != last; ++iter )
    { 
      if ( MbAssembly * assembly = isa_cast<MbAssembly *>(*iter) )
      {
        return assembly;
      }
    }
    // There is no assembly, create a new.
    // До сих пор нет сборок - создадим новую.
    SPtr<MbAssembly> assembly( new MbAssembly );
    return static_cast<MbAssembly*>( model->AddItem(*assembly) );

  }
  return nullptr;
}


//----------------------------------------------------------------------------------------
// Подготовить объект для участия в системе ограничений
// Prepare the object to participate in a system of constraints.
//---
static void _Prepare( const MbItem * item )
{
  if ( MbModel * model = _CurrentModel() ) 
  {    
    if ( MbAssembly * assm = _PrepareAssembly() )
    {
      if ( item )
      {
        if ( item == model->SubItem(item->GetItemName()) )
        {
          SPtr<MbItem> itemSptr( const_cast<MbItem *>(item) );
          model->DetachItem( itemSptr );
          assm->AddItem( *itemSptr );
        }
      }
    }
  }
}

//----------------------------------------------------------------------------------------
// Создать сопряжение
// Create mate
//---
void CreateMateConstraint( int mateType )
{
  const GCM_c_type cType = static_cast<GCM_c_type>( mateType );
  MbItem * obj1 = nullptr;
  MbItem * obj2 = nullptr;
  SPtr<const MbRefItem> gItem1, gItem2;

  if ( GetConstraintArgs(mateType, obj1, gItem1, obj2, gItem2) )
  {
    _Prepare( obj1 );
    _Prepare( obj2 );
    MtGeomArgument g1( gItem1, obj1 );
    MtGeomArgument g2( gItem2, obj2 );
    MtParVariant   dVar;
    double dVal = 0.0;
    switch ( cType )
    {
      case GCM_ANGLE:
      case GCM_DISTANCE:
      {
        if ( GetDouble(IDS_ENTER_DIMENSION, dVal) )
        {
          dVar = dVal;
        }
        TestVariables::viewManager->AddConstraint( cType, g1, g2, dVar );
        break;
      }
      default:
        TestVariables::viewManager->AddConstraint( cType, g1, g2, MtParVariant::undef );
    }    
  }
}

//----------------------------------------------------------------------------------------
// Выбрать точку, принадлежащую геометрическому объекту модели.
// Select a point belonging to an geometric object of the model.
//---
static bool GetPoint( const TCHAR * strResID, MbCartPoint3D & pnt, SPtr<MbItem> & item, PathMatrix & pathFrom )
{
  if ( GetPoint(strResID, pnt) ) 
  {
    MbItem * iPtr = nullptr;
    MbRefItem * geom = nullptr;
    if ( WorkWindow * wind = TestVariables::viewManager->GetActiveWindow() ) 
    {
      if ( wind->FindGeometricItem( st_SpaceItem, tt_Undefined, pt_PlaneItem, TestVariables::cursorAxis, SENSE_RADIUS, &iPtr, &geom, pathFrom ) ) 
      {
        item = iPtr;
        return true;
      }
    }    
  }
  return false;
}

//----------------------------------------------------------------------------------------
// Получение контрольной точки для привязки к геометрическом объекту.
// Get a control point to snap to a geometric object.
//---
static GcArgument GetControlPoint( MbItem & item, const MbCartPoint3D & ptrPnt )
{
  GcArgument geom;

  if ( item.IsA() == st_PlaneInstance )
  {
    const MbPlaneInstance * pInst = static_cast<const MbPlaneInstance*>( &item );
    MbCartPoint pnt;
    pInst->GetPlacement().PointProjection(ptrPnt, pnt.x, pnt.y );
    if ( const MbPlaneItem * pItem = pInst->GetPlaneItem() )
    {
      if ( pItem->Family() == pt_Curve )
      {
        const MbCurve * crv = static_cast<const MbCurve*> ( pItem );
        MbCartPoint cp; // Контрольная точка
        
        double dis = MB_MAXDOUBLE;

        crv->GetLimitPoint( 1, cp );
        if ( pnt.DistanceToPoint(cp) < dis )
        {
          dis = pnt.DistanceToPoint( cp );
          geom.m_geom = GeomCode::FIRST_END;
        }     

        crv->GetLimitPoint( 2, cp );
        if ( pnt.DistanceToPoint(cp) < dis )
        {
          dis = pnt.DistanceToPoint( cp );
          geom.m_geom = GeomCode::SECOND_END;
        }        
        
        if ( crv->GetCentre( cp ) && pnt.DistanceToPoint(cp) < dis )
        {
          dis = pnt.DistanceToPoint( cp );
          geom.m_geom = GeomCode::CENTRE_POINT;
        } 

        if ( geom.m_geom != GeomCode::NULL_GEOM )
        {
          geom.m_item = &item;
        }
      }
    }
  }

  return geom;  
}

//----------------------------------------------------------------------------------------
// Получение кривую, как аргумент ограничения; например для касания
// Get a curve as argument of constraint; For example - one curve of tangent constraint.
//---
static GcArgument GetCurveArgument( MbItem & item )
{
  GcArgument geom;

  if ( MbPlaneInstance * pInst = isa_cast<MbPlaneInstance*>(&item) )
  { 
    if ( const MbPlaneItem * pItem = pInst->GetPlaneItem() )
    {
      switch( pItem->IsA() )
      {
      case pt_Arc:
        {
          const MbArc * arc = static_cast<const MbArc*> ( pItem );            
          geom.m_geom = arc->IsCircle() ? GeomCode::CIRCLE : GeomCode::ELLIPSE;
          break;
        }
      case pt_Line:
      case pt_LineSegment:
        {
          geom.m_geom = GeomCode::LINE;
          break;
        }
      case pt_PolyCurve:
      case pt_Polyline:
      case pt_Bezier:
      case pt_Hermit:
      case pt_Nurbs:
      case pt_CubicSpline:
        {
          geom.m_geom = GeomCode::SPLINE;
          break;
        }
      default: break;
      }

      if ( geom.m_geom != GeomCode::NULL_GEOM )
      {
        geom.m_item = &item;
      }    
    }
  }

  return geom;  
}

//----------------------------------------------------------------------------------------
// Совпадение пары точек.
// Coincidence of two points.
//---
void AddCoincident2Points() 
{
  MbCartPoint3D p1, p2;
  SPtr<MbItem> i1, i2;
  PathMatrix pathFrom1, pathFrom2;
  if ( GetPoint(IDS_SELECT_OBJECT_POINT, p1, i1, pathFrom1 ) ) 
  {
    if ( GetPoint(IDS_SELECT_OBJECT_POINT, p2, i2, pathFrom2 ) ) 
    {
      GcArgument g1 = GetControlPoint( *i1, p1 );
      GcArgument g2 = GetControlPoint( *i2, p2 );      
      TestVariables::viewManager->AddConstraint( MbConstraint(GCE_COINCIDENT, g1, g2) );
    }
  }
}

//----------------------------------------------------------------------------------------
// Добавить касание.
// Set a tangency.
//---
void AddTangency()
{
  MbCartPoint3D p1, p2;
  SPtr<MbItem> i1, i2;
  PathMatrix pathFrom1, pathFrom2;
  if ( GetPoint( IDS_SELECT_CURVE, p1, i1, pathFrom1 ) ) {
    if ( GetPoint( IDS_SELECT_CURVE, p2, i2, pathFrom2 ) ) {
      GcArgument g1 = GetCurveArgument( *i1 );
      GcArgument g2 = GetCurveArgument( *i2 );      
      TestVariables::viewManager->AddConstraint( MbConstraint(GCE_TANGENT, g1, g2) );
    }
  }
}

//----------------------------------------------------------------------------------------
// Добавить перпендикулярность
// Set a perpendicularity.
//---
void AddPerpendicular()
{
  MbCartPoint3D p1, p2;
  SPtr<MbItem> i1, i2;
  PathMatrix pathFrom1, pathFrom2;
  if ( GetPoint( IDS_SELECT_LINE, p1, i1, pathFrom1 ) ) 
  {
    if ( GetPoint( IDS_SELECT_LINE, p2, i2, pathFrom2 ) ) 
    {
      GcArgument g1 = GetCurveArgument( *i1 );
      GcArgument g2 = GetCurveArgument( *i2 );      
      TestVariables::viewManager->AddConstraint( MbConstraint(GCE_PERPENDICULAR, g1, g2) );
    }
  }
}

// eof
