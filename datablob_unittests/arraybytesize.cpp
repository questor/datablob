
#ifdef TEST_DATABLOB

#include "datablobreader.h"
#include "datablobwriter.h"
using namespace DataBlob;

#include "eastl/extra/murmurhash.h"
using namespace eastl;

#include "gtest/gtest.h"


TEST(BasicTest, ArrayByteSizeUInt8) {
   ArrayByteSizeWriter array(eUInt8);
   for(int i=0; i<150; ++i)
      array.addUInt8(i);
   Writer test;
   test.addArray(array);
   //======================
   ArrayByteSizeReader arrayReader(test.getData(), 1);  //skip array marker
   EXPECT_EQ(150, arrayReader.getNumberElements());
   EXPECT_EQ(1+1+1+150*sizeof(uint8_t), test.getData().size());
   for(int i=0; i<150; ++i) {
      EXPECT_EQ(i, arrayReader.getUInt8(i));
   }
}

TEST(BasicTest, ArrayByteSizeUInt16) {
   ArrayByteSizeWriter array(eUInt16);
   for(int i=0; i<150; ++i)
      array.addUInt16(i);
   Writer test;
   test.addArray(array);
   //======================
   ArrayByteSizeReader arrayReader(test.getData(), 1);  //skip array marker
   EXPECT_EQ(150, arrayReader.getNumberElements());
   EXPECT_EQ(1+1+1+150*sizeof(uint16_t), test.getData().size());
   for(int i=0; i<150; ++i) {
      EXPECT_EQ(i, arrayReader.getUInt16(i));
   }
}

TEST(BasicTest, ArrayByteSizeUInt32) {
   ArrayByteSizeWriter array(eUInt32);
   for(int i=0; i<150; ++i)
      array.addUInt32(i);
   Writer test;
   test.addArray(array);
   //======================
   ArrayByteSizeReader arrayReader(test.getData(), 1);  //skip array marker
   EXPECT_EQ(150, arrayReader.getNumberElements());
   EXPECT_EQ(1+1+1+150*sizeof(uint32_t), test.getData().size());
   for(int i=0; i<150; ++i) {
      EXPECT_EQ(i, arrayReader.getUInt32(i));
   }
}

TEST(BasicTest, ArrayByteSizeUInt64) {
   ArrayByteSizeWriter array(eUInt64);
   for(int i=0; i<150; ++i)
      array.addUInt64(i);
   Writer test;
   test.addArray(array);
   //======================
   ArrayByteSizeReader arrayReader(test.getData(), 1);  //skip array marker
   EXPECT_EQ(150, arrayReader.getNumberElements());
   EXPECT_EQ(1+1+1+150*sizeof(uint64_t), test.getData().size());
   for(int i=0; i<150; ++i) {
      EXPECT_EQ(i, arrayReader.getUInt64(i));
   }
}

TEST(BasicTest, ArrayByteSizeInt8) {
   ArrayByteSizeWriter array(eInt8);
   for(int i=0; i<120; ++i)
      array.addInt8(i);
   Writer test;
   test.addArray(array);
   //======================
   ArrayByteSizeReader arrayReader(test.getData(), 1);  //skip array marker
   EXPECT_EQ(120, arrayReader.getNumberElements());
   EXPECT_EQ(1+1+1+120*sizeof(int8_t), test.getData().size());
   for(int i=0; i<120; ++i) {
      EXPECT_EQ(i, arrayReader.getInt8(i));
   }
}

TEST(BasicTest, ArrayByteSizeInt16) {
   ArrayByteSizeWriter array(eInt16);
   for(int i=0; i<150; ++i)
      array.addInt16(i);
   Writer test;
   test.addArray(array);
   //======================
   ArrayByteSizeReader arrayReader(test.getData(), 1);  //skip array marker
   EXPECT_EQ(150, arrayReader.getNumberElements());
   EXPECT_EQ(1+1+1+150*sizeof(int16_t), test.getData().size());
   for(int i=0; i<150; ++i) {
      EXPECT_EQ(i, arrayReader.getInt16(i));
   }
}

TEST(BasicTest, ArrayByteSizeInt32) {
   ArrayByteSizeWriter array(eInt32);
   for(int i=0; i<150; ++i)
      array.addInt32(i);
   Writer test;
   test.addArray(array);
   //======================
   ArrayByteSizeReader arrayReader(test.getData(), 1);  //skip array marker
   EXPECT_EQ(150, arrayReader.getNumberElements());
   EXPECT_EQ(1+1+1+150*sizeof(int32_t), test.getData().size());
   for(int i=0; i<150; ++i) {
      EXPECT_EQ(i, arrayReader.getInt32(i));
   }
}

TEST(BasicTest, ArrayByteSizeFloat) {
   ArrayByteSizeWriter array(eFloat);
   for(int i=0; i<150; ++i)
      array.addFloat(i);
   Writer test;
   test.addArray(array);
   //======================
   ArrayByteSizeReader arrayReader(test.getData(), 1);  //skip array marker
   EXPECT_EQ(150, arrayReader.getNumberElements());
   EXPECT_EQ(1+1+1+150*sizeof(float), test.getData().size());
   for(int i=0; i<150; ++i) {
      EXPECT_EQ(i, arrayReader.getFloat(i));
   }
}

TEST(BasicTest, ArrayByteSizeVector2f) {
   ArrayByteSizeWriter array(eVector2f);
   for(int i=0; i<150; ++i) {
      array.addVector2f(i, i*4);
   }
   Writer test;
   test.addArray(array);
   //======================
   ArrayByteSizeReader arrayReader(test.getData(), 1);  //skip array marker
   EXPECT_EQ(150, arrayReader.getNumberElements());
   EXPECT_EQ(1+1+1+150*sizeof(float)*2, test.getData().size());
   for(int i=0; i<150; ++i) {
      float v1, v2;
      arrayReader.getVector2f(i, v1, v2);
      EXPECT_EQ(i, v1);
      EXPECT_EQ(i*4, v2);
   }
}

TEST(BasicTest, ArrayByteSizeVector2i) {
   ArrayByteSizeWriter array(eVector2i);
   for(int i=0; i<150; ++i) {
      array.addVector2i(i, i*3);
   }
   Writer test;
   test.addArray(array);
   //======================
   ArrayByteSizeReader arrayReader(test.getData(), 1);  //skip array marker
   EXPECT_EQ(150, arrayReader.getNumberElements());
   EXPECT_EQ(1+1+1+150*sizeof(int32_t)*2, test.getData().size());
   for(int i=0; i<150; ++i) {
      int_t v1, v2;
      arrayReader.getVector2i(i, v1, v2);
      EXPECT_EQ(i, v1);
      EXPECT_EQ(i*3, v2);
   }
}

TEST(BasicTest, ArrayByteSizeVector3f) {
   ArrayByteSizeWriter array(eVector3f);
   for(int i=0; i<150; ++i) {
      array.addVector3f(i, i*5, i*6);
   }
   Writer test;
   test.addArray(array);
   //======================
   ArrayByteSizeReader arrayReader(test.getData(), 1);  //skip array marker
   EXPECT_EQ(150, arrayReader.getNumberElements());
   EXPECT_EQ(1+1+1+150*sizeof(float)*3, test.getData().size());
   for(int i=0; i<150; ++i) {
      float v1, v2, v3;
      arrayReader.getVector3f(i, v1, v2, v3);
      EXPECT_EQ(i, v1);
      EXPECT_EQ(i*5, v2);
      EXPECT_EQ(i*6, v3);
   }
}

#endif
