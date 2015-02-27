//
// PNNativeAdModel.h
//
// Created by David Martin on 19/10/14.
// Copyright (c) 2014 PubNative
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#ifndef __PUBNATIVE_PNNativeAdModel__
#define __PUBNATIVE_PNNativeAdModel__

#include <stdio.h>
#include "cocos2d.h"
#include "PNAppDetailsModel.h"
#include "PNBeaconsModel.h"
#include "PNModel.h"

using namespace std;

namespace cocos2d
{
    namespace pubnative
    {
        class PNNativeAdModel : public PNModel
        {
        public:
            
            string              click_url;
            int                 points;
            string              cta_text;
            string              type;
            string              title;
            string              description;
            string              icon_url;
            CCImage             *icon_url_image;
            string              banner_url;
            CCImage             *banner_url_image;
            string              portrait_banner_url;
            CCImage             *portrait_banner_url_image;
            
            PNAppDetailsModel   *app_details;
            PNBeaconsModel      *beacons;
            
            PNNativeAdModel(void);
            ~PNNativeAdModel(void);
            
            static PNNativeAdModel* createWithJSON(string json);
            void load();
        protected:
            void onImageDownloaded(CCImage *image, std::string name);
        private:
            int _requestCounter;
        };
    }
}

#endif