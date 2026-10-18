////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief Создание топологических объектов по данным конвертеров.
*/
////////////////////////////////////////////////////////////////////////////////
#ifndef __CONV_TOPOLOGY_CREATOR_H
#define __CONV_TOPOLOGY_CREATOR_H


#include <vector>
#include <memory>
#include <templ_sptr.h>
#include <topology.h>
#include <se_elementary.h>

class MbSolid;

namespace c3d
{
  namespace converter
  {

    //------------------------------------------------------------------------------
    /** \brief \ru Менеджер вспомогательных операций работы с граничным представлением конвертеров.
               \en . \~
        \note \ru ЭКСПЕРИМЕНТАЛЬНАЯ.
              \en EXPEREIMENTAL. \~
    */
    class IConverterBrepManager
    {
    public:

      virtual bool MakeSTEPEntities( MbSolid const & c3dBrep, std::vector<std::shared_ptr<const converter::SeBase>>& enitites ) = 0;

      IConverterBrepManager() = default;
      virtual ~IConverterBrepManager() = default;

      private:
      IConverterBrepManager( IConverterBrepManager const & ) = delete;
      IConverterBrepManager( IConverterBrepManager && ) = delete;
      const IConverterBrepManager& operator =( IConverterBrepManager const & ) = delete;      
    };


    CONV_FUNC( std::shared_ptr<IConverterBrepManager> ) GetConverterBrepManager();

    CONV_FUNC( void ) ReleaseConverterBrepManager( std::shared_ptr<IConverterBrepManager>& );

    class ITopologyCreator;

    //------------------------------------------------------------------------------
    /** \brief \ru Перечисление типов ошибок входных данных.
               \en Enumeration of input error types . \~
        \note \ru ЭКСПЕРИМЕНТАЛЬНАЯ.
              \en EXPEREIMENTAL. \~
    */
    enum ETcCheckError {
      eTcErrLoopTopo,         /// \ru Цикл не замкнут топологически.               \en Loop is not closed topologically .
      eTcErrFacePlusMinus,    /// \ru Некорректные указания на левые/правые грани. \en Incorrect pointers to the left/right faces.
      eTcErrCurveType,        /// \ru Некорректный тип кривой.                     \en Incorrect curve type .
      eTcErrCount             /// \ru Количество типов ошибок.                     \en Number of error types.
    };

    //------------------------------------------------------------------------------
    /** \brief \ru Базовый класс топологических объектов.
               \en . \~
        \note \ru ЭКСПЕРИМЕНТАЛЬНАЯ.
              \en EXPEREIMENTAL. \~
    */
    class CONV_CLASS TcObject
    {
    protected:
      mutable std::vector< ETcCheckError > aErr;
      int thisID;

    public:
      /// \ru Конструктор. \en Constructor.
      TcObject();
      ///< \ru Конструктор копирования. \en Copy constructor.
      TcObject( const TcObject& init );
      /// \ru Деструктор. \en Destructor.
      ~TcObject() {};

      // Проверить исходные данные. \en Check input data.
      virtual bool CheckInputData( ITopologyCreator& topCreator ) const;
      // Исправить исходные данные. \en Heal input data.
      virtual bool HealAllInputData( ITopologyCreator& topCreator );
      virtual bool HealInputData( ITopologyCreator& topCreator, const ETcCheckError& err );
    };

    //------------------------------------------------------------------------------
    /** \brief \ru Грань.
               \en Face. \~
        \note \ru ЭКСПЕРИМЕНТАЛЬНАЯ.
              \en EXPEREIMENTAL. \~
    */
    class CONV_CLASS TcFace : public TcObject
    {
      SPtr<MbSurface> surface;    ///< \ru Поверхность грани. \en Face surface.
      std::vector<size_t> loops;  ///< \ru Индексы циклов в TopologyCreator. \en Loops indexes in TopologyCreator.
      bool faceSense;             ///< \ru Ориентация грани. \en Face orientation.
      SPtr<MbFace> mbFace;        ///< \ru Созданная грань. \en Created face.

    public:
      /// \ru Конструктор. \en Constructor.
      TcFace( MbSurface & _surface, const std::vector<size_t> & _loops, bool _faceSense );
      ///< \ru Конструктор копирования. \en Copy constructor.
      TcFace( const TcFace & init );
      ///< \ru Оператор присваивания. \en Assignment operator.
      const TcFace & operator= ( const TcFace & init );
      /// \ru Деструктор. \en Destructor.
      ~TcFace();
      /// \ru Выдать грань. \en Get face.
      MbFace * GetFace() const;

      /// \ru Создать грань. \en Create face.
      void Create( ITopologyCreator & topCreator );

      // Проверить исходные данные. \en Check input data.
      virtual bool CheckInputData( ITopologyCreator& topCreator) const;
      // Исправить исходные данные. \en Heal input data.
      virtual bool HealInputData( ITopologyCreator& topCreator, const ETcCheckError& err );
    };


    //------------------------------------------------------------------------------
    /** \brief \ru Цикл.
               \en Loop. \~
    */
    class CONV_CLASS TcLoop : public TcObject
    {
      std::vector<size_t> orientedEdges;  ///< \ru Индексы ориентированных ребер в TopologyCreator. \en Oriented edges indexes in TopologyCreator.
      SPtr<MbLoop> mbLoop;                ///< \ru Созданный цикл. \en Created loop.

    public:
      /// \ru Конструктор. \en Constructor.
      TcLoop( const std::vector<size_t> & _orientedEdges );
      ///< \ru Конструктор копирования. \en Copy constructor.
      TcLoop( const TcLoop & init );
      ///< \ru Оператор присваивания. \en Assignment operator.
      const TcLoop & operator= ( const TcLoop & init );
      /// \ru Деструктор. \en Destructor.
      ~TcLoop();
      /// \ru Выдать цикл. \en Get loop.
      MbLoop * GetLoop() const;
      /// \ru Выдать ориентированные ребра. \en Get oriented edges.
      void GetOrientedEdges( std::vector<size_t>& aEdges ) const;

      /// \ru Создать цикл. \en Create loop.
      void Create( ITopologyCreator & topCreator );

      // Проверить исходные данные. \en Check input data.
      virtual bool CheckInputData( ITopologyCreator& topCreator) const;
      // Исправить исходные данные. \en Heal input data.
      virtual bool HealInputData( ITopologyCreator& topCreator, const ETcCheckError& err );
    };


    //------------------------------------------------------------------------------
    /** \brief \ru Ориентированное ребро.
               \en Oriented edge. \~
        \note \ru ЭКСПЕРИМЕНТАЛЬНАЯ.
              \en EXPEREIMENTAL. \~
    */
    class CONV_CLASS TcOrientedEdge : public TcObject
    {
      size_t curveEdge;  ///< \ru Индекс ребра в TopologyCreator. \en Edge index in TopologyCreator.
      bool orient;       ///< \ru Ориентация ребра. \en Edge orientation.
      SPtr<MbOrientedEdge> mbOrientedEdge;  ///< \ru Созданное ориентированное ребро. \en Created oriented edge.

    public:
      /// \ru Конструктор. \en Constructor.
      TcOrientedEdge( size_t _curveEdge, bool _orient );
      ///< \ru Конструктор копирования. \en Copy constructor.
      TcOrientedEdge( const TcOrientedEdge & init );
      ///< \ru Оператор присваивания. \en Assignment operator.
      const TcOrientedEdge & operator= ( const TcOrientedEdge & init );
      /// \ru Деструктор. \en Destructor.
      ~TcOrientedEdge();
      /// \ru Выдать ориентированное ребро. \en Get oriented edge.
      MbOrientedEdge * GetOrientedEdge() const;
      /// \ru Выдать индекс грани справа от ребра. \en Get index of the face to the right of the edge.
      size_t GetFacePlusIndex( ITopologyCreator& topCreator ) const;
      /// \ru Выдать индекс грани слева от ребра. \en Get index of the face to the left of the edge.
      size_t GetFaceMinusIndex( ITopologyCreator& topCreator ) const;
      /// \ru Выдать ориентацию ребро. \en Get orientation.
      bool   GetOrient()    const { return orient;  }
      /// \ru Выдать индекс ребра. \en Get index of the edge.
      size_t GetCurveEdge() const { return curveEdge; }

      /// \ru Создать ориентированное ребро. \en Create oriented edge.
      void Create( ITopologyCreator & topCreator );
      //// Проверить исходные данные. \en Check input data.
      //virtual bool CheckInputData( TopologyCreator& topCreator) const;
      //// Исправить исходные данные. \en Heal input data.
      //virtual bool HealInputData( TopologyCreator& topCreator, const ETcCheckError& err );

    };


    //------------------------------------------------------------------------------
    /** \brief \ru Ребро.
               \en Edge. \~
        \note \ru ЭКСПЕРИМЕНТАЛЬНАЯ.
              \en EXPEREIMENTAL. \~
    */
    class CONV_CLASS TcCurveEdge : public TcObject
    {
      SPtr<MbCurve3D> curve3D;        ///< \ru Трехмерная кривая ребра. \en 3D curve of edge.
      size_t facePlus, faceMinus;     ///< \ru Индексы граней ребра в TopologyCreator. \en Edge faces indexes in TopologyCreator.
      size_t begVertex, endVertex;    ///< \ru Индексы вершин ребра в TopologyCreator. \en Edge vertices indexes in TopologyCreator.
      SPtr<MbCurveEdge> mbCurveEdge;  ///< \ru Созданное ребро. \en Created edge.

    public:
      /// \ru Конструктор. \en Constructor.
      TcCurveEdge( MbCurve3D & _curve3D, size_t _facePlus, size_t _faceMinus, size_t _begVertex, size_t _endVertex );
      ///< \ru Конструктор копирования. \en Copy constructor.
      TcCurveEdge( const TcCurveEdge & init );
      ///< \ru Оператор присваивания. \en Assignment operator.
      const TcCurveEdge & operator= ( const TcCurveEdge & init );
      /// \ru Деструктор. \en Destructor.
      ~TcCurveEdge();
      /// \ru Выдать ребро. \en Get edge.
      MbCurveEdge * GetCurveEdge() const;
      /// \ru Выдать индекс начальной вершины. \en Get index of the begin vertex.
      size_t GetBegVertex( const bool orient ) const { return orient? begVertex : endVertex; }
      /// \ru Выдать индекс конечной вершины. \en Get index of the end vertex.
      size_t GetEndVertex( const bool orient ) const { return orient? endVertex : begVertex; }
      /// \ru Выдать индекс грани справа от ребра. \en Get index of the face to the right of the edge.
      size_t GetFacePlus() const { return facePlus; }
      /// \ru Выдать индекс грани слева от ребра. \en Get index of the face to the left of the edge.
      size_t GetFaceMinus() const { return faceMinus; }

      /// \ru Создать ребро. \en Create edge.
      void Create( ITopologyCreator & topCreator );

      // Проверить исходные данные. \en Check input data.
      bool CheckInputData( ITopologyCreator& topCreator ) const final;
      // Исправить исходные данные. \en Heal input data.
      bool HealInputData( ITopologyCreator& topCreator, const ETcCheckError& err ) final;
    };


    //------------------------------------------------------------------------------
    /** \brief \ru Вершина.
               \en Vertex. \~
        \note \ru ЭКСПЕРИМЕНТАЛЬНАЯ.
              \en EXPEREIMENTAL. \~
    */
    class CONV_CLASS TcVertex : public TcObject
    {
      MbCartPoint3D point;      ///< \ru Трехмерная точка. \en 3D point.
      SPtr<MbVertex> mbVertex;  ///< \ru Созданная вершина. \en Created vertex.

    public:
      /// \ru Конструктор. \en Constructor.
      TcVertex( const MbCartPoint3D & _point );
      ///< \ru Конструктор копирования. \en Copy constructor.
      TcVertex( const TcVertex & init );
      ///< \ru Оператор присваивания. \en Assignment operator.
      const TcVertex & operator= ( const TcVertex & init );
      /// \ru Деструктор. \en Destructor.
      ~TcVertex();
      /// \ru Выдать вершину. \en Get vertex.
      MbVertex * GetVertex() const;

      /// \ru Создать вершину. \en Create vertex.
      void Create( ITopologyCreator & topCreator );
      //// Проверить исходные данные. \en Check input data.
      //virtual bool CheckInputData( TopologyCreator& topCreator) const;
      //// Исправить исходные данные. \en Heal input data.
      //virtual bool HealInputData( TopologyCreator& topCreator, const ETcCheckError& err );
    };


    //------------------------------------------------------------------------------
    /** \brief \ru Класс для создания тела по данным топологии.
               \en Class for creating a solid by topology data. \~
        \note \ru ЭКСПЕРИМЕНТАЛЬНАЯ.
              \en EXPEREIMENTAL. \~
    */
    class CONV_CLASS ITopologyCreator
    {

    public:

      /// \ru Конструктор по умолчанию пустой. \en Default constructor.
      ITopologyCreator() = default;

      /// \ru Деструктор. \en Destructor.
      virtual ~ITopologyCreator() = default;

      /// \ru Инициализировать. \en Initialize.
      virtual void Init( const std::vector<TcFace> & _faces, const std::vector<TcLoop> & _loops, const std::vector<TcOrientedEdge> & _orientedEdges, const std::vector<TcCurveEdge> & _curveEdges, const std::vector<TcVertex> & _vertices ) = 0;

      /// \ru Выдать грань. \en Get face.
      virtual TcFace * GetFace( size_t ) = 0;
      /// \ru Выдать цикл. \en Get loop.
      virtual TcLoop * GetLoop( size_t ) = 0;
      /// \ru Выдать ориентированное ребро. \en Get oriented edge.
      virtual TcOrientedEdge * GetOrientedEdge( size_t ) = 0;
      /// \ru Выдать ребро. \en Get edge.
      virtual TcCurveEdge * GetCurveEdge( size_t ) = 0;
      /// \ru Выдать вершину. \en Get vertex.
      virtual TcVertex * GetVertex( size_t ) = 0;

      /// \ru Создать тело. \en Create solid.
      virtual SPtr<MbSolid> CreateSolid() = 0;

    private:

      ITopologyCreator( ITopologyCreator const & ) = delete;
      ITopologyCreator( ITopologyCreator && ) = delete;
      ITopologyCreator const & operator=( ITopologyCreator const & ) = delete;
      
    };

    /// \ru Получить формирователь граничного представления C3D по 3D-данным. \en Get C3D boundary representation generator.
    CONV_FUNC( std::shared_ptr<ITopologyCreator> ) GetC3DTopologyBuilder();

    /// \ru Завершить работу формирователя граничного представления формата STEP. \en Release STEP boundary representation generator.
    CONV_FUNC (void) ReleaseC3DTopologyBuilder( std::shared_ptr<ITopologyCreator>& );


  };
};

#endif //__CONV_TOPOLOGY_CREATOR_H