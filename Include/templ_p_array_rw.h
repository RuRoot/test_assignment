////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Сериализация PArray.
         \en Serialization of PArray. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __TEMPL_P_ARRAY_RW_H
#define __TEMPL_P_ARRAY_RW_H


#include <templ_p_array.h>
#include <templ_sp_array.h>
#include <templ_csp_array.h>
#include <io_tape.h>


//------------------------------------------------------------------------------
// \ru Чтение массива из потока в объект (на добавление объектов к существующему массиву не рассчитано). \en Reading of array from a stream to an object (adding new objects to an existed array is not supported).
// ---
template <class Type>
reader & operator >> ( reader & in, PArray<Type> & ref )
{
  ref.Flush();

  if ( in.good() )
  {
    uint8 owns;
    in >> owns;

    const size_t count = ReadCOUNT( in, true/*uint_val*/ );

    if ( in.good() )
    {
      ref.owns = !!owns;

      if ( count )
      {
        // \ru половина адресного пространства для 32-разрядного приложения \en a half of address space for 32-bit application 
        if ( ::TestNewSize( SIZE_OF_POINTER, count ) )
        { 
          ref.SetSize( count, true/*clear*/ );
          
          const Type ** parr = ref.GetAddr();

          if ( parr != nullptr )
          {
            ref.count = 0; // \ru Err #69421 сколько штук реально прочитано \en Err #69421 how many elements were actually counted 

            size_t  i;
            // \ru поочередное чтение объектов массива \en successive reading of objects from an array 
            for ( i = 0; i < count && in.good(); i++ ) 
            {
              Type * el = nullptr;
              in >> el;
              parr[i] = el;

              ref.count++; // \ru Err #69421 сколько штук реально прочитано \en Err #69421 how many elements were actually counted 

              // \ru были записаны не все данные (при условии что запись массива эмулировалась, \en not all data has been written (in condition that writing of array was emulated,  
              // \ru писалось не через operator << (writer& out, const PArray<Type>& ref) ) \en it was written without the operator << (writer& out, const PArray<Type>& ref) )  
              if ( in.eof() ) // \ru вычитали-ли весь файл и ничего не осталось \en the whole file was read and nothing is left 
                break; // for
            }
          }
          else
          {
            ref.SetSize( 0, true/*clear*/ );
            C3D_ASSERT_UNCONDITIONAL( false );
          }

          // \ru количество прочитанных должно совпадать с количеством записанных \en the number of read elemetns should be equal to the number of written elements 
          if ( ref.count != count )
          {
            C3D_ASSERT_UNCONDITIONAL( false ); // \ru не все эл-ты прочитаны \en not all elements was read 
            in.setState( io::fail ); // \ru есть ошибка \en there is an error 
          }
          // \ru Err #69421 ref.count = i; // сколько штук реально прочитано \en Err #69421 ref.count = i; // how many elements were actually read 
        }
        else {
          C3D_ASSERT_UNCONDITIONAL( false ); // \ru не бывает столько памяти \en incorrect size of memory 
          in.setState( io::fail ); // \ru ошибка чтения \en reading error 
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
writer & operator << ( writer& out, const PArray<Type> & ref )
{
  if ( out.good() ) {
    out << uint8(ref.owns);
    
    WriteCOUNT( out, ref.count );
    
    const Type **parr = ref.GetAddr();
    for ( size_t i = 0; i < ref.count && out.good(); i++ ) {
      Type *el = (Type *)parr[i];
      out << el;
    }
  }
  else
    C3D_ASSERT_UNCONDITIONAL( false ); // \ru Ошибка Записи \en Writing Error 

  return out;
}


//------------------------------------------------------------------------------
// \ru чтение массива из потока в указатель \en reading of array from a stream to a pointer 
// ---
template <class Type>
reader & operator >> ( reader & in, PArray<Type> *& ptr )
{
  ptr = nullptr;
  if ( in.good() ) {
    if ( in.MathVersion() < 0x06000012L ) 
      ptr = new PArray<Type>;
    else {
      uint8 existPtr;
      in >> existPtr; 
      if ( existPtr ) 
        ptr = new PArray<Type>;
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
writer & operator << ( writer & out, const PArray<Type> * ptr )
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
// \ru чтение массива из потока в объект \en reading of array from a stream to an object  
// ---
template <class Type>
reader & operator >> ( reader & in, SPArray<Type> & ref ) {
  return in >> (PArray<Type> &)ref;
}


//------------------------------------------------------------------------------
// \ru запись массива в поток из объекта \en writing of array from an object to a stream 
// ---
template <class Type>
writer & operator << ( writer & out, const SPArray<Type> & ref ) {
  return out << (const PArray<Type> &)ref;
}


//------------------------------------------------------------------------------
// \ru чтение массива из потока в объект \en reading of array from a stream to an object 
//
template <class Type>
reader & operator >> ( reader & in, CSPArray<Type> & ref ) {
  in >> (SPArray<Type> &)ref; 
  int b; in >> b; ref.m_sort = !!b; //OV_x64 in >> ref.sort;
  return in;
}


//------------------------------------------------------------------------------
// \ru запись массива в поток из объекта \en writing of array from an object to a stream 
// ---
template <class Type>
writer& operator << ( writer & out, const CSPArray<Type> & ref ) {
  out << (const SPArray<Type> &)ref;
  int b = !!ref.m_sort; out << b; //OV_x64 out << ref.sort;
  return out;
}


#endif // __TEMPL_P_ARRAY_RW_H
