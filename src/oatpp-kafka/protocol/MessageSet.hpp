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

#ifndef oatpp_kafka_protocol_MessageSet_hpp
#define oatpp_kafka_protocol_MessageSet_hpp

#include "oatpp-kafka/protocol/mapping/Types.hpp"

#include "oatpp/core/parser/Caret.hpp"
#include "oatpp/core/data/stream/Stream.hpp"

#include "oatpp/core/data/mapping/type/Object.hpp"
#include "oatpp/core/macro/codegen.hpp"

namespace oatpp { namespace kafka { namespace protocol {
#include OATPP_CODEGEN_BEGIN(DTO)

class MessageV0 : public oatpp::data::mapping::type::Object {
  
  DTO_INIT(MessageV0, Object)
  
  DTO_FIELD(Int32, crc);
  DTO_FIELD(Int8, magicByte) = 0;
  DTO_FIELD(Int8, attributes);
  DTO_FIELD(Bytes, key);
  DTO_FIELD(Bytes, value);
  
};
  
class MessageV1 : public oatpp::data::mapping::type::Object {
  
  DTO_INIT(MessageV1, Object)
  
  DTO_FIELD(Int32, crc);
  DTO_FIELD(Int8, magicByte) = 1;
  DTO_FIELD(Int8, attributes);
  DTO_FIELD(Int64, timestamp);
  DTO_FIELD(Bytes, key);
  DTO_FIELD(Bytes, value);
  
};
  
class MessageSet : public oatpp::base::Controllable {
public:
  typedef oatpp::data::mapping::type::ObjectWrapper<MessageSet, mapping::type::__class::MessageSet> ObjectWrapper;
public:
  OBJECT_POOL(MessageSet_POOL, MessageSet, 32)
  SHARED_OBJECT_POOL(SHARED_MessageSet_POOL, MessageSet, 32)
private:
  oatpp::collection::LinkedList<MessageV0::ObjectWrapper> m_messages;
public:
  
  static ObjectWrapper createShared(){
    return ObjectWrapper(SHARED_MessageSet_POOL::allocateShared());
  }
  
  static oatpp::data::mapping::type::AbstractObjectWrapper deserialize(oatpp::parser::Caret& caret);
  void serialize(const std::shared_ptr<oatpp::data::stream::OutputStream>& stream);
  
  void addMessage(const MessageV0::ObjectWrapper& message);
  
};
  
#include OATPP_CODEGEN_END(DTO)
}}}

#endif /* oatpp_kafka_protocol_MessageSet_hpp */
