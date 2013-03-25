
#ifdef TEST_DATABLOB

#include "datablobreader.h"
#include "datablobwriter.h"
using namespace DataBlob;

#include "eastl/extra/murmurhash.h"
using namespace eastl;

#include "gtest/gtest.h"


TEST(BasicTest, Dictionary) {
   DictionaryWriter str;
   str.addFloat(FixedMurmurHash("Name1"), 1234.56f);
   str.addInt8(FixedMurmurHash("Name2"), -0x68);
   str.addInt16(FixedMurmurHash("Name3"), -0x4876);
   str.addInt32(FixedMurmurHash("Name4"), -0x12345678);
   str.addString(FixedMurmurHash("Name5"), "ThisIsASmallTestString");
   str.addUInt8(FixedMurmurHash("Name6"), 0x32);
   str.addUInt16(FixedMurmurHash("Name7"), 0x8347);
   str.addUInt32(FixedMurmurHash("Name8"), 0x65873133);
   str.addUInt64(FixedMurmurHash("Name9"), 0x947d7a568b5f2950);
   str.addVector2f(FixedMurmurHash("Name10"), 74.63f, 1727.98f);
   str.addVector2i(FixedMurmurHash("Name11"), 9382, 71276);
   str.addVector3f(FixedMurmurHash("Name12"), 7126.1f, 17235.3f, 47836.1f);

   Writer test;
   test.addDictionary(str);

   //======================

   Reader reader(test.getData());
   EXPECT_EQ(reader.getFloat(reader.queryOffsetTo("Name1")), 1234.56f);
   EXPECT_EQ(reader.getInt8(reader.queryOffsetTo("Name2")), -0x68);
   EXPECT_EQ(reader.getInt16(reader.queryOffsetTo("Name3")), -0x4876);
   EXPECT_EQ(reader.getInt32(reader.queryOffsetTo("Name4")), -0x12345678);

   EXPECT_EQ(reader.getString(reader.queryOffsetTo("Name5")), "ThisIsASmallTestString");

   EXPECT_EQ(reader.getUInt8(reader.queryOffsetTo("Name6")), 0x32);
   EXPECT_EQ(reader.getUInt16(reader.queryOffsetTo("Name7")), 0x8347);
   EXPECT_EQ(reader.getUInt32(reader.queryOffsetTo("Name8")), 0x65873133);
   EXPECT_EQ(reader.getUInt64(reader.queryOffsetTo("Name9")), 0x947d7a568b5f2950);

   float fv1, fv2, fv3;
   reader.getVector2f(reader.queryOffsetTo("Name10"), fv1, fv2);
   EXPECT_EQ(74.63f, fv1);
   EXPECT_EQ(1727.98f, fv2);
   int iv1, iv2;
   reader.getVector2i(reader.queryOffsetTo("Name11"), iv1, iv2);
   EXPECT_EQ(9382, iv1);
   EXPECT_EQ(71276, iv2);
   reader.getVector3f(reader.queryOffsetTo("Name12"), fv1, fv2, fv3);
   EXPECT_EQ(7126.1f, fv1);
   EXPECT_EQ(17235.3f, fv2);
   EXPECT_EQ(47836.1f, fv3);
}

#endif
