/*

 Copyright (c) 2013, SMB Phone Inc. / Hookflash Inc.
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 1. Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.
 2. Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

 The views and conclusions contained in the software and documentation are those
 of the authors and should not be interpreted as representing official policies,
 either expressed or implied, of the FreeBSD Project.
 
 */

#pragma once

#include <ortc/types.h>

namespace ortc
{
  namespace internal
  {
    using zsLib::Noop;
    using zsLib::MessageQueue;
    using zsLib::IMessageQueuePtr;
    using zsLib::MessageQueuePtr;
    using zsLib::MessageQueueAssociator;
    using zsLib::IMessageQueueNotify;
    using zsLib::IMessageQueueMessagePtr;
    using zsLib::IMessageQueueThread;
    using zsLib::MessageQueueThread;
    using zsLib::IMessageQueueThreadPtr;
    using zsLib::MessageQueueThreadPtr;
    
    class RTCConnection;
    typedef boost::shared_ptr<RTCConnection> RTCConnectionPtr;
    typedef boost::weak_ptr<RTCConnection> RTCConnectionWeakPtr;
    
    class RTCSocket;
    typedef boost::shared_ptr<RTCSocket> RTCSocketPtr;
    typedef boost::weak_ptr<RTCSocket> RTCSocketWeakPtr;
    
    class RTCStream;
    typedef boost::shared_ptr<RTCStream> RTCStreamPtr;
    typedef boost::weak_ptr<RTCStream> RTCStreamWeakPtr;
    
    class RTCTrack;
    typedef boost::shared_ptr<RTCTrack> RTCTrackPtr;
    typedef boost::weak_ptr<RTCTrack> RTCTrackWeakPtr;
    
    class RTCDTMFTrack;
    typedef boost::shared_ptr<RTCDTMFTrack> RTCDTMFTrackPtr;
    typedef boost::weak_ptr<RTCDTMFTrack> RTCDTMFTrackWeakPtr;
    
    class RTCDataChannel;
    typedef boost::shared_ptr<RTCDataChannel> RTCDataChannelPtr;
    typedef boost::weak_ptr<RTCDataChannel> RTCDataChannelWeakPtr;
    
    class MediaManager;
    typedef boost::shared_ptr<MediaManager> MediaManagerPtr;
    typedef boost::weak_ptr<MediaManager> MediaManagerWeakPtr;
    
    class MediaStream;
    typedef boost::shared_ptr<MediaStream> MediaStreamPtr;
    typedef boost::weak_ptr<MediaStream> MediaStreamWeakPtr;
    
    class MediaStreamTrack;
    typedef boost::shared_ptr<MediaStreamTrack> MediaStreamTrackPtr;
    typedef boost::weak_ptr<MediaStreamTrack> MediaStreamTrackWeakPtr;
  }
}