////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Журнал построения объекта.
         \en The history tree of object. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __CREATOR_TRANSACTION_H
#define __CREATOR_TRANSACTION_H


#include <templ_p_array.h>
#include <mb_enum.h>
#include <creator.h>
#include <vector>


class    reader;
class    writer;
class    IProgressIndicator;
class    MbCartPoint3D;
class    MbVector3D;
class    MbAxis3D;
class    MbMatrix3D;
class    MbSpaceItem;
class    MbFaceShell;
class    MbWireFrame;
class    MbPointFrame;
class    MbProperties;
struct   MbControlData3D;
class              MbRegDuplicate;
class              MbRegTransform;
enum               MbeCopyMode;


//------------------------------------------------------------------------------
/** \brief \ru Журнал построения объекта.
           \en The history tree of object. \~
  \details \ru Журнал построения содержит упорядоченное множество строителей, 
    последовательная работа которых строит объект. \n
    Неактивные строители (с состоянием mps_Skip) не принимают участия в построении объекта.
           \en The history tree contains an ordered set of creators 
    whose successive work creates the objects. \n
    Inactive creators (with state mps_Skip) are not used in the object construction. \~
  \ingroup Model_Creators
*/
// ---
class MATH_CLASS MbTransactions 
{
private:
  c3d::CreatorsVector transactions; ///< \ru Упорядоченное множество строителей. \en An ordered set of creators. 

protected:
  /// \ru Конструктор копирования с регистратором. \en Copy-constructor with registrator. 
  MbTransactions( const MbTransactions &, MbRegDuplicate * iReg );
public:
  /// \ru Конструктор без параметров. \en Constructor without parameters. 
  MbTransactions();
  /// \ru Конструктор по строителям. \en Constructor by creators. 
  template <class Creators>
  MbTransactions( const Creators & creators )
    : transactions()
  {
    size_t iCount = creators.size();
    if ( iCount > 0 ) {
      transactions.reserve( iCount ); 
      for ( size_t i = 0; i < iCount; i++ ) {
        MbCreator * creator = const_cast<MbCreator *>( creators[i] );
        if ( creator != nullptr ) {
          creator->AddRef();
          transactions.push_back( creator );
        }
      }
    }
  }

  /// \ru Деструктор. \en Destructor. 
  virtual ~MbTransactions();
public:

  /// \ru Перестроить объект по протоколу построения. \en Reconstruct object according to the history tree. 
  virtual bool        RebuildItem( MbeCopyMode sameShell, RPArray<MbSpaceItem> * items, IProgressIndicator * progInd );

          /// \ru Очистить присланный журнал и скопировать в него строители. \en Clear the given history tree and copy the creators to it.  
          void        CreatorsCopy     ( MbTransactions & other, MbRegDuplicate * iReg = nullptr ) const;
          /// \ru Очистить журнал и скопировать в него строители из присланного журнала. \en Clear the history tree and copy the creators from the given history tree to it.  
          void        CreatorsAssign   ( const MbTransactions & other ); 
          /// \ru Сделать строители равными соответствующим строителям присланного журнала, если строители подобны. \en Make the creators equal to the creators from the given history tree if the creators are similar. 
          bool        SetCreatorsEqual ( const MbTransactions & other ); 
          /// \ru Проверить, являются ли соответствующие строители присланного журнала подобными. \en Check whether the corresponding creators of the given history tree are similar. 
          bool        IsCreatorsSimilar( const MbTransactions & other ) const; 
          // Посчитать внутренние построители по типу. \en Count internal creators by type.
          size_t      GetCreatorsCount( MbeCreatorType ct ) const;
          /// \ru Преобразовать согласно матрице строители. \en Transform the creators according to the matrix. 
          void        CreatorsTransform( const MbMatrix3D &, MbRegTransform * = nullptr );
          /// \ru Сдвинуть вдоль вектора строители. \en Move creators along the vector. 
          void        CreatorsMove     ( const MbVector3D &, MbRegTransform * = nullptr );
          /// \ru Повернуть вокруг оси строители на заданный угол. \en Rotate the creators about the axis by the given angle. 
          void        CreatorsRotate   ( const MbAxis3D &, double angle, MbRegTransform * = nullptr );
          /// \ru Выдать количество строителей. \en Get the creators count. 
          size_t      GetCreatorsCount() const { return transactions.size(); } 
          /// \ru Зарезервировать место для строителей. \en Reserve space for creators. 
          void        Reserve( size_t count ) { transactions.reserve( transactions.size() + count ); } 
          /// \ru Выдать строитель по его индексу. \en Get constructor by its index. 
    const MbCreator * GetCreator( size_t ind ) const; 
          /// \ru Выдать строитель по его индексу с возможностью редактирования. \en Get constructor by its index with possibility of editing. 
          MbCreator * SetCreator( size_t ind );
          /// \ru Добавить свои строители в присланный массив. \en Add your own creators to the given array. 
  virtual bool        GetCreators( RPArray<MbCreator> & ) const;
          /// \ru Добавить свои строители в присланный массив. \en Add your own creators to the given array. 
  virtual bool        GetCreators( c3d::CreatorsSPtrVector & ) const;
          /// \ru Добавить копии своих строителей в присланный массив. \en Add copies of your own creators to the given array. 
          template <class CreatorsVector>
          bool        GetCreatorsCopies( CreatorsVector & creators, MbRegDuplicate * iReg ) const
          {
            bool res = false;
            size_t addCnt = transactions.size();
            if ( addCnt > 0 ) {
              c3d::CreatorSPtr creator;
              creators.reserve( creators.size() + addCnt );
              for ( size_t i = 0; i < addCnt; ++i ) {
                if ( transactions[i] != nullptr ) {
                  creator = static_cast<MbCreator *>(&transactions[i]->Duplicate( iReg ));
                  creators.push_back( creator );
                  ::DetachItem( creator );
                  res = true;
                }
              }
            }
            return res;
          }
          /// \ru Найти номер строителя в журнале или вернуть SYS_MAX_T в случае отсутствия. \en Find the number of creators in the history tree or return SYS_MAX_T if it is absent. 
          size_t      FindCreator( const MbCreator * creator ); 
          /// \ru Добавить строитель (addSame = false) или его копию (addSame = true) в журнал. \en Add the constructor (addSame = false) or its copy (addSame = true) to the history tree. 
          bool        AddCreator ( const MbCreator &, bool addSame = false ); 
          /// \ru Добавить строитель (addSame = false) или его копию (addSame = true) в журнал. \en Add the constructor (addSame = false) or its copy (addSame = true) to the history tree. 
          bool        AddCreator ( const MbCreator *, bool addSame = false );
          /// \ru Добавить строители в журнал. \en Add creators to the history tree. 
          template <class CreatorsVector>
          void        AddCreators( const CreatorsVector & creators )
          {
            if ( !Math::SupressCreators() ) {
              c3d::CreatorSPtr creator;
              for ( size_t i = 0, addCnt = creators.size(); i < addCnt; ++i ) {
                creator = creators[i];
                if ( creator != nullptr ) {
                  creator->AddRef();
                  transactions.push_back( creator );
                  ::DetachItem( creator );
                }
              }
            }
            else
              DeleteCreators();
          }
          /// \ru Вытереть строитель с указанным номером из журнала и отдать его. \en Remove the constructor with the specified index from the history tree and return it. 
          MbCreator * DetachCreator ( size_t ind ); 
          /// \ru Удалить строитель с указанным номером и вытереть его из журнала. \en Delete the constructor with the specified index and remove it from the history tree. 
          bool        DeleteCreator ( size_t ind );
          /// \ru Удалить все строители и очистить журнал. \en Delete all the creators and clear the history tree. 
          void        DeleteCreators();
          /// \ru Дать статус строителя с указанным номером. \en Get the status of creator with the specified index. 
          int         GetCreatorStatus( size_t ind ) const; 
          /// \ru Установить строителю с указанным номером статус. \en Set status to creator with the specified index. 
          bool        SetCreatorStatus( size_t ind, MbeProcessState );
          /// \ru Дать количество активных строителей. \en Get the active creators count. 
          size_t      GetActiveCreatorsCount() const; 
          /// \ru Установить количество активных строителей от начала до заданного номера. \en Set the count of active creators from the beginning to the given index. 
          bool        SetActiveCreatorsCount( size_t activeCount ); 
          /// \ru Выдать создаваемый заданным числом строителей объект и базовые объекты остальных строителей. \en Get the object created by the specified number of creators and the basis items of the other creators. 
          void        BreakCreatorsToBasisItem( size_t c, RPArray<MbSpaceItem> & ); 
          /// \ru Выдать базовые объекты строителей. \en Get the basis items of the creators. 
          void        GetCreatorsBasisItems ( RPArray<MbSpaceItem>   & );
          /// \ru Выдать базовые точки строителей. \en Get the basis points of the creators. 
          void        GetCreatorsBasisPoints( MbControlData3D & ) const; 
          /// \ru Изменить объект по контрольным точкам. \en Change the object by control points. 
          void        SetCreatorsBasisPoints( const MbControlData3D & ); 
          /// \ru Выдать свойства строителей (на копиях или на оригиналах строителей). \en Get properties of the creators (using original creators or their copies). 
          void        GetProperties( MbProperties &, bool sameCreators = false ); 
          /// \ru Установить свойства строителей. \en Set properties of the creators. 
          void        SetProperties( const MbProperties & ); 
          /// \ru Прочитать строители из потока. \en Read creators from the stream. 
          void        CreatorsRead ( reader & in  ); 
          /// \ru Записать строители в поток. \en Write creators to the stream. 
          void        CreatorsWrite( writer & out ) const; 

OBVIOUS_PRIVATE_COPY( MbTransactions )
};


#endif // __CREATOR_TRANSACTION_H
