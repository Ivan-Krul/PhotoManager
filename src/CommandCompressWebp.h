#pragma once
#include <iostream>

#include "CommandBase.h"
#include "Selector.h"

class CommandCompressWebp : public CommandBase {
public:
  CommandCompressWebp(Status& status) : CommandBase(status) {
    Selector select(status);
    select.filterByDate();

    for (size_t i = 0; i < select.size(); i++) {
      if (select[i].extension().string() != ".jpg") continue;
      std::cout << "Progress: " << i + 1 << '/' << select.size() << ": " << select[i].filename() << '\n';
      mFilePath = select[i];
      mOutPath = select[i];
      mOutPath.replace_extension(".webp");
      if (mExceptionTriggered = readFromJpeg()) return;
      if (mExceptionTriggered = writeToWebpInBuffer()) return;
    }
  }

private:
  char calculateOptimalLineFeed();
  bool readFromJpeg();
  bool writeToWebpInBuffer();
  bool writeToWebpInExif();

private:
  Path mFilePath;
  Path mOutPath;
  uint8_t* mData;
  uint8_t* mDataBuf;
  unsigned short mWidth;
  unsigned short mHeight;
  unsigned short mChannels;
};