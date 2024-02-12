#include "third_party/blink/renderer/core/html/jimber/canvas_streamer.h"
#define TARGET "nalfile.nal"
#define OUTPUT_CODEC AV_CODEC_ID_H264
#define OUTPUT_PIX_FMT AV_PIX_FMT_YUV420P
#define MAGICKCORE_HDRI_ENABLE 1
#define MAGICKCORE_QUANTUM_DEPTH 16
#include <Magick++.h>
#include <csignal>
#include <fstream>
#include <iostream>
#include "third_party/blink/renderer/core/html/canvas/html_canvas_element.h"

namespace blink {
std::ofstream myfile;

CanvasStreamer::CanvasStreamer(HTMLCanvasElement* element)
    : element_(element),
      width_(element_->width()),
      height_(element_->height()),
      crf_(23) {
  Magick::InitializeMagick(nullptr);

  // std::cout << "CanvasStreamer::CanvasStreamer " << width_ << " " << height_
  //           << std::endl;
  // myfile.open("test.av1");
  std::string name = "/tmp/nals/";
  name.append(std::to_string((intptr_t)this));
  name.append(".nal");

  myfile.open(name, std::ios_base::binary);

  avformat_network_init();

  int err;
  // AVOutputFormat *fmt;

  av_register_all();
  fmt_ctx_ = avformat_alloc_context();
  if (fmt_ctx_ == nullptr) {
    std::cerr << "[CanvasStreamer::ctor] can not alloc av context" << std::endl;
  }

  fmt_ctx_->debug = AV_LOG_DEBUG;
  snprintf(fmt_ctx_->filename, sizeof(fmt_ctx_->filename), "%s", TARGET);

  codec_ = avcodec_find_encoder(OUTPUT_CODEC);
  if (!codec_) {
    std::cerr << "[CanvasStreamer::ctor] can't find encoder" << std::endl;
  }
  codec_ctx_ = avcodec_alloc_context3(codec_);
};

void CanvasStreamer::cleanup() {
  return;
  if (frame_) {
    av_free(frame_->data[0]);
    av_frame_free(&frame_);
    if (fmt_ctx_) {
      avio_close(fmt_ctx_->pb);
    }
  }
}

CanvasStreamer::~CanvasStreamer() {
  // std::cout << "CanvasStreamer::~CanvasStreamer " << this << std::endl;
  cleanup();
}

void CanvasStreamer::sendVideoData(uint8_t* rgba,
                                   int realWidth,
                                   int realHeight) {
  const int in_linesize[1] = {4 * width_};

  sws_scale(sws_context_, (const uint8_t* const*)&rgba, in_linesize, 0, height_,
            frame_->data, frame_->linesize);

  int err;
  int got_output = 1;
  AVPacket pkt = {0};

  av_init_packet(&pkt);

  // Set frame pts, monotonically increasing, starting from 0
  // if (frame != NULL)
  frame_->pts =
      pts_++;  // we use frame == NULL to write delayed packets in destructor

  err = avcodec_encode_video2(this->codec_ctx_, &pkt, frame_, &got_output);

  if (err < 0) {
    std::cerr << "[CanvasStreamer::sendVideoData]"
              << AVException(err, "avcodec_encode_video2").what() << std::endl;
  }

  if (got_output) {
    pkt.stream_index = 1;
    myfile.write((char*)pkt.data, pkt.size);
    JCommandHandler::instance().sendCanvasVideoData(
        element_, realWidth, realHeight,
        reinterpret_cast<const char*>(pkt.data), pkt.size);
  } else {
    std::cerr << "[CanvasStreamer::sendVideoData] pkt is empty?" << std::endl;
  }
};

void CanvasStreamer::resize(int width, int height) {
  if (width_ == width && height_ == height) {
    return;
  }

  width_ = width;
  height_ = height;
  cleanup();
  createNewContext();
}

void CanvasStreamer::createNewContext() {
  std::cout << "[CanvasStreamer::createNewContext] " << width_ << " " << height_
            << std::endl;
  // Set codec_ctx to stream's codec structure
  codec_ctx_ = avcodec_alloc_context3(codec_);
  /* put sample parameters */

  // width = 700;
  // height = 800;
  codec_ctx_->time_base.den = 25;
  codec_ctx_->time_base.num = 1;
  codec_ctx_->width = width_;
  codec_ctx_->height = height_;
  codec_ctx_->pix_fmt = OUTPUT_PIX_FMT;
  codec_ctx_->gop_size = 5000;
  codec_ctx_->max_b_frames = 1;
  /* Apparently it's in the example in master but does not work in V11
  if (o_format_ctx->oformat->flags & AVFMT_GLOBALHEADER)
      codec_ctx->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
  */
  // H.264 specific options
  std::string s = std::to_string(crf_);
  char const* crfString = s.c_str();  // use char const* as target type

  // std::cout << "crfstring " << crfString << std::endl;
  int err;
  err = av_opt_set(codec_ctx_->priv_data, "crf", crfString, 0);
  if (err < 0) {
    std::cerr << "Error crf : " << AVException(err, "av_opt_set crf").what()
              << std::endl;
  }

  // Required for h264 - idk why
  err = av_opt_set(codec_ctx_->priv_data, "profile", "baseline", 0);
  if (err < 0) {
    std::cerr << "Error : profile "
              << AVException(err, "av_opt_set profile").what() << std::endl;
  }

  err = av_opt_set(codec_ctx_->priv_data, "tune", "zerolatency", 0);
  if (err < 0) {
    // codec_ctx_->time_base.den = 25;
    // codec_ctx_->time_base.num = 1;
    std::cerr << "Error : " << AVException(err, "av_opt_set zerolatency").what()
              << std::endl;
  }
  // end

  // Optional for h264
  err = av_opt_set(codec_ctx_->priv_data, "preset", "ultrafast", 0);
  if (err < 0) {
    std::cerr << "Error :  preset"
              << AVException(err, "av_opt_set preset").what() << std::endl;
  }
  // end

  // It's necessary to open stream codec to link it to "codec" (the encoder).
  err = avcodec_open2(codec_ctx_, codec_, NULL);
  char errbuf[256];

  // if (err < 0) {
  //   av_strerror(err, errbuf, sizeof(errbuf));
  //   std::cerr << errbuf << std::endl;
  //   std::cerr << AVException(err, "avcodec_open2").what() << std::endl;
  // }

  // dump av format informations
  // std::cout << codec_ctx_ << " " << target << " ############# " <<
  // std::flush; av_dump_format(fmt_ctx_, 0, TARGET, 1);
  //
  AVDictionary* options = NULL;
  // av_dict_set(&options, "pkt_size", "130", 0);
  // av_dict_set(&options, "buffer_size", "0", 0);

  av_dict_set(&options, "pkt_size", "5000", 0);
  av_dict_set(&options, "buffer_size", "0", 0);
  int flags = AVIO_FLAG_WRITE;
  flags |= AVIO_FLAG_DIRECT;

  err = avio_open2(&fmt_ctx_->pb, TARGET, flags, NULL, &options);
  // char errbuf[256];
  if (err < 0) {
    av_strerror(err, errbuf, sizeof(errbuf));
    std::cerr << AVException(err, "avio_open2").what() << std::endl;
    std::cerr << errbuf << std::endl;
  }
  frame_ = av_frame_alloc();

  frame_->format = OUTPUT_PIX_FMT;
  frame_->width = width_;
  frame_->height = height_;
  err = av_image_alloc(frame_->data, frame_->linesize, width_, height_,
                       OUTPUT_PIX_FMT, 32);
  if (err < 0) {
    std::cerr << AVException(err, "av_image_alloc").what() << std::endl;
  }

  if (!frame_) {
    std::cerr
        << "[CanvasStreamer::createNewContext] Could not allocate video frame"
        << std::endl;
  }
  sws_context_ = sws_getCachedContext(NULL, width_, height_, AV_PIX_FMT_RGBA,
                                      width_, height_, OUTPUT_PIX_FMT,
                                      // 394, 656, AV_PIX_FMT_YUV420P,
                                      0, 0, 0, 0);
}

void CanvasStreamer::SendNewFrame(
    scoped_refptr<StaticBitmapImage> image_bitmap,
    base::WeakPtr<WebGraphicsContext3DProviderWrapper>) {
  return;  // performance is too bad
  if (!element_->isConnected()) {
    return;
  }
  const int element_width = element_->width();
  const int element_height = element_->height();

  int size = element_width * element_height * 4;

  // std::cout << "Total size " << element_width << " " << element_height << "
  // "
  //           << size << std::endl;

  std::unique_ptr<std::vector<uint8_t>> bytes(new std::vector<uint8_t>(size));

  base::span<uint8_t> imageBytes(bytes.get()->data(), size);
  const CanvasColorParams& color_params = element_->ColorParams();
  IntRect image_data_rect(0, 0, element_width, element_height);
  bool success = StaticBitmapImage::CopyToByteArray(
      image_bitmap, imageBytes, image_data_rect, color_params);

  if (!success) {
    std::cerr << "[CanvasStreamer::SendNewFrame] Couldnt "
                 "get bytes from image"
              << std::endl;
  }

  // logImageBytes(imageBytes, 50);
  // {
  //   std::cout << "[CanvasStreamer::SendNewFrame] making zeros " << size
  //             << std::endl;
  //   std::vector<uint8_t> zeros(size);
  //   std::cout << "[CanvasStreamer::SendNewFrame] memcmp?" << std::endl;
  //   if (memcmp(bytes.get(), &zeros[0], size) == 0) {
  //     JCommandHandler::instance().sendClearCanvas(element_);
  //     return;
  //   } else {
  //     std::cout << "[CanvasStreamer::SendNewFrame] memcmp not all zero!"
  //               << std::endl;
  //   }
  // }
  // check if image contains alpha
  // if (doesImageContainAlphaValues(imageBytes)) {
  //   std::cout << "[CanvasStreamer::SendNewFrame]"
  //                "image does contain alpha "
  //             << std::endl;
  sendImageData(reinterpret_cast<uint8_t*>(imageBytes.data()), element_width,
                element_height);
  return;
  // }

  std::cout << "[CanvasStreamer::SendNewFrame] image does not contain alpha"
            << std::endl;

  // else convert video

  int img_width = element_width;
  int img_height = element_height;
  int mod = img_width % 4;

  if (mod == 1) {
    img_width -= 1;
  } else if (mod == 2) {
    img_width += 2;
  } else if (mod == 3) {
    img_width += 1;
  }

  if (img_height % 2 != 0) {
    img_height++;
  }
  if (img_height < 113) {
    img_height = 113;
  }

  if (img_width != width() || img_height != height()) {
    // std::cout << "[CanvasStreamer::SendNewFrame] " << img_width << " "
    //           << img_height << std::endl;
    resize(img_width, img_height);
  }

  this->sendVideoData(reinterpret_cast<uint8_t*>(imageBytes.data()),
                      element_width, element_height);
}

// This doesn't remove the extra alpha bytes for the array, it just sets it to
// 255 ¯\_(ツ)_/¯
void CanvasStreamer::rgbaToRgb(base::span<uint8_t>& imageBytes) {
  int alpha = imageBytes[3];
  imageBytes[0] = (1 - (alpha / 255.0)) * 0 + (alpha / 255.0) * imageBytes[0];
  int alphaIndex = 3;
  for (int i = 1; i < imageBytes.size(); i++) {
    if (i % alphaIndex != 0) {
      imageBytes[i] =
          (1 - (alpha / 255.0)) * 0 + (alpha / 255.0) * imageBytes[i];
    } else {
      if (i != imageBytes.size() - 1) {
        if (imageBytes[i] != 0) {
          imageBytes[i] = 255;
        }
        alpha = imageBytes[i + 4];
        alphaIndex += 4;
      }
    }
  }
}

void CanvasStreamer::sendImageData(uint8_t* rgba,
                                   int realWidth,
                                   int realHeight) {
  Magick::Image image;
  image.read(realWidth, realHeight, "RGBA", Magick::CharPixel, rgba);

  Magick::Blob blob;
  image.magick("PNG");
  image.write(&blob);
  // std::string name = "/tmp/images/";
  // name.append(std::to_string((intptr_t)this));
  // name.append("-");
  // name.append(std::to_string((intptr_t)rgba));
  // name.append(".png");
  // image.write(name);

  JCommandHandler::instance().sendCanvasImageData(
      element_, realWidth, realHeight,
      reinterpret_cast<const char*>(blob.data()), blob.length());

  // JCommandHandler::instance().sendCanvasImageData(
  //     element_, realWidth, realHeight,
  //     reinterpret_cast<const char*>(rgba), 1);
}
bool CanvasStreamer::doesImageContainAlphaValues(
    const base::span<uint8_t>& imageBytes) {
  // std::cout << "[CanvasStreamer::doesImageContainAlphaValues] " << std::endl;

  for (int alphaIndex = 3; alphaIndex < imageBytes.size(); alphaIndex += 4) {
    if ((alphaIndex != imageBytes.size() - 1) && imageBytes[alphaIndex] < 255) {
      return true;
    }
  }
  return false;
}

void CanvasStreamer::logImageBytes(base::span<uint8_t>& imageBytes,
                                   int length) {
  std::cout << "[CanvasStreamer::logImageBytes] start" << std::endl;
  int alphaIndex = 3;
  std::cout << +imageBytes[0] << " ";

  for (int i = 1; i < length; i++) {
    if (i % alphaIndex != 0) {
      std::cout << +imageBytes[i] << " ";
    } else {
      std::cout << +imageBytes[i] << " - ";
      alphaIndex += 4;
    }
  }

  std::cout << std::endl;
  std::cout << "[CanvasStreamer::logImageBytes] end" << std::endl;
}
}  // namespace blink

// stuff for vp9
// err = av_opt_set(codec_ctx_->priv_data, "quality", "realtime", 0);
// if (err < 0) {
//   std::cerr << "Error : " << AVException(err, "av_opt_set quality").what()
//             << std::endl;
// }
// err = av_opt_set(codec_ctx_->priv_data, "speed", "8", 0);
// if (err < 0) {
//   std::cerr << "Error : " << AVException(err, "av_opt_set quality").what()
//             << std::endl;
// }

// err = av_opt_set(codec_ctx_->priv_data, "rc_lookahead", "0", 0);
// if (err < 0) {
//   std::cerr << "Error : " << AVException(err, "rc_lookahead
//   quality").what()
//             << std::endl;
// }
// end