
#include "datablob.h"

namespace DataBlob {

uint32_t getPureDataSize(const eastl::vector<uint8_t> &data, uint32_t offset) {
   uint8_t type = data[offset];
   switch(type) {
   case eUInt8:
      return sizeof(uint8_t);
   case eUInt16:
      return sizeof(uint16_t);
   case eUInt32:
      return sizeof(uint32_t);
   case eUInt64:
      return sizeof(uint64_t);
   case eInt8:
      return sizeof(int8_t);
   case eInt16:
      return sizeof(int16_t);
   case eInt32:
      return sizeof(int32_t);
   case eFloat:
      return sizeof(float);
   case eVector2f:
      return sizeof(float)*2;
   case eVector3f:
      return sizeof(float)*3;
   case eVector4f:
      return sizeof(float)*4;
   case eVector2i:
      return sizeof(int32_t)*2;
   case eString:
      {
         uint16_t length = data[offset+1];
         length <<= 8;
         length |= data[offset+2];
         return length;
      }
   case eArrayByteSize: {
         uint8_t numberElements = data[offset+1];
         return numberElements * getPureDataSize(data, offset+2);
      }
   case eDictionaryStructOffsets: {
         uint8_t numberData = data[offset+1];
         uint8_t numberSubTypes = data[offset+2];

         //FastPath: take last element and add size of the last element and return this as size
         offset += 3 + (numberSubTypes-1)*(4+1+4);

         uint32_t offsetInStruct = data[offset+8];
         offsetInStruct <<= 8;
         offsetInStruct |= data[offset+7];
         offsetInStruct <<= 8;
         offsetInStruct |= data[offset+6];
         offsetInStruct <<= 8;
         offsetInStruct |= data[offset+5];

         uint32_t lastVarSize = getPureDataSize(data, offset+4);
         return (offsetInStruct+lastVarSize)*numberData;
      }
   default:
      //Dictionary not allowed in Offset Lists because of unknown sizes in for example strings
      ASSERT(0);
   }
   return 0;
}

uint32_t getCompleteSize(const eastl::vector<uint8_t> &data, uint32_t offset) {
   uint8_t type = data[offset];
   switch(type) {
   case eUInt8:
      return 1 + sizeof(uint8_t);
   case eUInt16:
      return 1 + sizeof(uint16_t);
   case eUInt32:
      return 1 + sizeof(uint32_t);
   case eUInt64:
      return 1 + sizeof(uint64_t);
   case eInt8:
      return 1 + sizeof(int8_t);
   case eInt16:
      return 1 + sizeof(int16_t);
   case eInt32:
      return 1 + sizeof(int32_t);
   case eFloat:
      return 1 + sizeof(float);
   case eVector2f:
      return 1 + sizeof(float)*2;
   case eVector3f:
      return 1 + sizeof(float)*3;
   case eVector4f:
      return 1 + sizeof(float)*4;
   case eVector2i:
      return 1 + sizeof(int32_t)*2;
   case eString:
      {
         uint16_t length = data[offset+2];
         length <<= 8;
         length |= data[offset+1];
         return 1 + 2 + length;
      }
   case eArrayByteSize: {
         uint8_t numberElements = data[offset+1];
         return 1 + 2 + numberElements * getPureDataSize(data, offset+2);
      }
   case eArrayShortSize: {
         uint16_t numberElements = data[offset+2];
		 numberElements <<= 8;
		 numberElements |= data[offset+1];
         return 1 + 3 + numberElements * getPureDataSize(data, offset+3);
      }
   case eArrayLongSize: {
         uint32_t numberElements = data[offset+4];
		 numberElements <<= 8;
		 numberElements |= data[offset+3];
		 numberElements <<= 8;
		 numberElements |= data[offset+2];
		 numberElements <<= 8;
		 numberElements |= data[offset+1];
         return 1 + 5 + numberElements * getPureDataSize(data, offset+5);
      }
   case eDictionary: {
         uint8_t numberSubTypes = data[offset+1];
         uint32_t size = 2;   //type and numberSubTypes
         offset += 2;
         for(int i=0; i<numberSubTypes; ++i) {
            uint32_t elementSize = getCompleteSize(data, offset+4);  //skip name
            size += 4 + elementSize;   //name + size
            offset += 4 + elementSize;
         }
         return size;
      }
   case eDictionaryStructOffsets:
      {
         uint8_t numberData = data[offset+1];
         uint8_t numberSubTypes = data[offset+2];

         //FastPath: take last element and add size of the last element
         offset += 3 + (numberSubTypes-1)*(1+4+4);

         uint32_t offsetInStruct = data[offset+8];
         offsetInStruct <<= 8;
         offsetInStruct |= data[offset+7];
         offsetInStruct <<= 8;
         offsetInStruct |= data[offset+6];
         offsetInStruct <<= 8;
         offsetInStruct |= data[offset+5];

         uint32_t lastVarSize = getPureDataSize(data, offset+4);
         return 1 + 2 + numberSubTypes*(4+1+4) + (offsetInStruct+lastVarSize)*numberData;
      }
   default:
      ASSERT(0);
   }
   return 0;
}

}; //namespace DataBlob
