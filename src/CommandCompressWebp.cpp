#include "CommandCompressWebp.h"

#include <iostream>
#include <fstream>

#include <jpeglib.h>
#include <webp/encode.h>

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
    std::cout << "Cannot open " << mFilePath << '\n';
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
    std::cout << "Unsupported format of channels (not R,G,B) in " << mFilePath << "\n";
    return true;
  }
  
  mDataBuf = nullptr;
  size_t size = WebPEncodeRGB(mData, mWidth, mHeight, mWidth * 3, 75, &mDataBuf);
  delete[] mData;

  if (!size) {
    std::cout << "Failed to encode WebP in " << mFilePath << '\n';
    return false;
  }

  mFilePath.replace_extension(".webp");

   std::ofstream fout(
     ((sStatus.branch.has_buffer_dir)
       ? sStatus.branch.path / "buffer" / mFilePath.filename()
       : mFilePath),
    std::ios::binary);
  
  fout.write(reinterpret_cast<char*>(mDataBuf), size);
  fout.close();


  WebPFree(mDataBuf);
  return false;
}
