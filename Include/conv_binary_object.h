////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __BINOBJ_H
#define __BINOBJ_H


//------------------------------------------------------------------------------
// бинарный объект
// ---
struct BinaryObj {

  void * p;
  void * pSort;

  BinaryObj( void *otherId, void *otherP ) : p( otherId ), pSort( otherP ) {}

  bool operator == ( const BinaryObj &o ) const { return pSort == o.pSort; }
  bool operator <  ( const BinaryObj &o ) const { return pSort <  o.pSort; }
};


#endif