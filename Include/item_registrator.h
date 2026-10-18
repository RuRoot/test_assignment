////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Регистраторы объектов: копирования и трансформации.
         \en Registrators of objects: copying and transformation. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __ITEM_REGISTRATOR_H
#define __ITEM_REGISTRATOR_H


#include <math_define.h>
#include <reference_item.h>


class  MbRefItem;


//-------------------------------------------------------------------------------
/** \brief \ru Создать копию объекта с использованием регистратора сдублированных объектов.
           \en Create a copy of the object using the registrator of duplicated objects. \~
  \details \ru Создать копию объекта с использованием регистратора сдублированных объектов.
           Если копия объекта уже зарегистрирована, то получить ее из регистратора. \n
           \en Create a copy of the object using the registrator of duplicated objects.
           If a copy of the object is already registered then get it from the registrator. \n \~
  \ingroup Base_Algorithms
*/
// ---
#define __REG_DUPLICATE_IMPL( __CLASS )                       \
MbRefItem * copyItem = nullptr;                               \
if ( iReg == nullptr || !iReg->IsReg( this, copyItem ) ) {    \
  copyItem = new __CLASS;                                     \
  if ( iReg != nullptr )                                      \
    iReg->SetReg( this, copyItem );                           \
}


//-------------------------------------------------------------------------------
/** \brief \ru Регистратор копируемых объектов.
           \en Registrator of copied objects. \~
  \details \ru Регистратор используется для построения корректных копий объектов, 
    содержащих указатели на другие геометрические объекты. \n
    Объект может содержаться указателем в нескольких других объектах, подлежащих копированию. 
    Для предотвращения многократного копирования объекта используется регистратор.
    Регистратор представляет собой два синхронных массива.
    В первом массиве лежат указатели скопированных объектов, а во втором массиве лежат указатели их копий. \n 
    При копировании объекта с использованием регистратора проверяется наличие копируемого объекта в первом массиве.
    Если такой объект присутствует, то из второго массива выдаётся указатель на его копию.
    Если такой объект отсутствует, то он заносится в первый массив, а его созданная копия заносится во второй массив и выдаётся.
           \en Registrator is used to construct the correct copies of objects. 
    which contain pointers to other geometries. \n
    Object pointer can be contained in several other objects for copying. 
    Registrar is used to prevent multiple copying of the object.
    Registrator consists of two synchronous arrays.
    The first array contains pointers to copied objects, the second array contains pointers to their copies. \n 
    When copying the object using the registrator, the existence of the copied object inside the first array is verified.
    If such object exists, then the pointer to its copy is given from the second array.
    If such objects is absent, then it is stored in the first array, after that its copy is stored in the second array and then this copy is returned. \~
  \ingroup Base_Algorithms
*/
// ---
class MATH_CLASS MbRegDuplicate {
public:
  /// \ru Конструктор. \en Constructor. 
  MbRegDuplicate() {}
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbRegDuplicate();
public:
  /** \brief \ru Проверить, зарегистрирована ли копия объекта.
             \en Check whether copy of the object is registered. \~
    \details \ru Найти зарегистрированную копию объекта. \n
             \en Find a registered copy of the object. \n \~
    \param[in] srcItem - \ru Исходный объект.
                         \en The initial object. \~
    \param[out] cpyItem - \ru Зарегистрированная копия объекта.
                          \en Registered copy of the object. \~
    \return \ru Возращает true, если копия объекта уже зарегистрирована.
            \en Returns true if a copy of the object is already registered. \~
  */
  virtual bool IsReg  ( const MbRefItem * srcItem, MbRefItem *& cpyItem ) = 0;

  /** \brief \ru Зарегистрировать копию объекта.
             \en Register copy of the object. \~
    \details \ru Зарегистрировать копию объекта. \n
             \en Register copy of the object. \n \~
    \param[in] srcItem - \ru Исходный объект.
                         \en The initial object. \~
    \param[out] cpyItem - \ru Копия объекта.
                          \en The object copy. \~
  */
  virtual void SetReg ( const MbRefItem * srcItem, MbRefItem * cpyItem  ) = 0;

  /** \brief \ru Зарегистрировать копию объекта с проверкой наличия уже зарегистрированной копии.
             \en Register a copy of the object with verification of the presence of already registered copy. \~
    \details \ru Зарегистрировать копию объекта с проверкой наличия уже зарегистрированной копии.
              Если уже существует зарегистрированная копия объекта, то присланная копия не регистрируется и удаляется.
              Функция возвращает зарегистрированную копию объекта. \n
             \en Register a copy of the object with verification of the presence of already registered copy.
              If a registered copy of the object already exists, then the input copy is not registered and deleted.
              The function returns a registered copy of the object.
             \n \~
    \param[in] srcItem      - \ru Исходный объект.
                              \en The initial object. \~
    \param[in/out] cpyItem  - \ru Копия объекта. Удаляется, если уже существует зарегистрированная копия.
                              \en The object copy. Deleted if a registered copy already exists. \~
    \return \ru Возращает зарегистрированную копию объекта.
            \en Returns the registered copy of the object. \~
  */
  virtual MbRefItem * SetRegCheck ( const MbRefItem * srcItem, MbRefItem *& cpyItem  ) = 0;

  /** \brief \ru Удалить сведения об объекте из регистратора.
             \en Remove the object from the registrator. \~
    \details \ru Удалить сведения об объекте из регистратора. \n
             \en Remove the object from the registrator. \n \~
    \param[in] srcItem - \ru Указатель на объект.
                         \en Pointer to the object. \~
  */
  virtual void UnsetReg( const MbRefItem * srcItem ) = 0;

  /// \ru Освободить используемую память и удалить себя. \en Free memory and remove itself. 
  virtual void Free() = 0;

  /// \ru Очистить все данные. \en Clear all data.
  virtual void Clear() = 0;

OBVIOUS_PRIVATE_COPY( MbRegDuplicate )
};


//-------------------------------------------------------------------------------
/** \brief \ru Авторегистратор дублирования.
           \en Auto-registrator of duplication. \~
  \details \ru Автоматическое создание локального регистратора дублирования
    и забота о его удалении после использования. \n
           \en Automatic creation of the local registrator of duplication
    and taking care of its removal after use. \n \~
  \ingroup Base_Algorithms
*/
// ---
class MATH_CLASS MbAutoRegDuplicate {
private:
  /// \ru Локальный регистратор. \en The local registrator. 
  MbRegDuplicate * locReg;
public:
  /// \ru Конструктор. \en Constructor. 
  /// \param notifyRefItem - \ru Отслеживать разрушение зарегистрированных объектов.
  ///                        \en To track destruction of registered objects. 
  MbAutoRegDuplicate( MbRegDuplicate *&, bool notifyRefItem = false );
  /// \ru Деструктор. \en Destructor. 
  ~MbAutoRegDuplicate();

OBVIOUS_PRIVATE_COPY( MbAutoRegDuplicate )
};


//-------------------------------------------------------------------------------
/** \brief \ru Авторегистратор дублирования условию.
           \en Auto-registrator of duplication by a condition. \~
  \details \ru Автоматическое создание локального регистратора дублирования
            при выполнении условия и забота о его удалении после использования. \n
           \en Automatic creation of the local registrator of duplication
            by a condition and taking care of its removal after use. \n \~
  \ingroup Base_Algorithms
*/
// ---
class MATH_CLASS MbIfAutoRegDuplicate {
private:
  /// \ru Локальный регистратор. \en The local registrator. 
  MbRegDuplicate * locReg;
public:
  /// \ru Конструктор. \en Constructor. 
  /// \param createReg -      \ru Создавать ли регистратор.
  ///                         \en Whether to create a registrator. 
  /// \param notifyRefItem -  \ru Отслеживать разрушение зарегистрированных объектов.
  ///                         \en To track destruction of registered objects. 
  MbIfAutoRegDuplicate( MbRegDuplicate *&, bool createReg, bool notifyRefItem = false );
  /// \ru Деструктор. \en Destructor. 
  ~MbIfAutoRegDuplicate();

OBVIOUS_PRIVATE_COPY( MbIfAutoRegDuplicate )
};


//-------------------------------------------------------------------------------
/** \brief \ru Регистратор трансформируемых объектов.
           \en Registrator of transformable objects. \~
  \details \ru Регистратор используется для корректной трансформации объектов, 
    содержащих указатели на другие геометрические объекты. \n
    Объект может содержаться указателем в нескольких других объектах, подлежащих преобразованию.
    Для предотвращения многократного преобразования объекта используется регистратор.
    При преобразовании объекта с использованием регистратора проверяется наличие объекта в регистраторе. 
    Если такой объект отсутствует, то он заносится в регистратор и выполняется его преобразование,
    в противном случае преобразование данного объекта не выполняется.
           \en Registrator is used to correct transformation of objects, 
    which contain pointers to other geometries. \n
    Object pointer can be contained in several other objects for transformations.
    Registrar is used to prevent multiple transformation of object.
    When transforming the object with registrator, the existence of the object inside the registrator is verified. 
    If such object is absent, it is stored to the registrator and transformed,
    otherwise, a transformation of the object is not performed. \~
  \ingroup Base_Algorithms
*/
// ---
class MATH_CLASS MbRegTransform {
public:
  /// \ru Конструктор. \en Constructor. 
  MbRegTransform() {}
  /// \ru Деструктор. \en Destructor. 
  virtual ~MbRegTransform();
public:
  /** \brief \ru Зарегистрировать трансформированный объект.
             \en Register a transformed object. \~
    \details \ru Зарегистрировать трансформированный объект в регистраторе. \n
             \en Register a transformed object in the registrator. \n \~
    \return \ru Возращает true, если объект не был ранее зарегистрирован.
            \en Returns true if the object hasn't been registered yet. \~
  */
  virtual bool IsSetReg( const MbRefItem * ) = 0;
  /// \ru Освободить используемую память и удалить себя. \en Free memory and remove itself. 
  virtual void Free() = 0; 

OBVIOUS_PRIVATE_COPY( MbRegTransform )
};


//-------------------------------------------------------------------------------
/** \brief \ru Авторегистратор трансформации.
           \en Auto-registrator of transformation. \~
  \details \ru Автоматическое создание локального регистратора трансформации
    и забота о его удалении после использования. \n
           \en Automatic creation of the local registrator of transformation
    and taking care of its removal after use. \n \~
  \ingroup Base_Algorithms
*/
// ---
class MATH_CLASS MbAutoRegTransform {
private:
  /// \ru Локальный регистратор. \en The local registrator. 
  MbRegTransform * locReg;
public:
  /// \ru Конструктор. \en Constructor. 
  MbAutoRegTransform( MbRegTransform *& );
  /// \ru Деструктор. \en Destructor. 
  ~MbAutoRegTransform();

OBVIOUS_PRIVATE_COPY( MbAutoRegTransform )
};


#endif // __ITEM_REGISTRATOR_H
