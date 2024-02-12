#ifndef THIRD_PARTY_BLINK_RENDERER_CORE_HTML_JIMBER_CANVAS_STREAMER_H_
#define THIRD_PARTY_BLINK_RENDERER_CORE_HTML_JIMBER_CANVAS_STREAMER_H_

#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include "third_party/blink/renderer/core/html/canvas/canvas_draw_listener.h"
#include "third_party/blink/renderer/platform/heap/handle.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavformat/avio.h>
#include <libavutil/imgutils.h>
#include <libavutil/opt.h>
#include <libswscale/swscale.h>
}
// #include "third_party/blink/renderer/core/html/canvas/html_canvas_element.h"

/*
  How to add an encoder to chromium-ffmpeg
  - allcodecs.c look for codec name
  - ctrl shift f look for codec file
  - if ifdef, look for ifdef and open
  ffmpeg/chromium/config/Chrome/linux/x64/config.h to enable the ifdef
  - add codec .c file to ffmpeg_generated.gni
  - add codec name to
  ffmpeg/chromium/config/Chrome/linux/x64/libavcodec/codec_list.c
  - add library to src/core/core_module.pro - LIBS_PRIVATE=
*/

namespace blink {

class HTMLCanvasElement;
class CanvasStreamer : public GarbageCollected<CanvasStreamer>,
                       public CanvasDrawListener {
  USING_GARBAGE_COLLECTED_MIXIN(CanvasStreamer);

 public:
  CanvasStreamer(HTMLCanvasElement* element);
  ~CanvasStreamer();
  int width() { return width_; }
  int height() { return height_; }

  void SendNewFrame(
      scoped_refptr<StaticBitmapImage>,
      base::WeakPtr<WebGraphicsContext3DProviderWrapper>) override;
  bool NeedsNewFrame() const { return true; };
  void RequestFrame() {}
  void Trace(Visitor*) override {};

 private:
  void setupCodec();
  void createNewContext();
  void cleanup();
  void resize(int width, int height);
  void sendVideoData(uint8_t* rgb, int realWidth, int realHeight);
  void sendImageData(uint8_t* rgba, int realWidth, int realHeight);
  void rgbaToRgb(base::span<uint8_t>& imageBytes);
  bool doesImageContainAlphaValues(const base::span<uint8_t>& imageBytes);
  void logImageBytes(base::span<uint8_t>& imageBytes, int length);

  HTMLCanvasElement* element_;
  SwsContext* sws_context_;
  AVFormatContext* fmt_ctx_;
  AVCodecContext* codec_ctx_;
  AVCodec* codec_;
  int width_;
  int height_;
  int crf_;
  AVFrame* frame_;
  int pts_ = 0;
  // std::mutex pktMutex_;
  // std::queue<AVPacket> pkts_;
  std::ofstream myfile;
};

}  // namespace blink

class AVException : public std::exception {
  std::string _message;

 public:
  AVException(int err, std::string message) {
    this->_message = std::to_string(err) + " " + message;
  };
  ~AVException() throw(){};
  std::string getMessage(void) { return _message; };

  virtual const char* what() const throw() { return _message.c_str(); }
};

#endif  // THIRD_PARTY_BLINK_RENDERER_CORE_HTML_JIMBER_CANVASE_STREAMER_H_