
#ifndef __DATABLOBREADER_H__
#define __DATABLOBREADER_H__

#include "datablob.h"

#include "eastl/string.h"
#include "eastl/extra/murmurhash.h"
#include "eaio/FileStream.h"

#ifdef DATABLOB_ENABLE_SNAPPY_COMPRESSION
#include "snappy/snappy-c.h"
#endif

namespace DataBlob {

#define DATABLOB_READ8BITS(var, offset) \
               var = mData[offset]
#define DATABLOB_READ16BITS(var, offset) \
               var = mData[offset+1]; \
               var <<= 8; \
               var |= mData[offset]
#define DATABLOB_READ32BITS(var, offset) \
               var = mData[offset+3]; \
               var <<= 8; \
               var |= mData[offset+2]; \
               var <<= 8; \
               var |= mData[offset+1]; \
               var <<= 8; \
               var |= mData[offset]
#define DATABLOB_READ64BITS(var, offset) \
               var = mData[offset+7]; \
               var <<= 8; \
               var |= mData[offset+6]; \
               var <<= 8; \
               var |= mData[offset+5]; \
               var <<= 8; \
               var |= mData[offset+4]; \
               var <<= 8; \
               var |= mData[offset+3]; \
               var <<= 8; \
               var |= mData[offset+2]; \
               var <<= 8; \
               var |= mData[offset+1]; \
               var <<= 8; \
               var |= mData[offset]

class DictionaryStructOffsetsReader {
public:
   DictionaryStructOffsetsReader(const eastl::vector<uint8_t> &data, uint32_t offset)
      : mData(data)
      , mOffset(offset+1)     //skip own type, hence +1
   {
   }
   uint8_t getNumberElements() {
      uint8_t numberElements;
      DATABLOB_READ8BITS(numberElements, mOffset);
      return numberElements;
   }

   uint32_t queryOffsetToElements() {
      uint8_t numberSubTypes;
      DATABLOB_READ8BITS(numberSubTypes, mOffset+1);
      return mOffset+2+numberSubTypes*(1+4+4);
   }
   uint32_t getSizeOneElement() {
      uint8_t numberSubTypes;
      DATABLOB_READ8BITS(numberSubTypes, mOffset+1);
      //FastPath: take last element and add size of the last element and return this as size
      uint32_t offset = mOffset + 2 + (numberSubTypes-1)*(1+4+4);

      uint32_t offsetInStruct;
      DATABLOB_READ32BITS(offsetInStruct, offset+5);

      uint32_t lastVarSize = getPureDataSize(mData, offset+4);
      return offsetInStruct + lastVarSize;
   }

   uint32_t queryOffsetTo(eastl::FixedMurmurHash nameToSearch) {
      uint8_t numberSubTypes;
      DATABLOB_READ8BITS(numberSubTypes, mOffset+1);
      uint32_t offset = mOffset+2;
      for(int i=0; i<numberSubTypes; ++i) {
         uint32_t name;
         DATABLOB_READ32BITS(name, offset);
         if(name == nameToSearch) {
            uint32_t offsetInStruct;
            DATABLOB_READ32BITS(offsetInStruct, offset+5);
            return offsetInStruct;
         }
         offset += 4+4+1;
      }
      ASSERT(0);
      return -1;
   }

protected:
   const eastl::vector<uint8_t> &mData;
   uint32_t mOffset;
};

//===================================

struct ByteSizeReadHelper {
   static const uint32_t offsetToType = 1;
   static const uint32_t offsetToData = 2;
   static uint_t getNumberElements(const eastl::vector<uint8_t> &mData, uint32_t offset) {
      uint8_t numberElements;
      DATABLOB_READ8BITS(numberElements, offset);
      return numberElements;
   }
};
struct ShortSizeReadHelper {
   static const uint32_t offsetToType = 2;
   static const uint32_t offsetToData = 3;
   static uint_t getNumberElements(const eastl::vector<uint8_t> &mData, uint32_t offset) {
      uint16_t numberElements;
      DATABLOB_READ16BITS(numberElements, offset);
      return numberElements;
   }
};
struct LongSizeReadHelper {
   static const uint32_t offsetToType = 4;
   static const uint32_t offsetToData = 5;
   static uint_t getNumberElements(const eastl::vector<uint8_t> &mData, uint32_t offset) {
      uint32_t numberElements;
      DATABLOB_READ32BITS(numberElements, offset);
      return numberElements;
   }
};

template<class SizeHelper> class GenericArrayReader {
public:
   GenericArrayReader(const eastl::vector<uint8_t> &data, uint32_t offset)
      : mData(data)
      , mOffset(offset+1)     //skip own type, hence +1
   {
   }
   uint_t getNumberElements() {
      return SizeHelper::getNumberElements(mData, mOffset);
   }
   uint8_t getTypeOfData() {
      uint8_t type;
      DATABLOB_READ8BITS(type, mOffset+SizeHelper::offsetToType);
      return type;
   }

   uint8_t getUInt8(uint32_t index) {
      ASSERT(mData[mOffset+SizeHelper::offsetToType] == eUInt8);
      uint8_t val;
      DATABLOB_READ8BITS(val, mOffset+SizeHelper::offsetToData+index*sizeof(uint8_t));
      return val;
   }
   uint16_t getUInt16(uint32_t index) {
      ASSERT(mData[mOffset+SizeHelper::offsetToType] == eUInt16);
      uint32_t val;
      DATABLOB_READ16BITS(val, mOffset+SizeHelper::offsetToData+index*sizeof(uint16_t));
      return val;
   }
   uint32_t getUInt32(uint32_t index) {
      ASSERT(mData[mOffset+SizeHelper::offsetToType] == eUInt32);
      uint32_t val;
      DATABLOB_READ32BITS(val, mOffset+SizeHelper::offsetToData+index*sizeof(uint32_t));
      return val;
   }
   uint64_t getUInt64(uint32_t index) {
      ASSERT(mData[mOffset+SizeHelper::offsetToType] == eUInt64);
      uint64_t val;
      DATABLOB_READ64BITS(val, mOffset+SizeHelper::offsetToData+index*sizeof(uint64_t));
      return val;
   }
   int8_t getInt8(uint32_t index) {
      ASSERT(mData[mOffset+SizeHelper::offsetToType] == eInt8);
      int8_t val;
      DATABLOB_READ8BITS(val, mOffset+SizeHelper::offsetToData+index*sizeof(int8_t));
      return val;
   }
   int16_t getInt16(uint32_t index) {
      ASSERT(mData[mOffset+SizeHelper::offsetToType] == eInt16);
      int32_t val;
      DATABLOB_READ16BITS(val, mOffset+SizeHelper::offsetToData+index*sizeof(int16_t));
      return val;
   }
   int32_t getInt32(uint32_t index) {
      ASSERT(mData[mOffset+SizeHelper::offsetToType] == eInt32);
      int32_t val;
      DATABLOB_READ32BITS(val, mOffset+SizeHelper::offsetToData+index*sizeof(int32_t));
      return val;
   }
   float getFloat(uint32_t index) {
      ASSERT(mData[mOffset+SizeHelper::offsetToType] == eFloat);
      uint32_t val;
      DATABLOB_READ32BITS(val, mOffset+SizeHelper::offsetToData+index*sizeof(float));
      float fval;
      *((uint32_t*)&fval) = val;
      return fval;
   }
   void getVector2f(uint32_t index, float &v1, float &v2) {
      ASSERT(mData[mOffset+SizeHelper::offsetToType] == eVector2f);
      uint32_t val;
      DATABLOB_READ32BITS(val, mOffset+SizeHelper::offsetToData+index*sizeof(float)*2);
      *((uint32_t*)&v1) = val;

      DATABLOB_READ32BITS(val, mOffset+SizeHelper::offsetToData+index*sizeof(float)*2+sizeof(float));
      *((uint32_t*)&v2) = val;
   }
   void getVector3f(uint32_t index, float &v1, float &v2, float &v3) {
      ASSERT(mData[mOffset+SizeHelper::offsetToType] == eVector3f);
      uint32_t val;
      DATABLOB_READ32BITS(val, mOffset+SizeHelper::offsetToData+index*sizeof(float)*3);
      *((uint32_t*)&v1) = val;

      DATABLOB_READ32BITS(val, mOffset+SizeHelper::offsetToData+index*sizeof(float)*3+sizeof(float));
      *((uint32_t*)&v2) = val;

      DATABLOB_READ32BITS(val, mOffset+SizeHelper::offsetToData+index*sizeof(float)*3+sizeof(float)*2);
      *((uint32_t*)&v3) = val;
   }
   void getVector4f(uint32_t index, float &v1, float &v2, float &v3, float &v4) {
      ASSERT(mData[mOffset+SizeHelper::offsetToType] == eVector4f);
      uint32_t val;
      DATABLOB_READ32BITS(val, mOffset+SizeHelper::offsetToData+index*sizeof(float)*3);
      *((uint32_t*)&v1) = val;

      DATABLOB_READ32BITS(val, mOffset+SizeHelper::offsetToData+index*sizeof(float)*3+sizeof(float));
      *((uint32_t*)&v2) = val;

      DATABLOB_READ32BITS(val, mOffset+SizeHelper::offsetToData+index*sizeof(float)*3+sizeof(float)*2);
      *((uint32_t*)&v3) = val;

      DATABLOB_READ32BITS(val, mOffset+SizeHelper::offsetToData+index*sizeof(float)*3+sizeof(float)*3);
      *((uint32_t*)&v4) = val;
   }
   void getVector2i(uint32_t index, int32_t &v1, int32_t &v2) {
      ASSERT(mData[mOffset+SizeHelper::offsetToType] == eVector2i);
      DATABLOB_READ32BITS(v1, mOffset+SizeHelper::offsetToData+index*sizeof(int32_t)*2);
      DATABLOB_READ32BITS(v2, mOffset+SizeHelper::offsetToData+index*sizeof(int32_t)*2+sizeof(int32_t));
   }

protected:
   const eastl::vector<uint8_t> &mData;
   uint32_t mOffset;
};

typedef GenericArrayReader<ByteSizeReadHelper> ArrayByteSizeReader;
typedef GenericArrayReader<ShortSizeReadHelper> ArrayShortSizeReader;
typedef GenericArrayReader<LongSizeReadHelper> ArrayLongSizeReader;


class DictionaryReader {
public:
   DictionaryReader(const eastl::vector<uint8_t> &data, uint32_t offset)
      : mData(data)
      , mOffset(offset)
   {}

   uint32_t queryOffsetTo(const eastl::FixedMurmurHash hash) {
      if(mData[mOffset+0] == eDictionary) {
         return queryDictionaryForOffsetTo(hash, 1);
      }
      return -1;
   }
   uint32_t queryOffsetTo(const uint32_t hash) {
       if(mData[mOffset+0] == eDictionary) {
          return queryDictionaryForOffsetTo(hash, 1);
       }
       return -1;
   }

   uint8_t getUInt8(uint32_t offset) {
      uint8_t val;
      ASSERT(mData[offset] == eUInt8);
      offset += 1;                        //skip type
      DATABLOB_READ8BITS(val, offset);
      return val;
   }
   uint8_t getUInt8(eastl::FixedMurmurHash hash) {
      return getUInt8(queryOffsetTo(hash));
   }

   uint16_t getUInt16(uint32_t offset) {
      uint32_t val;
      ASSERT(mData[offset] == eUInt16);
      offset += 1;                        //skip type
      DATABLOB_READ16BITS(val, offset);
      return val;
   }
   uint16_t getUInt16(eastl::FixedMurmurHash hash) {
      return getUInt16(queryOffsetTo(hash));
   }

   uint32_t getUInt32(uint32_t offset) {
      uint32_t val;
      ASSERT(mData[offset] == eUInt32);
      offset += 1;                        //skip type
      DATABLOB_READ32BITS(val, offset);
      return val;
   }
   uint32_t getUInt32(eastl::FixedMurmurHash hash) {
      return getUInt32(queryOffsetTo(hash));
   }

   uint64_t getUInt64(uint32_t offset) {
      uint64_t val;
      ASSERT(mData[offset] == eUInt64);
      offset += 1;                        //skip type
      DATABLOB_READ64BITS(val, offset);
      return val;
   }
   uint64_t getUInt64(eastl::FixedMurmurHash hash) {
      return getUInt64(queryOffsetTo(hash));
   }

   int8_t getInt8(uint32_t offset) {
      int8_t val;
      ASSERT(mData[offset] == eInt8);
      offset += 1;                        //skip type
      DATABLOB_READ8BITS(val, offset);
      return val;
   }
   int8_t getInt8(eastl::FixedMurmurHash hash) {
      return getInt8(queryOffsetTo(hash));
   }

   int16_t getInt16(uint32_t offset) {
      int32_t val;
      ASSERT(mData[offset] == eInt16);
      offset += 1;                        //skip type
      DATABLOB_READ16BITS(val, offset);
      return val;
   }
   int16_t getInt16(eastl::FixedMurmurHash hash) {
      return getInt16(queryOffsetTo(hash));
   }

   int32_t getInt32(uint32_t offset) {
      int32_t val;
      ASSERT(mData[offset] == eInt32);
      offset += 1;                        //skip type
      DATABLOB_READ32BITS(val, offset);
      return val;
   }
   int32_t getInt32(eastl::FixedMurmurHash hash) {
      return getInt32(queryOffsetTo(hash));
   }

   float getFloat(uint32_t offset) {
      uint32_t val;
      ASSERT(mData[offset] == eFloat);
      offset += 1;                        //skip type
      DATABLOB_READ32BITS(val, offset);
      float fval;
      *((uint32_t*)&fval) = val;
      return fval;
   }
   float getFloat(eastl::FixedMurmurHash hash) {
      return getFloat(queryOffsetTo(hash));
   }

   void getVector2f(uint32_t offset, float &v1, float &v2) {
      uint32_t val;
      ASSERT(mData[offset] == eVector2f);
      offset += 1;                        //skip type
      DATABLOB_READ32BITS(val, offset);
      *((uint32_t*)&v1) = val;

      DATABLOB_READ32BITS(val, offset+4);
      *((uint32_t*)&v2) = val;
   }
   void getVector2f(eastl::FixedMurmurHash hash, float &v1, float &v2) {
      getVector2f(queryOffsetTo(hash), v1, v2);
   }

   void getVector3f(uint32_t offset, float &v1, float &v2, float &v3) {
      uint32_t val;
      ASSERT(mData[offset] == eVector3f);
      offset += 1;                        //skip type
      DATABLOB_READ32BITS(val, offset);
      *((uint32_t*)&v1) = val;

      DATABLOB_READ32BITS(val, offset+4);
      *((uint32_t*)&v2) = val;

      DATABLOB_READ32BITS(val, offset+8);
      *((uint32_t*)&v3) = val;
   }
   void getVector3f(eastl::FixedMurmurHash hash, float &v1, float &v2, float &v3) {
      getVector3f(queryOffsetTo(hash), v1, v2, v3);
   }

   void getVector4f(uint32_t offset, float &v1, float &v2, float &v3, float &v4) {
      uint32_t val;
      ASSERT(mData[offset] == eVector4f);
      offset += 1;                        //skip type
      DATABLOB_READ32BITS(val, offset);
      *((uint32_t*)&v1) = val;

      DATABLOB_READ32BITS(val, offset+4);
      *((uint32_t*)&v2) = val;

      DATABLOB_READ32BITS(val, offset+8);
      *((uint32_t*)&v3) = val;

      DATABLOB_READ32BITS(val, offset+12);
      *((uint32_t*)&v4) = val;
   }
   void getVector4f(eastl::FixedMurmurHash hash, float &v1, float &v2, float &v3, float &v4) {
      getVector4f(queryOffsetTo(hash), v1, v2, v3, v4);
   }

   void getVector2i(uint32_t offset, int32_t &v1, int32_t &v2) {
      ASSERT(mData[offset] == eVector2i);
      offset += 1;                        //skip type
      DATABLOB_READ32BITS(v1, offset);
      DATABLOB_READ32BITS(v2, offset+4);
   }
   void getVector2i(eastl::FixedMurmurHash hash, int32_t &v1, int32_t &v2) {
      getVector2i(queryOffsetTo(hash), v1, v2);
   }

   eastl::string getString(uint32_t offset) {
      uint16_t length;
      ASSERT(mData[offset] == eString);
      offset += 1;                        //skip type
      DATABLOB_READ16BITS(length, offset);
      return eastl::string((char*)mData.data()+offset+2, (char*)mData.data()+offset+2+length);
   }
   eastl::string getString(eastl::FixedMurmurHash hash) {
      return getString(queryOffsetTo(hash));
   }

protected:
   uint32_t queryDictionaryForOffsetTo(uint32_t nameHash, uint32_t offsetToData) {
      offsetToData += mOffset;
      uint8_t numberSubTypes = mData[offsetToData];
      ++offsetToData;   //skip numberSubTypes
      for(int i=0; i<numberSubTypes; ++i) {
         uint32_t name;
         DATABLOB_READ32BITS(name, offsetToData);
         if(name == nameHash) {
            return offsetToData + 4;  //4(name)
         }
         //skip this datatype!
         offsetToData += 4 + getCompleteSize(mData, offsetToData+4); //4 for sizeof(name)
      }
      return -1;
   }

   const eastl::vector<uint8_t> &mData;
   const uint32_t mOffset;
};

class DataBlobFileReader {
public:
   DataBlobFileReader(const char *filename) {
      eaio::FileStream filestream(filename);
      if(filestream.open(eaio::kAccessFlagRead) == false) {
          ASSERT(!"FILE NOT FOUND");
      }
      size_t filelen = filestream.getSize();
      //check for signature
      uint32_t signature;
      filestream.read(&signature, sizeof(uint32_t));
      if(signature == ('S'<<24) + ('N'<<16) + ('P'<<8) + 0x01) {
#ifdef DATABLOB_ENABLE_SNAPPY_COMPRESSION
         filelen -= sizeof(uint32_t);     //remove size of signature
         uint8_t *inputBuffer = new uint8_t[filelen];
         filestream.read(inputBuffer, filelen);
         size_t outputLength;
         snappy_uncompressed_length((const char*)inputBuffer, filelen, &outputLength);
         mData.resize(outputLength);
         snappy_status status = snappy_uncompress((const char*)inputBuffer, filelen, (char*)mData.data(), &outputLength);
         if(status != SNAPPY_OK) {
            ASSERT(!"snappy uncompress failed with error code");
         }
         delete[] inputBuffer;
#else
         ASSERT(!"snappy compressed file found, but support library not present");
#endif
      } else {
         filestream.setPosition(0);    //rewind and read without compression
         mData.resize(filelen);
         filestream.read(mData.data(), filelen);
         filestream.close();
      }
   }
   const eastl::vector<uint8_t> &getData() {
      return mData;
   }

protected:
   eastl::vector<uint8_t> mData;
};

#undef DATABLOB_READ8BITS
#undef DATABLOB_READ16BITS
#undef DATABLOB_READ32BITS
#undef DATABLOB_READ64BITS

};    //namespace DataBlob

#endif
