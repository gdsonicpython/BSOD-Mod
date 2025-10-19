#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <sstream>
#include <iomanip>
#include <random>
#include <Geode/cocos/layers_scenes_transitions_nodes/CCTransition.h>
#include <Geode/cocos/platform/win32/CCApplication.h>
#include <Geode/cocos/include/cocos2d.h>
#include <Geode/ui/TextInput.hpp>
#include <Geode/cocos/sprite_nodes/CCSprite.h>
#include <Geode/loader/Mod.hpp>
#include <Geode/utils/general.hpp>
USING_NS_CC;

#include "BSODLayer.hpp"

bool m_leftMouseDown = false;

BSODLayer* BSODLayer::create() {
    auto ret = new BSODLayer();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
};

CCScene* BSODLayer::scene() {
    auto layer = BSODLayer::create();
    auto scene = CCScene::create();
    scene->addChild(layer);
    return scene;
}

bool BSODLayer::init() {
    if(!CCLayer::init())
        return false;

    setID("BSODLayer");

    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    int percent = 0;

    CCSprite* iconSad = CCSprite::createWithSpriteFrameName("BSODIcon.png"_spr);
    iconSad->setID("sad-icon");
    iconSad->setScale(1.4);
    iconSad->setRotation(-90.f);
    iconSad->setPosition({(0-winSize.width)*0.3305613305f, winSize.height*0.30625f});

    auto main_text = CCLabelBMFont::create("Your GD ran into a problem and needs to restart. We're\n not collecting error info, but then we'll restart for\n you.", "MyFont.fnt"_spr);
    main_text->setScale(0.625);
    main_text->setPosition({(0-winSize.width)*0.0625f, winSize.height*0.109375f});

    std::string percentLbl = std::to_string(percent) + "% complete.";

    auto percent_text = CCLabelBMFont::create(percentLbl.c_str(), "MyFont.fnt"_spr);
    percent_text->setID("percent-text");
    percent_text->setScale(0.625);
    percent_text->setPosition({(0-winSize.width)*0.2916666665f, (0-winSize.height)*0.05f});

    CCSprite* qr = CCSprite::createWithSpriteFrameName("qr.png"_spr);
    qr->setID("qr");
    qr->setScale(0.44);
    qr->setPosition({(0-winSize.width)*0.327708333f, (0-winSize.height)*0.1984375f});

    auto web_text = CCLabelBMFont::create("For no information about this non-issue and no possible fixes, visit out website.", "MyFont.fnt"_spr);
    web_text->setScale(0.3);
    web_text->setPosition({(0-winSize.width)*0.05f, (0-winSize.height)*0.134375f});

    auto talk_text = CCLabelBMFont::create("If you call a support person, give them this info:", "MyFont.fnt"_spr);
    talk_text->setScale(0.3);
    talk_text->setPosition({(0-winSize.width)*0.134375f, (0-winSize.height)*0.2275f});

    auto stopText = std::string("Stop Code: 0x") + generateHex();

    auto stop_code = CCLabelBMFont::create(stopText.c_str(), "MyFont.fnt"_spr);
    stop_code->setScale(0.3);
    stop_code->setPosition({(0-winSize.width)*0.199479167f, (0-winSize.height)*0.259375f});

    CCMenu* iconMenu = CCMenu::create();
    iconMenu->setID("icon-menu");
    iconMenu->addChild(iconSad);
    addChild(iconMenu, 1);
    iconMenu->addChild(main_text);
    iconMenu->addChild(percent_text);
    iconMenu->addChild(qr);
    iconMenu->addChild(web_text);
    iconMenu->addChild(talk_text);
    iconMenu->addChild(stop_code);

    m_background = CCLayerColor::create({53, 126, 199, 255});
    m_background->setID("background");
    m_background->setAnchorPoint({ 0.f, 0.f });
    m_background->setContentSize(CCDirector::get()->getWinSize());
    addChild(m_background, -3);


    //what sorcery did i do to make this work
    auto updatePercent = std::make_shared<std::function<void(int)>>();

    *updatePercent = [percent_text, this, updatePercent](int percent) {
        if (percent > 105) {
            game::restart();
            return;
        }

        int percentCopy = percent;
        float delayTime = this->randomFloat();

        percent_text->runAction(CCSequence::create(
            CCDelayTime::create(delayTime),


            CallFuncExt::create([percent_text, percentCopy]() {
                if (percent_text) {
                    std::string percentLbl = std::to_string(percentCopy) + "% complete.";
                    percent_text->setString(percentLbl.c_str());
                }
            }),

            CallFuncExt::create([updatePercent, percent]() {
                (*updatePercent)(percent + 5);
            }),

            nullptr
        ));
    };

    (*updatePercent)(0);

    return true;
}

std::string BSODLayer::generateHex() {
    std::stringstream ss;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<unsigned int> dis(0, 0xFFFFFFFF);

    ss << std::uppercase << std::setfill('0') << std::setw(8) << std::hex << dis(gen);
    return ss.str();
}

float BSODLayer::randomFloat() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.5f, 2.5f);

    float num = dis(gen);

    // Round to 1 decimal place
    num = std::round(num * 10.0f) / 10.0f;

    return num;
}