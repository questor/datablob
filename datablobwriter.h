
#ifndef __DATABLOBWRITER_H__
#define __DATABLOBWRITER_H__

#include "eastl/vector.h"
#include "eastl/string.h"
#include "eaio/FileStream.h"
#include "datablob.h"

#ifdef DATABLOB_ENABLE_SNAPPY_COMPRESSION
#include "snappy/snappy-c.h"
#endif

namespace DataBlob {

//TAKE CARE! SIDEEFFECTS BY SHIFTING OF THE VARIABLE!!!
#define DATABLOB_WRITE8BIT(x) \
            mData.pushBack((x)&0xff)
#define DATABLOB_WRITE16BIT(x) \
            mData.pushBack((x)&0xff); \
            x >>= 8; \
            mData.pushBack((x)&0xff)
#define DATABLOB_WRITE32BIT(x) \
            mData.pushBack((x)&0xff); \
            x >>= 8; \
            mData.pushBack((x)&0xff); \
            x >>= 8; \
            mData.pushBack((x)&0xff); \
            x >>= 8; \
            mData.pushBack((x)&0xff)
#define DATABLOB_WRITE64BIT(x) \
            mData.pushBack((x)&0xff); \
            x >>= 8; \
            mData.pushBack((x)&0xff); \
            x >>= 8; \
            mData.pushBack((x)&0xff); \
            x >>= 8; \
            mData.pushBack((x)&0xff); \
            x >>= 8; \
            mData.pushBack((x)&0xff); \
            x >>= 8; \
            mData.pushBack((x)&0xff); \
            x >>= 8; \
            mData.pushBack((x)&0xff); \
            x >>= 8; \
            mData.pushBack((x)&0xff)
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

//WHEN CHANGING THE MACROS TAKE CARE THAT IN THE ARRAY FUNCTIONS THE MEMORY WRITE CAN BE CHANGED, TOO

class DictionaryStructOffsetsWriter {
public:
   DictionaryStructOffsetsWriter(uint8_t numberElements)
      : mCurrentOffset(0)
   {
      mData.pushBack(numberElements);  //numberData
      mData.pushBack(0);               //numberSubTypes
   }
   void addType(uint32_t name, uint8_t type) {
      ASSERT( (type != eArrayByteSize) && (type != eDictionary) && (type != eDictionaryStructOffsets) && (type != eString));
      ++mData[1];    //numberSubTypes

      DATABLOB_WRITE32BIT(name);

      mData.pushBack(type);
      uint32_t size = getPureDataSize(mData, mData.size()-1);

      uint32_t offset = mCurrentOffset;
      DATABLOB_WRITE32BIT(offset);

      mCurrentOffset += size;
   }

   //to fill in the data:
   void addUInt8(uint8_t val) {
      DATABLOB_WRITE8BIT(val);
   }
   void addUInt16(uint16_t val) {
      DATABLOB_WRITE16BIT(val);
   }
   void addUInt32(uint32_t val) {
      DATABLOB_WRITE32BIT(val);
   }
   void addUInt64(uint64_t val) {
      DATABLOB_WRITE64BIT(val);
   }
   void addInt8(int8_t val) {
      DATABLOB_WRITE8BIT(val);
   }
   void addInt16(int16_t val) {
      DATABLOB_WRITE16BIT(val);
   }
   void addInt32(int32_t val) {
      DATABLOB_WRITE32BIT(val);
   }
   void addFloat(float fval) {
      uint32_t val = *((uint32_t*)&fval);
      DATABLOB_WRITE32BIT(val);
   }
   void addVector2f(float v1, float v2) {
      uint32_t val = *((uint32_t*)&v1);
      DATABLOB_WRITE32BIT(val);

      val = *((uint32_t*)&v2);
      DATABLOB_WRITE32BIT(val);
   }
   void addVector3f(float v1, float v2, float v3) {
      uint32_t val = *((uint32_t*)&v1);
      DATABLOB_WRITE32BIT(val);

      val = *((uint32_t*)&v2);
      DATABLOB_WRITE32BIT(val);

      val = *((uint32_t*)&v3);
      DATABLOB_WRITE32BIT(val);
   }
   void addVector4f(float v1, float v2, float v3, float v4) {
      uint32_t val = *((uint32_t*)&v1);
      DATABLOB_WRITE32BIT(val);

      val = *((uint32_t*)&v2);
      DATABLOB_WRITE32BIT(val);

      val = *((uint32_t*)&v3);
      DATABLOB_WRITE32BIT(val);

      val = *((uint32_t*)&v4);
      DATABLOB_WRITE32BIT(val);
   }
   void addVector2i(int32_t v1, int32_t v2) {
      DATABLOB_WRITE32BIT(v1);
      DATABLOB_WRITE32BIT(v2);
   }

   const eastl::vector<uint8_t> &getData() {
      return mData;
   }
protected:
   uint32_t mCurrentOffset;
   eastl::vector<uint8_t> mData;
};

//=================================== Helper for Arrays
struct ByteSizeWriteHelper {
   static void initNumberElements(eastl::vector<uint8_t> &mData) {
      DATABLOB_WRITE8BIT(0);
   }
   static void incrementNumberElements(eastl::vector<uint8_t> &mData) {
      ASSERT(mData[0] != 0xff);     //overflow?
      ++mData[0];
   }
};
struct ShortSizeWriteHelper {
   static void initNumberElements(eastl::vector<uint8_t> &mData) {
      DATABLOB_WRITE8BIT(0);
      DATABLOB_WRITE8BIT(0);
   }
   static void incrementNumberElements(eastl::vector<uint8_t> &mData) {
      uint16_t count;
      DATABLOB_READ16BITS(count, 0);
      ASSERT(count != 0xffff);     //overflow?
      ++count;
      //WE CAN'T USE THE MACROS BECAUSE WE DON'T WANT TO PUSH THE VALUES!
      mData[0] = count & 0xff;
      count >>= 8;
      mData[1] = count & 0xff;
   }
};
struct LongSizeWriteHelper {
   static void initNumberElements(eastl::vector<uint8_t> &mData) {
      DATABLOB_WRITE8BIT(0);
      DATABLOB_WRITE8BIT(0);
      DATABLOB_WRITE8BIT(0);
      DATABLOB_WRITE8BIT(0);
   }
   static void incrementNumberElements(eastl::vector<uint8_t> &mData) {
      uint32_t count;
      DATABLOB_READ32BITS(count, 0);
      ASSERT(count != 0xffffffff);     //overflow?
      ++count;
      //WE CAN'T USE THE MACROS BECAUSE WE DON'T WANT TO PUSH THE VALUES!
      mData[0] = count & 0xff;
      count >>= 8;
      mData[1] = count & 0xff;
      count >>= 8;
      mData[2] = count & 0xff;
      count >>= 8;
      mData[3] = count & 0xff;
   }
};


template<class SizeHelper> class GenericArrayWriter {
public:
   GenericArrayWriter(uint8_t type)
      : mType(type)
   {
      SizeHelper::initNumberElements(mData);
      DATABLOB_WRITE8BIT(type);
   }
   void addUInt8(uint8_t val) {
      ASSERT(mType == eUInt8);
      SizeHelper::incrementNumberElements(mData);
      DATABLOB_WRITE8BIT(val);
   }
   void addUInt16(uint16_t val) {
      ASSERT(mType == eUInt16);
      SizeHelper::incrementNumberElements(mData);
      DATABLOB_WRITE16BIT(val);
   }
   void addUInt32(uint32_t val) {
      ASSERT(mType == eUInt32);
      SizeHelper::incrementNumberElements(mData);
      DATABLOB_WRITE32BIT(val);
   }
   void addUInt64(uint64_t val) {
      ASSERT(mType == eUInt64);
      SizeHelper::incrementNumberElements(mData);
      DATABLOB_WRITE64BIT(val);
   }
   void addInt8(int8_t val) {
      ASSERT(mType == eInt8);
      SizeHelper::incrementNumberElements(mData);
      DATABLOB_WRITE8BIT(val);
   }
   void addInt16(int16_t val) {
      ASSERT(mType == eInt16);
      SizeHelper::incrementNumberElements(mData);
      DATABLOB_WRITE16BIT(val);
   }
   void addInt32(int32_t val) {
      ASSERT(mType == eInt32);
      SizeHelper::incrementNumberElements(mData);
      DATABLOB_WRITE32BIT(val);
   }
   void addFloat(float fval) {
      ASSERT(mType == eFloat);
      SizeHelper::incrementNumberElements(mData);
      uint32_t val = *((uint32_t*)&fval);
      DATABLOB_WRITE32BIT(val);
   }
   void addVector2f(float v1, float v2) {
      ASSERT(mType == eVector2f);
      SizeHelper::incrementNumberElements(mData);
      uint32_t val = *((uint32_t*)&v1);
      DATABLOB_WRITE32BIT(val);

      val = *((uint32_t*)&v2);
      DATABLOB_WRITE32BIT(val);
   }
   void addVector3f(float v1, float v2, float v3) {
      ASSERT(mType == eVector3f);
      SizeHelper::incrementNumberElements(mData);
      uint32_t val = *((uint32_t*)&v1);
      DATABLOB_WRITE32BIT(val);

      val = *((uint32_t*)&v2);
      DATABLOB_WRITE32BIT(val);

      val = *((uint32_t*)&v3);
      DATABLOB_WRITE32BIT(val);
   }
   void addVector4f(float v1, float v2, float v3, float v4) {
      ASSERT(mType == eVector4f);
      SizeHelper::incrementNumberElements(mData);
      uint32_t val = *((uint32_t*)&v1);
      DATABLOB_WRITE32BIT(val);

      val = *((uint32_t*)&v2);
      DATABLOB_WRITE32BIT(val);

      val = *((uint32_t*)&v3);
      DATABLOB_WRITE32BIT(val);

      val = *((uint32_t*)&v4);
      DATABLOB_WRITE32BIT(val);
   }
   void addVector2i(int32_t v1, int32_t v2) {
      ASSERT(mType == eVector2i);
      SizeHelper::incrementNumberElements(mData);
      DATABLOB_WRITE32BIT(v1);
      DATABLOB_WRITE32BIT(v2);
   }

   uint8_t getType() {
      return mType;
   }
   const eastl::vector<uint8_t> &getData() {
      return mData;
   }
protected:
   uint8_t mType;
   eastl::vector<uint8_t> mData;
};

typedef GenericArrayWriter<ByteSizeWriteHelper> ArrayByteSizeWriter;
typedef GenericArrayWriter<ShortSizeWriteHelper> ArrayShortSizeWriter;
typedef GenericArrayWriter<LongSizeWriteHelper> ArrayLongSizeWriter;

class DictionaryWriter {
public:
   DictionaryWriter() {
      mData.pushBack(0);               //numberSubTypes
   }
   void addUInt8(uint32_t name, uint8_t val) {
      ++mData[0];       //numberSubTypes

      DATABLOB_WRITE32BIT(name);
      DATABLOB_WRITE8BIT(eUInt8);
      DATABLOB_WRITE8BIT(val);
   }
   void addUInt16(uint32_t name, uint16_t val) {
      ++mData[0];       //numberSubTypes

      DATABLOB_WRITE32BIT(name);
      DATABLOB_WRITE8BIT(eUInt16);
      DATABLOB_WRITE16BIT(val);
   }
   void addUInt32(uint32_t name, uint32_t val) {
      ++mData[0];       //numberSubTypes

      DATABLOB_WRITE32BIT(name);
      DATABLOB_WRITE8BIT(eUInt32);
      DATABLOB_WRITE32BIT(val);
   }
   void addUInt64(uint32_t name, uint64_t val) {
      ++mData[0];       //numberSubTypes

      DATABLOB_WRITE32BIT(name);
      DATABLOB_WRITE8BIT(eUInt64);
      DATABLOB_WRITE64BIT(val);
   }
   void addInt8(uint32_t name, int8_t val) {
      ++mData[0];       //numberSubTypes

      DATABLOB_WRITE32BIT(name);
      DATABLOB_WRITE8BIT(eInt8);
      DATABLOB_WRITE8BIT(val);
   }
   void addInt16(uint32_t name, int16_t val) {
      ++mData[0];       //numberSubTypes

      DATABLOB_WRITE32BIT(name);
      DATABLOB_WRITE8BIT(eInt16);
      DATABLOB_WRITE16BIT(val);
   }
   void addInt32(uint32_t name, int32_t val) {
      ++mData[0];       //numberSubTypes

      DATABLOB_WRITE32BIT(name);
      DATABLOB_WRITE8BIT(eInt32);
      DATABLOB_WRITE32BIT(val);
   }
   void addFloat(uint32_t name, float fval) {
      ++mData[0];       //numberSubTypes

      DATABLOB_WRITE32BIT(name);
      DATABLOB_WRITE8BIT(eFloat);
      uint32_t val = *((uint32_t*)&fval);
      DATABLOB_WRITE32BIT(val);
   }
   void addVector2f(uint32_t name, float v1, float v2) {
      ++mData[0];       //numberSubTypes

      DATABLOB_WRITE32BIT(name);
      DATABLOB_WRITE8BIT(eVector2f);

      uint32_t val = *((uint32_t*)&v1);
      DATABLOB_WRITE32BIT(val);

      val = *((uint32_t*)&v2);
      DATABLOB_WRITE32BIT(val);
   }
   void addVector3f(uint32_t name, float v1, float v2, float v3) {
      ++mData[0];       //numberSubTypes

      DATABLOB_WRITE32BIT(name);
      DATABLOB_WRITE8BIT(eVector3f);

      uint32_t val = *((uint32_t*)&v1);
      DATABLOB_WRITE32BIT(val);

      val = *((uint32_t*)&v2);
      DATABLOB_WRITE32BIT(val);

      val = *((uint32_t*)&v3);
      DATABLOB_WRITE32BIT(val);
   }
   void addVector4f(uint32_t name, float v1, float v2, float v3, float v4) {
      ++mData[0];       //numberSubTypes

      DATABLOB_WRITE32BIT(name);
      DATABLOB_WRITE8BIT(eVector4f);

      uint32_t val = *((uint32_t*)&v1);
      DATABLOB_WRITE32BIT(val);

      val = *((uint32_t*)&v2);
      DATABLOB_WRITE32BIT(val);

      val = *((uint32_t*)&v3);
      DATABLOB_WRITE32BIT(val);

      val = *((uint32_t*)&v4);
      DATABLOB_WRITE32BIT(val);
   }
   void addVector2i(uint32_t name, int32_t v1, int32_t v2) {
      ++mData[0];       //numberSubTypes

      DATABLOB_WRITE32BIT(name);
      DATABLOB_WRITE8BIT(eVector2i);

      DATABLOB_WRITE32BIT(v1);
      DATABLOB_WRITE32BIT(v2);
   }
   void addString(uint32_t name, eastl::string str) {
      ++mData[0];       //numberSubTypes

      DATABLOB_WRITE32BIT(name);
      DATABLOB_WRITE8BIT(eString);

      uint16_t length = str.size();
      ASSERT(length <= 0xffff);
      DATABLOB_WRITE16BIT(length);

      for(uint_t i=0, end=str.size(); i<end; ++i) {
         mData.pushBack(str[i]);
      }
   }

   void addDictionaryStructOffsets(uint32_t name, DictionaryStructOffsetsWriter &blob) {
      ++mData[0];       //numberSubTypes

      DATABLOB_WRITE32BIT(name);
      DATABLOB_WRITE8BIT(eDictionaryStructOffsets);
      mData.insert(mData.end(), blob.getData().begin(), blob.getData().end());
   }

   void addDictionary(uint32_t name, DictionaryWriter &blob) {
      ++mData[0];       //numberSubTypes

      DATABLOB_WRITE32BIT(name);
      DATABLOB_WRITE8BIT(eDictionary);
      mData.insert(mData.end(), blob.getData().begin(), blob.getData().end());
   }

   void addArray(uint32_t name, ArrayByteSizeWriter &arrayBlob) {
      ++mData[0];       //numberSubTypes

      DATABLOB_WRITE32BIT(name);
      DATABLOB_WRITE8BIT(eArrayByteSize);
      mData.insert(mData.end(), arrayBlob.getData().begin(), arrayBlob.getData().end());
   }
   void addArray(uint32_t name, ArrayShortSizeWriter &arrayBlob) {
      ++mData[0];       //numberSubTypes

      DATABLOB_WRITE32BIT(name);
      DATABLOB_WRITE8BIT(eArrayShortSize);
      mData.insert(mData.end(), arrayBlob.getData().begin(), arrayBlob.getData().end());
   }
   void addArray(uint32_t name, ArrayLongSizeWriter &arrayBlob) {
      ++mData[0];       //numberSubTypes

      DATABLOB_WRITE32BIT(name);
      DATABLOB_WRITE8BIT(eArrayLongSize);
      mData.insert(mData.end(), arrayBlob.getData().begin(), arrayBlob.getData().end());
   }

   const eastl::vector<uint8_t> &getData() {
      return mData;
   }
protected:
   eastl::vector<uint8_t> mData;
};


class Writer {
public:
   void addArray(ArrayByteSizeWriter &arrayBlob) {
      mData.pushBack(eArrayByteSize);
      mData.insert(mData.end(), arrayBlob.getData().begin(), arrayBlob.getData().end());
   }
   void addArray(ArrayShortSizeWriter &arrayBlob) {
      mData.pushBack(eArrayShortSize);
      mData.insert(mData.end(), arrayBlob.getData().begin(), arrayBlob.getData().end());
   }
   void addArray(ArrayLongSizeWriter &arrayBlob) {
      mData.pushBack(eArrayLongSize);
      mData.insert(mData.end(), arrayBlob.getData().begin(), arrayBlob.getData().end());
   }
   void addDictionaryStructOffsets(DictionaryStructOffsetsWriter &blob) {
      mData.pushBack(eDictionaryStructOffsets);
      mData.insert(mData.end(), blob.getData().begin(), blob.getData().end());
   }
   void addDictionary(DictionaryWriter &blob) {
      mData.pushBack(eDictionary);
      mData.insert(mData.end(), blob.getData().begin(), blob.getData().end());
   }

   enum Compression {
      eNone,
      eSnappy,
   };
   void writeToDisc(const char *filename, Compression &compression, bool verify = false) {
      bool dumpToFile = false;
      if(compression == eSnappy) {
#ifdef DATABLOB_ENABLE_SNAPPY_COMPRESSION
         size_t outLength = snappy_max_compressed_length(mData.size());
         uint8_t *outBuffer = new uint8_t[outLength];
         snappy_status status = snappy_compress((const char*)mData.data(), mData.size(), (char*)outBuffer, &outLength);
         if((status == SNAPPY_OK) && (outLength < mData.size())) {
            if(verify) {
               status = snappy_validate_compressed_buffer((const char*)outBuffer, outLength);
               if(status != SNAPPY_OK) {
                  printf("snappy verify failed!\n");
               }
            }

            printf("data compressed with snappy from %d bytes to %d bytes\n", mData.size(), outLength);
            eaio::FileStream filestream(filename);
            filestream.open(eaio::kAccessFlagWrite);
            uint32_t signature = ('S'<<24) + ('N'<<16) + ('P'<<8) + 0x01;
            filestream.write(&signature, sizeof(uint32_t));
            filestream.write(outBuffer, outLength);
            filestream.close();
         } else if((status == SNAPPY_BUFFER_TOO_SMALL) || (outLength >= mData.size())) {
            printf("compressed data is bigger than source, dumping bytes without compression(outLength %d, original size %d)\n", outLength, mData.size());
            printf("snappy status %d\n", status);
            dumpToFile = true;
            compression = eNone;
         } else {
            printf("! compression returned with error %d!\n", status);
         }
         delete[] outBuffer;
#else
         ASSERT(!"Snappy compression requested, but not compiled with support for it");
#endif
      } else if(compression == eNone) {
         dumpToFile = true;
      } else {
         ASSERT(!"compression not supported");
      }

      if(dumpToFile) {
         eaio::FileStream filestream(filename);
         filestream.open(eaio::kAccessFlagWrite);
         filestream.write(mData.data(), mData.size());
         filestream.close();
      }
   }

   void dumpToConsole() {
      printf("\nUsedMemory: %d\n", mData.size());
      for(uint_t i=0; i<mData.size(); ++i) {
         printf("%02X ", mData[i]);
      }
   }

   const eastl::vector<uint8_t> &getData() {
      return mData;
   }
protected:
   eastl::vector<uint8_t> mData;
};

#undef DATABLOB_WRITE8BIT
#undef DATABLOB_WRITE16BIT
#undef DATABLOB_WRITE32BIT
#undef DATABLOB_WRITE64BIT

};    //namespace DataBlob

#endif
