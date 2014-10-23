//
// PNImageAdModel.cpp
//
// Created by David Martin on 19/10/14.
//
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

#include "PNImageAdModel.h"
#include "document.h"

using namespace std;
using namespace cocos2d;
using namespace cocos2d::pubnative;

static string PNIMAGEADMODEL_IMAGE_URL = "image_url";

//string type;
//int width;
//int height;
//string image_url;
//CCImage *image_url_image;
//string click_url;
//PNBeaconsModel *beacons;

PNImageAdModel::PNImageAdModel(void)
: PNModel()
, type("")
, width(0)
, height(0)
, image_url("")
, click_url("")
{
    //Constructor
    image_url_image = NULL;
    beacons = NULL;
}

PNImageAdModel::~PNImageAdModel(void)
{
    //Destructor
    CC_SAFE_RELEASE(beacons);
}

PNImageAdModel* PNImageAdModel::createWithJSON(string json)
{
    PNImageAdModel *result = NULL;
    
    rapidjson::Document document = parseJson(json);
    if(!document.HasParseError())
    {
        result = new PNImageAdModel();
        if (result != NULL)
        {
            result->type        = processValueString(document["type"]);
            result->width       = processValueInt(document["width"]);
            result->height      = processValueInt(document["height"]);
            result->image_url   = processValueString(document["image_url"]);
            result->click_url   = processValueString(document["click_url"]);
            result->beacons     = PNBeaconsModel::createWithJSON(getJSON(document["beacons"]));
            result->beacons->retain();
            
            result->autorelease();
        }
    }
    
    return result;
}

void PNImageAdModel::load()
{
    downloadImage(image_url, PNIMAGEADMODEL_IMAGE_URL);
}

void PNImageAdModel::onImageDownloaded(cocos2d::CCImage *image, std::string name)
{
    if (image == NULL)
    {
        invokeFailCallback();
        return;
    }
    
    image->retain();
    
    if (PNIMAGEADMODEL_IMAGE_URL == name)
    {
        image_url_image = image;
    }
    
    invokeReadyCallback();
}