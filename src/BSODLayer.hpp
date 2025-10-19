#pragma once

using namespace geode::prelude;

class BSODLayer : public cocos2d::CCLayer {
protected:
    CCLayerColor* m_background;
    std::string generateHex();
    float randomFloat();
    //     bool _holding = false;
    // 	static inline BSODLayer* get = nullptr;
public:
    static BSODLayer* create();
    static cocos2d::CCScene* scene();
    bool init();
};