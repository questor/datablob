
#ifdef TEST_DATABLOB


#include "datablobreader.h"
#include "datablobwriter.h"
using namespace DataBlob;

#include "eastl/extra/murmurhash.h"
using namespace eastl;

#include "gtest/gtest.h"

TEST(BasicTest, VerticesStruct) {
   DictionaryWriter structure;

   DictionaryStructOffsetsWriter vertices(3);
   vertices.addType(FixedMurmurHash("PositionX"), eUInt32);
   vertices.addType(FixedMurmurHash("PositionY"), eUInt32);
   vertices.addType(FixedMurmurHash("PositionZ"), eUInt32);
   for(int i=0; i<3; ++i) {
      vertices.addUInt32(0x11111110+i);
      vertices.addUInt32(0x22222220+i);
      vertices.addUInt32(0x33333330+i);
   }
   structure.addDictionaryStructOffsets(FixedMurmurHash("Vertices"), vertices);

   DictionaryStructOffsetsWriter faces(2);
   faces.addType(FixedMurmurHash("Index1"), eUInt32);
   faces.addType(FixedMurmurHash("Index2"), eUInt32);
   faces.addType(FixedMurmurHash("Index3"), eUInt32);
   faces.addUInt32(0);
   faces.addUInt32(1);
   faces.addUInt32(2);
   faces.addUInt32(3);
   faces.addUInt32(2);
   faces.addUInt32(1);
   structure.addDictionaryStructOffsets(FixedMurmurHash("Faces"), faces);

   Writer test;
   test.addDictionary(structure);

   //============================

   Reader reader(test.getData());
   DictionaryStructOffsetsReader structReader(test.getData(), reader.queryOffsetTo("Vertices"));
   uint32_t offsetPosY = structReader.queryOffsetTo(FixedMurmurHash("PositionY"));
   EXPECT_EQ(offsetPosY, 4);
   uint32_t offsetToData = structReader.queryOffsetToElements();
   EXPECT_EQ(offsetToData, 36);
   uint32_t elementSize = structReader.getSizeOneElement();
   EXPECT_EQ(elementSize, 12);

   uint32_t posY1 = reader.getUInt32(offsetToData+offsetPosY);
   EXPECT_EQ(posY1, 0x22222220);
   uint32_t posY2 = reader.getUInt32(offsetToData+offsetPosY+elementSize);
   EXPECT_EQ(posY2, 0x22222221);
}

#endif
