
// Copyright (c) 2019 Agora.io. All rights reserved

// This program is confidential and proprietary to Agora.io.
// And may not be copied, reproduced, modified, disclosed to others, published
// or used, in whole or in part, without the express prior written permission
// of Agora.io.

#pragma once  // NOLINT(build/header_guard)

#include "../AgoraBase.h"
#include "../AgoraRefPtr.h"
#include "NGIAgoraMediaNodeFactory.h"
#include "IAgoraService.h"

namespace agora {
namespace rtc {
class IVideoFilter;
class IVideoRenderer;
class IVideoTrackStateObserver;

/**
 * The IVideoTrack class.
 */
class IVideoTrack : public RefCountInterface {
 public:
  /**
   * Adds a video filter to the video track.
   *
   * To add a video filter, choose either of the following:
   * - Use the IMediaNodeFactory object to create a built-in video filter.
   * - Customize your own video filter by implementing a subclass of the ICustomVideoFilter object.
   *
   * To add multiple filters, call this method multiple times. The order of the added filters depends on when
   * the app successfully adds the filter.
   *
   * @param filter The video filter that you want to add to the video track: IVideoFilter.
   * @return
   * - true: The video filter is added successfully.
   * - false: The video filter fails to be added.
   */
  virtual bool addVideoFilter(agora_refptr<IVideoFilter> filter, bool internal = false) = 0;
  /**
   * Removes the video filter added by addVideoFilter() from the video track.
   *
   * @param filter The video filter that you want to remove: IVideoFilter.
   * @return
   * - true: The video filter is removed successfully.
   * - false: The video filter fails to be removed.
   */
  virtual bool removeVideoFilter(agora_refptr<IVideoFilter> filter) = 0;

  /**
   * Adds a video renderer to the video track.
   *
   * To add a video renderer, choose either of the following:
   * - Use the IMediaNodeFactory object to create a built-in video renderer.
   * - Customize your own video renderer by implementing a subclass of the IVideoSinkBase object.
   *
   * @param videoRenderer The video renderer that you want to add: IVideoSinkBase.
   *
   * @return
   * - true: The video renderer is added successfully.
   * - false: The video renderer fails to be added.
   */
  virtual bool addRenderer(agora_refptr<IVideoSinkBase> videoRenderer) = 0;
  /**
   * Removes the video renderer added by addRenderer() from the video track.
   *
   * @param videoRenderer The video renderer that you want to remove: IVideoSinkBase.
   * @return
   * - true: The video renderer is removed successfully.
   * - false: The video renderer fails to be removed.
   */
  virtual bool removeRenderer(agora_refptr<IVideoSinkBase> videoRenderer) = 0;

 protected:
  ~IVideoTrack() {}
};

/**
 * Statistics of the local video track.
 */
struct LocalVideoTrackStats {
  uint64_t number_of_streams = 0;
  uint64_t bytes_major_stream = 0;
  uint64_t bytes_minor_stream = 0;
  uint32_t frames_encoded = 0;
  uint32_t ssrc_major_stream = 0;
  uint32_t ssrc_minor_stream = 0;
  int input_frame_rate = 0;
  int encode_frame_rate = 0;
  int target_media_bitrate_bps = 0;
  int media_bitrate_bps = 0;
  int total_bitrate_bps = 0;  // Include FEC
  int width = 0;
  int height = 0;
  uint32_t encoder_type = 0;
};

/**
 * The ILocalVideoTrack class.
 */
class ILocalVideoTrack : public IVideoTrack {
 public:
  /**
   * Enables or disables the local video track.
   *
   * Once enabled, the SDK allows for local video capturing, processing, and encoding.
   *
   * @param enable Determines whether to enable video.
   * - true: Enable the video.
   * - false: Disable the video.
   */
  virtual void setEnabled(bool enable) = 0;

  /**
   * Sets the video encoder configuration.
   *
   * Each video encoder configuration corresponds to a set of video parameters, including the
   * resolution, frame rate, bitrate, and video orientation.
   *
   * The parameters specified in this method are the maximum values under ideal network conditions. If
   * the video engine cannot render the video using the specified parameters due to poor network
   * conditions, the parameters further down the list are considered until a successful configuration
   * is found.
   *
   * @param config The video encoder configuration: VideoEncoderConfiguration.
   * @return
   * - 0: Success.
   * - < 0: Failure.
   */
  virtual int setVideoEncoderConfiguration(const VideoEncoderConfiguration& config) = 0;

  /**
   * Enables or disables the simulcast stream mode.
   *
   * @param enabled Determines whether to enable or disable the simulcast stream mode.
   * - true: Enable the simulcast stream mode.
   * - false: Disable the simulcast stream mode.
   * @param config Configurations fot the simulcast stream mode: SimulcastStreamConfig.
   * @return
   * - 0: Success.
   * - < 0: Failure.
   */
  virtual int enableSimulcastStream(bool enabled, const SimulcastStreamConfig& config) = 0;

  /**
   * Gets the state of the local video.
   *
   * @return The current state of the local video: #LOCAL_VIDEO_STREAM_STATE.
   */
  virtual LOCAL_VIDEO_STREAM_STATE getState() = 0;

  /**
   * Gets the statistics of the local video track.
   *
   * @param stats The statistics of the local video track: LocalVideoTrackStats.
   * @return
   * - true: Success.
   * - false: Failure.
   */
  virtual bool getStatistics(LocalVideoTrackStats& stats) = 0;

 protected:
  ~ILocalVideoTrack() {}
};
/**
 * The struct of RemoteVideoTrackStats.
 */
struct RemoteVideoTrackStats {
  /**
   * The ID of the remote user.
   */
  uid_t uid = 0;
  /**
   * The time delay (ms).
   */
  int32_t delay = 0;
  /**
   * The width of the remote video.
   */
  int32_t width = 0;
  /**
   * The height of the remote video.
   */
  int32_t height = 0;
  /**
   * The bitrate received in the reported interval.
   */
  int32_t receivedBitrate = 0;
  /**
   * The frame rate received in the reported interval.
   */
  int32_t receivedFrameRate = 0;
  /**
   * The video stream type: #REMOTE_VIDEO_STREAM_TYPE.
   */
  REMOTE_VIDEO_STREAM_TYPE streamType = REMOTE_VIDEO_STREAM_HIGH;
  /**
   * The total number of the decoded frames.
   */
  int totalDecodedFrames = 0;
};

/**
 * The IRemoteVideoTrack class.
 */
class IRemoteVideoTrack : public IVideoTrack {
 public:

  /**
   * Gets the statistics of the remote video track.
   * @param stats The statistics of the remote video track: RemoteVideoTrackStats.
   * @return
   * - true: Success.
   * - failure: Failure.
   */
  virtual bool getStatistics(RemoteVideoTrackStats& stats) = 0;
  /**
   * Gets the state of the remote video.
   * @return The state of the remote video stream: #REMOTE_VIDEO_STREAM_STATE.
   */
  virtual REMOTE_VIDEO_STREAM_STATE getState() = 0;
  /**
   * Gets the information of the remote video track.
   * @param info Information of the remote video track: RemoteVideoTrackInfo.
   * @return
   * - true: Success.
   * - false: Failure.
   */
  virtual bool getTrackInfo(VideoTrackInfo& info) = 0;
  /**
   * Registers an IVideoEncodedImageReceiver object.
   *
   * You need to implement the IVideoEncodedImageReceiver class in this method. Once you successfully register
   * the encoded image receiver, the SDK triggers the onEncodedVideoImageReceived callback when it receives the
   * encoded video image.
   *
   * @param videoReceiver The pointer to the IVideoEncodedImageReceiver object.
   * @return
   * - 0: Success.
   * - < 0: Failure.
   */
  virtual int registerVideoEncodedImageReceiver(IVideoEncodedImageReceiver* videoReceiver) = 0;
  /**
   * Releases the IVideoEncodedImageReceiver object.
   * @param videoReceiver The pointer to the IVideoEncodedImageReceiver object.
   * @return
   * - 0: Success.
   * - < 0: Failure.
   */
  virtual int unregisterVideoEncodedImageReceiver(IVideoEncodedImageReceiver* videoReceiver) = 0;
  
  /**
   * Registers an IMediaPacketReceiver object.
   *
   * You need to implement the IMediaPacketReceiver class in this method. Once you successfully register
   * the media packet receiver, the SDK triggers the onMediaPacketReceived callback when it receives the
   * video packet.
   *
   * @param videoReceiver The pointer to the IMediaPacketReceiver object.
   * @return
   * - 0: Success.
   * - < 0: Failure.
   */
  virtual int registerMediaPacketReceiver(IMediaPacketReceiver* videoReceiver) = 0;
  /**
   * Releases the IMediaPacketReceiver object.
   * @param videoReceiver The pointer to the IMediaPacketReceiver object.
   * @return
   * - 0: Success.
   * - < 0: Failure.
   */
  virtual int unregisterMediaPacketReceiver(IMediaPacketReceiver* videoReceiver) = 0;

 protected:
  ~IRemoteVideoTrack() {}
};

}  // namespace rtc
}  // namespace agora
