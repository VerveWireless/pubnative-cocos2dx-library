#include "HelloWorldScene.h"

USING_NS_CC;

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
    
    CCLabelTTF* pLabel = CCLabelTTF::create("Hello PUBNATIVE", "Arial", 24);
    pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - pLabel->getContentSize().height));
    this->addChild(pLabel, 1);
    
    this->request_ad();
    
    return true;
}

void HelloWorld::request_ad()
{
    PNNativeRequest *request = PNNativeRequest::create();
    
    if(CC_PLATFORM_IOS == CC_TARGET_PLATFORM)
    {
        request->setURLParameter("app_token", "d26bba3bb7956ab4ed4f7db25393f154e30c13f5a1874e8fc08a21c919cce17b");
        request->setURLParameter("os", "ios");
    }
    else if (CC_PLATFORM_ANDROID == CC_TARGET_PLATFORM)
    {
        request->setURLParameter("app_token", "f0e78ba02145f25a08e8fe846e6e64cdeb8a768b4a1f590ee3071d02bc8de08a");
        request->setURLParameter("os", "android");
    }
    
    request->setURLParameter("bundle_id", "com.pubnative.pubnativeSample"); //SystemUtils::BundleID());
    request->setURLParameter("os_version", "7");
    request->setURLParameter("device_model", "iphone");
    request->setReadyCallback(this, pubnative_callback_ready(HelloWorld::request_ready_callback));
    request->requestAds();
}

void HelloWorld::request_ready_callback(PNAdRequest *request, bool success)
{
    if(success)
    {
        CCArray *ads = request->getAds();
        if (ads->count() > 0)
        {
            PNModel *model = (PNModel*)ads->objectAtIndex(0);
            model->setReadyCallback(this, pubnative_callback_ad_ready(HelloWorld::model_ready_callback));
            model->load();
        }
    }
}

void HelloWorld::model_ready_callback(PNModel *model, bool success)
{
    if (success && model != NULL)
    {
        PNNativeAdModel *adModel = dynamic_cast<PNNativeAdModel*>(model);
        
        CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
        CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();
        
        //=============
        // ADD TITLE
        //=============
        CCLabelTTF* pLabel = CCLabelTTF::create(adModel->title.c_str(), "Arial", 24);
        pLabel->setPosition(ccp(origin.x + visibleSize.width/2,
                                origin.y + visibleSize.height - (pLabel->getContentSize().height * 2)));
        this->addChild(pLabel, 1);
        
        //=============
        //  ADD ICON
        //=============
        CCTexture2D *pTexture = new CCTexture2D();
        pTexture->initWithImage(adModel->icon_url_image);
        CCSprite* pSprite = CCSprite::create();
        pSprite->initWithTexture(pTexture);
        pSprite->setPosition(ccp(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));
        this->addChild(pSprite, 0);
        pTexture->autorelease();
        
        adModel->beacons->confirmBeacon("impression");
    }
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
}
