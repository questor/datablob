datablob
========

binary datablob implementation

small sample from a 3d mesh loader
============
```
   char buffer[128];
   sprintf(buffer, "Mesh%d", index);

   DictionaryReader reader(mFileReader.getData(), mDictReader.queryOffsetTo(eastl::murmurString(buffer)));

   int features = reader.getUInt32("VertexFeatures");
...
   ArrayLongSizeReader positions(mFileReader.getData(), reader.queryOffsetTo("Vertices"));
   uint32_t numberVertices = positions.getNumberElements();
   for(uint_t i=0; i<numberVertices; ++i) {
      positions.getVector3f(i, ...);
   }
   ArrayLongSizeReader indices(mFileReader.getData(), reader.queryOffsetTo("Faces"));
   uint32_t numberIndices = indices.getNumberElements();
   for(uint_t i=0; i<numberIndices; ++i) {
      indexBuffer.pushBack(indices.getUInt32(i));
   }


```

implementation details
======================

```
Memory Layout:
 Magic "I100" (always read as uint32_t to check for correct endianess)
    uint8 Type
    Type data
    type == eString:
       uin16 strLength
       uin8 data
    if(Type==Array)         has always only one type in his store!
       uint8 numberObjectsInArray
       uint8 typeOfData
       type data
    if(Type==StringArray)
       uint8 numberObjectsInArray
       uint32 lengthOfAllData
       type data
    else if(Type == Dictionary)      describes always one object!
       uint8 numberSubTypes
          uint32 name
          uint8 TypeOfData
          Type data
    else if(Type == DictionaryStructOffsets
       uint8 numberData
       uint8 numberSubTypes
          uint32 name
          uint8 TypeOfData
          uint32 offsetInStructToData
```

compile options:

-`DATABLOB_ENABLE_SNAPPY_COMPRESSION`
  file is prefixed with "SNP"+0x01 and compressed
