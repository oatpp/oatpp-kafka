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

#ifndef oatpp_kafka_protocol_mapping_Types_hpp
#define oatpp_kafka_protocol_mapping_Types_hpp

#include "oatpp/core/Types.hpp"

namespace oatpp { namespace kafka { namespace protocol { namespace mapping { namespace type {

typedef oatpp::data::mapping::type::Type Type;
  
namespace __class {
  
  class Bytes; // FWD
  class Data; // FWD
  class VarInt; // FWD
  
  class AbstractBatch {
  public:
    constexpr static const char* const CLASS_NAME = "kafka::Batch";
  };
  
  template<class T>
  class Batch; // FWD
  
  class RecordBatch {
  public:
    constexpr static const char* const CLASS_NAME = "kafka::object::RecordBatch";
    
    static Type* getType(){
      static Type type(CLASS_NAME);
      return &type;
    }
    
  };
  
}

/**
 *  Should be serialized as: (32bit-length, <bytes>)
 */
typedef oatpp::data::mapping::type::ObjectWrapper<oatpp::base::StrBuffer, __class::Bytes> Bytes;

/**
 *  Should be serialized as: (Varint, <bytes>)
 */
typedef oatpp::data::mapping::type::ObjectWrapper<oatpp::base::StrBuffer, __class::Data> Data;

/**
 *  Varint. See protobuf Varint
 */
typedef oatpp::data::mapping::type::Primitive<v_int64, __class::VarInt>::ObjectWrapper VarInt;
  
/**
 *  This used to serialize List without preceding 32bit length
 */
template<class T>
using Batch = oatpp::data::mapping::type::ListTypeTemplate<T, __class::Batch<T>>;
  
namespace __class {

  class Bytes {
  public:
    constexpr static const char* const CLASS_NAME = "kafka::Bytes";
    
    static Type* getType(){
      static Type type(CLASS_NAME);
      return &type;
    }
    
  };
  
  class Data {
  public:
    constexpr static const char* const CLASS_NAME = "kafka::Data";
    
    static Type* getType(){
      static Type type(CLASS_NAME);
      return &type;
    }
    
  };
  
  class VarInt {
  public:
    constexpr static const char* const CLASS_NAME = "kafka::VarInt";
    
    static Type* getType(){
      static Type type(CLASS_NAME);
      return &type;
    }
    
  };
  
  template<class T>
  class Batch : public AbstractBatch {
  public:
    
    static Type* getType(){
      static Type* type = static_cast<Type*>(oatpp::kafka::protocol::mapping::type::Batch<T>::Z__CLASS_GET_TYPE());
      return type;
    }
    
  };
  
}
  
}}}}}

namespace oatpp { namespace kafka { namespace protocol {
  
  typedef mapping::type::Bytes Bytes;
  typedef mapping::type::Data Data;
  typedef mapping::type::VarInt VarInt;
  
  template<class T>
  using Batch = mapping::type::Batch<T>;
  
}}}

#endif /* oatpp_kafka_protocol_mapping_Types_hpp */
