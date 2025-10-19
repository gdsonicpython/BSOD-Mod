#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class Random : public CCLayer {
public:
	static Random* create();
	static void addRandomizer();
	void showAlert(float dt);

private:
	bool init();
	bool randombool(int n);
};
