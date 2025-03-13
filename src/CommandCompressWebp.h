#pragma once
#include <webp/encode.h>
#include <iostream>

#include "CommandBase.h"
#include "Selector.h"

class CommandCompressWebp : public CommandBase {
public:
  CommandCompressWebp(Status& status) : CommandBase(status) {
    Selector select(status);

    WebPConfig config;
    if (mExceptionTriggered = (!WebPConfigPreset(&config, WEBP_PRESET_PHOTO, 0.75))) {
      std::cout << "Version error in configuration: " << WebPGetEncoderVersion() << "\n";
      return;
    }

    WebPPicture pic;
    if (mExceptionTriggered = (!WebPPictureInit(&pic))) {
      std::cout << "Version error in picture initialization: " << WebPGetEncoderVersion() << "\n";
      return;
    }

    WebPEncode(&config, &pic);

    select.filterByDate();
  }

};