////////////////////////////////////////////////////////////////////////////////
//
// \ru DXF - конвертер.
// \en DXF - converter. \~
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __DXF_DATA_H
#define __DXF_DATA_H 

#include <templ_s_array.h>
#include <templ_p_array.h>
#include <templ_array2.h>
#include <curve.h>
#include <mb_placement3d.h>
#include <attribute_container.h>
#include <iostream>


class MbCartPoint;
class MbCurve;
class MbCartPoint3D;
class MbVector3D;
class MbPlacement3D;
class MbCurve3D;
class MbSolid;
class MbFace;
class MbGrid;
class DXFConverter;


//------------------------------------------------------------------------------
// \ru Объект формата DXF.
// \en Object of DXF format. \~
// ---
class CONV_CLASS DXFEntity {
protected:
  MbAttributeContainer attributes; ///< \ru Атрибуты. \en Attributes. 

protected:
  // \ru Конструктор. \en Constructor. 
  DXFEntity();
  // \ru Деструктор. \en Destructor. 
  virtual ~DXFEntity();

public:
          // \ru Установить цветовые атрибуты. \en Set color attributes. 
          void SetAttributes( MbAttributeContainer & attribs );
  virtual bool Convert      ( DXFConverter & converter ) = 0;

private:
  DXFEntity              ( const DXFEntity & ); // \ru не реализовано \en not implemented 
  DXFEntity & operator = ( const DXFEntity & ); // \ru не реализовано \en not implemented 

};


//------------------------------------------------------------------------------
// \ru Анализатор потока SAT.
// \en SAT stream analyzer. \~
// ---
class CONV_CLASS DXFModelerGeometry : public DXFEntity {
private:
  std::iostream & out; //< \ru анализируемый поток. \en stream being analyzed. 

public:
  DXFModelerGeometry( std::iostream & out );
  virtual ~DXFModelerGeometry();

  virtual bool Convert( DXFConverter & converter );

private:
  DXFModelerGeometry              ( const DXFModelerGeometry & ); // \ru не реализовано \en not implemented 
  DXFModelerGeometry & operator = ( const DXFModelerGeometry & ); // \ru не реализовано \en not implemented 

};


//------------------------------------------------------------------------------
// \ru Грань.
// \en Face. \~
// ---
class CONV_CLASS DXFFace : public DXFEntity {
private:
  // \ru Цикл.
  // \en Loop. \~
  // \ru Цикл объявлен внутри DXFFace.
  // \en The Loop is declared inside DXFFace. \~
  class DXFLoop {
  public:
    SArray<MbCartPoint3D> points; // \ru Набор точек. \en Point set. 

  public:
    DXFLoop( const SArray<MbCartPoint3D> & points );
    ~DXFLoop();

  private:
    DXFLoop              ( const DXFLoop & ); // \ru не реализовано \en not implemented 
    DXFLoop & operator = ( const DXFLoop & ); // \ru не реализовано \en not implemented 

  };

private:
  PArray<DXFLoop> loops; ///< \ru Набор циклов. \en Loop set. 

public:
  DXFFace( const SArray<MbCartPoint3D> & points );
  virtual ~DXFFace();

  virtual bool      Convert ( DXFConverter & converter              );
          MbFace *  MakeFace(                                       ) const;
          MbGrid*   MakeGrid(                                       ) const;
          void      AddHole ( const SArray<MbCartPoint3D> & points  );
          void      Scale   ( double factor                         );

private:
  DXFFace              ( const DXFFace & ); // \ru не реализовано \en not implemented 
  DXFFace & operator = ( const DXFFace & ); // \ru не реализовано \en not implemented 

};


//------------------------------------------------------------------------------
// \ru Сеть на основе граней.
// \en Mesh on the base of faces. \~
// ---
class CONV_CLASS DXFPolyfaceMesh : public DXFEntity {
private:
  const PArray<DXFFace> & faces; // \ru Набор граней. \en Face set. 

public:
  DXFPolyfaceMesh( const PArray<DXFFace> & faces );
  virtual ~DXFPolyfaceMesh();

  virtual bool Convert( DXFConverter & converter );
          void Scale  ( double factor            );

private:
  DXFPolyfaceMesh              ( const DXFPolyfaceMesh & ); // \ru не реализовано \en not implemented 
  DXFPolyfaceMesh & operator = ( const DXFPolyfaceMesh & ); // \ru не реализовано \en not implemented 

};


//------------------------------------------------------------------------------
// \ru Сеть на основе вершин DXF.
// \en Mesh on the base of DXF vertices. \~
// ---
class CONV_CLASS DXFPolygonMesh : public DXFEntity {
private:
  Array2<MbCartPoint3D> & points;   // \ru Набор вершин. \en Vertex set. 
  bool                    uclosed;  // \ru Признак замкнутости по u. \en Flag of closedness by u. 
  bool                    vclosed;  // \ru Признак замкнутости по v. \en Flag of closedness by v. 

public:
  DXFPolygonMesh( Array2<MbCartPoint3D> & points, bool uclosed, bool vclosed );
  virtual ~DXFPolygonMesh();

  virtual bool Convert( DXFConverter & converter );
          void Scale  ( double factor            );

private:
  DXFPolygonMesh              ( const DXFPolygonMesh & ); // \ru не реализовано \en not implemented 
  DXFPolygonMesh & operator = ( const DXFPolygonMesh & ); // \ru не реализовано \en not implemented 
};


//------------------------------------------------------------------------------
// \ru Составная кривая.
// \en Polyline. \~
// ---
class CONV_CLASS DXFPolyline : public DXFEntity {
public:
  // \ru Сегмент составной кривой.
  // \en Polyline segment. \~
  // \ru Класс объявлен внутри DXFPolyline;
  // The class is declared inside DXFPolyline; \~
  class CONV_CLASS DXFSegment {
  private:
            MbCurve & curve;  // \ru Кривая. \en A curve. 
            double    width1; // \ru Толщина. \en The thickness. 
            double    width2; // \ru Толщина. \en The thickness. 
    
    mutable SPtr<MbCurve> left;
    mutable SPtr<MbCurve> right;
    mutable SPtr<MbCurve> top;
    mutable SPtr<MbCurve> bottom;

  public:
    DXFSegment( MbCurve & _curve, double _width1, double _width2 );
    ~DXFSegment();

    const MbCurve & Curve       () const { return curve;  }
          double    Width1      () const { return width1; }
          double    Width2      () const { return width2; }
          bool      IsWidth1Zero() const { return (width1 < NULL_EPSILON);  }
          bool      IsWidth2Zero() const { return (width2 < NULL_EPSILON);  }
          void      ChangeLeft  ( MbCurve & left   ) const;
          void      ChangeRight ( MbCurve & right  ) const;
          void      ChangeTop   ( MbCurve & top    ) const;
          void      ChangeBottom( MbCurve & bottom ) const;
    const MbCurve * GetLeft     () const { return left;   }
    const MbCurve * GetRight    () const { return right;  }
    const MbCurve * GetTop      () const { return top;    }
    const MbCurve * GetBottom   () const { return bottom; }
          void      MakeContours( std::vector< SPtr<MbContour> > & contours ) const;

  private:
    DXFSegment              ( const DXFSegment & ); // \ru не реализовано \en not implemented 
    DXFSegment & operator = ( const DXFSegment & ); // \ru не реализовано \en not implemented 

  };

private:
   const PArray<DXFSegment> & segments;   // \ru Сегменты. \en Segments. 
         bool                 closed;     // \ru Признак замкнутости. \en Flag of closedness. 
         MbPlacement3D        placement;  // \ru Локальная система координат. \en Local coordinate system. 
         MbVector3D           direction;  // \ru Направление. \en Direction. 

public:
  DXFPolyline( const PArray<DXFSegment> & _segments, bool _closed, const MbPlacement3D & _placement, const MbVector3D & _direction );
  virtual ~DXFPolyline();

  virtual bool Convert( DXFConverter & converter );
          void Scale  ( double factor            );

private:
  DXFPolyline              ( const DXFPolyline & ); // \ru не реализовано \en not implemented 
  DXFPolyline & operator = ( const DXFPolyline & ); // \ru не реализовано \en not implemented 

};


//------------------------------------------------------------------------------
// \ru Кривая.
// \en A curve. \~
// ---
class CONV_CLASS DXFCurve : public DXFEntity {
private:
  MbCurve3D & curve;      // \ru Кривая. \en A curve. 
  MbVector3D  direction;  // \ru Направление. \en Direction. 

public:
  DXFCurve( MbCurve3D & _curve, const MbVector3D & _direction );
  virtual ~DXFCurve();

  virtual bool Convert( DXFConverter & converter );
          void Scale  ( double factor            );

private:
  DXFCurve              ( const DXFCurve & ); // \ru не реализовано \en not implemented 
  DXFCurve & operator = ( const DXFCurve & ); // \ru не реализовано \en not implemented 

};


//------------------------------------------------------------------------------
// \ru Пространственная кривая.
// \en A space curve. \~
// \ru Используется для передачи каркасных моделей.
// \en Used for wireframe models transfer. \~
// ---
class CONV_CLASS DXFCurve3D : public DXFEntity {
private:
  MbCurve3D & curve;  // \ru Кривая. \en A curve. 

public:
  DXFCurve3D( MbCurve3D & _curve );
  virtual ~DXFCurve3D();

  virtual bool Convert( DXFConverter & converter );
          void Scale  ( double factor            );

private:
  DXFCurve3D              ( const DXFCurve3D & ); // \ru не реализовано \en not implemented 
  DXFCurve3D & operator = ( const DXFCurve3D & ); // \ru не реализовано \en not implemented 

};


//------------------------------------------------------------------------------
// \ru Точка.
// \en Point. \~
// ---
class CONV_CLASS DXFPoint : public DXFEntity {
private:
  MbCartPoint3D point;      // \ru Точка. \en A point. 
  MbVector3D    direction;  // \ru Направление. \en Direction. 

public:
  DXFPoint( const MbCartPoint3D & _point, const MbVector3D & _direction );
  virtual ~DXFPoint();

  virtual bool Convert( DXFConverter & converter );
          void Scale  ( double factor            );

private:
  DXFPoint              ( const DXFPoint & ); // \ru не реализовано \en not implemented 
  DXFPoint & operator = ( const DXFPoint & ); // \ru не реализовано \en not implemented 

};


//------------------------------------------------------------------------------
//
// ---
void StitchFacesAndCreateSolids( const RPArray<MbFace> & faces, std::vector< SPtr<MbSolid> > & solids );


//------------------------------------------------------------------------------
//
// ---
void UnStitchFacesAndCreateSolids( const RPArray<MbFace> & faces, std::vector< SPtr<MbSolid> > & solids );


#endif // __DXF_DATA_H 