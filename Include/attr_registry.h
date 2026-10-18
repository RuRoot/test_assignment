////////////////////////////////////////////////////////////////////////////////
/** 
  \file
  \brief \ru Инстанс определения атрибута.
         \en Attribute definition instance. \~

*/
////////////////////////////////////////////////////////////////////////////////

#ifndef __ATTR_REGISTRY_H
#define __ATTR_REGISTRY_H


#include <math_x.h>
#include <map>
#include <math_define.h>
#include <tool_uuid.h>


class IAttrDefinition;


//------------------------------------------------------------------------------
/** \brief \ru Идентификатор пользовательского атрибута.
           \en Identifier of external attribute. \~
  \details \ru Идентификатор пользовательского атрибута.
           \en Identifier of external attribute. \~
  \ingroup Model_Attributes
 */
// KVT class MbUserAttribType 
// KVT {
// KVT public:
// KVT   uint subtype1;
// KVT   uint subtype2;
// KVT   uint subtype3;
// KVT 
// KVT public:
// KVT   MbUserAttribType()
// KVT     : subtype1( 0 ), subtype2( 0 ), subtype3( 0 ) {}
// KVT   MbUserAttribType( uint type1, uint type2, uint type3 )
// KVT     : subtype1( type1 ), subtype2( type2 ), subtype3( type3 ) {}
// KVT   MbUserAttribType( const MbUserAttribType & other )
// KVT     : subtype1( other.subtype1 ), subtype2( other.subtype2 ), subtype3( other.subtype3 ) {}
// KVT 
// KVT   bool operator == ( const MbUserAttribType & other ) const
// KVT   { return subtype1 == other.subtype1 && subtype2 == other.subtype2 && subtype3 == other.subtype3; }
// KVT   bool operator < ( const MbUserAttribType & other ) const
// KVT   {
// KVT     if (subtype1 != other.subtype1)
// KVT       return subtype1 < other.subtype1;
// KVT     else if (subtype2 != other.subtype2)
// KVT       return subtype2 < other.subtype2;
// KVT     else if (subtype3 != other.subtype3)
// KVT       return subtype3 < other.subtype3;
// KVT 
// KVT     return false;
// KVT   }
// KVT 
// KVT private:
// KVT   void operator = ( const MbUserAttribType & ); // \ru Не реализовано \en Not implemented 
// KVT };

typedef MbUuid MbUserAttribType;

//------------------------------------------------------------------------------
/** \brief \ru Инстанс определения атрибута.
           \en Attribute definition instance. \~ 
  \ingroup Model_Attributes
 */
class MATH_CLASS AttrDefInstance 
{
private:
  MbUserAttribType id_;
public:
  AttrDefInstance( const MbUserAttribType & id );
  virtual ~AttrDefInstance();

public:
  virtual IAttrDefinition * GetAttrDefinition() = 0;
};


//------------------------------------------------------------------------------
/** \brief \ru Найти определение пользовательского атрибута.
           \en Find an external attribute definition. \~
  \ingroup Model_Attributes
*/
MATH_FUNC (IAttrDefinition *) GetUserAttrDefinition( const MbUserAttribType & id );


#endif // __ATTR_REGISTRY_H
