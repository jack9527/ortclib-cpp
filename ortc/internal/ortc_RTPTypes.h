/*

 Copyright (c) 2014, Hookflash Inc. / Hookflash Inc.
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

#include <ortc/internal/types.h>

#include <ortc/IMediaStreamTrack.h>
#include <ortc/IRTPTypes.h>

namespace ortc
{
  namespace internal
  {
    class RTPTypesHelper
    {
    public:
      typedef IRTPTypes::PayloadType PayloadType;

      ZS_DECLARE_TYPEDEF_PTR(IRTPTypes::CodecParameters, CodecParameters)
      ZS_DECLARE_TYPEDEF_PTR(IRTPTypes::EncodingParameters, EncodingParameters)
      ZS_DECLARE_TYPEDEF_PTR(IRTPTypes::Parameters, Parameters)
      ZS_DECLARE_TYPEDEF_PTR(std::list<ParametersPtr>, ParametersPtrList)

      ZS_DECLARE_TYPEDEF_PTR(Parameters, OldParameters)
      ZS_DECLARE_TYPEDEF_PTR(Parameters, NewParameters)

      typedef std::pair<OldParametersPtr, NewParametersPtr> OldNewParametersPair;

      ZS_DECLARE_TYPEDEF_PTR(std::list<OldNewParametersPair>, ParametersPtrPairList)

      struct FindCodecOptions
      {
        typedef std::set<PayloadType> PayloadTypeSet;

        Optional<PayloadType> mPayloadType;
        Optional<IMediaStreamTrackTypes::Kinds> mKind;
        Optional<bool> mAllowNeutralKind;
        Optional<IRTPTypes::CodecKinds> mCodecKind;
        Optional<IRTPTypes::SupportedCodecs> mSupportedCodec;
        Optional<ULONG> mClockRate;
        Optional<bool> mMatchClockRateNotSet;
        PayloadTypeSet mDisallowedPayloadtypeMatches;
        Optional<bool> mDisallowMultipleMatches;

        ElementPtr toDebug() const;
      };
      

      static void splitParamsIntoChannels(
                                          const Parameters &params,
                                          ParametersPtrList &outParamsGroupedIntoChannels
                                          );

      static void calculateDeltaChangesInChannels(
                                                  Optional<IMediaStreamTrackTypes::Kinds> kind,
                                                  const ParametersPtrList &inExistingParamsGroupedIntoChannels,
                                                  const ParametersPtrList &inNewParamsGroupedIntoChannels,
                                                  ParametersPtrPairList &outUnchangedChannels,
                                                  ParametersPtrList &outNewChannels,
                                                  ParametersPtrPairList &outUpdatedChannels,
                                                  ParametersPtrList &outRemovedChannels
                                                  );

      static bool isGeneralizedSSRCCompatibleChange(
                                                    const Parameters &oldParams,
                                                    const Parameters &newParams
                                                    );

      static bool isCompatibleCodec(
                                    const CodecParameters &oldCodec,
                                    const CodecParameters &newCodec,
                                    float &ioRank
                                    );

      static const CodecParameters *findCodec(
                                              const Parameters &params,
                                              const FindCodecOptions &options
                                              );

      static const CodecParameters *pickCodec(
                                              Optional<IMediaStreamTrackTypes::Kinds> kind,
                                              const Parameters &params,
                                              Optional<PayloadType> packetPayloadType = Optional<PayloadType>(),
                                              const EncodingParameters *encoding = NULL,
                                              const EncodingParameters *baseEncoding = NULL
                                              );

      static bool isRankableMatch(
                                  Optional<IMediaStreamTrackTypes::Kinds> kind,
                                  const Parameters &oldChannelParams,
                                  const Parameters &newChannelParams,
                                  float &outRank
                                  );

      static const CodecParameters *pickRTXCodec(
                                                 Optional<IMediaStreamTrackTypes::Kinds> kind,
                                                 const Parameters &params,
                                                 Optional<PayloadType> packetRTXPayloadType = Optional<PayloadType>(),
                                                 const EncodingParameters *encoding = NULL,
                                                 const EncodingParameters *baseEncoding = NULL
                                                 );

      static const CodecParameters *pickFECCodec(
                                                 Optional<IMediaStreamTrackTypes::Kinds> kind,
                                                 const Parameters &params,
                                                 Optional<PayloadType> packetFECPayloadType = Optional<PayloadType>(),
                                                 const EncodingParameters *encoding = NULL,
                                                 const EncodingParameters *baseEncoding = NULL
                                                 );

      static EncodingParameters *findEncodingBase(
                                                  Parameters &inParams,
                                                  EncodingParameters *inEncoding
                                                  );

      static EncodingParameters *pickEncodingToFill(
                                                    Optional<IMediaStreamTrackTypes::Kinds> kind,
                                                    PayloadType packetPayloadType,
                                                    Parameters &filledParams,
                                                    const CodecParameters * &outCodecParameters,
                                                    IRTPTypes::SupportedCodecs &outSupportedCodec,
                                                    IRTPTypes::CodecKinds &outCodecKind,
                                                    EncodingParameters * &outBaseEncoding
                                                    );

      static Optional<IMediaStreamTrackTypes::Kinds> getCodecsKind(const Parameters &params);

      static Log::Params slog(const char *message);
    };
  }
}
