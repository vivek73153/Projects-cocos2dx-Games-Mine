#ifndef ENEMYGENERATOR_H_
#define ENEMYGENERATOR_H_

#include "cocos2d.h"
#include "GameObjects/Enemy.h"

/**
 * This class will generate random opponents (e.g. birds and planes).
 */
class EnemyGenerator :
	public cocos2d::CCNode
{
public:
	EnemyGenerator();
	void init(cocos2d::CCLayer* pLayer);
	void start();
	cocos2d::CCMutableArray<Enemy*>* getEnemies();
	void remove(cocos2d::CCMutableArray<Enemy*>* pEnemies);

private:
	/**
	 * Generated objects will be added as children of this layer.
	 * It should be the MainLayer.
	 */
	cocos2d::CCLayer*	 					  __pLayer;

	/**
	 * Holds generated enemy objects.
	 */
	cocos2d::CCMutableArray<Enemy*>*	  	  __pEnemies;

	/**
	 * Size of the screen.
	 */
	cocos2d::CCSize							  __winSize;

	/**
	 * This value is increased with every new enemy.
	 */
	float									  __enemySpeed;

	void generate(cocos2d::ccTime dt);
};

#endif /* ENEMYGENERATOR_H_ */
