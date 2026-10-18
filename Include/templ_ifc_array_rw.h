////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Сериализация IFC_Array.
         \en Serialization of IFC_Array. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __TEMPL_IFC_ARRAY_RW_H
#define __TEMPL_IFC_ARRAY_RW_H


#include <templ_ifc_array.h>
#include <io_tape.h>


//------------------------------------------------------------------------------
// \ru чтение массива из потока в объект (на добавление объектов к существующему массиву) \en reading of array from a stream to an object (adding new objects to an existed array is not supported) 
// ---
template <class Type>
reader & operator >> ( reader& in, IFC_Array<Type> & ref ) 
{
  ref.Flush();  
  if ( in.good() ) {
    const size_t count = ReadCOUNT( in, true/*uint_val*/ );

    if ( in.good() ) 
    {
      if ( count ) 
      {
        // \ru половина адресного пространства для 32-разрядного приложения \en a half of address space for 32-bit application 
        if ( ::TestNewSize( SIZE_OF_POINTER, count ) )
        {
          ref.Reserve( count, false );
          C3D_ASSERT( ref.Count() == 0 && ref.Upper() == count );
          
          for ( size_t i = 0; i < count && in.good(); ++i )
          {
            Type * el = 0;
            in >> el;
            if ( in.good() )
              ref.Add( el );
          }

          if ( ref.Count() != count )
          {
            ref.Flush();
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
writer & operator << ( writer& out, const IFC_Array<Type> & ref )
{  
  WriteCOUNT( out, ref.count );
  if ( out.good() ) 
  {
    const Type **parr = ref.GetAddr();
    for ( size_t i = 0; i < ref.count && out.good(); i++ ) 
    {
      out << parr[i];
    }
  }
  return out;
}


#endif // __TEMPL_IFC_ARRAY_RW_H
