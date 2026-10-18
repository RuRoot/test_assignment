////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Маркер.
         \en Marker. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __MARKER_H
#define __MARKER_H


#include <legend.h>
#include <mb_cart_point3d.h>


//------------------------------------------------------------------------------
/** \brief \ru Маркер со свойствами геометрического объекта.
           \en Marker with properties of a geometric object. \~
  \par \ru Определение
    Маркером называется тройка объектов: точка и пара ортонормированных векторов.
    Например, в системе КОМПАС-3D маркером задается "присоединительная точка",
    которая применяется в качестве геометрического коннектора для сопряжения и
    позиционирования тел в пространстве.\n
       \en Definition
    The marker is a triple of objects: a point and a pair of orthonormalized vectors.
    For example, in the COMPAS-3D system marker sets the "connecting point", 
    which is used as geometric connector for conjugation and
    positioning of solids in space.\n \~

  \details \ru Термин "Маркер" позаимствован из книги Г.Крамера, Geometric constraint solving in kinematics.\n    
      С помощью маркера можно задавать вспомогательные построения, передавать 
    геометрию кинематических соединений или сопряжений. Маркер всегда принадлежит 
    какому-то подпространству, например, ЛСК твердого тела.\n
      Для маркера всегда выполняется требование; его вектора (в отличие от локальной 
    системы координат) всегда ортонормированы. Ось Z всегда нормирована, ось X всегда 
    ортогональна Z (может быть, что X = 0).  Маркер может задавать любые геометрические 
    объекты, которые удобно задать точкой или векторами; это унифицированная форма записи 
    таких объектов, как точка, прямая, плоскость, ортонормированная правая СК и т.д.
    При дополнении маркера радиусом он используется, как сжатая форма записи цилиндра, 
    окружности, сферы, тора и т.д. \n
           \en The term "Marker" is taken from the book "Geometric constraint solving in kinematics" (G.Kramera ).\n    
      With the help of marker it is possible to set auxiliary constructions and to transmit 
    geometry of kinematic compounds or conjugations. The marker always belongs 
    to some subspace, for example, to LSC of solid. \n
      For the marker the following requirement is always satisfied: its vectors (as opposed to the local 
    coordinate system) are always orthonormalized. Z-axis is always normalized, X-axis is always  
    orthogonal to Z (it could be that X = 0).  Marker can set any geometric 
    objects which are easy to set by point or vectors, this is a unified form of writing 
    of objects as point, line, plane, right orthonormal CS etc.
    When adding a radius to a marker it is used as a compressed form of writing of cylinder, 
    circle, sphere, torus etc. \n \~
  \sa #MtMarker, #MtUnifiedGeom, #MtMatingGeometry, #MtMatingGeom
  \ingroup Legend
*/
// ---
class MATH_CLASS MbMarker: public MbLegend 
{
  MbCartPoint3D origin; ///< \ru Точка маркера. \en Marker point. 
  MbVector3D    axisZ;  ///< \ru Нормированный вектор оси OZ. \en Normalized vector of OZ-axis. 
  MbVector3D    axisX;  ///< \ru Ортонормированный вектор оси OX. \en Orthonormalized vector of OX-axis. 

public:
  /// \ru Конструктор копирования. \en Copy constructor. 
  MbMarker( const MbMarker & );
  /// \ru Конструктор по точке и вектору. \en Constructor by a point and a vector. 
  MbMarker( const MbCartPoint3D &, const MbVector3D & );
  /// \ru Конструктор по точке и векторам. \en Constructor by a point and vectors. 
  MbMarker( const MbCartPoint3D &, const MbVector3D &, const MbVector3D & );  
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbMarker();

public:

  // \ru Общие функции геометрического объекта \en Common functions of a geometric object  
  MbeSpaceType  IsA()  const override; // \ru Тип объекта. \en Type of the object. 
  MbeSpaceType  Type() const override; // \ru Тип объекта. \en Type of the object. 
  MbSpaceItem & Duplicate( MbRegDuplicate * = nullptr ) const override; // \ru Создать копию. \en Create a copy.
  void    Transform( const MbMatrix3D &, MbRegTransform * = nullptr ) override; // \ru Преобразовать согласно матрице. \en Transform according to the matrix.
  void    Move( const MbVector3D &, MbRegTransform * = nullptr ) override; // \ru Сдвинуть вдоль вектора. \en Translate along a vector.
  void    Rotate( const MbAxis3D   &, double angle, MbRegTransform * = nullptr ) override; // \ru Повернуть вокруг оси. \en Rotate around an axis.
  bool    IsSame( const MbSpaceItem & other, double accuracy = LENGTH_EPSILON ) const override;     // \ru Являются ли объекты равными? \en Determine whether objects are equal. 
  bool    IsSimilar( const MbSpaceItem & init ) const override;  // \ru Являются ли объекты подобными? \en Determine whether objects are similar. 
  bool    SetEqual ( const MbSpaceItem & init ) override;        // \ru Сделать объекты равным. \en Make objects equal. 
  double  DistanceToPoint ( const MbCartPoint3D & ) const override; // \ru Вычислить расстояние до точки. \en Calculate the distance to a point. 
  void    AddYourGabaritTo( MbCube & r ) const override;         // \ru Добавь свой габарит в куб. \en Add bounding box into a cube. 
  void    CalculateMesh( const MbStepData & stepData, const MbFormNote & note, MbMesh & mesh ) const override; // \ru Построить полигональную копию mesh. \en Build polygonal copy mesh.
  
  // \ru Свойства \en Properties 
  MbProperty &  CreateProperty( MbePrompt n ) const override;      // \ru Создать собственное свойство. \en Create a custom property.
  void    GetProperties( MbProperties & properties ) override;   // \ru Выдать свойства объекта. \en Get properties of the object. 
  void    SetProperties( const MbProperties & properties ) override;   // \ru Установить свойства объекта. \en Set properties of the object. 
  void    GetBasisPoints( MbControlData3D & ) const override; // \ru Выдать контрольные точки объекта. \en Get control points of object.
  void    SetBasisPoints( const MbControlData3D & ) override; // \ru Изменить объект по контрольным точкам. \en Change the object by control points.

  /** \ru \name Функции маркера.
      \en \name Functions of marker.
      \{ */
          /// \ru Получить точку маркера. \en Get point of marker. 
  const   MbCartPoint3D & GetOrigin() const { return origin; }
          /// \ru Получить ось маркера. \en Get axis of marker. 
  const   MbVector3D    & GetAxisX() const { return axisX; }
          /// \ru Получить вторую ось маркера. \en Get the second axis of marker. 
  const   MbVector3D    & GetAxisZ() const { return axisZ; }
          /// \ru Задать нулевую ось X. \en Set the X-axis to null. 
          void            SetNullX() { axisX.SetZero(); }
          /// \ru Перевернуть ось Z. \en Invert the Z-axis.
          MbMarker &      InvertZ();

  /** \} */

private:  
  MbMarker & operator = ( const MbMarker & );          

DECLARE_PERSISTENT_CLASS_NEW_DEL( MbMarker )
};


IMPL_PERSISTENT_OPS( MbMarker )


#endif // __MARKER_H
