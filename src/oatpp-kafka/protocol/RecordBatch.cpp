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

#include "RecordBatch.hpp"

#include "oatpp-kafka/protocol/mapping/Serializer.hpp"
#include "oatpp-kafka/protocol/mapping/Deserializer.hpp"

namespace oatpp { namespace kafka { namespace protocol {
  
oatpp::data::mapping::type::AbstractObjectWrapper RecordBatch::deserialize(oatpp::parser::Caret& caret) {
  
  auto header = oatpp::data::mapping::type::static_wrapper_cast<RecordBatchHeader>
  (mapping::Deserializer::deserialize(caret, RecordBatchHeader::ObjectWrapper::Class::getType()));
  
  auto records = Batch<Record::ObjectWrapper>::createShared();
  
  for(v_int32 i = 0; i < header->length; i++) {
    auto record = oatpp::data::mapping::type::static_wrapper_cast<Record>
    (mapping::Deserializer::deserialize(caret, Record::ObjectWrapper::Class::getType()));
    records->pushBack(record);
  }
  
  auto result = RecordBatch::createShared();
  result->header = header;
  result->records = records;
  return oatpp::data::mapping::type::AbstractObjectWrapper(result.getPtr(), RecordBatch::ObjectWrapper::Class::getType());
  
}

void RecordBatch::serialize(const std::shared_ptr<oatpp::data::stream::OutputStream>& stream) {
  mapping::Serializer::serialize(stream, header);
  mapping::Serializer::serialize(stream, records);
}
  
}}}
