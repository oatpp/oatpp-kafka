/***************************************************************************
 *
 * Project         _____    __   ____   _      _
 *                (  _  )  /__\ (_  _)_| |_  _| |_
 *                 )(_)(  /(__)\  )( (_   _)(_   _)
 *                (_____)(__)(__)(__)  |_|    |_|
 *
 *
 * Copyright 2018-present, Leonid Stryzhevskyi, <lganzzzo@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ***************************************************************************/

#ifndef oatpp_kafka_protocol_RecordBatch_hpp
#define oatpp_kafka_protocol_RecordBatch_hpp

#include "oatpp-kafka/protocol/mapping/Types.hpp"

#include "oatpp/core/parser/Caret.hpp"
#include "oatpp/core/data/stream/Stream.hpp"

#include "oatpp/core/data/mapping/type/Object.hpp"
#include "oatpp/core/macro/codegen.hpp"

namespace oatpp { namespace kafka { namespace protocol {
#include OATPP_CODEGEN_BEGIN(DTO)
  
class Header : public oatpp::data::mapping::type::Object {
  
  DTO_INIT(Header, Object)
  
  DTO_FIELD(Data, headerKey);
  DTO_FIELD(Data, headerValue);
  
};

class Record : public oatpp::data::mapping::type::Object {
  
  DTO_INIT(Record, Object)
  
  DTO_FIELD(VarInt, length);
  DTO_FIELD(Int8, attributes);
  DTO_FIELD(VarInt, timestampDelta);
  DTO_FIELD(VarInt, offsetDelta);
  DTO_FIELD(Data, key);
  DTO_FIELD(Data, value);
  DTO_FIELD(List<Header::ObjectWrapper>::ObjectWrapper, headers);
  
};

class RecordBatchHeader : public oatpp::data::mapping::type::Object {
  
  DTO_INIT(RecordBatchHeader, Object)
  
  DTO_FIELD(Int64, firstOffset);
  DTO_FIELD(Int32, length);
  DTO_FIELD(Int32, partitionLeaderEpoch);
  DTO_FIELD(Int8, magic);
  DTO_FIELD(Int32, crc);
  DTO_FIELD(Int16, attributes);
  
  DTO_FIELD(Int32, lastOffsetDelta);
  DTO_FIELD(Int64, firstTimestamp);
  DTO_FIELD(Int64, maxTimestamp);
  DTO_FIELD(Int64, producerId);
  
  DTO_FIELD(Int16, producerEpoch);
  DTO_FIELD(Int32, firstSequence);
  
};

class RecordBatch : public oatpp::base::Countable {
public:
  typedef oatpp::data::mapping::type::ObjectWrapper<RecordBatch, mapping::type::__class::RecordBatch> ObjectWrapper;
public:
  OBJECT_POOL(RecordBatch_POOL, RecordBatch, 32)
  SHARED_OBJECT_POOL(SHARED_RecordBatch_POOL, RecordBatch, 32)
public:
  
  static ObjectWrapper createShared(){
    return ObjectWrapper(SHARED_RecordBatch_POOL::allocateShared());
  }
  
  static oatpp::data::mapping::type::AbstractObjectWrapper deserialize(oatpp::parser::Caret& caret);
  void serialize(const std::shared_ptr<oatpp::data::stream::OutputStream>& stream);
  
  RecordBatchHeader::ObjectWrapper header;
  Batch<Record::ObjectWrapper>::ObjectWrapper records;
  
};
  
#include OATPP_CODEGEN_END(DTO)
}}}

#endif /* oatpp_kafka_protocol_RecordBatch_hpp */
