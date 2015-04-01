![Pubnative](/docs/PNLogo.png)

PubNative is an API-based publisher platform dedicated to native advertising which does not require the integration of an SDK. Through PubNative, publishers can request over 20 parameters to enrich their ads and thereby create any number of combinations for unique and truly native ad units.

# Pubnative library

Pubnative library is a collection of Open Source tools, to implement API based native ads in Unity.

## Contents

* [Requisites](#requisites)
* [Install](#install)
    * [Sources](#install_sources)
    * [Android](#install_android)
    * [iOS](#install_ios)
* [Usage](#usage)
    * [Request](#usage_request)
    * [Load](#usage_load)
    * [Use](#usage_use) 
* [Misc](#misc)
    * [Author](#misc_author)
    * [License](#misc_license)
    * [Contribution](#misc_contribution)

<a name='requisites'></a>
## Requisites

* Cocos2D-X v2.X
* **Warning**: Still not compatible with Cocos2D-X 3.X

<a name='install'></a> 
## Install

<a name='install_sources'></a> 
### Source files

1. Download the Pubnative repository
2. Copy the pubnative/ folder into your project

<a name='install_android'></a>
### Android

Once you imported the files you should edit the Android.mk file

* **please note that you should point wherever you have imported your pubnative copy

```text
LOCAL_SRC_FILES := <YOUR_FILES> \
                   ../../Classes/pubnative/model/PNAppDetailsModel.cpp \
                   ../../Classes/pubnative/model/PNBeaconsModel.cpp \
                   ../../Classes/pubnative/model/PNModel.cpp \
                   ../../Classes/pubnative/model/PNNativeAdModel.cpp \
                   ../../Classes/pubnative/request/PNAdRequest.cpp \
                   ../../Classes/pubnative/request/PNAdRequestData.cpp \
                   ../../Classes/pubnative/request/PNNativeRequest.cpp
```

```text
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes/pubnative/model
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes/pubnative/request
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../../extensions/CocoStudio/Json/rapidjson
```

<a name='install_ios'></a>
### iOS

No further installation steps are necessary for iOS

<a name='usage'></a>
## Usage

* Include the needed classes for your request

```cpp
#include "PNNativeRequest.h"
#include "PNNativeAdModel.h"
```

All classes are described under the following namespace

```cpp
using namespace cocos2d::pubnative;
```

<a name='usage_request'></a>
### Request

* Create and fill a request for the API using `PNNativeRequest` class.

Remember that you should set URLParameters as described in the [pubnative wiki](https://pubnative.atlassian.net/wiki/display/PUB/API+Documentation#APIDocumentation-3.Request)

**This will download raw data from the API**

```cpp
PNNativeRequest *request = cocos2d::pubnative::PNNativeRequest::create();
request->setURLParameter("app_token", <PUBNATIVE_APP_TOKEN>);
request->setURLParameter("bundle_id", <APP_BUNDLE_ID>);
request->setURLParameter("os", <OS>);
request->setURLParameter("os_version", <OS_VERSION>);
request->setURLParameter("device_model", <DEVICE_MODEL>);
request->setReadyCallback(<CALLBACK_TARGET>, pubnative_callback_ready(<YOUR_CLASS>::<REQUEST_READY_CALLBACK>));
request->requestAds();
```

<a name='usage_load'></a>
### Load

You need to load manually each downloded ad resources, so you can control when the images are downloaded from the URL into `CCImage`.

**This will load the resouces from the model into Cocos2D types**

```cpp
void YOUR_CLASS::REQUEST_READY_CALLBACK(cocos2d::pubnative::PNAdRequest *request, bool success)
{
    if(success)
    {
        CCArray *ads = request->getAds();
        if (ads->count() > 0)
        {
            PNModel *model = (PNModel*)ads->objectAtIndex(0);
            model->setReadyCallback(this, pubnative_callback_ad_ready(<YOUR_CLASS>::<AD_READY_CALLBACK>));
            model->load();
        }
    }
}
```

<a name='usage_use'></a>
### Use 

* Cast the returned object to your model `PNNativeAdModel` 
* Confirm the impression beacon.

```cpp
void YOUR_CLASS::AD_READY_CALLBACK(PNModel *model, bool success)
{
    if (success && model != NULL)
    {
		PNNativeAdModel *adModel = dynamic_cast<PNNativeAdModel*>(model);
		
		// TODO: Present the ad in the scene
		
		adModel->beacons->confirmBeacon("impression");
	 }
}
```

<a name='misc'></a>
## Misc

<a name='misc_author'></a>
### Author

David Martin [@cerberillo](http://www.github.com/cerberillo)

<a name='misc_license'></a>
### License

This code is distributed under the terms and conditions of the MIT license. 

<a name='misc_contribution'></a>
### Contribution guidelines

If you fix a bug you discovered or have development ideas, feel free to make a pull request.

