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

#ifndef oatpp_kafka_protocol_mapping_ObjectMapper_hpp
#define oatpp_kafka_protocol_mapping_ObjectMapper_hpp

#include "./Serializer.hpp"
#include "./Deserializer.hpp"

#include "oatpp/core/data/mapping/ObjectMapper.hpp"

namespace oatpp { namespace kafka { namespace protocol { namespace mapping {
  
class ObjectMapper : public oatpp::base::Controllable, public oatpp::data::mapping::ObjectMapper {
private:
  static Info& getMapperInfo() {
    static Info info("kafka-binary-protocol");
    return info;
  }
public:
  ObjectMapper()
    : oatpp::data::mapping::ObjectMapper(getMapperInfo())
  {}
public:
  
  static std::shared_ptr<ObjectMapper> createShared(){
    return std::shared_ptr<ObjectMapper>(new ObjectMapper());
  }
  
  void write(const std::shared_ptr<oatpp::data::stream::OutputStream>& stream,
             const oatpp::data::mapping::type::AbstractObjectWrapper& variant) const override {
    Serializer::serialize(stream, variant);
  }
  
  oatpp::data::mapping::type::AbstractObjectWrapper
  read(oatpp::parser::Caret& caret,
       const oatpp::data::mapping::type::Type* const type) const override {
    return Deserializer::deserialize(caret, type);
  }
  
};
  
}}}}

#endif /* oatpp_kafka_protocol_mapping_ObjectMapper_hpp */
