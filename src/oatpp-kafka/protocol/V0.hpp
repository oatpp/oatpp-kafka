//
//  V0.hpp
//  kafka-integration
//
//  Created by Leonid on 7/22/18.
//  Copyright © 2018 oatpp. All rights reserved.
//

#ifndef V0_hpp
#define V0_hpp

#include "oatpp/core/data/mapping/type/Object.hpp"
#include "oatpp/core/macro/codegen.hpp"

namespace oatpp { namespace kafka { namespace protocol { namespace v0 {
#include OATPP_CODEGEN_BEGIN(DTO)
  
class Request : public oatpp::data::mapping::type::Object {

  DTO_INIT(Request, Object)
  
  DTO_FIELD(Int16, apiKey);
  DTO_FIELD(Int16, apiVersion);
  DTO_FIELD(Int32, correlationId);
  DTO_FIELD(String, clientId);
  
};
  
class MetadataRequest : public Request {
  
  DTO_INIT(MetadataRequest, Request)
  
  DTO_FIELD(List<String>::ObjectWrapper, topics) = List<String>::createShared();
  
  static ObjectWrapper createRequest() {
    auto result = createShared();
    result->apiKey = 3;
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
  
class Broker : public oatpp::data::mapping::type::Object {
  
  DTO_INIT(Broker, Object)
  
  DTO_FIELD(Int32, nodeId);
  DTO_FIELD(String, host);
  DTO_FIELD(Int32, port);
  
};
  
class PartitionMetadata : public oatpp::data::mapping::type::Object {
  
  DTO_INIT(PartitionMetadata, Object)
  
  DTO_FIELD(Int16, partitionErrorCode);
  DTO_FIELD(Int32, partitionId);
  DTO_FIELD(Int32, leader);
  DTO_FIELD(List<Int32>::ObjectWrapper, replicas);
  DTO_FIELD(List<Int32>::ObjectWrapper, isr);
  
};
  
class TopicMetadata : public oatpp::data::mapping::type::Object {
  
  DTO_INIT(TopicMetadata, Object)
  
  DTO_FIELD(Int16, topicErrorCode);
  DTO_FIELD(String, topicName);
  DTO_FIELD(List<PartitionMetadata::ObjectWrapper>::ObjectWrapper, partitionMetadata);
  
};
  
class MetadataResponse : public Response {
  
  DTO_INIT(MetadataResponse, Response)
  
  DTO_FIELD(List<Broker::ObjectWrapper>::ObjectWrapper, brockers);
  DTO_FIELD(List<TopicMetadata::ObjectWrapper>::ObjectWrapper, topicMetadata);
  
  /*
  MetadataResponse => [Broker][TopicMetadata]
  Broker => NodeId Host Port  (any number of brokers may be returned)
   NodeId => int32
   Host => string
   Port => int32
  TopicMetadata => TopicErrorCode TopicName [PartitionMetadata]
   TopicErrorCode => int16
  PartitionMetadata => PartitionErrorCode PartitionId Leader Replicas Isr
   PartitionErrorCode => int16
   PartitionId => int32
   Leader => int32
   Replicas => [int32]
   Isr => [int32]
   */
  
};
  
#include OATPP_CODEGEN_END(DTO)
}}}}

#endif /* V0_hpp */
