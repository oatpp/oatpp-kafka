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

#ifndef oatpp_kafka_protocol_mapping_Deserializer_hpp
#define oatpp_kafka_protocol_mapping_Deserializer_hpp

#include "oatpp/core/data/mapping/type/List.hpp"
#include "oatpp/core/data/mapping/type/Object.hpp"
#include "oatpp/core/data/mapping/type/Primitive.hpp"
#include "oatpp/core/data/mapping/type/Type.hpp"

#include "oatpp/core/parser/Caret.hpp"

#include "oatpp/core/Types.hpp"

namespace oatpp { namespace kafka { namespace protocol { namespace mapping {
  
class Deserializer {
public:
  typedef oatpp::data::mapping::type::Type Type;
  typedef oatpp::data::mapping::type::Type::Property Property;
  
  typedef oatpp::data::mapping::type::Object Object;
  
  typedef oatpp::data::mapping::type::AbstractObjectWrapper AbstractObjectWrapper;
  
  typedef oatpp::data::mapping::type::List<AbstractObjectWrapper> AbstractList;
  
  template<class T>
  using PolymorphicWrapper = data::mapping::type::PolymorphicWrapper<T>;
private:
  
  static void readInt8(oatpp::parser::Caret& caret, AbstractObjectWrapper& polymorph);
  static void readInt16(oatpp::parser::Caret& caret, AbstractObjectWrapper& polymorph);
  static void readInt32(oatpp::parser::Caret& caret, AbstractObjectWrapper& polymorph);
  static void readInt64(oatpp::parser::Caret& caret, AbstractObjectWrapper& polymorph);
  
  static void readString(oatpp::parser::Caret& caret, AbstractObjectWrapper& polymorph);
  
  static void readList(oatpp::parser::Caret& caret, AbstractObjectWrapper& polymorph);
  static void readObject(oatpp::parser::Caret& caret, AbstractObjectWrapper& polymorph);
  
  static void readField(oatpp::parser::Caret& caret, AbstractObjectWrapper& polymorph);
  
  static AbstractObjectWrapper readObject(const Type* const type, oatpp::parser::Caret& caret);
  
public:
  
  static AbstractObjectWrapper deserialize(oatpp::parser::Caret& caret, const Type* const type) {
    if(type->name == oatpp::data::mapping::type::__class::AbstractObject::CLASS_NAME){
      return readObject(type, caret);
    }
    return AbstractObjectWrapper::empty();
  }
  
};
  
}}}}

#endif /* oatpp_kafka_protocol_mapping_Deserializer_hpp */
