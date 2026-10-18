////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Сериализация списка List.
         \en Serialization of list. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __TEMPL_T_LIST_H
#define __TEMPL_T_LIST_H


#include <templ_s_list.h>
#include <io_tape.h>
#include <math_version.h>


//------------------------------------------------------------------------------
// \ru чтение списка из потока в объект \en reading of list from a stream to an object 
//
template <class Type>
reader & operator >> ( reader& in, List<Type>& ref ) {
  ref.Flush();

  if ( in.good() ) {
    uint8 val = 0;
    in >> val;

    size_t count = ReadCOUNT( in, false/*uint_val*/ );

    if ( in.good() ) {
      ref.owns = !!val;

      if ( count ) {
        Type* el;
        for ( size_t i = 0; i < count; i++ ) 
        {
          in >> el;
          if ( in.good() )
            ref.Add( el );
          else {
            if ( ref.owns ) // \ru ЯТ 03.01.01 \en ЯТ 03.01.01 
              delete el;    // \ru ЯТ 03.01.01 \en ЯТ 03.01.01 
            break;
          }
        }
      }
    }
  }

  return in;
}


//------------------------------------------------------------------------------
// \ru запись списка в поток из объекта \en writing of list from an object to a stream 
// ---
template <class Type>
writer& operator << ( writer& out, const List<Type>& ref ) {
  out << uint8(ref.owns);

  /////////////////////////////////
  size_t refCount = ref.Count();

  WriteCOUNT( out, refCount );

  size_t count = 0; // \ru ЯТ защита от возможной рассогласованности списка \en ЯТ protection from the possible mismatch of the list 
  LIterator<Type> iter( ref );
  while( iter && out.good() && count < refCount ) {
    out << iter++;
    count++;
  }

  // \ru ЯТ проверка на возможную рассогласованность списка \en ЯТ a check for the possible mismatch of the list 
  if ( out.good() ) {
    C3D_ASSERT( count == refCount );
  }  

  return out;
}


//------------------------------------------------------------------------------
// \ru чтение списка из потока в указатель \en reading of list from a stream to a pointer 
//
template <class Type>
reader& operator >> ( reader& in, List<Type>*& ptr ) {
  ptr = nullptr;

  if ( in.good() ) {
    if ( in.MathVersion() < 0x06000012L ) 
      ptr = new List<Type>;
    else {
      uint8 existPtr = 0;
      in >> existPtr; 
      if ( existPtr ) 
        ptr = new List<Type>;
    }

    if ( ptr )
      in >> *ptr; // \ru чтение тела \en reading of a solid 
  }

  return in;
}


//------------------------------------------------------------------------------
// \ru запись списка в поток из указателя \en writing of list from a pointer to a stream 
// ---
template <class Type>
writer& operator << ( writer& out, const List<Type>* ptr ) {
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


#endif // __TEMPL_T_LIST_H
