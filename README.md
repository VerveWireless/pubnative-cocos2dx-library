![Pubnative](/docs/PNLogo.png)

PubNative is an API-based publisher platform dedicated to native advertising which does not require the integration of an SDK. Through PubNative, publishers can request over 20 parameters to enrich their ads and thereby create any number of combinations for unique and truly native ad units.

# Pubnative library

Pubnative library is a collection of Open Source tools, to implement API based native ads in Unity.

## Requisites

* Cocos2D-X v2.2.5 or greater
 
## Install

### Source files

1. Download the Pubnative repository
2. Copy the pubnative/ folder into your project

## Usage

### Import

You would need to import the needed classes for your request

For native API request:
```cpp
#import "PNNativeRequest.h"
#import "PNNativeAdModel.h"
```

For image API request:

```cpp
#import "PNImageRequest.h"
#import "PNImageAdModel.h"
```

All classes are described under the following namespace

```cpp
using namespace cocos2d::pubnative;
```

### API Request

You need to create and fill a request for the native or image API. You could use `PNNativeRequest` or `PNImageRequest`, but the process is the same. 

Remember that you should set URLParameters as described in the [pubnative wiki](https://pubnative.atlassian.net/wiki/display/PUB/API+Documentation#APIDocumentation-3.Request)

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

### Ad Load

You need to load manually each downlaoded ad, so you can control when the images are downloaded from the URL into `CCImage`. This step is the same for both requests `PNNativeRequest` or `PNImageRequest`.

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

### Ad usage

You would need to cast the model returned to your request model `PNNativeAdModel` or `PNImageAdModel`. 

And once the ad has been shown, confirm the "impression" beacon.

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

## Misc

### Author

David Martin [@cerberillo](http://www.github.com/cerberillo)

### License

This code is distributed under the terms and conditions of the MIT license. 

### Contribution guidelines

If you fix a bug you discovered or have development ideas, feel free to make a pull request.

