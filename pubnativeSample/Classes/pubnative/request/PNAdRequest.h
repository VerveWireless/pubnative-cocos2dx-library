//
// PNAdRequest.h
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

#ifndef __PUBNATIVE_PNAdRequest__
#define __PUBNATIVE_PNAdRequest__

#include <stdio.h>
#include "cocos2d.h"
#include "PNAdRequestData.h"
#include "PNModel.h"
#include "HttpClient.h"

namespace cocos2d
{
    namespace pubnative
    {
        class PNAdRequest;
        
        typedef void (CCObject::*Pubnative_Ready_Callback)(PNAdRequest *request, bool success);
        #define pubnative_callback_ready(_SELECTOR) (Pubnative_Ready_Callback)(&_SELECTOR)
        
        class PNAdRequest : public CCObject
        {
            
        public:
            CCArray* getAds();
            void setReadyCallback(CCObject *pTarget, Pubnative_Ready_Callback pSelector);
            void setURLParameter(std::string key, std::string value);
            void requestAds(void);
            
        protected:
            PNAdRequest(void);
            ~PNAdRequest(void);
            
            typedef enum
            {
                PNAdRequestType_Native,
                PNAdRequestType_Image
            }PNAdRequestType;
            PNAdRequestType _api;
            CCArray         *_ads;
            
            virtual PNModel* parseAd(std::string adJSON){return NULL;}
            
            void invokeReadyCallback();
            void invokeFailCallback(std::string description);
            
        private:
            PNAdRequestData             *_requestData;
            CCObject                    *_callback_target_ready;
            Pubnative_Ready_Callback    _callback_selector_ready;
            
            void onAdRequestFinished(cocos2d::extension::CCHttpClient* client, cocos2d::extension::CCHttpResponse* response);
            void invokeCallback(bool success);
        };
        
    }
}

#endif