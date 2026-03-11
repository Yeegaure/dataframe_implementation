//
// Created by Egor Smirnov on 10.03.2026.
//

#ifndef DF_DOWNLOADER_H
#define DF_DOWNLOADER_H
#include "DataFrame.h"
#include "Downloader.h"

class DF_Downloader : public Downloader {
private:
    const DataFrame &df_;
public:
    explicit DF_Downloader(const DataFrame &df);
    void save(const std::string &name) const override;
};

#endif //DF_DOWNLOADER_H
