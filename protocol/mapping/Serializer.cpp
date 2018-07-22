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

#include "Serializer.hpp"

#include <arpa/inet.h>

namespace oatpp { namespace kafka { namespace protocol { namespace mapping {
  
void Serializer::writeInt8(oatpp::data::stream::OutputStream* stream, const PolymorphicWrapper<Int8::ObjectType>& value) {
  if(value){
    v_int8 v = value->getValue();
    stream->write(&v, 1);
  } else {
    throw std::runtime_error("[oatpp::kafka::protocol::mapping::Serializer::writeInt8]: Value should not be null");
  }
}

void Serializer::writeInt16(oatpp::data::stream::OutputStream* stream, const PolymorphicWrapper<Int16::ObjectType>& value) {
  if(value){
    v_int16 v = htons(value->getValue());
    stream->write(&v, 2);
  } else {
    throw std::runtime_error("[oatpp::kafka::protocol::mapping::Serializer::writeInt16]: Value should not be null");
  }
}

void Serializer::writeInt32(oatpp::data::stream::OutputStream* stream, const PolymorphicWrapper<Int32::ObjectType>& value) {
  if(value){
    v_int32 v = htonl(value->getValue());
    stream->write(&v, 4);
  } else {
    throw std::runtime_error("[oatpp::kafka::protocol::mapping::Serializer::writeInt32]: Value should not be null");
  }
}

void Serializer::writeInt64(oatpp::data::stream::OutputStream* stream, const PolymorphicWrapper<Int64::ObjectType>& value) {
  throw std::runtime_error("[oatpp::kafka::protocol::mapping::Serializer::writeInt64]: not implemented!");
}

void Serializer::writeString(oatpp::data::stream::OutputStream* stream, const PolymorphicWrapper<oatpp::base::StrBuffer>& value) {
  if(value) {
    if(value->getSize() > 32767) {
      throw std::runtime_error("[oatpp::kafka::protocol::mapping::Serializer::writeString]: Too big string. String should not be larger than 32767");
    }
    v_int16 size = htons((v_int16) value->getSize());
    stream->write(&size, 2);
    stream->write(value->getData(), value->getSize());
  } else {
    v_int16 size = htons(-1);
    stream->write(&size, 2);
  }
}

void Serializer::writeList(oatpp::data::stream::OutputStream* stream, const AbstractList::ObjectWrapper& list) {
  
  if(list) {
    
    v_int32 size = htonl(list->count());
    stream->write(&size, 4);
    
    auto curr = list->getFirstNode();
    while (curr != nullptr) {
      writeField(stream, curr->getData());
      curr = curr->getNext();
    }
    
  } else {
    v_int32 size = htonl(-1);
    stream->write(&size, 4);
  }
}

void Serializer::writeObject(oatpp::data::stream::OutputStream* stream, const AbstractObjectWrapper& polymorph) {
  auto value = oatpp::data::mapping::type::static_wrapper_cast<Object>(polymorph);
  if(value){
    writeObject(stream, value.valueType, value.get());
  } else {
    throw std::runtime_error("[oatpp::kafka::protocol::mapping::Serializer::writeObject]: Object should not be null");
  }
}
  
void Serializer::writeField(oatpp::data::stream::OutputStream* stream, const AbstractObjectWrapper& polymorph) {
  
  const Type* type = polymorph.valueType;
  
  if(type->name == oatpp::data::mapping::type::__class::Int8::CLASS_NAME) {
    writeInt8(stream, oatpp::data::mapping::type::static_wrapper_cast<Int8::ObjectType>(polymorph));
  } else if(type->name == oatpp::data::mapping::type::__class::Int16::CLASS_NAME) {
    writeInt16(stream, oatpp::data::mapping::type::static_wrapper_cast<Int16::ObjectType>(polymorph));
  } else if(type->name == oatpp::data::mapping::type::__class::Int32::CLASS_NAME) {
    writeInt32(stream, oatpp::data::mapping::type::static_wrapper_cast<Int32::ObjectType>(polymorph));
  } else if(type->name == oatpp::data::mapping::type::__class::Int64::CLASS_NAME) {
    writeInt64(stream, oatpp::data::mapping::type::static_wrapper_cast<Int64::ObjectType>(polymorph));
  } else if(type->name == oatpp::data::mapping::type::__class::String::CLASS_NAME) {
    writeString(stream, oatpp::data::mapping::type::static_wrapper_cast<oatpp::base::StrBuffer>(polymorph));
  } else if(type->name == oatpp::data::mapping::type::__class::AbstractList::CLASS_NAME) {
    writeList(stream, oatpp::data::mapping::type::static_wrapper_cast<AbstractList>(polymorph));
  } else if(type->name == oatpp::data::mapping::type::__class::AbstractObject::CLASS_NAME) {
    writeObject(stream, polymorph);
  } else {
    throw std::runtime_error("[oatpp::kafka::protocol::mapping::Serializer::writeField]: Unknown data type");
  }
  
}
  
void Serializer::writeObject(oatpp::data::stream::OutputStream* stream, const Type* type, Object* object) {
  auto& list = type->properties->getList();
  for(Property* property : list) {
    auto polymorph = property->get(object);
    writeField(stream, polymorph);
  }
}
  
}}}}
