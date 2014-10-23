//
// PNNativeAdModel.cpp
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

#include "PNNativeAdModel.h"
#include "document.h"
#include "stringbuffer.h"
#include "writer.h"

using namespace std;
using namespace cocos2d;
using namespace cocos2d::pubnative;

static std::string PNNATIVE_ICON_URL = "icon_url";
static std::string PNNATIVE_BANNER_URL = "banner_url";
static std::string PNNATIVE_PORTRAIT_BANNER_URL = "portrait_banner_url";

PNNativeAdModel::PNNativeAdModel(void)
: PNModel()
, click_url("")
, points(0)
, cta_text("")
, type("")
, title("")
, description("")
, icon_url("")
, banner_url("")
, portrait_banner_url("")
, _requestCounter(0)
{
    icon_url_image = NULL;
    banner_url_image = NULL;
    portrait_banner_url_image = NULL;
    beacons = NULL;
    app_details = NULL;
}

PNNativeAdModel::~PNNativeAdModel(void)
{
    CC_SAFE_RELEASE(app_details);
    CC_SAFE_RELEASE(beacons);
    CC_SAFE_RELEASE(icon_url_image);
    CC_SAFE_RELEASE(banner_url_image);
    CC_SAFE_RELEASE(portrait_banner_url_image);
}

PNNativeAdModel* PNNativeAdModel::createWithJSON(std::string json)
{
    PNNativeAdModel *result = NULL;
    
    rapidjson::Document document = parseJson(json);
    if(!document.HasParseError())
    {
        result = new PNNativeAdModel();
        if (result != NULL)
        {
            result->click_url           = processValueString(document["click_url"]);
            result->points              = processValueInt(document["points"]);
            result->cta_text            = processValueString(document["cta_text"]);
            result->type                = processValueString(document["type"]);
            result->title               = processValueString(document["title"]);
            result->description         = processValueString(document["description"]);
            result->icon_url            = processValueString(document["icon_url"]);
            result->banner_url          = processValueString(document["banner_url"]);
            result->portrait_banner_url = processValueString(document["portrait_banner_url"]);
            result->app_details         = PNAppDetailsModel::createWithJSON(getJSON(document["app_details"]));
            result->app_details->retain();
            result->beacons             = PNBeaconsModel::createWithJSON(getJSON(document["beacons"]));
            result->beacons->retain();
            result->autorelease();
        }
    }
    
    return result;
}

void PNNativeAdModel::load()
{
    _requestCounter = 3;
    downloadImage(icon_url, "icon_url");
    downloadImage(banner_url, "banner_url");
    downloadImage(portrait_banner_url, "portrait_banner_url");
}

void PNNativeAdModel::onImageDownloaded(cocos2d::CCImage *image, std::string name)
{
    if (image == NULL)
    {
        invokeFailCallback();
        return;
    }
    
    image->retain();
    _requestCounter--;
    
    if (PNNATIVE_ICON_URL == name)
    {
        icon_url_image = image;
    }
    else if (PNNATIVE_BANNER_URL == name)
    {
        banner_url_image = image;
    }
    else if (PNNATIVE_PORTRAIT_BANNER_URL == name)
    {
        portrait_banner_url_image = image;
    }
    
    if (_requestCounter <= 0)
    {
        invokeReadyCallback();
    }
}


