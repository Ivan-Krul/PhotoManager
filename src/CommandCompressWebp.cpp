#include "CommandCompressWebp.h"

#include <iostream>
#include <fstream>

#include <jpeglib.h>
#include <webp/encode.h>
#include <exiv2/exiv2.hpp>

char CommandCompressWebp::calculateOptimalLineFeed() {
  if (mHeight < 50) return 1;
  else if (mHeight < 100) return 2;
  else if (mHeight < 500) return 10;
  else if (mHeight < 1000) return 20;
  else return 50;
}

bool CommandCompressWebp::readFromJpeg() {
  FILE* fin = fopen(mFilePath.string().c_str(), "rb");
  if (!fin) {
    std::cerr << "Cannot open " << mFilePath << '\n';
    return true;
  }

  jpeg_decompress_struct info;
  jpeg_error_mgr err;
  info.err = jpeg_std_error(&err);
  jpeg_create_decompress(&info);
  jpeg_stdio_src(&info, fin); // hook the stream
  jpeg_read_header(&info, true); //: for analysing what's goin on in the file
  jpeg_start_decompress(&info);

  mWidth = info.output_width;
  mHeight = info.output_height;
  mChannels = info.output_components;
  mData = new uint8_t[mWidth * mHeight * mChannels];

  int byte_width = mWidth * mChannels;
  mDataBuf = mData;
  //char feed = calculateOptimalLineFeed();
  //char feed_align = feed - (feed % mHeight);

  //if (info.output_scanline < mHeight) {
  //  jpeg_read_scanlines(&info, &mDataBuf, feed + feed_align);
  //}

  while (info.output_scanline < mHeight) {
    jpeg_read_scanlines(&info, &mDataBuf, 1);
    mDataBuf += byte_width;
  }

  jpeg_finish_decompress(&info);
  jpeg_destroy_decompress(&info);
  fclose(fin);
  return false;
}

bool CommandCompressWebp::writeToWebpInBuffer() {
  if (mChannels != 3) {
    std::cerr << "Unsupported format of channels (not R,G,B) in " << mFilePath << "\n";
    return true;
  }
  
  mDataBuf = nullptr;
  size_t size = WebPEncodeRGB(mData, mWidth, mHeight, mWidth * 3, 75, &mDataBuf);
  delete[] mData;

  if (!size) {
    std::cerr << "Failed to encode WebP in " << mFilePath << '\n';
    return true;
  }

   std::ofstream fout(mOutPath, std::ios::binary);
  
  fout.write(reinterpret_cast<char*>(mDataBuf), size);
  fout.close();


  WebPFree(mDataBuf);
  return false;
}

bool CommandCompressWebp::writeToWebpInExif() {
  auto image_ptr = Exiv2::ImageFactory::open(mFilePath.string());
  auto outpt_ptr = Exiv2::ImageFactory::open(mOutPath.string());

  if ((image_ptr.get() == 0) || (outpt_ptr.get() == 0)) {
    std::cerr << "Failed to load WebP image for metadata override\n";
    return true;
  }

  image_ptr->readMetadata();
  outpt_ptr->setExifData(image_ptr->exifData());

  try {
    outpt_ptr->writeMetadata();
  } catch (const Exiv2::Error&) {
    std::cerr << "Could not write metadata to (" << mOutPath << ")\n";
    return true;
  }

  return false;
}
