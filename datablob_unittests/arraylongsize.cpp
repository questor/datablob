
#ifdef TEST_DATABLOB

#include "datablobreader.h"
#include "datablobwriter.h"
using namespace DataBlob;

#include "eastl/extra/murmurhash.h"
using namespace eastl;

#include "gtest/gtest.h"

static const int cNumberElements = 70000;

TEST(BasicTest, ArrayLongSizeUInt8) {
   ArrayLongSizeWriter array(eUInt8);
   for(int i=0; i<cNumberElements; ++i)
      array.addUInt8(i);
   Writer test;
   test.addArray(array);
   //======================
   ArrayLongSizeReader arrayReader(test.getData(), 1);  //skip array marker
   EXPECT_EQ(cNumberElements, arrayReader.getNumberElements());
   EXPECT_EQ(4+1+1+cNumberElements*sizeof(uint8_t), test.getData().size());
   for(int i=0; i<cNumberElements; ++i) {
      EXPECT_EQ(i&0xff, arrayReader.getUInt8(i));
   }
}

TEST(BasicTest, ArrayLongSizeUInt16) {
   ArrayLongSizeWriter array(eUInt16);
   for(int i=0; i<cNumberElements; ++i)
      array.addUInt16(i);
   Writer test;
   test.addArray(array);
   //======================
   ArrayLongSizeReader arrayReader(test.getData(), 1);  //skip array marker
   EXPECT_EQ(cNumberElements, arrayReader.getNumberElements());
   EXPECT_EQ(4+1+1+cNumberElements*sizeof(uint16_t), test.getData().size());
   for(int i=0; i<cNumberElements; ++i) {
      EXPECT_EQ(i&0xffff, arrayReader.getUInt16(i));
   }
}

TEST(BasicTest, ArrayLongSizeUInt32) {
   ArrayLongSizeWriter array(eUInt32);
   for(int i=0; i<cNumberElements; ++i)
      array.addUInt32(i);
   Writer test;
   test.addArray(array);
   //======================
   ArrayLongSizeReader arrayReader(test.getData(), 1);  //skip array marker
   EXPECT_EQ(cNumberElements, arrayReader.getNumberElements());
   EXPECT_EQ(4+1+1+cNumberElements*sizeof(uint32_t), test.getData().size());
   for(int i=0; i<cNumberElements; ++i) {
      EXPECT_EQ(i, arrayReader.getUInt32(i));
   }
}

TEST(BasicTest, ArrayLongSizeUInt64) {
   ArrayLongSizeWriter array(eUInt64);
   for(int i=0; i<cNumberElements; ++i)
      array.addUInt64(i);
   Writer test;
   test.addArray(array);
   //======================
   ArrayLongSizeReader arrayReader(test.getData(), 1);  //skip array marker
   EXPECT_EQ(cNumberElements, arrayReader.getNumberElements());
   EXPECT_EQ(4+1+1+cNumberElements*sizeof(uint64_t), test.getData().size());
   for(int i=0; i<cNumberElements; ++i) {
      EXPECT_EQ(i, arrayReader.getUInt64(i));
   }
}

TEST(BasicTest, ArrayLongSizeInt8) {
   ArrayLongSizeWriter array(eInt8);
   for(int i=0; i<cNumberElements; ++i)
      array.addInt8(i);
   Writer test;
   test.addArray(array);
   //======================
   ArrayLongSizeReader arrayReader(test.getData(), 1);  //skip array marker
   EXPECT_EQ(cNumberElements, arrayReader.getNumberElements());
   EXPECT_EQ(4+1+1+cNumberElements*sizeof(int8_t), test.getData().size());
   for(int i=0; i<cNumberElements; ++i) {
      char expectedValue = i;
      EXPECT_EQ(expectedValue, arrayReader.getInt8(i));
   }
}

TEST(BasicTest, ArrayLongSizeInt16) {
   ArrayLongSizeWriter array(eInt16);
   for(int i=0; i<cNumberElements; ++i)
      array.addInt16(i);
   Writer test;
   test.addArray(array);
   //======================
   ArrayLongSizeReader arrayReader(test.getData(), 1);  //skip array marker
   EXPECT_EQ(cNumberElements, arrayReader.getNumberElements());
   EXPECT_EQ(4+1+1+cNumberElements*sizeof(int16_t), test.getData().size());
   for(int i=0; i<cNumberElements; ++i) {
      short expectedValue = i;
      EXPECT_EQ(expectedValue, arrayReader.getInt16(i));
   }
}

TEST(BasicTest, ArrayLongSizeInt32) {
   ArrayLongSizeWriter array(eInt32);
   for(int i=0; i<cNumberElements; ++i)
      array.addInt32(i);
   Writer test;
   test.addArray(array);
   //======================
   ArrayLongSizeReader arrayReader(test.getData(), 1);  //skip array marker
   EXPECT_EQ(cNumberElements, arrayReader.getNumberElements());
   EXPECT_EQ(4+1+1+cNumberElements*sizeof(int32_t), test.getData().size());
   for(int i=0; i<cNumberElements; ++i) {
      EXPECT_EQ(i, arrayReader.getInt32(i));
   }
}

TEST(BasicTest, ArrayLongSizeFloat) {
   ArrayLongSizeWriter array(eFloat);
   for(int i=0; i<cNumberElements; ++i)
      array.addFloat(i);
   Writer test;
   test.addArray(array);
   //======================
   ArrayLongSizeReader arrayReader(test.getData(), 1);  //skip array marker
   EXPECT_EQ(cNumberElements, arrayReader.getNumberElements());
   EXPECT_EQ(4+1+1+cNumberElements*sizeof(float), test.getData().size());
   for(int i=0; i<cNumberElements; ++i) {
      EXPECT_EQ(i, arrayReader.getFloat(i));
   }
}

TEST(BasicTest, ArrayLongSizeVector2f) {
   ArrayLongSizeWriter array(eVector2f);
   for(int i=0; i<cNumberElements; ++i) {
      array.addVector2f(i, i*4);
   }
   Writer test;
   test.addArray(array);
   //======================
   ArrayLongSizeReader arrayReader(test.getData(), 1);  //skip array marker
   EXPECT_EQ(cNumberElements, arrayReader.getNumberElements());
   EXPECT_EQ(4+1+1+cNumberElements*sizeof(float)*2, test.getData().size());
   for(int i=0; i<cNumberElements; ++i) {
      float v1, v2;
      arrayReader.getVector2f(i, v1, v2);
      EXPECT_EQ(i, v1);
      EXPECT_EQ(i*4, v2);
   }
}

TEST(BasicTest, ArrayLongSizeVector2i) {
   ArrayLongSizeWriter array(eVector2i);
   for(int i=0; i<cNumberElements; ++i) {
      array.addVector2i(i, i*3);
   }
   Writer test;
   test.addArray(array);
   //======================
   ArrayLongSizeReader arrayReader(test.getData(), 1);  //skip array marker
   EXPECT_EQ(cNumberElements, arrayReader.getNumberElements());
   EXPECT_EQ(4+1+1+cNumberElements*sizeof(int32_t)*2, test.getData().size());
   for(int i=0; i<cNumberElements; ++i) {
      int_t v1, v2;
      arrayReader.getVector2i(i, v1, v2);
      EXPECT_EQ(i, v1);
      EXPECT_EQ(i*3, v2);
   }
}

TEST(BasicTest, ArrayLongSizeVector3f) {
   ArrayLongSizeWriter array(eVector3f);
   for(int i=0; i<cNumberElements; ++i) {
      array.addVector3f(i, i*5, i*6);
   }
   Writer test;
   test.addArray(array);
   //======================
   ArrayLongSizeReader arrayReader(test.getData(), 1);  //skip array marker
   EXPECT_EQ(cNumberElements, arrayReader.getNumberElements());
   EXPECT_EQ(4+1+1+cNumberElements*sizeof(float)*3, test.getData().size());
   for(int i=0; i<cNumberElements; ++i) {
      float v1, v2, v3;
      arrayReader.getVector3f(i, v1, v2, v3);
      EXPECT_EQ(i, v1);
      EXPECT_EQ(i*5, v2);
      EXPECT_EQ(i*6, v3);
   }
}

#endif
