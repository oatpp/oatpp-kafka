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
    result->apiVersion = 2;
    result->correlationId = 1;
    result->clientId = "oatpp::kafka/v1";
    return result;
  }
  
};
  
#include OATPP_CODEGEN_END(DTO)
}}}}

#endif /* V0_hpp */
