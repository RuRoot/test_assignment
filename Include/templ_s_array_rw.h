////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Сериализация SArray.
         \en Serialization of SArray. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __TEMPL_S_ARRAY_RW_H
#define __TEMPL_S_ARRAY_RW_H


#include <templ_s_array.h>
#include <templ_ss_array.h>
#include <templ_css_array.h>
#include <templ_im_array.h>
#include <io_tape.h>


//------------------------------------------------------------------------------
/**\ru Чтение массива из потока в объект.
   \en Reading of array from a stream to an object \~
  \ingroup Base_Tools_Containers
*/
//
template <class Type>
reader & operator >> ( reader & in, SArray<Type> & ref )
{
  ref.Flush();
  if ( in.good() ) 
  {
    size_t count = ReadCOUNT( in, true/*uint_val*/ );

    if ( in.good() && count )  
    {
      size_t sizeOfType = sizeof(Type);
      // \ru половина адресного пространства для 32-разрядного приложения \en a half of address space for 32-bit application 
      if ( ::TestNewSize( sizeOfType, count ) )
      { 
        ref.SetSize( count, true/*clear*/ );
        C3D_ASSERT( ref.upper >= count );

        if ( ref.GetAddr() != nullptr ) {
          size_t i;
          for ( i = 0; i < count && in.good(); i++ )
          {
            in >> ref.parr[i];

            ref.count++;

            // \ru были записаны не все данные (при условии что запись массива эмулировалась, \en not all data has been written (in condition that writing of array was emulated,  
            // \ru писалось не через operator << (writer& out, const PArray<Type>& ref) ) \en it was written without the operator << (writer& out, const PArray<Type>& ref) )  
            if ( in.eof() ) // \ru вычитали-ли весь файл и ничего не осталось \en the whole file was read and nothing is left 
              break; // for
          }
        }
        else {
          ref.SetSize( 0, true/*clear*/ );
          C3D_ASSERT_UNCONDITIONAL( false );
          in.setState( io::fail ); // \ru ошибка чтения \en reading error 
        }
      }
      else 
        in.setState( io::fail ); // \ru ошибка чтения \en reading error 
    }
  }

  return in;
}


//------------------------------------------------------------------------------
/**\ru Запись массива в поток из объекта.
   \en Writing of array from an object to a stream. \~
  \ingroup Base_Tools_Containers
*/
// ---
template <class Type>
writer& operator << ( writer& out, const SArray<Type>& ref ) {
  //OV_x64 out << ref.count;
  WriteCOUNT( out, ref.count );

  for( size_t i = 0; i < ref.count && out.good(); i++ )
    out << ref.parr[i];
  return out;
}


//------------------------------------------------------------------------------
/**\ru Чтение массива из потока в указатель.
   \en Reading of array from a stream to a pointer. \~
  \ingroup Base_Tools_Containers
*/
//
template <class Type>
reader& operator >> ( reader& in, SArray<Type>*& ptr ) {
  ptr = nullptr;
  if ( in.good() ) {
    if ( in.MathVersion() < 0x06000012L ) 
      ptr = new SArray<Type>;
    else {
      uint8 existPtr;
      in >> existPtr; 
      if ( existPtr ) 
        ptr = new SArray<Type>;
    }

    if ( ptr )
      in >> *ptr; // \ru чтение тела \en reading of a solid 
  }

  return in;
}


//------------------------------------------------------------------------------
/**\ru Запись массива в поток из указателя.
   \en Writing of array from a pointer to a stream. \~
  \ingroup Base_Tools_Containers
*/
// ---
template <class Type>
writer& operator << ( writer& out, const SArray<Type>* ptr ) {
  // \ru ЯТ К6 при записи в старую версию оставляю без проверки указателя \en ЯТ К6 while writing to an old version the pointer is not checked 
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
/**\ru Чтение массива из потока в объект.
   \en Reading of array from a stream to an object. \~
  \ingroup Base_Tools_Containers
*/
//
template <class Type>
reader & operator >> ( reader& in, SSArray<Type>& ref ) {
  return in >> (SArray<Type> &)ref;
}


//------------------------------------------------------------------------------
/**\ru Запись массива в поток из объекта.
   \en Writing of array from an object to a stream. \~
  \ingroup Base_Tools_Containers
*/
// ---
template <class Type>
writer& operator << ( writer& out, const SSArray<Type>& ref ) {
  return out << (const SArray<Type> &)ref;
}


//------------------------------------------------------------------------------
/**\ru Чтение массива из потока в объект.
   \en Reading of array from a stream to an object. \~
  \ingroup Base_Tools_Containers
*/
//
template <class Type>
reader & operator >> ( reader& in, CSSArray<Type>& ref ) {
  in >> (SSArray<Type> &)ref; 
  int b; in >> b; ref.m_sort = !!b; //OV_x64 in >> ref.sort;
  return in;
}


//------------------------------------------------------------------------------
/**\ru Запись массива в поток из объекта.
   \en Writing of array from an object to a stream. \~
  \ingroup Base_Tools_Containers
*/
// ---
template <class Type>
writer& operator << ( writer& out, const CSSArray<Type>& ref ) {
  out << (const SSArray<Type> &)ref;
  int b = !!ref.m_sort; out << b; //OV_x64 out << ref.sort;
  return out;
}


//------------------------------------------------------------------------------
/**\ru Чтение массива из потока в объект.
   \en Reading of array from a stream to an object. \~
  \ingroup Base_Tools_Containers
*/
//
template <class Type>
reader & operator >> ( reader& in, IMArray<Type>& ref ) {
  // \ru OV x64 return in >> (SArray<uint> &)ref; // ЯТ К6  не Type, а uint !!! \en OV x64 return in >> (SArray<uint> &)ref; // ЯТ К6  not Type, but uint !!! 
  if ( in.good() ) {
    size_t count = ReadCOUNT( in, true/*uint_val*/ );
    if ( in.good() && count ) {
      ref.SetSize( count, true/*clear*/ );
      for ( size_t i = 0; i < count && in.good(); i++ ) {
        size_t item = ReadCOUNT( in ); //OV_x64 in >> ref.parr[i];
        ((SArray<size_t> &)ref).Add( item ); 
      }
    }
  }
  return in;

}


//------------------------------------------------------------------------------
/**\ru Запись массива в поток из объекта.
   \en Writing of array from an object to a stream. \~
  \ingroup Base_Tools_Containers
*/
// ---
template <class Type>
writer& operator << ( writer& out, const IMArray<Type>& ref ) {
// \ru OV x64 return out << (const SArray<uint> &)ref; // ЯТ К6  не Type, а uint !!! \en OV x64 return out << (const SArray<uint> &)ref; // ЯТ К6  not Type, but uint !!! 
  WriteCOUNT( out, ref.count );
  for( size_t i = 0; i < ref.count && out.good(); i++ )
    WriteCOUNT( out, (size_t)ref(i) );
  return out;
}


#endif // __TEMPL_S_ARRAY_RW_H
