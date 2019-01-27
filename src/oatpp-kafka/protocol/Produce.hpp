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

#ifndef oatpp_kafka_protocol_Produce_hpp
#define oatpp_kafka_protocol_Produce_hpp

#include "MessageSet.hpp"

namespace oatpp { namespace kafka { namespace protocol {
#include OATPP_CODEGEN_BEGIN(DTO)
  
class ProduceRequestTopicPartitionData : public oatpp::data::mapping::type::Object {
  
  DTO_INIT(ProduceRequestTopicPartitionData, Object)
  
  DTO_FIELD(Int32, partition);
  DTO_FIELD(MessageSet::ObjectWrapper, messageSet);
  
};
  
class TopicData : public oatpp::data::mapping::type::Object {
  
  DTO_INIT(TopicData, Object)
  
  DTO_FIELD(String, topicName);
  DTO_FIELD(List<ProduceRequestTopicPartitionData::ObjectWrapper>::ObjectWrapper, partitions);
  
};
  
class Request : public oatpp::data::mapping::type::Object {
  
  DTO_INIT(Request, Object)
  
  DTO_FIELD(Int16, apiKey);
  DTO_FIELD(Int16, apiVersion);
  DTO_FIELD(Int32, correlationId);
  DTO_FIELD(String, clientId);
  
};
  
class ProduceRequest : public Request {
  
  DTO_INIT(ProduceRequest, Request)
  
  DTO_FIELD(Int16, requiredAcks);
  DTO_FIELD(Int32, timeout);
  DTO_FIELD(List<TopicData::ObjectWrapper>::ObjectWrapper, topics);
  
  static ObjectWrapper createRequest() {
    auto result = createShared();
    result->apiKey = 0;
    result->apiVersion = 0;
    result->correlationId = 1;
    result->clientId = "oatpp::kafka/v1";
    return result;
  }
  
};
  
class Response : public oatpp::data::mapping::type::Object {
  
  DTO_INIT(Response, Object)
  
  DTO_FIELD(Int32, correlationId);
  
};
  
class ProduceResponseTopicPartitionData : public oatpp::data::mapping::type::Object {
  
  DTO_INIT(ProduceResponseTopicPartitionData, Object)
  
  DTO_FIELD(Int32, partition);
  DTO_FIELD(Int16, errorCode);
  DTO_FIELD(Int64, offset);
  
};
  
class ProduceResponseTopicData : public oatpp::data::mapping::type::Object {
  
  DTO_INIT(ProduceResponseTopicData, Object)
  
  DTO_FIELD(String, topicName);
  DTO_FIELD(List<ProduceResponseTopicPartitionData::ObjectWrapper>::ObjectWrapper, partitions);
  
};
  
class ProduceResponse : public Response {
  
  DTO_INIT(ProduceResponse, Response)
  
  DTO_FIELD(List<ProduceResponseTopicData::ObjectWrapper>::ObjectWrapper, topics);
  
};
  
#include OATPP_CODEGEN_END(DTO)
}}}

#endif /* oatpp_kafka_protocol_Produce_hpp */
