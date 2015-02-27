//
// PNAdRequestData.cpp
//
// Created by David Martin on 17/10/14.
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

#include "PNAdRequestData.h"

using namespace std;
using namespace cocos2d;
using namespace cocos2d::pubnative;

static string PNAdRequest_SERVER_URL                      = "http://api.pubnative.net/api/partner/v2/promotions/";

/*
 REQUIRED
 ---
 app_token;
 bundle_id;
 os;
 os_version;
 device_model;
 
 OPTIONAL
 ---
 zone_id;
 ad_count;
 user_agent;
 ip;
 locale;
 icon_size;
 banner_size;
 portrait_banner_size;
 device_resolution;
 device_type;
 latitude;
 longitude;
 gender;
 age;
 keywords;
 partner;
 apple_idfa;
 android_id;
 android_imei;
 android_advertiser_id;
 mac_address;
 */

PNAdRequestData::PNAdRequestData() : CCObject()
{
    _parameters = NULL;
}

PNAdRequestData::~PNAdRequestData()
{
    if(_parameters != NULL)
    {
        _parameters->removeAllObjects();
    }
    
    CC_SAFE_RELEASE(_parameters);
}

bool PNAdRequestData::isValid()
{
    bool result = false;
    
    CCString *app_token     = (CCString*)_parameters->objectForKey("app_token");
    CCString *bundle_id     = (CCString*)_parameters->objectForKey("bundle_id");
    CCString *os            = (CCString*)_parameters->objectForKey("os");
    CCString *os_version    = (CCString*)_parameters->objectForKey("os_version");
    CCString *device_model  = (CCString*)_parameters->objectForKey("device_model");
    
    if(app_token && bundle_id && os && os_version && device_model)
    {
        result = true;
    }
    
    return result;
}

void PNAdRequestData::encondeParameter(std::string key, std::string value)
{
//    CCString *sha1Key = CCString::createWithFormat("%s_sha1", key.c_str());
//    CCString *sha1Value = NULL;
//    //TODO: Calculate SHA1
//    setRequestParameter(sha1Key->getCString(), sha1Value->getCString());
    
    
//    CCString *md5Key = CCString::createWithFormat("%s_md5", key.c_str());
//    CCString *md5Value = NULL;
//    //TODO: Calculate MD5 & uncomment
//    setRequestParameter(md5Key->getCString(), md5Value->getCString());
    
   CCLOG("TODO -> void PNAdRequestData::encondeParameter(std::string key, std::string value)");
}

void PNAdRequestData::setRequestParameter(std::string key, std::string value)
{
    if(_parameters == NULL)
    {
        _parameters = CCDictionary::create();
        _parameters->retain();
    }
    
    _parameters->setObject(CCStringMake(value), key);
    
    if("apple_idfa"             ==  key ||
       "android_id"             ==  key ||
       "android_imei"           ==  key ||
       "android_advertiser_id"  ==  key ||
       "mac_address"            ==  key)
    {
        encondeParameter(key, value);
    }
}

std::string PNAdRequestData::parametersString(std::string api)
{
    CCString *result = CCStringMake(string());
    
    if(_parameters != NULL && isValid())
    {
        CCString *user_id = NULL;
        if(CC_PLATFORM_IOS == CC_TARGET_PLATFORM)
        {
            user_id = (CCString*)_parameters->objectForKey("apple_idfa");
        }
        else if(CC_PLATFORM_ANDROID == CC_TARGET_PLATFORM)
        {
            user_id = (CCString*)_parameters->objectForKey("android_id");
        }
        
        if(user_id == NULL)
        {
            this->setRequestParameter("no_user_id", "1");
        }
        
        bool firstParameter = true;
        CCString *urlParameters = CCString::createWithFormat("%s%s?", PNAdRequest_SERVER_URL.c_str(), api.c_str());
        CCDictElement* pElement = NULL;
        CCDICT_FOREACH(_parameters, pElement)
        {
            std::string key = pElement->getStrKey();
            CCString *value = (CCString*)pElement->getObject();
            
            CCString *parameterString = CCString::createWithFormat("%s=%s", key.c_str(), value->getCString());
            
            string format = "%s&%s";
            if(firstParameter)
            {
                firstParameter = false;
                format = "%s%s";
            }
            
            urlParameters = CCString::createWithFormat(format.c_str(), urlParameters->getCString(), parameterString->getCString());
        }
        result = urlParameters;
    }
    else
    {
        CCLOG("ERROR - PNAdRequestData::parametersString(std::string api) : PARAMETERS INVALID");
    }
    
    return result->getCString();
}


