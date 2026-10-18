////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Сериализация FDPArray.
         \en Serialization of FDPArray. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __TEMPL_FDP_ARRAY_RW_H
#define __TEMPL_FDP_ARRAY_RW_H


#include <templ_fdp_array.h>
#include <templ_sfdp_array.h>
#include <io_tape.h>


////////////////////////////////////////////////////////////////////////////////
//
// \ru при записи FDPArray<Type> владение не записывается т.к. не умею записывать \en while writing FDPArray<Type> ownership is not written 
// \ru адрес функции удаления элементов. \en addess of elements deletion function. 
// \ru После чтения адрес функции удаления элементов придется выставлять вручную \en After reading the address of elements deletion function should be set manually 
//
////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
// \ru чтение массива из потока в объект (на добавление объектов к существующему массиву не рассчитано) \en reading of array from a stream to an object (adding new objects to an existed array is not supported) 
// ---
template <class Type>
reader & operator >> ( reader & in, FDPArray<Type> & ref )
{
  ref.Flush();

  if ( in.good() ) {
    size_t count = ReadCOUNT( in, true/*uint_val*/ );

    if ( in.good() ) 
    {
      if ( count ) 
      {
        // \ru половина адресного пространства для 32-разрядного приложения \en a half of address space for 32-bit application 
        if ( ::TestNewSize( SIZE_OF_POINTER, count ) )
        {
          ref.SetSize( count, true/*clear*/ );

          const Type ** parr = ref.GetAddr();

          if ( parr != nullptr ) {
            size_t i;
            // \ru поочередное чтение объектов массива \en successive reading of objects from an array 
            for ( i = 0; i < count && in.good(); i++ ) {
              Type * el = nullptr;
              in >> el;
              parr[i] = el;
            }
            ref.count = i; // \ru сколько штук реально прочитано \en the number of read objects 
          }
          else {
            ref.SetSize( 0, true/*clear*/ );
            in.setState( io::fail ); // \ru ошибка чтения \en reading error 
            C3D_ASSERT_UNCONDITIONAL( false );
          }
        }
        else {
          in.setState( io::fail ); // \ru ошибка чтения \en reading error 
          C3D_ASSERT_UNCONDITIONAL( false ); // \ru не бывает столько памяти \en incorrect size of memory 
        }
      }
    }
  }

  return in;
}


//------------------------------------------------------------------------------
// \ru запись массива в поток из объекта \en writing of array from an object to a stream 
// ---
template <class Type>
writer & operator << ( writer & out, const FDPArray<Type> & ref )
{
  WriteCOUNT( out, ref.count );

  const Type **parr = ref.GetAddr();
  for ( size_t i = 0; i < ref.count && out.good(); i++ ) {
    Type * el = (Type *)parr[i];
    out << el;
  }

  return out;
}


//------------------------------------------------------------------------------
// \ru чтение массива из потока в указатель \en reading of array from a stream to a pointer 
// ---
template <class Type>
reader & operator >> ( reader & in, FDPArray<Type> *& ptr )
{
  ptr = nullptr;
  if ( in.good() ) {
    if ( in.MathVersion() < 0x06000012L ) 
      ptr = new FDPArray<Type>;
    else {
      uint8 existPtr = 0;
      in >> existPtr; 
      if ( existPtr ) 
        ptr = new FDPArray<Type>;
    }

    if ( ptr )
      in >> *ptr;
  }

  return in;
}


//------------------------------------------------------------------------------
// \ru запись массива в поток из указателя \en writing of array from a pointer to a stream 
// ---
template <class Type>
writer & operator << ( writer & out, const FDPArray<Type> * ptr )
{
  // \ru при записи в старую версию оставляю без проверки указателя \en While writing to an old version the pointer is not checked 
  if ( out.MathVersion() < 0x06000012L ) {
    C3D_ASSERT( ptr );
    out << *ptr;
  }
  else {
    uint8 existPtr = !!ptr;
    out << existPtr; 
    if ( existPtr ) 
      out << *ptr; // \ru запись телом \en writing by a solid 
  }

  return out;
}


//------------------------------------------------------------------------------
// \ru Т.к. наследование от базового класса сделано private, то делаю доступ к оператору базового класса.
// \en Since there is a private inheritance from the base class, I give an access to the operator of the base class.
// ---
template <class Type>
reader & operator >> ( reader & in, SFDPArray<Type> & ref ) {
  return in >> (FDPArray<Type> &)ref;
}


//------------------------------------------------------------------------------
// \ru запись массива в поток из объекта. Т.к. наследование от базового класса сделано private, то делаю доступ к оператору базового класса.
// \en Writing of array from an object to a stream. Since there is a private inheritance from the base class, I give an access to the operator of the base class.
// ---
template <class Type>
writer & operator << ( writer & out, const SFDPArray<Type> & ref ) {
  return out << (const FDPArray<Type> &)ref;
}


// -----------------------------------------------------------------------------
// \ru удаление всех габаритов - функция удаления массива указателей \en deletion of all bounding boxes - the function of deletion of pointers array 
// ---
template <class Type>
static void TotalDestroy( Type ** arr, size_t count ) {
  if ( arr ) {
    size_t i = 0;
    for ( Type** parr = arr; i < count; i++, parr++ ) {
      Type *del = *parr;
      *parr = nullptr; // \ru Сначала обнулить ... \en Set to null at first...
      delete del;   // \ru ... потом удалить \en ... then delete 
    }
  }
}


#endif // __TEMPL_FDP_ARRAY_RW_H
