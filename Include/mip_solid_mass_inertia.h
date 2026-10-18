////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Инерционные характеристики тела.
         \en Inertial properties of solid. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __MIP_SOLID_MASS_INERTIA_H
#define __MIP_SOLID_MASS_INERTIA_H


#include <templ_p_array.h>
#include <mb_matrix3d.h>
#include <solid.h>


struct IfProgressIndicator;


/// \ru Неинициализированное значение double. \en Uninitialized value of double. 
constexpr double NOT_INITIAL_DBL = -DETERMINANT_MAX;


//------------------------------------------------------------------------------
/** \brief \ru Инерционные характеристики тела.
           \en Inertial properties of solid. \~
  \details \ru Инерционные характеристики тела.\n
    Векторы direction дают направления главных осей инерции. \n
    Если все главные моменты инерции general[i] i=1,2,3 разные, 
    то все векторы direction[i] i=1,2,3 не равны нулю. \n
    Если все главные моменты инерции general[i] i=1,2,3 одинаковые, 
    то все векторы direction[i] i=1,2,3 равны нулю и 
    главными направлениями могут служить любые три взаимно ортогональных вектора. \n
    Если два из трех главных моментов инерции равны, например general[j]==general[k], 
    то два из трёх векторов равны нулю direction[j]=direction[k]=0, 
    а не равный нулю вектор direction[i] определяет направление главной оси инерции, 
    момент general[i] относительно которой отличается от других, 
    двумя другими главными направлениями могут служить любые два взаимно ортогональных 
    и ортогональных не равному нулю вектору direction[i] вектора.
           \en Inertial properties of solid.
    Vectors direction give directions of the principal axes of inertia.\n
    If all principal moments of inertia (general[i] i=1,2,3) different,
    all vectors (direction[i] i=1,2,3) not zero.\n
    If all principal moments of inertia (general[i] i=1,2,3) are the same,
    all vectors (direction[i] i=1,2,3) zero and
    principal directions can be any three mutually orthogonal vectors.\n
    If two of the three principal moments of inertia are equal, for example (general[j]==general[k]),
    then two of the three vectors are zero (direction[j]=direction[k]=0),
    a non-zero vector (direction[i]) defines the direction of the principal axis of inertia,
    time general[i] with respect to which differs from other.
          \n \~
  \ingroup Inertia_Computation
*/
// ---
class MATH_CLASS InertiaProperties {
public :
  double area;                                    ///< \ru Площадь поверхности. \en Surface area. 
  double volume;                                  ///< \ru Объем. \en Volume. 
  double mass;                                    ///< \ru Масса. \en Mass. 
  double inertia[c3d::SPACE_DIM];                 ///< \ru Статические моменты. \en Static moments. 
  double initial[c3d::SPACE_DIM][c3d::SPACE_DIM]; ///< \ru Моменты инерции относительно исходных осей координат. \en Moments of inertia relative to source coordinate axes. 
  double moments[c3d::SPACE_DIM][c3d::SPACE_DIM]; ///< \ru Моменты инерции относительно центральных осей координат. \en Moments of inertia relative to central coordinate axes. 
  double general[c3d::SPACE_DIM];                 ///< \ru Главные центральные моменты инерции. \en Principal central moments of inertia. 

  MbCartPoint3D center;                           ///< \ru Центр тяжести. \en Center of gravity. 
  MbVector3D    direction[c3d::SPACE_DIM];        ///< \ru Векторы направлений главных центральных осей инерции. \en Direction vectors of the principal central axes of inertia. 

public:
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор инерционных характеристик с умолчательными значениями полей.\n
      Умолчательные значения означают, что параметры не заданы.
             \en Constructor of inertial properties with default fields values.\n
      Default values mean that the parameters are not set. \~
  */
  InertiaProperties() { Init(); }
  /// \ru Конструктор копирования. \en Copy constructor. 
  InertiaProperties( const InertiaProperties & other ) { operator = ( other ); }
  /// \ru Деструктор. \en Destructor. 
  ~InertiaProperties() {}
    
  /** \brief \ru Инициализация.
             \en Initialization. \~
    \details \ru Инициализация инерционных характеристик умолчательными значениями полей.\n
      Умолчательные значения означают, что параметры не заданы.
             \en Initialization of inertial properties with default fields values.\n
      Default values mean that the parameters are not set. \~
  */
  void        Init();

  /** \brief \ru Проверить данные.
             \en Check data. \~
    \details \ru Проверить данные на корректность.\n
             \en Check data for correctness.\n \~
    \return \ru true, если данные корректны,\n
      данные могут быть некорректны в случае нулевой площади поверхности.
            \en True if the data is correct,\n
      data may be incorrect in case of zero surface area. \~
  */
  bool        CheckData(); 
  
  /** \brief \ru Определить массо-центровочных характеристики.
             \en Determine mass-inertial properties. \~
    \details \ru Определить массо-центровочных характеристики набора граней.\n
      Каждое тело представлено совокупностью граней, описывающих его поверхность. 
    Определение объёма, центра масс и моментов инерции тела приводит к вычислению объёмных интегралов. 
    С помощью формулы Остроградского-Гаусса интегралы по объёму тела сводятся к 
    интегралам по поверхностям граней тела.\n
      При численном интегрировании по поверхности область определения параметров грани 
    разбивается на небольшие четырёхугольные или треугольные подобласти. 
    От размеров подобластей зависит точность вычисления.
    В качестве управляющего параметра разбиения области интегрирования используется 
    угловое отклонение нормали поверхности в подобласти deviateAngle.
    Размер каждой подобласти определён условием: 
    угловое изменение нормали поверхности в подобласти не должно превышать deviateAngle.\n
      Для четырёхугольных подобластей по каждому параметру поверхности 
    интегрирование выполняется с помощью квадратурных формул Гаусса. 
    Для треугольных областей удобно перейти от координат u и v к 
    трём барицентрическим координатам a, b, c, построенным по точкам pa, pb, pc. 
    Координаты произвольной точки p=(u,v) через барицентрические координаты a, b, c выражаются с помощью формул\n
    u = a ua + b ub + c uc,\n 
    v = a va + b vb + c vc.\n 
      Барицентрические координаты удовлетворяют равенству: a + b + c = 1.\n
      Во всех случаях каждый поверхностный интеграл вычисляется как взвешенная сумма 
    значений интегрируемой функции внутри области интегрирования.
    Для каждого отдельного тела расчёт выполняется в местной системе координат.
    Затем характеристики тела переводятся в глобальную систему координат и суммируются.
    Результатом являются характеристики сборки тел в глобальной, 
    центральной и главной центральной системе координат.
             \en Determine mass-inertial properties of set of faces.\n
      Each solid is represented by a set of faces, describing its surface. 
    Determination of volume, center of mass and moments of inertia of the solid leads to the calculation of the volume integrals. 
    By means of the divergence (Gauss-Ostrogradsky) theorem volume integrals of the solid are reduced to the 
    integrals over the surfaces of the solid faces.\n
      Numerical integration over the surface involves face parametric domain 
    subdivision into small rectangular or triangular subdomains. 
    Computational accuracy depends on sizes of subdomains.
    As driving parameter of integration domain partitioning used 
    angular deviation of surface normal in 'deviateAngle' subdomain.
    Size of each subdomain is defined by condition: 
    angular deviation of surface normal in subdomain shouldn't exceed deviateAngle.\n
      For quadrangular subdomains 
    integration by each surface parameter is performed using the Gauss quadratures. 
    For triangular subdomains it is handy to transform u and v coordinates to 
    three barycentric coordinates a, b, c, constructed by points pa, pb, pc. 
    Coordinates of arbitrary point p=(u,v) has corresponding barycentric notation:\n
    u = a ua + b ub + c uc,\n 
    v = a va + b vb + c vc.\n 
      Barycentric coordinates satisfy the equation: a + b + c = 1.\n
      In all cases each surface integral is calculated as weighted summ 
    of integrable function values inside integration domain.
    Calculation is performed in local coordinate system for each particular solid.
    Then solid properties are transformed to global coordinate system and summarized.
    Result is solids assembly properties in global, 
    central and principal central coordinate system. \~
    \note    \ru В многопоточном режиме выполняется параллельно
                 (временно: многопоточность работает, только если IfProgressIndicator не определен).
             \en In multithreaded mode m_Items runs in parallel
                 (temporarily: multithreading is working only if IfProgressIndicator is not defined). \~
    \param[in] faces        - \ru Набор граней.
                              \en A set of faces. \~
    \param[in] closed       - \ru Замкнутость набора граней.
                              \en Closing of set of faces. \~
    \param[in] density      - \ru Плотность (closed == true) или удельная масса на единицу площади (closed == false).
                              \en Density (closed == true) or mass per unit square (closed == false). \~
    \param[in] deviateAngle - \ru Параметр управления точностью расчёта - угловое отклонение нормпли поверхности или касательных кривой на участке численного интегрирования.
                              \en Tolerance - the angular deviation of surface or curve in the neighboring points on the region of numerical integration. \~
    \param[in] progress     - \ru Индикатор прогресса выполнения.
                              \en A run progress indicator. \~
  */
  void        CalculateIntegrals ( const RPArray<MbFace> & faces, bool closed, double density,
                                   double deviateAngle, IfProgressIndicator * progress );

  /** \brief \ru Определить масс-центровочных характеристики.
             \en Determine mass-inertial properties. \~
    \details \ru Определить масс-центровочных характеристики набора граней.\n
             \en Determine mass-inertial properties of set of faces.\n \~
    \param[in] faces        - \ru Набор граней.
                              \en A set of faces. \~
    \param[in] closed       - \ru Замкнутость набора граней.
                              \en Closing of set of faces. \~
    \param[in] density      - \ru Плотность (closed == true) или удельная масса на единицу площади (closed == false).
                              \en Density (closed == true) or mass per unit square (closed == false). \~
    \param[in] calculateAll - \ru Если false, проводится расчет интегралов инерции,\n
                              если true, то еще проводится расчет центра масс, центральных моментов инерции, главных центральных моментов инерции.
                              \en If false, then performs integrals of inertia calculation,\n
                              if true, then performs also calculation of center of mass, central moments of inertia, principal central moments of inertia. \~
    \param[in] deviateAngle - \ru Параметр управления точностью расчёта - угловое отклонение нормали поверхности или касательных кривой на участке численного интегрирования.
                              \en Tolerance - the angular deviation of surface or curve in the neighboring points on the region of numerical integration. \~
    \param[in] progress     - \ru Индикатор прогресса выполнения.
                              \en A run progress indicator. \~
  */
  void        CalculateProperties( const RPArray<MbFace> & faces, bool closed, double density, 
                                   bool calculateAll, double deviateAngle, 
                                   IfProgressIndicator * progress ); 
  
  /** \brief \ru Определить масс-центровочных характеристики.
             \en Determine mass-inertial properties. \~
    \details \ru Определить масс-центровочных характеристики полигонального объекта.\n
             \en Determine mass-inertial properties of polygonal object.\n \~
    \param[in] mesh    - \ru Полигональный объект.
                         \en A polygonal object. \~
    \param[in] density - \ru Плотность (closed == true) или удельная масса на единицу площади (closed == false).
                         \en Density (closed == true) or mass per unit square (closed == false). \~
  */
  void        CalculateIntegrals( const MbMesh & mesh, double density );
             
  /** \brief \ru Определить масс-центровочных характеристики.
             \en Determine mass-inertial properties. \~
    \details \ru Определить масс-центровочных характеристики полигонального объекта.\n
             \en Determine mass-inertial properties of polygonal object.\n \~
    \param[in] mesh    - \ru Полигональный объект.
                         \en A polygonal object. \~
    \param[in] density - \ru Плотность или удельная масса на единицу площади.
                         \en Density or mass per unit square. \~
    \param[in] calculateAll - \ru Если false, проводится расчет интегралов инерции,\n
                              если true, то еще проводится расчет центра масс, центральных моментов инерции, главных центральных моментов инерции.
                              \en If false, then performs integrals of inertia calculation,\n
                              if true, then performs also calculation of center of mass, central moments of inertia, principal central moments of inertia. \~
  */
  void        CalculateProperties( const MbMesh & mesh, double density, bool calculateAll );

  /** \brief \ru Вычислить интегралы инерции.
             \en Calculate integrals of inertia. \~
    \details \ru Вычислить интегралы инерции по заданным пользователем моментам инерции.\n
      Присваивает себе значение площади, объема, массы, центра масс эталона,
      пересчитывает моменты инерции относительно центральных осей координат по заданным 
      моментам инерции эталона.
             \en Calculate integrals of inertia by user-given moments of inertia.\n
      Assigns the values of area, volume, mass, reference center of mass,
      recalculates moments of inertia relative to central coordinate axes by given 
      reference moments of inertia. \~
    \param[in] etalon  - \ru Эталон.
                         \en Reference. \~
  */
  void        GetIntegrals( const InertiaProperties & etalon );

  /** \brief \ru Учесть частично заданных характеристик.
             \en Consider partially defined properties. \~
    \details \ru Пересчитать интегралы инерции с учётом частично заданных пользователем моментов инерции.\n
      Присваивает себе значение площади, объема, массы, центра масс эталона, моментов инерции,
      если эти величины у эталона заданы.\n
      Площадь, объем, масса, центр масс, моменты инерции объекта не изменяются, 
      если соответствующая величина у эталона не задана.
             \en Recalculate integrals of inertia with user-given partially defined moments of inertia.\n
      Assigns the values of area, volume, mass, reference center of mass, moments of inertia,
      if reference values are defined.\n
      Area, volume, mass, center of mass, moments of inertia of an object are not changed 
      if corresponding reference values are not defined. \~
    \param[in] prop - \ru Частично заданные инерционные характеристики.
                      \en Partially defined inertial properties. \~ 
  */
  void        CrossIntegrals( const InertiaProperties & prop ); 

  /** \brief \ru Рассчитать главную центральную систему.
             \en Calculate principal central coordinate system. \~
    \details \ru Расчёт главной центральной системы координат и проверка.\n
      Входит: расчет центра масс и центральных моментов инерции,
      расчет главных центральных моментов инерции.
             \en Calculation and checking of principal central coordinate system.\n
      Included: calculation of center of mass and central moments of inertia,
      calculation of principal central moments of inertia. \~
    \return \ru true в случае корректных данных.
            \en True if data is correct. \~
  */
  bool        Calculate(); 
  
  /** \brief \ru Рассчитать центр масс и центральные моменты инерции.
             \en Calculate center of mass and central moments of inertia. \~
    \details \ru Рассчитать центр масс и центральные моменты инерции.\n
             \en Calculate center of mass and central moments of inertia.\n \~       
    \return \ru true в случае корректных данных.
            \en True if data is correct. \~
  */
  bool        CalculateCenter( bool isCheckData = true );  
         
  /** \brief \ru Рассчитать главные центральные моменты инерции.
             \en Calculate principal central moments of inertia. \~
    \details \ru Рассчитать главные центральные моменты инерции.\n
             \en Calculate principal central moments of inertia.\n \~       
    \return \ru true в случае корректных данных.
            \en True if data is correct. \~
  */
  bool        CalculateGeneral(); 

  /** \brief \ru Трансформировать.
             \en Transform. \~
    \details \ru Трансформировать данные по матрице.\n
             \en Transform data by matrix.\n \~       
    \param[in] m - \ru Матрица трансформации.
                   \en Transformation matrix. \~
  */
  void        Transform( const MbMatrix3D & m );

  /** \brief \ru Трансформировать.
             \en Transform. \~
    \details \ru Трансформировать интегралы инерции в соответствии с матрицей.\n
             \en Transform integrals of inertia according to matrix.\n \~       
    \param[in] m - \ru Матрица трансформации.
                   \en Transformation matrix. \~
  */
  void        IntegralsTransform( const MbMatrix3D & m );

  /// \ru Добавить к полям объекта полей другого объекта. \en Add fields of other object to current.  
  void        Add( const InertiaProperties & );
  /// \ru Изменить плотность. \en Change density. 
  void        ChangeDensity( double density );
  /// \ru Являются ли объекты равными? \en Determine whether objects are equal. 
  bool        IsSame( const InertiaProperties & other, double accuracy ) const;
  
  /// \ru Оператор присваивания. \en Assignment operator. 
  void        operator = ( const InertiaProperties & );

}; // InertiaProperties


typedef InertiaProperties SolidMIProperties;


//------------------------------------------------------------------------------
/** \brief \ru Тело, его характеристики и матрицы преобразования копий тела.
           \en Solid, its properties and transformation matrices of solid duplicates. \~
  \details \ru Тело, его характеристики и матрицы преобразования копий тела.\n
           \en Solid, its properties and transformation matrices of solid duplicates.\n \~
  \ingroup Inertia_Computation
*/
// ---
class MATH_CLASS SolidMIAttire {
private :
  const MbSolid &     solid;      ///< \ru Тело. \en A solid. 
  double              density;    ///< \ru Плотность или удельная масса на единицу площади. \en Density or mass per unit square. 
  MbMatrix3D          matrix;     ///< \ru Матрица преобразования тела в систему ближайшей сборки (хозяина). \en A matrix of solid transformation to the coordinate system of nearest assembly (owner). 
  InertiaProperties * properties; ///< \ru Характеристики тела (может быть nullptr). \en Solid properties (can be nullptr).
  bool                ready;      ///< \ru Флаг, показывающий, что характеристики не требуется считать. \en Flag of already calculated properties. 

public:
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор объекта с нулевыми характеристиками тела.\n
             \en Constructor of object with zero solid properties.\n \~
    \param[in] s - \ru Тело.
                   \en A solid. \~
    \param[in] d - \ru Плотность (s.IsClosed()) или удельная масса на единицу площади (!s.IsClosed()).
                   \en Density (s.IsClosed()) or mass per unit square (!s.IsClosed()). \~
    \param[in] d - \ru Плотность.
                   \en Density. \~
    \param[in] m - \ru Матрица преобразования.
                   \en A transform matrix. \~  
  */
  SolidMIAttire( const MbSolid & s, double d, const MbMatrix3D & m );
  /// \ru Деструктор. \en Destructor. 
  ~SolidMIAttire();

  /**\ru \name Функции доступа к данным.
     \en \name Functions for access to data.
      \{ */
  
  /// \ru Тело. \en A solid. 
  const MbSolid & GetSolid() const { return solid; } 
 /// \ru Плотность тела. \en A solid density. 
  double      GetDensity() const { return density; } 
  /// \ru Матрица преобразования. \en A transform matrix. 
  const MbMatrix3D & GetMatrix() const { return matrix; } 
  /// \ru Инерционные характеристики тела. \en Inertial properties of solid. 
  const InertiaProperties * GetProperties() const { return properties; } 
  /// \ru Флаг, показывающий, что характеристики посчитаны. \en Flag of already calculated properties. 
  bool        IsReady() const { return ready; } 
  
  /** \} */  
  /**\ru \name Функции изменения данных.
     \en \name Functions for changing data.
      \{ */    
  
  /// \ru Установить характеристики тела. \en Set solid properties.       
  void        SetProperties( InertiaProperties & p );
  /// \ru Установить флаг, показывающий, что характеристики посчитаны. \en Set flag of already calculated properties. 
  void        SetReady( bool r = true ) { ready = r; }

  /** \} */  
  /**\ru \name Функции расчета данных.
     \en \name Functions for calculating data.
      \{ */      

  /** \brief \ru Расчёт аддитивных характеристик тела.
             \en Calculation of additive solid properties. \~
    \details \ru Расчёт аддитивных характеристик тела.\n
             \en Calculation of additive solid properties.\n \~
    \param[in]  deviateAngle - \ru Параметр управления точностью расчёта - угловое отклонение нормали поверхности или касательных кривой на участке численного интегрирования.
                               \en Tolerance - the angular deviation of surface or curve in the neighboring points on the region of numerical integration. \~
    \param[out] mp           - \ru Рассчитанные инерционные характеристики с учётом пользовательских данных.
                               \en Inertial properties calculated with user-defined data. \~
    \param[in]  progress     - \ru Индикатор прогресса выполнения. Для прекращения долгих вычислений.
                               \en A run progress indicator. For termination of slow computations. \~
  */
  void        CalculateAdditiveValues( double deviateAngle, InertiaProperties & mp, 
                                       IfProgressIndicator * progress = nullptr ) const;
  /** \} */

  // \ru Объявление конструктора копирования и оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration without Implementation of the copy constructor and assignment operator to prevent an assignment by default. 
  OBVIOUS_PRIVATE_COPY( SolidMIAttire )
}; // SolidMIAttire


//------------------------------------------------------------------------------
/** \brief \ru Сборка.
           \en Assembly. \~
  \details \ru Сборка, её подсборки, тела и характеристики.\n
           \en Assembly, its subassembly, solids and properties.\n \~
  \ingroup Inertia_Computation
*/
// ---
class MATH_CLASS AssemblyMIAttire {
private :
  RPArray<AssemblyMIAttire> assemblies; ///< \ru Подсборки. \en Subassemblies. 
  RPArray<SolidMIAttire>    solids;     ///< \ru Тела сборки. \en Solids in an assembly. 
  MbMatrix3D                matrix;     ///< \ru Матрица преобразования сборки в систему ближайшей сборки (хозяина). \en A matrix of assembly transformation to the coordinate system of nearest assembly (owner). 
  InertiaProperties *       properties; ///< \ru Характеристики сборки (может быть nullptr). \en Assembly properties (can be nullptr).
  bool                      ready;      ///< \ru Характеристики не требуется считать. \en Properties already calculated. 

public:
  
  /** \brief \ru Конструктор.
             \en Constructor. \~
    \details \ru Конструктор объекта с нулевыми характеристиками тела.\n
      Данные массивов перекладываются в массивы класса, 
      где будут уничтожены при деструктурировании.
             \en Constructor of object with zero solid properties.\n
      Arrays data are moved to arrays of class 
      and will be deleted at destruction. \~
    \param[in] a - \ru Набор подсборок.
                   \en A set of subassemblies. \~
    \param[in] s - \ru Набор тел.
                   \en A set of solids. \~
    \param[in] m - \ru Матрица преобразования.
                   \en A transform matrix. \~  
  */
  AssemblyMIAttire( RPArray<AssemblyMIAttire> & a, RPArray<SolidMIAttire> & s, 
                    const MbMatrix3D & m = MbMatrix3D::identity );

  /// \ru Деструктор. \en Destructor. 
  ~AssemblyMIAttire();

  /**\ru \name Функции доступа к данным.
     \en \name Functions for access to data.
      \{ */   

  /// \ru Набор подсборок. \en A set of subassemblies. 
  const RPArray<AssemblyMIAttire> & GetAssemblies() const { return assemblies; } 
  /// \ru Набор тел. \en A set of solids.    
  const RPArray<SolidMIAttire> & GetSolids() const { return solids; } 
  /// \ru Матрица преобразования. \en A transform matrix.   
  const MbMatrix3D & GetMatrix() const { return matrix; } 
  /// \ru Инерционные характеристики. \en Inertial properties. 
  const InertiaProperties * GetProperties() const { return properties; } 
  /// \ru Готовы ли инерционные характеристики? \en Properties already calculated. 
  bool        IsReady()       const { return ready; } 
  /// \ru Вычислить количество граней. \en Calculate faces count. 
  size_t      GetFacesCount() const;                       
  
  /** \} */  
  /**\ru \name Функции изменения данных.
     \en \name Functions for changing data.
      \{ */    
                
  /// \ru Установить инерционные характеристики. \en Set inertial properties.   
  void        SetProperties( InertiaProperties & p ); 
  /// \ru Флаг готовых инерционных характеристик. \en Flag of already calculated properties. 
  void        SetReady( bool r = true ) { ready = r; } 
  
  /** \} */  
  /**\ru \name Функции расчета данных.
     \en \name Functions for calculating data.
      \{ */       

  /** \brief \ru Расчёт аддитивных характеристик тела.
             \en Calculation of additive solid properties. \~
    \details \ru Расчёт аддитивных характеристик тела.\n
             \en Calculation of additive solid properties.\n \~
    \note    \ru В многопоточном режиме выполняется параллельно.
             \en In multithreaded mode runs in parallel. \~
    \param[in]  deviateAngle - \ru Параметр управления точностью расчёта - угловое отклонение нормали поверхности или касательных кривой на участке численного интегрирования.
                               \en Tolerance - the angular deviation of surface or curve in the neighboring points on the region of numerical integration. \~
    \param[out] mp           - \ru Рассчитанные инерционные характеристики с учётом пользовательских данных.
                               \en Inertial properties calculated with user-defined data. \~
    \param[in]  progress     - \ru Индикатор прогресса выполнения. Для прекращения долгих вычислений.
                               \en A run progress indicator. For termination of slow computations. \~
  */
  void        CalculateAdditiveValues( double deviateAngle, InertiaProperties & mp, 
                                       IfProgressIndicator * progress = nullptr ) const;
  /** \} */  

  // \ru Объявление конструктора копирования и оператора присваивания без реализации, чтобы не было присваивания по умолчанию. \en The declaration without Implementation of the copy constructor and assignment operator to prevent an assignment by default. 
  OBVIOUS_PRIVATE_COPY( AssemblyMIAttire )
}; // AssemblyMIAttire

                                        
//------------------------------------------------------------------------------
/** \brief \ru Вычисление масс-центровочных характеристик.
           \en Calculation of mass-inertial properties. \~
  \details \ru Вычисление масс-центровочных характеристик тела.
           \en Calculation of solid mass-inertial properties. \~
  \note    \ru В многопоточном режиме mtm_Items выполняется параллельно.
           \en In multithreaded mode mtm_Items runs in parallel. \~
  \param[in]  solid        - \ru Тело.
                             \en A solid. \~
  \param[in]  density      - \ru Плотность (solid->IsClosed()) или удельная масса на единицу площади (!solid->IsClosed()).
                             \en Density (solid->IsClosed()) or mass per unit square (!solid->IsClosed()). \~
  \param[in]  deviateAngle - \ru Параметр управления точностью расчёта - угловое отклонение нормали поверхности или касательных кривой на участке численного интегрирования.
                             \en Tolerance - the angular deviation of surface or curve in the neighboring points on the region of numerical integration. \~
  \param[out] mp           - \ru Рассчитанные инерционные характеристики.
                             \en Calculated inertial properties. \~
  \param[in]  progress     - \ru Индикатор прогресса выполнения.
                             \en A run progress indicator. \~
  \ingroup Inertia_Computation
*/
// ---
MATH_FUNC (void) MassInertiaProperties( const MbSolid *             solid, 
                                              double                density,
                                              double                deviateAngle, // (0.35 - 0.01)
                                              InertiaProperties &   mp, 
                                              IfProgressIndicator * progress = nullptr );


//------------------------------------------------------------------------------
/** \brief \ru Вычисление масс-центровочных характеристик.
           \en Calculation of mass-inertial properties. \~
  \details \ru Вычисление масс-центровочных характеристик тел.
           \en Calculation of mass-inertial properties of solids. \~
  \note    \ru В многопоточном режиме выполняется параллельно.
           \en In multithreaded mode runs in parallel. \~
  \param[in]  solids       - \ru Тела.
                             \en Solids. \~
  \param[in]  densities    - \ru Плотности тел или удельная масса на единицу площади.\n
                             Количество элементов в массиве должно совпадать с количеством тел.
                             \en Density of solids or mass per unit square of solids.\n
                             Count of elements in array must be equal to count of solids. \~
  \param[in]  matrs        - \ru Матрицы преобразования тел в глобальную систему координат.\n
                             Количество элементов в массиве должно совпадать с количеством тел.
                             \en Matrices of solids transformation to global coordinate system.\n
                             Count of elements in array must be equal to count of solids. \~
  \param[in]  mpSolids     - \ru Имеющиеся характеристики тел. Может содержать nullptr.\n
                             Количество элементов в массиве должно совпадать с количеством тел.
                             \en Calculated properties of solids. Can contain nullptr.\n
                             Count of elements in array must be equal to count of solids. \~
  \param[in]  deviateAngle - \ru Параметр управления точностью расчёта - угловое отклонение нормали поверхности или касательных кривой на участке численного интегрирования.
                             \en Tolerance - the angular deviation of surface or curve in the neighboring points on the region of numerical integration. \~
  \param[out] mp - \ru Рассчитанные инерционные характеристики.
                   \en Calculated inertial properties. \~
  \param[in]  progress - \ru Индикатор прогресса выполнения.
                         \en A run progress indicator. \~
  \ingroup Inertia_Computation
*/
// ---
MATH_FUNC (void) MassInertiaProperties( const RPArray<MbSolid> &           solids, 
                                        const SArray<double> &             densities, 
                                        const SArray<MbMatrix3D> &         matrs,
                                        const RPArray<InertiaProperties> & mpSolids,
                                              double                       deviateAngle, // (0.35 - 0.01)
                                              InertiaProperties &          mp,
                                              IfProgressIndicator *        progress = nullptr );


//------------------------------------------------------------------------------
/** \brief \ru Вычисление масс-центровочных характеристик.
           \en Calculation of mass-inertial properties. \~
  \details \ru Вычисление масс-центровочных характеристик сборки.
           \en Calculation of mass-inertial properties of assembly. \~
  \note    \ru В многопоточном режиме m_Items выполняется параллельно.
           \en In multithreaded mode m_Items runs in parallel. \~
  \param[in]  assembly     - \ru Сборка.
                             \en Assembly. \~    
  \param[in]  deviateAngle - \ru Параметр управления точностью расчёта - угловое отклонение нормали поверхности или касательных кривой на участке численного интегрирования.
                             \en Tolerance - the angular deviation of surface or curve in the neighboring points on the region of numerical integration. \~
  \param[out] mp           - \ru Рассчитанные инерционные характеристики.
                             \en Calculated inertial properties. \~
  \param[in]  progress     - \ru Индикатор прогресса выполнения.
                             \en A run progress indicator. \~
  \ingroup Inertia_Computation
*/
// ---
MATH_FUNC (void) MassInertiaProperties( const AssemblyMIAttire &    assembly, 
                                              double                deviateAngle, // (0.35 - 0.01)
                                              InertiaProperties &   mp,
                                              IfProgressIndicator * progress = nullptr );


//------------------------------------------------------------------------------
/** \brief \ru Вычисление масс-центровочных характеристик.
           \en Calculation of mass-inertial properties. \~
  \details \ru Вычисление масс-центровочных характеристик полигонального объекта.
           \en Calculation mass-inertial properties of polygonal object. \~
  \note    \ru В многопоточном режиме выполняется параллельно.
           \en In multithreaded mode runs in parallel. \~
  \param[in]  mesh         - \ru Полигональный объект.
                             \en A polygonal object. \~
  \param[in]  density      - \ru Плотность или удельная масса на единицу площади.
                             \en Density or mass per unit square. \~
  \param[out] mp           - \ru Рассчитанные инерционные характеристики.
                             \en Calculated inertial properties. \~
  \ingroup Inertia_Computation
*/
// ---
MATH_FUNC (void) MassInertiaProperties( const MbMesh *            mesh, 
                                              double              density,
                                              InertiaProperties & mp );


//------------------------------------------------------------------------------
/** \brief \ru Вычисление масс-центровочных характеристик.
           \en Calculation of mass-inertial properties. \~
  \details \ru Вычисление масс-центровочных характеристик множества полигональных объектов.
           \en Calculation of mass-inertial properties of polygonal objects. \~
  \note    \ru В многопоточном режиме выполняется параллельно.
           \en In multithreaded mode runs in parallel. \~
  \param[in]  meshes       - \ru Множество полигональных объектов.
                             \en Set of polygonal objects. \~
  \param[in]  densities    - \ru Плотности объектов или удельная масса на единицу площади.\n
                             Количество элементов в массиве должно совпадать с количеством объектов.
                             \en Density of solids or mass per unit square of polygonal objects.\n
                             Count of elements in array must be equal to count of polygonal objects. \~
  \param[in]  matrices     - \ru Матрицы преобразования полигональных объектов в глобальную систему координат.\n
                             Количество элементов в массиве должно совпадать с количеством объектов.
                             \en Matrices of solids transformation to global coordinate system.\n
                             Count of elements in array must be equal to count of polygonal objects. \~
  \param[out] mp - \ru Рассчитанные инерционные характеристики.
                   \en Calculated inertial properties. \~
  \ingroup Inertia_Computation
*/
// ---
MATH_FUNC (void) MassInertiaProperties( const std::vector<const MbMesh *> & meshes, 
                                        const std::vector<double> &         densities, 
                                        const std::vector<MbMatrix3D> &     matrices, 
                                              InertiaProperties &           mp );


#endif // __MIP_SOLID_MASS_INERTIA_H
