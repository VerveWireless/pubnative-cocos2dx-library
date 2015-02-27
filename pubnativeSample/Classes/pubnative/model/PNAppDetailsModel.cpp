//
// PNAppDetailsModel.cpp
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

#include "PNAppDetailsModel.h"

using namespace std;
using namespace cocos2d;
using namespace cocos2d::pubnative;

PNAppDetailsModel::PNAppDetailsModel()
: PNModel()
, name()
, platform()
, review()
, review_url()
, publisher()
, developer()
, version()
, size()
, age_rating()
, store_description()
, store_url()
, store_id()
, url_scheme()
, release_date()
, installs()
, category()
, sub_category()
{
    //Constructor
    review_pros = NULL;
    review_cons = NULL;
    store_rating = 0;
    total_ratings = 0;
    store_categories = NULL;
}

PNAppDetailsModel::~PNAppDetailsModel()
{
    //Destructor
    if(review_pros != NULL)
    {
        review_pros->removeAllObjects();
    }
    if(review_cons != NULL)
    {
        review_cons->removeAllObjects();
    }
    if(store_categories != NULL)
    {
        store_categories->removeAllObjects();
    }
    CC_SAFE_RELEASE(review_pros);
    CC_SAFE_RELEASE(review_cons);
    CC_SAFE_RELEASE(store_categories);
}

PNAppDetailsModel* PNAppDetailsModel::createWithJSON(std::string json)
{
    PNAppDetailsModel *result = NULL;
 
    rapidjson::Document document = parseJson(json);
    if(!document.HasParseError())
    {
        result = new PNAppDetailsModel();
        if(result != NULL)
        {
            result->name                = processValueString(document["name"]);
            result->publisher           = processValueString(document["publisher"]);
            result->developer           = processValueString(document["developer"]);
            result->store_rating        = processValueDouble(document["store_rating"]);
            result->category            = processValueString(document["category"]);
            result->platform            = processValueString(document["platform"]);
            result->review              = processValueString(document["review"]);
            result->review_url          = processValueString(document["review_url"]);
            result->review_pros         = getStringArray(document["review_pros"]);
            result->review_pros->retain();
            result->review_cons         = getStringArray(document["review_cons"]);
            result->review_cons->retain();
            result->version             = processValueString(document["version"]);
            result->size                = processValueString(document["size"]);
            result->age_rating          = processValueString(document["age_rating"]);
            result->store_description   = processValueString(document["store_description"]);
            result->store_url           = processValueString(document["store_url"]);
            result->release_date        = processValueString(document["release_date"]);
            result->total_ratings       = processValueDouble(document["total_ratings"]);
            result->installs            = processValueString(document["installs"]);
            result->store_categories    = getStringArray(document["store_categories"]);
            result->store_categories->retain();
            result->sub_category        = processValueString(document["sub_category"]);
            result->store_id            = processValueString(document["store_id"]);
            result->url_scheme          = processValueString(document["url_scheme"]);
            result->autorelease();
        }
    }
    
    return result;
}