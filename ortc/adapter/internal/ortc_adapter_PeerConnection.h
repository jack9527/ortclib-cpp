/*

 Copyright (c) 2016, Hookflash Inc. / Hookflash Inc.
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


#include <ortc/adapter/internal/types.h>

#include <ortc/adapter/IPeerConnection.h>

#include <ortc/IRTPListener.h>

#include <openpeer/services/IWakeDelegate.h>

namespace ortc
{
  namespace adapter
  {
    namespace internal
    {
      ZS_DECLARE_INTERACTION_PTR(IMediaStreamForPeerConnection);

      //-------------------------------------------------------------------------
      //-------------------------------------------------------------------------
      //-------------------------------------------------------------------------
      //-------------------------------------------------------------------------
      #pragma mark
      #pragma mark PeerConnection
      #pragma mark

      class PeerConnection : public Noop,
                             public MessageQueueAssociator,
                             public SharedRecursiveLock,
                             public IPeerConnection,
                             public IICEGathererDelegate,
                             public IICETransportDelegate,
                             public IDTLSTransportDelegate,
                             public ISRTPSDESTransportDelegate,
                             public IRTPListenerDelegate,
                             public ISCTPTransportDelegate,
                             public ISCTPTransportListenerDelegate,
                             public IWakeDelegate
      {
      protected:
        struct make_private {};

        friend interaction IPeerConnectionFactory;

      public:
        PeerConnection(
                       const make_private &,
                       IMessageQueuePtr queue,
                       IPeerConnectionDelegatePtr delegate,
                       const Optional<Configuration> &configuration
                       );

        ~PeerConnection();

      public:
        PeerConnection(
                       const Noop &,
                       IMessageQueuePtr queue
                       ) :
          SharedRecursiveLock(SharedRecursiveLock::create()),
          MessageQueueAssociator(queue) {}

        void init();

        static PeerConnectionPtr convert(IPeerConnectionPtr object);

        enum InternalStates
        {
          InternalState_First,

          InternalState_Pending = InternalState_First,
          InternalState_Ready,
          InternalState_ShuttingDown,
          InternalState_Shutdown,

          InternalState_Last = InternalState_Shutdown
        };

        static const char *toString(InternalStates state);

        enum NegotiationStates
        {
          NegotiationState_First,

          NegotiationState_Agreed = NegotiationState_First,
          NegotiationState_PendingOffer,
          NegotiationState_LocalOffered,
          NegotiationState_RemoteOffered,
          NegotiationState_Rejected,

          NegotiationState_Last = NegotiationState_Rejected,
        };

        static const char *toString(NegotiationStates state);

        enum PendingMethods
        {
          PendingMethod_First,

          PendingMethod_CreateOffer = PendingMethod_First,
          PendingMethod_CreateAnswer,
          PendingMethod_CreateCapabilities,
          PendingMethod_SetLocalDescription,
          PendingMethod_SetRemoteDescription,

          PendingMethod_Last = PendingMethod_SetRemoteDescription,
        };

        static const char *toString(PendingMethods method);

        typedef String TransportID;
        typedef String MediaLineID;
        typedef String SenderID;
        typedef String ReceiverID;

        ZS_DECLARE_STRUCT_PTR(TransportInfo);
        ZS_DECLARE_STRUCT_PTR(MediaLineInfo);
        ZS_DECLARE_STRUCT_PTR(RTPMediaLineInfo);
        ZS_DECLARE_STRUCT_PTR(SCTPMediaLineInfo);
        ZS_DECLARE_STRUCT_PTR(SenderInfo);
        ZS_DECLARE_STRUCT_PTR(ReceiverInfo);
        ZS_DECLARE_STRUCT_PTR(PendingMethod);

        ZS_DECLARE_TYPEDEF_PTR(IMediaStreamForPeerConnection, UseMediaStream);
        ZS_DECLARE_TYPEDEF_PTR(std::list<UseMediaStreamPtr>, UseMediaStreamList);

        struct TransportInfo
        {
          struct Details
          {
            IICEGathererPtr mGatherer;
            IICETransportPtr mTransport;
            IICETypes::CandidateList mCandidates;
            bool mRTPEndOfCandidates {false};

            IDTLSTransportPtr mDTLSTransport;
            ISRTPSDESTransportPtr mSRTPSDESTransport;

            ISCTPTransportPtr mSCTPTransport;

            ElementPtr toDebug() const;
          };

          TransportID mID;

          NegotiationStates mNegotiationState {NegotiationState_First};

          Details mRTP;
          Details mRTCP;

          ElementPtr toDebug() const;
        };

        struct MediaLineInfo
        {
          MediaLineID mID;
          size_t mLineIndex {};

          String mTransportID;

          NegotiationStates mNegotiationState {NegotiationState_First};

          ElementPtr toDebug() const;
        };

        struct RTPMediaLineInfo : public MediaLineInfo
        {
          ElementPtr toDebug() const;
        };

        struct SCTPMediaLineInfo : public MediaLineInfo
        {
          ElementPtr toDebug() const;
        };

        struct SenderInfo
        {
          SenderID mID;
          MediaLineID mMediaLineID;

          IMediaStreamTrackPtr mTrack;
          UseMediaStreamList mMediaStreams;

          NegotiationStates mNegotiationState {NegotiationState_First};

          IRTPSenderPtr mSender;

          ElementPtr toDebug() const;
        };

        struct ReceiverInfo
        {
          ReceiverID mID;
          MediaLineID mMediaLineID;

          NegotiationStates mNegotiationState {NegotiationState_First};

          IRTPReceiverPtr mReceiver;
          UseMediaStreamList mMediaStreams;

          ElementPtr toDebug() const;
        };

        struct PendingMethod
        {
          PendingMethods mMethod {PendingMethod_First};
          PromisePtr mPromise;

          Optional<OfferOptions> mOfferOptions;
          Optional<AnswerOptions> mAnswerOptions;
          Optional<CapabilityOptions> mCapabilityOptions;

          ISessionDescriptionPtr mSessionDescription;

          PendingMethod(
                        PendingMethods method,
                        PromisePtr promise
                        ) :
            mMethod(method),
            mPromise(promise) {}

          ElementPtr toDebug() const;
        };

        typedef std::map<TransportID, TransportInfoPtr> TransportInfoMap;
        typedef std::map<MediaLineID, RTPMediaLineInfoPtr> RTPMediaLineInfoMap;
        typedef std::map<MediaLineID, SCTPMediaLineInfoPtr> SCTPMediaLineInfoMap;
        typedef std::map<SenderID, SenderInfoPtr> SenderInfoMap;
        typedef std::map<ReceiverID, ReceiverInfoPtr> ReceiverInfoMap;
        typedef std::list<ICECandidatePtr> CandidateList;
        typedef std::list<PendingMethodPtr> PendingMethodList;

      protected:
        //---------------------------------------------------------------------
        #pragma mark
        #pragma mark PeerConnection => IPeerConnection
        #pragma mark

        static PeerConnectionPtr create(
                                        IPeerConnectionDelegatePtr delegate,
                                        const Optional<Configuration> &configuration = Optional<Configuration>()
                                        );

        virtual IPeerConnectionSubscriptionPtr subscribe(IPeerConnectionDelegatePtr delegate) override;

        virtual PromiseWithDescriptionPtr createOffer(const Optional<OfferOptions> &configuration = Optional<OfferOptions>()) override;
        virtual PromiseWithDescriptionPtr createAnswer(const Optional<AnswerOptions> &configuration = Optional<AnswerOptions>()) override;

        virtual PromiseWithDescriptionPtr createCapabilities(const Optional<CapabilityOptions> &configuration = Optional<CapabilityOptions>()) override;

        virtual PromisePtr setLocalDescription(ISessionDescriptionPtr description) override;

        virtual ISessionDescriptionPtr localDescription() const override;
        virtual ISessionDescriptionPtr currentDescription() const override;
        virtual ISessionDescriptionPtr pendingDescription() const override;

        virtual PromisePtr setRemoteDescription(ISessionDescriptionPtr description) override;
        virtual ISessionDescriptionPtr remoteDescription() const override;
        virtual ISessionDescriptionPtr currentRemoteDescription() const override;
        virtual ISessionDescriptionPtr pendingRemoteDescription() const override;
        virtual void addICECandidate(const ICECandidate &candidate) override;

        virtual SignalingStates signalingState() const override;
        virtual ICEGatheringStates iceGatheringState() const override;
        virtual ICEConnectionStates iceConnectionState() const override;
        virtual PeerConnectionStates connectionState() const override;
        virtual bool canTrickleCandidates() const override;

        static ServerListPtr getDefaultIceServers();

        virtual ConfigurationPtr getConfiguration() const override;
        virtual void setConfiguration(const Configuration &configuration) override;

        virtual void close() override;

        virtual SenderListPtr getSenders() const override;
        virtual ReceiverListPtr getReceivers() const override;
        virtual IRTPSenderPtr addTrack(
                                       IMediaStreamTrackPtr track,
                                       const MediaStreamTrackConfiguration &configuration = MediaStreamTrackConfiguration()
                                       ) override;
        virtual IRTPSenderPtr addTrack(
                                       IMediaStreamTrackPtr track,
                                       const MediaStreamList &mediaStreams,
                                       const MediaStreamTrackConfiguration &configuration = MediaStreamTrackConfiguration()
                                       ) override;

        virtual void removeTrack(IRTPSenderPtr sender) override;

        virtual IDataChannelPtr createDataChannel(const IDataChannelTypes::Parameters &parameters) override;

        //---------------------------------------------------------------------
        #pragma mark
        #pragma mark PeerConnection => IICEGathererDelegate
        #pragma mark

        virtual void onICEGathererStateChange(
                                              IICEGathererPtr gatherer,
                                              IICEGatherer::States state
                                              ) override;

        virtual void onICEGathererLocalCandidate(
                                                 IICEGathererPtr gatherer,
                                                 CandidatePtr candidate
                                                 ) override;

        virtual void onICEGathererLocalCandidateComplete(
                                                         IICEGathererPtr gatherer,
                                                         CandidateCompletePtr candidate
                                                         ) override;

        virtual void onICEGathererLocalCandidateGone(
                                                     IICEGathererPtr gatherer,
                                                     CandidatePtr candidate
                                                     ) override;

        virtual void onICEGathererError(
                                         IICEGathererPtr gatherer,
                                         ErrorEventPtr errorEvent
                                         ) override;

        //---------------------------------------------------------------------
        #pragma mark
        #pragma mark PeerConnection => IICETransportDelegate
        #pragma mark

        virtual void onICETransportStateChange(
                                               IICETransportPtr transport,
                                               IICETransport::States state
                                               ) override;

        virtual void onICETransportCandidatePairAvailable(
                                                          IICETransportPtr transport,
                                                          CandidatePairPtr candidatePair
                                                          ) override;
        virtual void onICETransportCandidatePairGone(
                                                     IICETransportPtr transport,
                                                     CandidatePairPtr candidatePair
                                                     ) override;

        virtual void onICETransportCandidatePairChanged(
                                                        IICETransportPtr transport,
                                                        CandidatePairPtr candidatePair
                                                        ) override;

        //---------------------------------------------------------------------
        #pragma mark
        #pragma mark PeerConnection => IDTLSTransportDelegate
        #pragma mark

        virtual void onDTLSTransportStateChange(
                                                IDTLSTransportPtr transport,
                                                IDTLSTransport::States state
                                                ) override;

        virtual void onDTLSTransportError(
                                          IDTLSTransportPtr transport,
                                          ErrorAnyPtr error
                                          ) override;


        //---------------------------------------------------------------------
        #pragma mark
        #pragma mark PeerConnection => ISRTPSDESTransportDelegate
        #pragma mark

        virtual void onSRTPSDESTransportLifetimeRemaining(
                                                          ISRTPSDESTransportPtr transport,
                                                          ULONG leastLifetimeRemainingPercentageForAllKeys,
                                                          ULONG overallLifetimeRemainingPercentage
                                                          ) override;

        virtual void onSRTPSDESTransportError(
                                              ISRTPSDESTransportPtr transport,
                                              ErrorAnyPtr errorCode
                                              ) override;

        //---------------------------------------------------------------------
        #pragma mark
        #pragma mark PeerConnection => IRTPListener
        #pragma mark

        virtual void onRTPListenerUnhandledRTP(
                                               IRTPListenerPtr listener,
                                               SSRCType ssrc,
                                               PayloadType payloadType,
                                               const char *mid,
                                               const char *rid
                                               ) override;


        //---------------------------------------------------------------------
        #pragma mark
        #pragma mark PeerConnection => ISCTPTransportDelegate
        #pragma mark

        virtual void onSCTPTransportStateChange(
                                                ISCTPTransportPtr transport,
                                                States state
                                                ) override;
        virtual void onSCTPTransportDataChannel(
                                                ISCTPTransportPtr transport,
                                                IDataChannelPtr channel
                                                ) override;

        //---------------------------------------------------------------------
        #pragma mark
        #pragma mark PeerConnection => ISCTPTransportListenerDelegate
        #pragma mark

        virtual void onSCTPTransport(ISCTPTransportPtr transport) override;

        //---------------------------------------------------------------------
        #pragma mark
        #pragma mark PeerConnection => IWakeDelegate
        #pragma mark

        virtual void onWake() override;

      protected:
        //---------------------------------------------------------------------
        #pragma mark
        #pragma mark PeerConnection => (internal)
        #pragma mark

        Log::Params log(const char *message) const;
        Log::Params debug(const char *message) const;
        virtual ElementPtr toDebug() const;

        bool isShuttingDown() const { return InternalState_ShuttingDown == mState; }
        bool isShutdown() const { return InternalState_Shutdown == mState; }
        bool isStopped() const { return isShuttingDown() || isShutdown(); }

        void wake();
        void notifyNegotiationNeeded();

        void cancel();

        void step();
        bool stepProcessPendingRemoteCandidates();

        void setState(InternalStates state);
        void setState(SignalingStates state);
        void setState(ICEGatheringStates state);
        void setState(ICEConnectionStates state);
        void setState(PeerConnectionStates state);

        void addCandidateToTransport(
                                     TransportInfo &transport,
                                     ICECandidatePtr candidate
                                     );

      protected:
        //---------------------------------------------------------------------
        #pragma mark
        #pragma mark PeerConnection => (data)
        #pragma mark

        AutoPUID mID;
        PeerConnectionWeakPtr mThisWeak;
        PeerConnectionPtr mGracefulShutdownReference;

        IPeerConnectionDelegateSubscriptions mSubscriptions;
        IPeerConnectionSubscriptionPtr mDefaultSubscription;

        Configuration mConfiguration;

        InternalStates mState {InternalState_Pending};

        SignalingStates mLastSignalingState {SignalingState_First};
        ICEGatheringStates mLastICEGatheringStates {IICEGathererTypes::State_First};
        ICEConnectionStates mLastICEConnectionState {IICETransport::State_First};
        PeerConnectionStates mLastPeerConnectionState {PeerConnectionState_First};

        std::atomic<bool> mWakeCalled {false};
        bool mNegotiationNeeded {false};

        ISessionDescriptionPtr mLocalDescription;
        ISessionDescriptionPtr mPendingLocalDescription;

        ISessionDescriptionPtr mRemoteDescription;
        ISessionDescriptionPtr mPendingRemoteDescription;

        PendingMethodList mPendingMethods;

        TransportInfoMap mTransports;
        RTPMediaLineInfoMap mRTPMedias;
        SCTPMediaLineInfoMap mSCTPMedias;
        SenderInfoMap mSenders;
        ReceiverInfoMap mReceiver;

        CandidateList mPendingRemoteCandidates;
      };

      //-----------------------------------------------------------------------
      //-----------------------------------------------------------------------
      //-----------------------------------------------------------------------
      //-----------------------------------------------------------------------
      #pragma mark
      #pragma mark IPeerConnectionFactory
      #pragma mark

      interaction IPeerConnectionFactory
      {
        typedef IPeerConnectionTypes::Configuration Configuration;
        typedef IPeerConnectionTypes::ServerListPtr ServerListPtr;

        static IPeerConnectionFactory &singleton();

        virtual PeerConnectionPtr create(
                                         IPeerConnectionDelegatePtr delegate,
                                         const Optional<Configuration> &configuration = Optional<Configuration>()
                                         );

        virtual ServerListPtr getDefaultIceServers();
      };

      class PeerConnectionFactory : public IFactory<IPeerConnectionFactory> {};

    }
  }
}
