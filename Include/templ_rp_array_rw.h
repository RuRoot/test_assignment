////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Сериализация RPArray.
         \en Serialization of RPArray. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __TEMPL_RP_ARRAY_RW_H
#define __TEMPL_RP_ARRAY_RW_H


#include <templ_rp_array.h>
#include <io_tape.h>


//------------------------------------------------------------------------------
// \ru Чтение массива из потока в объект (на добавление объектов к существующему массиву не рассчитано). \en reading of array from a stream to an object (adding new objects to an existed array is not supported).
// ---
template <class Type>
reader & operator >> ( reader & in, RPArray<Type> & ref )
{
  ref.DetachAll();

  if ( in.good() )
  {
    size_t count = ReadCOUNT( in, true/*uint_val*/ );

    if ( in.good() )
    {
      if ( count )
      {
        // \ru половина адресного пространства для 32-разрядного приложения \en a half of address space for 32-bit application 
        if ( ::TestNewSize( SIZE_OF_POINTER, count ) )
        {
          ref.Reserve( count );

          const Type ** parr = ref.GetAddr();

          if ( parr != nullptr ) {
            size_t i;
            // \ru поочередное чтение объектов массива \en successive reading of objects from an array 
            for ( i = 0; i < count && in.good(); i++ ) {
              Type *el;
              in >> el;
              parr[i] = el;
            }
            ref.count = i; // \ru сколько штук реально прочитано \en the number of read objects 
          }
          else {
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
writer& operator << ( writer& out, const RPArray<Type>& ref ) {
//OV_x64 out << ref.count;
  WriteCOUNT( out, ref.count );

  const Type **parr = ref.GetAddr();
  for ( size_t i = 0; i < ref.count && out.good(); i++ ) {
    Type *el = (Type *)parr[i];
    out << el;
  }

  return out;
}


//------------------------------------------------------------------------------
// \ru чтение массива из потока в указатель \en reading of array from a stream to a pointer 
// ---
template <class Type>
reader& operator >> ( reader& in, RPArray<Type>*& ptr ) {
  ptr = nullptr;
  if ( in.good() ) {
    uint8 existPtr;
    in >> existPtr; 
    if ( existPtr ) {
      ptr = new RPArray<Type>;
      in >> *ptr;
    }
  }

  return in;
}


//------------------------------------------------------------------------------
// \ru запись массива в поток из указателя \en writing of array from a pointer to a stream 
// ---
template <class Type>
writer& operator << ( writer& out, const RPArray<Type>* ptr ) {

  uint8 existPtr = !!ptr;
  out << existPtr; 
  if ( existPtr ) 
    out << *ptr; // \ru запись телом \en writing by a solid 

  return out;
}


#endif // __TEMPL_RP_ARRAY_RW_H
