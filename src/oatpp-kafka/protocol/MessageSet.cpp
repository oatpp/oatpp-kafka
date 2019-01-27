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

#include "MessageSet.hpp"

#include "oatpp-kafka/protocol/mapping/Serializer.hpp"
#include "oatpp-kafka/protocol/mapping/Deserializer.hpp"

#include "oatpp/core/data/stream/ChunkedBuffer.hpp"
#include "oatpp/algorithm/CRC.hpp"

#include "arpa/inet.h"

namespace oatpp { namespace kafka { namespace protocol {
  
void MessageSet::serialize(const std::shared_ptr<oatpp::data::stream::OutputStream>& stream) {
  
  auto messageStream = oatpp::data::stream::ChunkedBuffer::createShared();
  
  auto curr = m_messages.getFirstNode();
  
  while (curr != nullptr) {
    
    messageStream->clear();
    mapping::Serializer::serialize(messageStream, curr->getData());
    
    auto message = messageStream->toString();
    
    p_word32 crc = (p_word32)&message->getData()[0]; // pointer to crc field
    *crc = htonl(oatpp::algorithm::CRC32::calc(&message->getData()[4], message->getSize() - 4)); // calc crc of the message buffer
    
    v_word64 offset = htonl(0); // Hardcoded offset. TODO remove hardcode.
    stream->write(&offset, 8);
    
    v_word32 messageSize = htonl(message->getSize());
    stream->write(&messageSize, 4);
    
    stream->write(message->getData(), message->getSize());
    
    curr = curr->getNext();
  }
  
}
  
void MessageSet::addMessage(const MessageV0::ObjectWrapper& message) {
  m_messages.pushBack(message);
}
  
}}}
