#ifndef __TEST_RENDERING_H
#define __TEST_RENDERING_H


#include <test_service.h>


class    MbItem;
class    MbGrid;


//------------------------------------------------------------------------------
// Раскрасить объекты в отдельном окне
// ---
void ScanLine( RPArray<MbItem> & objs );


//------------------------------------------------------------------------------
// Раскрасить объекты в отдельном окне
// ---
void ScanLineRender( RPArray<MbGrid> & gridirons,
                     SArray<double> & ambient, SArray<double> & diffuse,
                     SArray<double> & specularity, SArray<double> & shininess,
                     SArray<double> & opacity, SArray<uint32> & solors );


//------------------------------------------------------------------------------
// Разбиение изображения на клетки и сортировка треугольников по клеткам
// ---
void SetGridBoxes( RPArray<MbGrid> & gridirons, ptrdiff_t tcount, MbRect & globalGab,
                   double X_DISCRETE, double Y_DISCRETE, ptrdiff_t discreteXCount, ptrdiff_t discreteYCount,
                   double factor, ptrdiff_t lineXCount, ptrdiff_t lineYCount,
                   PArray< SArray<ptrdiff_t> > & boxes );


//------------------------------------------------------------------------------
// Находится ли точка pnt в тени gridirons
// ---
bool GetVisibleNormal( MbGrid * gr, ptrdiff_t k, bool transpBuffer, double delta,
                       double & pointz, MbCartPoint3D & pnt, MbVector3D & norm );


//------------------------------------------------------------------------------
// Находится ли точка pnt в тени gridirons
// ---
bool InShadow( RPArray<MbGrid> & gridirons, MbCartPoint3D & pnt, MbVector3D & light0 );


//------------------------------------------------------------------------------
// Определение цвета
// ---
void GetColors( MbVector3D & light0, double ka, double kp, double ks, double kb, bool shade,
                double zmin, double zmax, double kz, double dz, double pointz, MbVector3D & norm,
                uint32 color, double & dColorR, double & dColorG, double & dColorB );


//------------------------------------------------------------------------------
// Раскрасить объекты в отдельном окне
// ---
void ChangeNormals( RPArray<MbGrid> & gridirons );


//------------------------------------------------------------------------------
// Изменить направление нормали
// ---
void SetNormalVector( MbVector3D & norm, int angle0 );


#endif // __TEST_RENDERING_H
