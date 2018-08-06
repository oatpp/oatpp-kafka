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

#ifndef oatpp_kafka_protocol_mapping_Serializer_hpp
#define oatpp_kafka_protocol_mapping_Serializer_hpp

#include "oatpp/core/data/mapping/type/ListMap.hpp"
#include "oatpp/core/data/mapping/type/List.hpp"
#include "oatpp/core/data/mapping/type/Object.hpp"
#include "oatpp/core/data/mapping/type/Primitive.hpp"
#include "oatpp/core/data/mapping/type/Type.hpp"
#include "oatpp/core/data/stream/ChunkedBuffer.hpp"

#include "oatpp/core/Types.hpp"

namespace oatpp { namespace kafka { namespace protocol { namespace mapping {
  
class Serializer {
public:
  typedef oatpp::data::mapping::type::Type Type;
  typedef oatpp::data::mapping::type::Type::Property Property;
  
  typedef oatpp::data::mapping::type::Object Object;
  
  typedef oatpp::data::mapping::type::AbstractObjectWrapper AbstractObjectWrapper;
  
  typedef oatpp::data::mapping::type::List<AbstractObjectWrapper> AbstractList;
  
  template<class T>
  using PolymorphicWrapper = data::mapping::type::PolymorphicWrapper<T>;
private:
  
  static void writeInt8(oatpp::data::stream::OutputStream* stream, const PolymorphicWrapper<Int8::ObjectType>& value);
  static void writeInt16(oatpp::data::stream::OutputStream* stream, const PolymorphicWrapper<Int16::ObjectType>& value);
  static void writeInt32(oatpp::data::stream::OutputStream* stream, const PolymorphicWrapper<Int32::ObjectType>& value);
  static void writeInt64(oatpp::data::stream::OutputStream* stream, const PolymorphicWrapper<Int64::ObjectType>& value);
  
  static void writeString(oatpp::data::stream::OutputStream* stream, const PolymorphicWrapper<oatpp::base::StrBuffer>& value);
  static void writeBytes(oatpp::data::stream::OutputStream* stream, const PolymorphicWrapper<oatpp::base::StrBuffer>& value);
  
  static void writeList(oatpp::data::stream::OutputStream* stream, const AbstractList::ObjectWrapper& list);
  static void writeObject(oatpp::data::stream::OutputStream* stream, const AbstractObjectWrapper& polymorph);
  
  static void writeMessageSet(oatpp::data::stream::OutputStream* stream, const AbstractObjectWrapper& polymorph);
  
  static void writeField(oatpp::data::stream::OutputStream* stream, const AbstractObjectWrapper& polymorph);
  
  static void writeObject(oatpp::data::stream::OutputStream* stream, const Type* type, Object* object);
  
public:
  
  static void serialize(const std::shared_ptr<oatpp::data::stream::OutputStream>& stream,
                        const oatpp::data::mapping::type::AbstractObjectWrapper& object){
    
    auto type = object.valueType;
    if(type->name == oatpp::data::mapping::type::__class::AbstractObject::CLASS_NAME) {
      writeObject(stream.get(), type, static_cast<Object*>(object.get()));
    } else {
      throw std::runtime_error("[oatpp::parser::json::mapping::Serializer::serialize()]: Unknown parameter type");
    }

  }
  
};
  
}}}}

#endif /* oatpp_kafka_protocol_mapping_Serializer_hpp */
