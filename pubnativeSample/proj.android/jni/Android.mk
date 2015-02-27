LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
                   ../../Classes/HelloWorldScene.cpp \
                   ../../Classes/pubnative/model/PNAppDetailsModel.cpp \
                   ../../Classes/pubnative/model/PNBeaconsModel.cpp \
                   ../../Classes/pubnative/model/PNImageAdModel.cpp \
                   ../../Classes/pubnative/model/PNModel.cpp \
                   ../../Classes/pubnative/model/PNNativeAdModel.cpp \
                   ../../Classes/pubnative/request/PNAdRequest.cpp \
                   ../../Classes/pubnative/request/PNAdRequestData.cpp \
                   ../../Classes/pubnative/request/PNImageRequest.cpp \
                   ../../Classes/pubnative/request/PNNativeRequest.cpp

COCOS2DX_ROOT=$(LOCAL_PATH)/../../cocos2d-x
COCOS2DX_EXT_ROOT=$(COCOS2DX_ROOT)/extensions

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes/pubnative/model
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../Classes/pubnative/request
LOCAL_C_INCLUDES += $(LOCAL_PATH)/../../../../extensions/CocoStudio/Json/rapidjson

LOCAL_WHOLE_STATIC_LIBRARIES += cocos2dx_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static
LOCAL_WHOLE_STATIC_LIBRARIES += box2d_static
LOCAL_WHOLE_STATIC_LIBRARIES += chipmunk_static
LOCAL_WHOLE_STATIC_LIBRARIES += cocos_extension_static

include $(BUILD_SHARED_LIBRARY)

$(call import-module,cocos2dx)
$(call import-module,cocos2dx/platform/third_party/android/prebuilt/libcurl)
$(call import-module,CocosDenshion/android)
$(call import-module,extensions)
$(call import-module,external/Box2D)
$(call import-module,external/chipmunk)
