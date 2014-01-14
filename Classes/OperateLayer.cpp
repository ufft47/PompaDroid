#include "OperateLayer.h"
#include "OperateDelegate.h"

using namespace cocos2d;
using namespace std;

OperateLayer::OperateLayer():
	m_pJoystick(NULL),
	m_pJoystickBg(NULL)
{

}

OperateLayer::~OperateLayer()
{

}

bool OperateLayer::init()
{
	bool ret = false;
	do {
		CC_BREAK_IF( !Layer::init() );

		m_pJoystick = Sprite::create("joystick.png");
		m_pJoystickBg = Sprite::create("joystick_bg.png");
		this->addChild(m_pJoystick);
		this->addChild(m_pJoystickBg);

		this->hideJoystick();

		auto dispatcher = Director::getInstance()->getEventDispatcher();
		auto listener = EventListenerTouchAllAtOnce::create();
		listener->onTouchesBegan = CC_CALLBACK_2(OperateLayer::onTouchesBegan, this);
		listener->onTouchesMoved = CC_CALLBACK_2(OperateLayer::onTouchesMoved, this);
		listener->onTouchesEnded = CC_CALLBACK_2(OperateLayer::onTouchesEnded, this); 
		dispatcher->addEventListenerWithSceneGraphPriority(listener, this);

		ret = true;

	} while(false);

	return ret;
}

void OperateLayer::showJoystick(Point pos)
{
	m_pJoystick->setPosition(pos);
	m_pJoystickBg->setPosition(pos);

	m_pJoystick->setVisible(true);
	m_pJoystickBg->setVisible(true);
}

void OperateLayer::hideJoystick()
{
	m_pJoystick->setPosition(m_pJoystickBg->getPosition());
	m_pJoystick->setVisible(false);
	m_pJoystickBg->setVisible(false);
}

void OperateLayer::updateJoystick(Point direction, float distance)
{
	Point start = m_pJoystickBg->getPosition();
	if(distance < 32)
	{
		m_pJoystick->setPosition(start + (direction * distance));
	}else if(distance > 96) {
		m_pJoystick->setPosition(start + (direction * 64));
	}else {
		m_pJoystick->setPosition(start + (direction * 32));
	}
}


void OperateLayer::onTouchesBegan(const vector<Touch*>& touches, Event *unused_event)
{
	Size winSize = Director::getInstance()->getWinSize();
	vector<Touch*>::const_iterator touchIter = touches.begin();
	while(touchIter != touches.end())
	{
		Touch *pTouch = (Touch*)(*touchIter);
		Point p = pTouch->getLocation();
		if(p.x <= winSize.width / 2)
		{
			this->showJoystick(p);
		}else {
			m_pOperDelegate->onAttack();
		}

		++ touchIter;
	}
}

void OperateLayer::onTouchesMoved(const vector<Touch*>& touches, Event *unused_event)
{
	Size winSize = Director::getInstance()->getWinSize();
	std::vector<Touch*>::const_iterator touchIter = touches.begin();
	Touch *pTouch = (Touch*)(*touchIter);
	Point start = pTouch->getStartLocation();
	if(start.x > winSize.width / 2)
	{
		return;
	}
	Point dest = pTouch->getLocation();
	float distance = start.getDistance(dest);
	Point direction = (dest - start).normalize();
	this->updateJoystick(direction, distance);

	m_pOperDelegate->onWalk(direction, distance);
}

void OperateLayer::onTouchesEnded(const vector<Touch*>& touches, Event *unused_event)
{
	this->hideJoystick();
	m_pOperDelegate->onStop();
}