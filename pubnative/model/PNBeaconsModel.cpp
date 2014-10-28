//
// PNBeaconsModel.cpp
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

#include "PNBeaconsModel.h"
#include "document.h"

using namespace std;
using namespace cocos2d;
using namespace cocos2d::extension;
using namespace cocos2d::pubnative;

PNBeaconsModel::PNBeaconsModel(void) : PNModel()
{
    beacons = CCDictionary::create();
    beacons->retain();
}

PNBeaconsModel::~PNBeaconsModel(void)
{
    if(beacons)
    {
        beacons->removeAllObjects();
    }
    CC_SAFE_RELEASE(beacons);
}

PNBeaconsModel* PNBeaconsModel::createWithJSON(string json)
{
    PNBeaconsModel *beaconsModel = new PNBeaconsModel();
    if(beaconsModel != NULL)
    {
        rapidjson::Document document;
        document.Parse<rapidjson::kParseDefaultFlags>(json.c_str());
        for (int i = 0; i < document.Size(); i++)
        {
            string beaconType   = processValueString(document[rapidjson::SizeType(i)]["type"]);
            string beaconURL    = processValueString(document[rapidjson::SizeType(i)]["url"]);
            beaconsModel->beacons->setObject(CCStringMake(beaconURL), beaconType);
        }
        beaconsModel->autorelease();
    }
    return beaconsModel;
}

string PNBeaconsModel::getBeacon(string type)
{
    string result = string();
    CCString *value = (CCString*)beacons->objectForKey(type);
    if(value != NULL)
    {
        result = value->m_sString;
    }
    return result;
}

void PNBeaconsModel::confirmBeacon(std::string type)
{
    string beaconURL = getBeacon(type);
    if(beaconURL.empty() == false)
    {
        CCHttpRequest *request = new CCHttpRequest();
        request->setUrl(beaconURL.c_str());
        request->setRequestType(CCHttpRequest::kHttpGet);
        request->setResponseCallback(this, httpresponse_selector(PNBeaconsModel::onBeaconConfirmed));
        CCString *typeString = CCStringMake(type);
        typeString->retain();
        request->setUserData(typeString);
        CCHttpClient::getInstance()->send(request);
        request->release();
    }
    else
    {
        CCLOG("PNBeaconsModel - Beacon error - beacon doesn't exist");
    }
}

void PNBeaconsModel::onBeaconConfirmed(cocos2d::extension::CCHttpClient* client, cocos2d::extension::CCHttpResponse* response)
{
    if (!response)
    {
        CCLOG("PNBeaconsModel - Network Error");
        return;
    }
    
    CCString *imageName = (CCString*)response->getHttpRequest()->getUserData();
    string nameString = imageName->m_sString;
    imageName->release();
    
    if (!response->isSucceed())
    {
        CCLOG("PNBeaconsModel - Network Error - Beacon %s", nameString.c_str());
        return;
    }
    
    int statusCode = response->getResponseCode();
    
    if(statusCode == 200)
    {
        if (!response->isSucceed())
        {
            CCLOG("PNBeaconsModel - Server Error - Beacon %s", nameString.c_str());
            return;
        }
        else
        {
            CCLOG("PNBeaconsModel - SUCCESS - Beacon %s", nameString.c_str());
        }
    }
    
    
}

