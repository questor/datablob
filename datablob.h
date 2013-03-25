
#ifndef __DATABLOB_H__
#define __DATABLOB_H__

#include "eastl/vector.h"

//Memory Layout:
// Magic "I100" (always read as uint32_t to check for correct endianess)
//    uint8 Type
//    Type data


//    type == eString:
//       uin16 strLength
//       uin8 data
//    if(Type==Array)         has always only one type in his store!
//       uint8 numberObjectsInArray
//       uint8 typeOfData
//       type data
//    if(Type==StringArray)
//       uint8 numberObjectsInArray
//       uint32 lengthOfAllData
//       type data
//    else if(Type == Dictionary)      describes always one object!
//       uint8 numberSubTypes
//          uint32 name
//          uint8 TypeOfData
//          Type data
//    else if(Type == DictionaryStructOffsets
//       uint8 numberData
//       uint8 numberSubTypes
//          uint32 name
//          uint8 TypeOfData
//          uint32 offsetInStructToData

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
