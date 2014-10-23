//
// PNAppDetailsModel.h
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

#ifndef __PUBNATIVE_PNAppDetailsModel__
#define __PUBNATIVE_PNAppDetailsModel__

#include <stdio.h>
#include "cocos2d.h"
#include "PNModel.h"
#include "document.h"

using namespace std;

namespace cocos2d
{
    namespace pubnative
    {
        class PNAppDetailsModel : public PNModel
        {
        public:
            
            string name;
            string platform;
            string review;
            string review_url;
            CCArray *review_pros;
            CCArray *review_cons;
            string publisher;
            string developer;
            string version;
            string size;
            string age_rating;
            double store_rating;
            string store_description;
            string store_url;
            string store_id;
            string url_scheme;
            string release_date;
            double total_ratings;
            string installs;
            string category;
            CCArray *store_categories;
            string sub_category;
            
            PNAppDetailsModel(void);
            ~PNAppDetailsModel(void);
            
            static PNAppDetailsModel* createWithJSON(string json);
        protected:
        private:
        };
    }
}

#endif