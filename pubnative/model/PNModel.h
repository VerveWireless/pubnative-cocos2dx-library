//
// PNModel.h
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

#ifndef __PUBNATIVE_PNModel__
#define __PUBNATIVE_PNModel__

#include <stdio.h>
#include "cocos2d.h"
#include "document.h"
#include "HttpClient.h"

namespace cocos2d
{
    namespace pubnative
    {
        class PNModel;
        
        typedef void (CCObject::*Pubnative_Callback_Ad_Ready)(PNModel *model, bool success);
        #define pubnative_callback_ad_ready(_SELECTOR) (Pubnative_Callback_Ad_Ready)(&_SELECTOR)
        
        class PNModel : public CCObject
        {
        public:
            PNModel(void);
            ~PNModel(void);
            
            virtual void load() {}
            
            void setReadyCallback(CCObject *target, Pubnative_Callback_Ad_Ready selector);
        protected:
            
            static rapidjson::Document parseJson(std::string json);
            static int processValueInt (rapidjson::Value &value);
            static double processValueDouble (rapidjson::Value &value);
            static std::string processValueString (rapidjson::Value &value);
            static std::string getJSON(rapidjson::Value &value);
            static CCArray* getStringArray(rapidjson::Value& stringArray);
            
            void downloadImage(std::string url, std::string name);
            void invokeReadyCallback();
            void invokeFailCallback();
            virtual void onImageDownloaded(CCImage *image, std::string name){};
        private:
            bool                        _isReady;
            CCObject                    *_callback_target_ready;
            Pubnative_Callback_Ad_Ready _callback_selector_ready;
            
            void invokeCallback(bool success);
            void onImageRequestFinished(cocos2d::extension::CCHttpClient* client, cocos2d::extension::CCHttpResponse* response);
        };
    }
}

#endif