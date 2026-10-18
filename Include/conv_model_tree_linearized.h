#ifndef MODEL_TREE_LINEARIZED_H
#define MODEL_TREE_LINEARIZED_H

#include <conv_model_document.h>
#include <mb_matrix.h>
#include <solid.h>
#include <vector>


//------------------------------------------------------------------------------
// Структура устарела и будет удалена до 1.06.2026.
// Данные терминатора
// ---
struct DEPRECATE_DECLARE TerminatorData
{
  int type;
  double parameter;
  double sizeX;
  double sizeY;
  bool sameDirection;
};


//------------------------------------------------------------------------------
// Структура устарела и будет удалена до 1.06.2026.
// Данные кривой с терминаторами
// ---
struct DEPRECATE_DECLARE DecoratedCurveData 
{
  int curveType;
  std::vector<TerminatorData> terminators;
};


//------------------------------------------------------------------------------
// Структура устарела и будет удалена до 1.06.2026.
// Данные размера
// ---
struct DEPRECATE_DECLARE DimensionData
{
  double value;
  double valuePlus;
  double valueMinus;
  bool isRangeSet;
  bool isValueDefined;
  std::vector<DecoratedCurveData> decoratedCurves;
};


//------------------------------------------------------------------------------
// Класс и его методы устарели и будут удалены до 1.06.2026.
// Контейнер элементов с вхождениями
// ---
template <typename ReferencedItemType>
class DEPRECATE_DECLARE LinearizedContainer {
  
  ReferencedItemType item;
  mutable std::vector<MbMatrix3D> references;
public:
  // Конструктор по умолчанию
  LinearizedContainer() = default;

  // Конструктор копирования по умолчанию
  LinearizedContainer(const LinearizedContainer& i) = default;

  // Конструктор
  LinearizedContainer(const ReferencedItemType& it)
    : item( it )
    , references() {
  }

  // Оператор сравнения
  bool operator ==(const LinearizedContainer& to) const {
    return item == to.item;
  }

  // Получить элемент-ключ
  ReferencedItemType Item() const {
    return item;
  }

  // Получить число вхождений
  size_t ReferencesCount() const {
    return references.size();
  }

  // Получить матрицу вхождения по индексу
  MbMatrix3D Reference( size_t index ) const {
    MbMatrix3D result;
    if ( index < ReferencesCount() )
      result = references[index];
    return result;
  }

  // Добавить матрицу вхождения
  void AddReference( const MbMatrix3D& matr ) {
    references.push_back( matr );
  }
};

typedef std::vector<LinearizedContainer<ModelPartPtr>> PartsLinearized;
typedef std::vector<LinearizedContainer<ModelAssemblyPtr>> AssembliesLinearized;
typedef std::vector<LinearizedContainer<c3d::SolidSPtr>> SolidsLinearized;

//------------------------------------------------------------------------------
// Класс и его методы устарели и будут удалены до 1.06.2026.
// Документ
// ---
//DEPRECATE_DECLARE
class CONV_CLASS LinearizedTreeDocument : public C3dModelDocument
{
  PartsLinearized parts;
  AssembliesLinearized assemblies;
  SolidsLinearized solids;
  std::vector<SPtr<MbCurve3D>> spaceCurves;
  std::vector<SPtr<MbMesh>> meshes;
  vector_of_annotation annotations;
  vector_of_annotation techReq;
  //IFC data
  std::vector<SPtr<MbInstance>> mappedItems;
  std::vector<SPtr<MbItem>>     axisGrids;
  std::vector<SPtr<MbAssistingItem>>     lscItems;
  std::vector<SPtr<MbItem>>     boundaryItems;

  bool calculateMIP;
  bool sliceSolids;

public:
  // Конструктор
  DEPRECATE_DECLARE
  LinearizedTreeDocument();

  // Деструктор
  DEPRECATE_DECLARE
  ~LinearizedTreeDocument();

  DEPRECATE_DECLARE
  PartsLinearized GetPartsList() const;

  DEPRECATE_DECLARE
  AssembliesLinearized GetAssembliesList() const;

  // Заполнить данные
  DEPRECATE_DECLARE
  void FillLinearizedData();

private:
  // Заполнены ли данные
  DEPRECATE_DECLARE
  bool IsFilled();

  DEPRECATE_DECLARE
  bool GetSolid( const size_t ind, c3d::SolidSPtr& sld ) const;
  
  DEPRECATE_DECLARE
  void SetSliceSolids( const bool val ) { sliceSolids = val; }

  // Заполнить списки компонент
  DEPRECATE_DECLARE
  void FillComponents();
  // Заполнить списки объектов
  DEPRECATE_DECLARE
  void FillObjectsFromComponents();
  // Обработать сборку со вставками
  DEPRECATE_DECLARE
  void ProcessAssembly( ModelAssemblyPtr assm, const MbMatrix3D & matr );
  // Добавить деталь
  DEPRECATE_DECLARE
  void AddPart( ModelPartPtr prt, const MbMatrix3D & matr );
  // Добавить сборку
  DEPRECATE_DECLARE
  void AddAssembly( ModelAssemblyPtr assm, const MbMatrix3D & matr );
  // Добавить тело
  DEPRECATE_DECLARE
  void AddSolid( c3d::SolidSPtr sld, const MbMatrix3D & matr );
    // Добавить объекты, специфичные для IFC
  template<typename PartAssType>
  DEPRECATE_DECLARE
  void AddIFCObjects(PartAssType part);
  // Добавить содержимое mappedItem
  DEPRECATE_DECLARE
  void AddMappedItemContent( MbItem &item, const MbMatrix3D& matr = MbMatrix3D());

  // Выдать данные размера
  DEPRECATE_DECLARE
  void GetPmiDimensionData( MaDimension & dimension, DimensionData & dimData );
  // Сбросить данные размера
  DEPRECATE_DECLARE
  void UnsetPmiDimensionData( DimensionData & dimData );
  // Выдать данные кривой с терминаторами
  DEPRECATE_DECLARE
  void GetPmiDecoratedCurveData( MaDecoratedCurve & decoratedCurve, DecoratedCurveData & decCurveData );
  // Выдать данные терминатора
  DEPRECATE_DECLARE
  void GetPmiTerminatorData( MaTerminatorSymbol & terminator, TerminatorData & termData );
  // Выдать идентификатор
  DEPRECATE_DECLARE
  bool GetCadItemIdentifier( const MbAttributeContainer& item, ptrdiff_t & id );
  // Резка тела координатными плоскостями
  DEPRECATE_DECLARE
  void SolidPlaneCutting( const MbSolid & solid, std::vector<MbResultType> & cutRes );
  // Резка тела
  DEPRECATE_DECLARE
  MbResultType SolidCut( const MbSolid & solid, const MbSurface & surf, bool forward );
};

#endif //MODEL_TREE_LINEARIZED_H