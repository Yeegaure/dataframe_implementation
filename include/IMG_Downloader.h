//
// Created by Egor Smirnov on 10.03.2026.
//

#ifndef IMG_DOWNLOADER_H
#define IMG_DOWNLOADER_H

#include "Downloader.h"
#include <string>

class IMG_Downloader : public Downloader {
private:
    std::string curr_img_path_;
public:
    explicit IMG_Downloader(const std::string &curr_img_path);
    void save(const std::string &name) const override;
};

#endif // IMG_DOWNLOADER_H
