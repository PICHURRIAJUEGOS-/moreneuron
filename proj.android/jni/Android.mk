LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := cocos2dcpp_shared

LOCAL_MODULE_FILENAME := libcocos2dcpp

LOCAL_SRC_FILES := hellocpp/main.cpp \
                   ../../Classes/AppDelegate.cpp \
	           ../../Classes/PresentationScene.cpp \
                   ../../Classes/NeuronMenuScene.cpp \
                   ../../Classes/MoreNeuronScene.cpp \
                   ../../Classes/Crux.cpp \
                   ../../Classes/CruxBoard.cpp \
		   ../../Classes/HelpScene.cpp \
		   ../../Classes/MoreNeuronBG.cpp \
		   ../../Classes/Ads.cpp

LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../Classes $(LOCAL_PATH)/../../Classes/TestAds

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static \
PluginProtocolStatic 
LOCAL_WHOLE_STATIC_LIBRARIES += cocosdenshion_static


include $(BUILD_SHARED_LIBRARY)

$(call import-module,2d)
$(call import-module,audio/android)

$(call import-module,protocols/android)

