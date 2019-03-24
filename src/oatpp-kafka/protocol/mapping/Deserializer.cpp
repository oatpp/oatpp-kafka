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

#include "Deserializer.hpp"

#include <arpa/inet.h>

namespace oatpp { namespace kafka { namespace protocol { namespace mapping {
  
void Deserializer::readInt8(oatpp::parser::Caret& caret, AbstractObjectWrapper& polymorph) {
  if(caret.getPosition() + 1 > caret.getDataSize()) {
    throw std::runtime_error("[oatpp::kafka::protocol::mapping::Deserializer::readInt8]: Data corrupted. Can't read value");
  }
  v_int8 value = *((p_int8)caret.getCurrData());
  caret.inc();
  polymorph.setPtr(Int8::ObjectType::createAbstract(value));
}

void Deserializer::readInt16(oatpp::parser::Caret& caret, AbstractObjectWrapper& polymorph) {
  if(caret.getPosition() + 2 > caret.getDataSize()) {
    throw std::runtime_error("[oatpp::kafka::protocol::mapping::Deserializer::readInt16]: Data corrupted. Can't read value");
  }
  v_int16 value = ntohs(*((p_int16)caret.getCurrData()));
  caret.inc(2);
  polymorph.setPtr(Int16::ObjectType::createAbstract(value));
}

void Deserializer::readInt32(oatpp::parser::Caret& caret, AbstractObjectWrapper& polymorph) {
  if(caret.getPosition() + 4 > caret.getDataSize()) {
    throw std::runtime_error("[oatpp::kafka::protocol::mapping::Deserializer::readInt32]: Data corrupted. Can't read value");
  }
  v_int32 value = ntohl(*((p_int32)caret.getCurrData()));
  caret.inc(4);
  polymorph.setPtr(Int32::ObjectType::createAbstract(value));
}

void Deserializer::readInt64(oatpp::parser::Caret& caret, AbstractObjectWrapper& polymorph) {
  if(caret.getPosition() + 8 > caret.getDataSize()) {
    throw std::runtime_error("[oatpp::kafka::protocol::mapping::Deserializer::readInt64]: Data corrupted. Can't read value");
  }
  v_int64 w1 = ntohl(*((p_int32)caret.getCurrData()));
  caret.inc(4);
  v_int64 w2 = ntohl(*((p_int32)caret.getCurrData()));
  caret.inc(4);
  polymorph.setPtr(Int64::ObjectType::createAbstract((w1 << 32) | w2));
}

void Deserializer::readString(oatpp::parser::Caret& caret, AbstractObjectWrapper& polymorph) {
  if(caret.getPosition() + 2 > caret.getDataSize()) {
    throw std::runtime_error("[oatpp::kafka::protocol::mapping::Deserializer::readString]: Data corrupted. Can't read value");
  }
  v_int16 size = ntohs(*((p_int16)caret.getCurrData()));
  caret.inc(2);
  if(size > 0) {
    if(caret.getPosition() + size > caret.getDataSize()) {
      throw std::runtime_error("[oatpp::kafka::protocol::mapping::Deserializer::readString]: Data corrupted. Can't read value");
    }
    std::shared_ptr<oatpp::base::StrBuffer> str = oatpp::base::StrBuffer::createShared(caret.getCurrData(), size, true);
    caret.inc(size);
    polymorph.setPtr(std::static_pointer_cast<oatpp::base::Countable>(str));
  } else {
    polymorph.setPtr(nullptr);
  }
}

void Deserializer::readList(oatpp::parser::Caret& caret, AbstractObjectWrapper& polymorph) {
  
  if(caret.getPosition() + 3 >= caret.getDataSize()) {
    throw std::runtime_error("[oatpp::kafka::protocol::mapping::Deserializer::readList]: Data corrupted. Can't read value");
  }
  v_int32 size = ntohl(*((p_int32)caret.getCurrData()));
  caret.inc(4);
  
  auto listWrapper = polymorph.valueType->creator();
  oatpp::data::mapping::type::PolymorphicWrapper<AbstractList>
  list(std::static_pointer_cast<AbstractList>(listWrapper.getPtr()), listWrapper.valueType);
  
  polymorph.setPtr(list.getPtr());
  
  Type* itemType = *polymorph.valueType->params.begin();
  
  for(v_int32 i = 0; i < size; i ++) {
    
    AbstractObjectWrapper polymorphicItem(itemType);
    readField(caret, polymorphicItem);
    list->addPolymorphicItem(polymorphicItem);
    
  }
  
}

void Deserializer::readObject(oatpp::parser::Caret& caret, AbstractObjectWrapper& polymorph) {
  auto object = readObject(polymorph.valueType, caret);
  polymorph.setPtr(object.getPtr());
}
  
void Deserializer::readField(oatpp::parser::Caret& caret, AbstractObjectWrapper& polymorph) {
  
  const Type* type = polymorph.valueType;
  
  if(type->name == oatpp::data::mapping::type::__class::Int8::CLASS_NAME) {
    readInt8(caret, polymorph);
  } else if(type->name == oatpp::data::mapping::type::__class::Int16::CLASS_NAME) {
    readInt16(caret, polymorph);
  } else if(type->name == oatpp::data::mapping::type::__class::Int32::CLASS_NAME) {
    readInt32(caret, polymorph);
  } else if(type->name == oatpp::data::mapping::type::__class::Int64::CLASS_NAME) {
    readInt64(caret, polymorph);
  } else if(type->name == oatpp::data::mapping::type::__class::String::CLASS_NAME) {
    readString(caret, polymorph);
  } else if(type->name == oatpp::data::mapping::type::__class::AbstractList::CLASS_NAME) {
    readList(caret, polymorph);
  } else if(type->name == oatpp::data::mapping::type::__class::AbstractObject::CLASS_NAME) {
    readObject(caret, polymorph);
  } else {
    throw std::runtime_error("[oatpp::kafka::protocol::mapping::Deserializer::readField]: Unknown data type");
  }
  
}
  
Deserializer::AbstractObjectWrapper Deserializer::readObject(const Type* const type, oatpp::parser::Caret& caret) {
  
  auto object = type->creator();
  
  auto& list = type->properties->getList();
  for(Property* property : list) {
    auto& polymorph = property->getAsRef(object.get());
    readField(caret, polymorph);
  }
  
  return object;
  
}
  
}}}}
