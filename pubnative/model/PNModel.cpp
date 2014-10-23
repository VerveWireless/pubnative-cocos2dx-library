//
// PNModel.cpp
//
// Created by David Martin on 20/10/14.
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

#include "PNModel.h"
#include "stringbuffer.h"
#include "writer.h"
#include "HttpClient.h"

using namespace std;
using namespace cocos2d;
using namespace cocos2d::extension;
using namespace cocos2d::pubnative;

PNModel::PNModel(void)
: CCObject()
{
    _isReady = false;
    
    _callback_selector_ready = NULL;
    _callback_target_ready = NULL;
}

PNModel::~PNModel(void)
{
    CC_SAFE_RELEASE(_callback_target_ready);
}

void PNModel::setReadyCallback(cocos2d::CCObject *target, Pubnative_Callback_Ad_Ready selector)
{
    if(_callback_target_ready)
    {
        _callback_target_ready->autorelease();
    }
    _callback_target_ready = target;
    _callback_selector_ready = selector;
    if(_callback_target_ready)
    {
        _callback_target_ready->retain();
    }
}

rapidjson::Document PNModel::parseJson(std::string json)
{
    rapidjson::StringStream stream(json.c_str());
    rapidjson::Document document;
    document.ParseStream<rapidjson::kParseDefaultFlags>(stream);
    
    if (document.HasParseError())
    {
        CCLOG("PNModel::parseJson - ERROR -> %s", document.GetParseError());
    }
    
    return document;
}

string PNModel::getJSON(rapidjson::Value &value)
{
    string result = "";
    
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    value.Accept(writer);
    
    result = buffer.GetString();
    
    return result;
}

int PNModel::processValueInt (rapidjson::Value &value)
{
    int result = 0;
    if (!value.IsNull() && value.IsInt())
    {
        result = value.GetInt();
    }
    return result;
}

double PNModel::processValueDouble (rapidjson::Value &value)
{
    double result = 0;
    if (!value.IsNull() && value.IsDouble())
    {
        result = value.GetDouble();
    }
    return result;
}

string PNModel::processValueString (rapidjson::Value &value)
{
    string result = "";
    if (!value.IsNull() && value.IsString())
    {
        result = value.GetString();
    }
    return result;
}

CCArray* PNModel::getStringArray(rapidjson::Value& stringArray)
{
    CCArray *result = CCArray::create();
    
    for (int i = 0; i < stringArray.Size();i++)
    {
        string item = stringArray[rapidjson::SizeType(i)].GetString();
        result->addObject(CCStringMake(item));
    }
    
    return result;
}

void PNModel::invokeReadyCallback()
{
    invokeCallback(true);
}

void PNModel::invokeFailCallback()
{
    invokeCallback(false);
}

void PNModel::invokeCallback(bool success)
{
    if (_callback_target_ready && _callback_selector_ready)
    {
        (_callback_target_ready->*_callback_selector_ready)(this, success);
    }
}

void PNModel::downloadImage(std::string url, std::string name)
{
    CCHttpRequest *request = new CCHttpRequest();
    request->setUrl(url.c_str());
    request->setRequestType(CCHttpRequest::kHttpGet);
    request->setResponseCallback(this, httpresponse_selector(PNModel::onImageRequestFinished));
    
    CCString *nameString = CCStringMake(name);
    nameString->retain();
    request->setUserData(nameString);
    
    CCHttpClient::getInstance()->send(request);
    
    request->release();
}

void PNModel::onImageRequestFinished(CCHttpClient* client, CCHttpResponse* response)
{
    if (!response)
    {
        onImageDownloaded(NULL, "");
        return;
    }
    
    CCString *imageName = (CCString*)response->getHttpRequest()->getUserData();
    std::string nameString = imageName->m_sString;
    imageName->release();
    
    if (!response->isSucceed())
    {
        onImageDownloaded(NULL, nameString);
        return;
    }
    
    int statusCode = response->getResponseCode();
    
    if(statusCode == 200)
    {
        if (!response->isSucceed())
        {
            onImageDownloaded(NULL, nameString);
            return;
        }
        
        std::vector<char>*buffer = response->getResponseData();
        
        CCImage *downloadedImage = new CCImage();
        
        if(downloadedImage != NULL)
        {
            downloadedImage->initWithImageData(&(buffer->front()), buffer->size());
        }
        onImageDownloaded(downloadedImage, nameString);
        downloadedImage->release();
    }
}

