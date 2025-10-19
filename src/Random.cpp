#include "Random.hpp"
#include "BSODLayer.hpp"
#include <cstdlib>
#include <ctime>
#include <iostream>

Random* Random::create() {
	auto ret = new Random();
	if (ret && ret->init()) {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_DELETE(ret);
	return nullptr;
}

void Random::addRandomizer() {
	auto sharedDir = CCDirector::sharedDirector();
	auto winSize = sharedDir->getWinSize();

	auto randomizer = Random::create();
	randomizer->setID("Randomizer"_spr);
	sharedDir->getRunningScene()->addChild(randomizer);
	SceneManager::get()->keepAcrossScenes(randomizer);
}

bool Random::init() {
    this->setTouchEnabled(true);

    this->schedule(schedule_selector(Random::showAlert), 1.0f);

    return true;
}

void Random::showAlert(float dt) {
	bool bluescreenbool = randombool(Mod::get()->template getSettingValue<int>("BSOD-rate"));
	if (bluescreenbool) {
		auto BSODLayer = BSODLayer::scene();
		CCDirector::sharedDirector()->pushScene(BSODLayer);
	}
}

bool Random::randombool(int n) {
    if (n <= 0) return false;

    int num = rand() % n + 1;
    return num == n;
}