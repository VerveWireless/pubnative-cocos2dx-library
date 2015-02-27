//
// PNAdRequest.cpp
//
// Created by David Martin on 16/10/14.
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

#include "PNAdRequest.h"
#include "PNNativeAdModel.h"
#include "PNImageAdModel.h"
#include "document.h"
#include "stringbuffer.h"
#include "writer.h"

using namespace std;
using namespace cocos2d;
using namespace cocos2d::extension;
using namespace cocos2d::pubnative;

string const kPNAdRequestApiNative    = "native";
string const kPNAdRequestApiImage     = "image";

PNAdRequest::PNAdRequest()
: CCObject()
{
    _ads = CCArray::create();
    _ads->retain();
    _requestData = NULL;
    
    _callback_target_ready      = NULL;
    _callback_selector_ready    = NULL;
}

PNAdRequest::~PNAdRequest(void)
{
    if(_ads)
    {
        _ads->removeAllObjects();
    }
    CC_SAFE_RELEASE(_ads);
    CC_SAFE_RELEASE(_requestData);
    CC_SAFE_RELEASE(_callback_target_ready);
}

CCArray* PNAdRequest::getAds()
{
    return _ads;
}

void PNAdRequest::setURLParameter(string key, string value)
{
    if(_requestData == NULL)
    {
        _requestData = new PNAdRequestData();
    }
    
    _requestData->setRequestParameter(key, value);
}

void PNAdRequest::setReadyCallback(CCObject *pTarget, Pubnative_Ready_Callback pSelector)
{
    if(_callback_target_ready)
    {
        CC_SAFE_RELEASE(_callback_target_ready);
    }
    _callback_target_ready = pTarget;
    _callback_selector_ready = pSelector;
    if(_callback_target_ready)
    {
        _callback_target_ready->retain();
    }
}

void PNAdRequest::requestAds(void)
{
    std::string api = string();
    
    switch (_api)
    {
        case PNAdRequestType_Native:  api = kPNAdRequestApiNative; break;
        case PNAdRequestType_Image:   api = kPNAdRequestApiImage;  break;
    }
    
    std::string url = _requestData->parametersString(api);
    
    if(url.empty() == false)
    {
        CCHttpRequest *request = new CCHttpRequest();
        request->setUrl(url.c_str());
        request->setRequestType(CCHttpRequest::kHttpGet);
        request->setResponseCallback(this, httpresponse_selector(PNAdRequest::onAdRequestFinished));
        request->setUserData(CCStringMake(api));
        CCHttpClient::getInstance()->send(request);
        request->release();
    }
    else
    {
        invokeFailCallback("PNAdRequest::requestAds - URL Error, cannot make request");
    }
}

void PNAdRequest::invokeFailCallback(string description)
{
    string errorString = "Pubnative ERROR - " + description;
    CCLOG("%s", errorString.c_str());
    invokeCallback(false);
}

void PNAdRequest::invokeReadyCallback()
{
    invokeCallback(true);
}

void PNAdRequest::invokeCallback(bool success)
{
    if (_callback_target_ready && _callback_selector_ready)
    {
        (_callback_target_ready->*_callback_selector_ready)(this, success);
    }
}

void PNAdRequest::onAdRequestFinished(CCHttpClient* client, CCHttpResponse* response)
{
    if (!response)
    {
        return;
    }
    
    if (!response->isSucceed())
    {
        invokeFailCallback("PNAdRequest::onAdRequestFinished - Connection to pubnative servers failed");
        return;
    }
    
    int statusCode = response->getResponseCode();
    
    if(statusCode == 200)
    {
        if (!response->isSucceed())
        {
            invokeFailCallback("PNAdRequest::onAdRequestFinished - Network");
            return;
        }
        
        // dump data
        vector<char> *buffer = response->getResponseData();
        string responseString = string(buffer->begin(), buffer->end());
        
        rapidjson::StringStream stream(responseString.c_str());
        rapidjson::Document document;
        document.ParseStream<rapidjson::kParseDefaultFlags>(stream);
        if(document.HasParseError())
        {
            invokeFailCallback("PNAdRequest::onAdRequestFinished - Parse errors: " + string(document.GetParseError()));
        }
        else
        {
            string status = document["status"].GetString();
            if (status == "ok")
            {
                _ads->removeAllObjects();
                for (int i=0;i<document["ads"].Size();i++)
                {
                    rapidjson::StringBuffer buffer;
                    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
                    document["ads"][rapidjson::SizeType(i)].Accept(writer);
                    string adJSON = buffer.GetString();
                    PNModel *ad = parseAd(adJSON);
                    if (ad != NULL)
                    {
                        _ads->addObject(ad);
                    }
                }
                invokeReadyCallback();
            }
            else if ("error" == status)
            {
                // error in request
                string error_message = document["error_message"].GetString();
                invokeFailCallback("PNAdRequest::onAdRequestFinished - Request error: " + error_message);
            }
            else
            {
                // unknown error
                invokeFailCallback("PNAdRequest::onAdRequestFinished - Unkown error");
            }
        }
    }
    else
    {
        invokeFailCallback("PNAdRequest::onAdRequestFinished - Network status code != 200");
    }
}
