#ifndef CONFIG_H
#define CONFIG_H

#include "cocos2d.h"
#include "ProtocolAds.h"
#include "PluginManager.h"

//Ads
#define ADMOB_ID_ANDROID        "ca-app-pub-6261045080177785/1554038152"
#define ADMOB_ID_ANDROID_MENU   "ca-app-pub-6261045080177785/7114390552"

class NeuroAdsListener : public cocos2d::plugin::AdsListener {
  //Ads
  virtual void onAdsResult(cocos2d::plugin::AdsResultCode code, const char* msg) {}
  virtual void onPlayerGetPoints(cocos2d::plugin::ProtocolAds* pAdsPlugin, int points) {}
};

//Ads
extern cocos2d::plugin::ProtocolAds* Gadmob;
extern cocos2d::plugin::ProtocolAds::AdsPos GposAdmob;
extern cocos2d::plugin::TAdsInfo GadInfo;
extern cocos2d::plugin::AdsListener* Glistener;
extern bool BigScreen;
#define ADMOB_NEURON_LOAD if(!Gadmob){cocos2d::plugin::TAdsDeveloperInfo devInfo; \
  devInfo["AdmobID"] = ADMOB_ID_ANDROID_MENU; \
  Glistener = new NeuroAdsListener(); \
  Gadmob = dynamic_cast<cocos2d::plugin::ProtocolAds*>(cocos2d::plugin::PluginManager::getInstance()->loadPlugin("AdsAdmob")); \
  GposAdmob = cocos2d::plugin::ProtocolAds::kPosTopLeft;			\
  Gadmob->configDeveloperInfo(devInfo); \
  Gadmob->setAdsListener(Glistener); \
  Gadmob->setDebugMode(false); \
  GadInfo["AdmobType"] = "1"; \
  GadInfo["AdmobSizeEnum"] = "1"; \
  Gadmob->showAds(GadInfo, GposAdmob);}else{Gadmob->showAds(GadInfo, GposAdmob);}	

#define ADMON_NEURON_TOP GposAdmob = cocos2d::plugin::ProtocolAds::kPosTop;
#define ADMON_NEURON_BOTTOM GposAdmob = cocos2d::plugin::ProtocolAds::kPosTop;

#define ADMOB_NEURON_SHOW if (Gadmob != NULL) \
    { Gadmob->showAds(GadInfo);}
#define ADMOB_NEURON_SHOW_TOP if (Gadmob != NULL) \
    { Gadmob->showAds(GadInfo, cocos2d::plugin::ProtocolAds::kPosTop);}
#define ADMOB_NEURON_SHOW_BOTTOM if (Gadmob != NULL) \
    { Gadmob->showAds(GadInfo, cocos2d::plugin::ProtocolAds::kPosBottom);}

#define ADMOB_NEURON_HIDE if (Gadmob != NULL) \
    { Gadmob->hideAds(GadInfo);}

#define ADMOB_NEURON_UNLOAD if (Gadmob != NULL) \
    { \
      Gadmob->hideAds(GadInfo); \
      cocos2d::plugin::PluginManager::getInstance()->unloadPlugin("AdsAdmob"); \
      Gadmob = NULL; \
    } \
  if(Glistener){ delete Glistener; Glistener = NULL;}

#endif
