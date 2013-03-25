
#ifndef __DATABLOB_H__
#define __DATABLOB_H__

#include "eastl/vector.h"

namespace DataBlob {

enum {
   eUInt8,
   eUInt16,
   eUInt32,
   eUInt64,
   eInt8,
   eInt16,
   eInt32,
   eFloat,
   eVector2f,
   eVector3f,
   eVector4f,
   eVector2i,
   eString,    //are only allowed in Dictionaries, not in arrays or structoffsets!
               //max length is 65536!
   eArrayByteSize,
   eArrayShortSize,
   eArrayLongSize,
   eStringArray,  //with complete length of block to easily skip it...
   eDictionary,
   eDictionaryStructOffsets
};

uint32_t getPureDataSize(const eastl::vector<uint8_t> &data, uint32_t offset);
uint32_t getCompleteSize(const eastl::vector<uint8_t> &data, uint32_t offset);

};    //namespace DataBlob

#endif
