//  http://www.levelhelper.org
//
//  Created by Bogdan Vladu
//  Copyright 2011 Bogdan Vladu. All rights reserved.
//
////////////////////////////////////////////////////////////////////////////////
//
//  This software is provided 'as-is', without any express or implied
//  warranty.  In no event will the authors be held liable for any damages
//  arising from the use of this software.
//  Permission is granted to anyone to use this software for any purpose,
//  including commercial applications, and to alter it and redistribute it
//  freely, subject to the following restrictions:
//  The origin of this software must not be misrepresented; you must not
//  claim that you wrote the original software. If you use this software
//  in a product, an acknowledgment in the product documentation would be
//  appreciated but is not required.
//  Altered source versions must be plainly marked as such, and must not be
//  misrepresented as being the original software.
//  This notice may not be removed or altered from any source distribution.
//  By "software" the author refers to this code file and not the application 
//  that was used to generate this file.
//  You do not have permission to use this code or any part of it if you don't
//  own a license to LevelHelper application.

#include "LevelHelperLoader.h"

#include "Nodes/LHSettings.h"
#include "Nodes/LHTouchMgr.h"
#include "Nodes/SHDocumentLoader.h"

std::string stringFromInt(const int& i){
    std::stringstream st;
    st << i;
    return st.str();    
}

////////////////////////////////////////////////////////////////////////////////
void LevelHelperLoader::initObjects(void)
{
    contactNode = NULL;
    wb = NULL;
    
    m_isPaused = false;
    
	loadingProgressId= NULL;
	loadingProgressSel= NULL;

    cocosLayer  = NULL;
    box2dWorld = NULL;
    
    LHSettings::sharedInstance()->setLhPtmRatio(32.0f);
}

LevelHelperLoader::LevelHelperLoader(const char* levelFile){

    CCAssert(NULL!=levelFile, "Invalid file given to LevelHelperLoader");
	
    initObjects();
    loadLevelHelperSceneFile(levelFile, "", "");
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

//void LevelHelperLoader::addSpritesToLayer(CCLayer* _cocosLayer)
//{	
//    
//    CCLog("Method addSpritesToLayer is not yet implemented. Please use addObjectsToWorld with all sprites set to NO PHYSICS");
//    /*
//	CCAssert(addObjectsToWordWasUsed!=true, "You can't use method addSpritesToLayer because you already used addObjectToWorld. Only one of the two can be used."); 
//	CCAssert(addSpritesToLayerWasUsed!=true, "You can't use method addSpritesToLayer again. You can only use it once. Create a new LevelHelperLoader object if you want to load the level again."); 
//	
//	addSpritesToLayerWasUsed = true;
//	
//	cocosLayer = _cocosLayer;
//	
//    addBatchNodesToLayer(cocosLayer);
//	
//    createAllAnimationsInfo();
//    
//    //we need to first create the path so we can assign the path to sprite on creation
//    //    for(NSDictionary* bezierDict in lhBeziers)
//    //    {
//    //        //NSString* uniqueName = [bezierDict objectForKey:@"UniqueName"];
//    //        if([[bezierDict objectForKey:@"IsPath"] boolValue])
//    //        {
//    //            [self createBezierPath:bezierDict];
//    //        }
//    //    }
//    
//    
//	for(NSDictionary* dictionary in lhSprites)
//	{
//		NSDictionary* spriteProp = [dictionary objectForKey:@"GeneralProperties"];
//		
//		//find the coresponding batch node for this sprite
//        //LHBatch* bNode = [batchNodesInLevel objectForKey:[spriteProp objectForKey:@"Image"]];
//		//CCSpriteBatchNode *batch = [bNode spriteBatchNode];
//		
//        LHBatch* bNode = [self batchNodeForFile:[spriteProp objectForKey:@"Image"]];
//        
//        if(bNode)
//        {
//            CCSpriteBatchNode *batch = [bNode spriteBatchNode];
//            if(nil != batch)
//            {
//                LHSprite* ccsprite = [self spriteWithBatchFromDictionary:spriteProp batchNode:bNode];
//                if(nil != ccsprite)
//                {
//                    [batch addChild:ccsprite];
//                    [spritesInLevel setObject:ccsprite forKey:[spriteProp objectForKey:@"UniqueName"]];
//                    
//                    [self setCustomAttributesForNonPhysics:spriteProp
//                                                    sprite:ccsprite];
//                }
//                
//                if(![[spriteProp objectForKey:@"PathName"] isEqualToString:@"None"])
//                {
//                    //we have a path we need to follow
//                    [self createPathOnSprite:ccsprite
//                              withProperties:spriteProp];
//                }
//                
//                [self createAnimationFromDictionary:spriteProp onCCSprite:ccsprite];
//            }
//        }
//	}
//    
//    for(NSDictionary* parallaxDict in lhParallax)
//    {
//        //NSMutableDictionary* nodeInfo = [[[NSMutableDictionary alloc] init] autorelease];
//        //       CCNode* node = [self parallaxNodeFromDictionary:parallaxDict layer:cocosLayer];
//        
//        //   if(nil != node)
//        // {
//        //[nodeInfo setObject:[parallaxDict objectForKey:@"ContinuousScrolling"] forKey:@"ContinuousScrolling"];
//        //[//nodeInfo setObject:[parallaxDict objectForKey:@"Speed"] forKey:@"Speed"];
//        //[nodeInfo setObject:[parallaxDict objectForKey:@"Direction"] forKey:@"Direction"];
//        //[nodeInfo setObject:node forKey:@"Node"];
//        //         [ccParallaxInScene setObject:node forKey:[parallaxDict objectForKey:@"UniqueName"]];
//        //}
//    }
//     */
//}
////////////////////////////////////////////////////////////////////////////////
void LevelHelperLoader::addObjectsToWorld(b2World* world, CCLayer* _cocosLayer)
{
//	CCAssert(addSpritesToLayerWasUsed!=true, "You can't use method addObjectsToWorld because you already used addSpritesToLayer. Only one of the two can be used."); 
//	CCAssert(addObjectsToWordWasUsed!=true, "You can't use method addObjectsToWorld again. You can only use it once. Create a new LevelHelperLoader object if you want to load the level again."); 
//	
//	addObjectsToWordWasUsed = true;
	
	cocosLayer = _cocosLayer;
    box2dWorld = world;
    LHSettings::sharedInstance()->setActiveBox2dWorld(world);

    callLoadingProgressObserverWithValue(0.10);
    createAllNodes();
    callLoadingProgressObserverWithValue(0.70f);    
    createAllJoints();
    callLoadingProgressObserverWithValue(0.80f);    
    createParallaxes();
    callLoadingProgressObserverWithValue(0.90f);    
    startAllPaths();
    callLoadingProgressObserverWithValue(1.0f);

    
//    createAllNodes();
    
	
    //order is important
//    addBatchNodesToLayer(cocosLayer);
//    callLoadingProgressObserverWithValue(0.20f);
//    createAllAnimationsInfo();
//    callLoadingProgressObserverWithValue(0.30f);
//    createAllBeziers();
//    callLoadingProgressObserverWithValue(0.50f);
//    createSpritesWithPhysics();
//    createParallaxes();
//    callLoadingProgressObserverWithValue(0.90f);
//    createJoints();
//    callLoadingProgressObserverWithValue(1.00f);
}
//------------------------------------------------------------------------------
void LevelHelperLoader::createAllNodes()
{
    for(int i = 0; i< lhNodes->count(); ++i)
    {
        LHDictionary* dictionary = lhNodes->dictAtIndex(i);

        if(dictionary->stringForKey("NodeType") == "LHLayer")
        {
            LHLayer* layer = LHLayer::layerWithDictionary(dictionary);
            cocosLayer->addChild(layer, layer->getZOrder());
            mainLHLayer = layer;
            mainLHLayer->setIsMainLayer(true);
            //we use the selector protocol so that we dont get warnings since this method is 
            //hidden from the user
            layer->setParentLoader(this);
            LHSettings::sharedInstance()->addLHMainLayer(mainLHLayer);
        }
    }
}
//------------------------------------------------------------------------------
void LevelHelperLoader::createAllJoints(){
    
    for(int i = 0; i < lhJoints->count(); ++i)
	{
        LHDictionary* jointDict = lhJoints->dictAtIndex(i);
        LHJoint* joint = LHJoint::jointWithDictionary(jointDict,box2dWorld,this);
        
        if(joint)
            jointsInLevel.setObject(joint, jointDict->stringForKey("UniqueName"));
	}	
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
LHParallaxNode*  LevelHelperLoader::parallaxNodeFromDictionary(LHDictionary* parallaxDict, CCLayer*layer)
{
	LHParallaxNode* node = LHParallaxNode::nodeWithDictionary(parallaxDict, this);
    
    if(layer != NULL && node != NULL){
        int z = parallaxDict->intForKey("ZOrder");
        layer->addChild(node, z);
    }
    LHArray* spritesInfo = parallaxDict->arrayForKey("Sprites");

    for(int i = 0; i< spritesInfo->count(); ++i)
    {
        LHDictionary* sprInfo = spritesInfo->dictAtIndex(i);

        float ratioX = sprInfo->floatForKey("RatioX");
        float ratioY = sprInfo->floatForKey("RatioY");
        std::string sprName = sprInfo->stringForKey("SpriteName");
        
		LHSprite* spr = spriteWithUniqueName(sprName);
		if(NULL != node && spr != NULL){
			node->addSprite(spr, ccp(ratioX, ratioY));
		}
    }
    return node;
}
//------------------------------------------------------------------------------
void LevelHelperLoader::createParallaxes()
{
    for(int i = 0; i < lhParallax->count(); ++i){
        LHDictionary* parallaxDict = lhParallax->dictAtIndex(i);

		LHParallaxNode* node = parallaxNodeFromDictionary(parallaxDict,cocosLayer);
        if(NULL != node){
            parallaxesInLevel.setObject(node, parallaxDict->stringForKey("UniqueName"));
		}
    }
}
//------------------------------------------------------------------------------
void LevelHelperLoader::startAllPaths(){
    if(!mainLHLayer)return;
    
    CCArray* allSprites = mainLHLayer->allSprites();
    
    for(int i = 0; i< allSprites->count(); ++i)
    {
        LHSprite* spr = (LHSprite*)allSprites->objectAtIndex(i);

        spr->prepareMovementOnPathWithUniqueName(spr->pathUniqueName());
        
        if(spr->pathDefaults.startAtLaunch)
            spr->startPathMovement();
    }
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
LHLayer* LevelHelperLoader::layerWithUniqueName(const std::string& name){

    if(mainLHLayer->getUniqueName() == name)return mainLHLayer;    
    return mainLHLayer->layerWithUniqueName(name);
}
LHBatch* LevelHelperLoader::batchWithUniqueName(const std::string& name){
    return mainLHLayer->batchWithUniqueName(name);
}
LHSprite* LevelHelperLoader::spriteWithUniqueName(const std::string& name){
    return mainLHLayer->spriteWithUniqueName(name);
}
LHBezier* LevelHelperLoader::bezierWithUniqueName(const std::string& name){
    return mainLHLayer->bezierWithUniqueName(name);
}
LHJoint* LevelHelperLoader::jointWithUniqueName(const std::string& name){
    return (LHJoint*)jointsInLevel.objectForKey(name);
}
//------------------------------------------------------------------------------
CCArray* LevelHelperLoader::allLayers(){
#if COCOS2D_VERSION >= 0x00020000
    CCArray* array = CCArray::create();
#else
    CCArray* array = CCArray::array();
#endif
    array->addObject(mainLHLayer);
    array->addObjectsFromArray(mainLHLayer->allLayers());
    return array;
}
CCArray* LevelHelperLoader::allBatches(){
    return mainLHLayer->allBatches();
}
CCArray* LevelHelperLoader::allSprites(){
    return mainLHLayer->allSprites();
}
CCArray* LevelHelperLoader::allBeziers(){
    return mainLHLayer->allBeziers();
}
CCArray* LevelHelperLoader::allJoints(){
    
#if COCOS2D_VERSION >= 0x00020000
    CCArray* keys = jointsInLevel.allKeys();
    CCArray* array = CCArray::create();
    if(keys)
    {
        for(int i = 0; i < (int)keys->count(); ++i){
            array->addObject((LHJoint*)jointsInLevel.objectForKey(((CCString*)keys->objectAtIndex(i))->getCString()));
        }
    }
    return array;
#else
    std::vector<std::string> keys = jointsInLevel.allKeys();
    CCArray* array = CCArray::array();    
    for(int i = 0; i < (int)keys.size(); ++i){
        array->addObject((LHJoint*)jointsInLevel.objectForKey(keys[i]));
    }
    return array;
#endif
}
//------------------------------------------------------------------------------
CCArray* LevelHelperLoader::layersWithTag(enum LevelHelper_TAG tag){
#if COCOS2D_VERSION >= 0x00020000
    CCArray* array = CCArray::create();
#else
    CCArray* array = CCArray::array();
#endif
    if(tag == mainLHLayer->getTag())
        array->addObject(mainLHLayer);
    array->addObjectsFromArray(mainLHLayer->layersWithTag(tag));
    return array;    
}
CCArray* LevelHelperLoader::batchesWithTag(enum LevelHelper_TAG tag){
    return mainLHLayer->batchesWithTag(tag);
}
CCArray* LevelHelperLoader::spritesWithTag(enum LevelHelper_TAG tag){
    return mainLHLayer->spritesWithTag(tag);
}
CCArray* LevelHelperLoader::beziersWithTag(enum LevelHelper_TAG tag){
    return mainLHLayer->beziersWithTag(tag);
}
CCArray* LevelHelperLoader::jointsWithTag(enum LevelHelper_TAG tag){

#if COCOS2D_VERSION >= 0x00020000
    
    CCArray* keys = jointsInLevel.allKeys();
    CCArray* array = CCArray::create();    
    for(int i = 0; i < (int)keys->count(); ++i){
        LHJoint* jt = (LHJoint*)jointsInLevel.objectForKey(((CCString*)keys->objectAtIndex(i))->getCString());
        
        if(jt && jt->getTag() == tag){
            array->addObject(jt);
        }
    }
    return array;
    
#else
    
    std::vector<std::string> keys = jointsInLevel.allKeys();
    CCArray* array = CCArray::array();    
    for(int i = 0; i < (int)keys.size(); ++i){
        LHJoint* jt = (LHJoint*)jointsInLevel.objectForKey(keys[i]);
        
        if(jt && jt->getTag() == tag){
            array->addObject(jt);
        }
    }
    return array;
    
#endif
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void LevelHelperLoader::registerLoadingProgressObserver(CCObject* loadingProgressObj, SEL_CallFuncFloat sel){
    loadingProgressId = loadingProgressObj;
    loadingProgressSel = sel;
}
//------------------------------------------------------------------------------
void LevelHelperLoader::callLoadingProgressObserverWithValue(float val){
    if(NULL != loadingProgressId && NULL != loadingProgressSel){         
		(loadingProgressId->*loadingProgressSel)(val);
    }
}
////------------------------------------------------------------------------------
////------------------------------------------------------------------------------
bool LevelHelperLoader::isPaused(void){
    return LHSettings::sharedInstance()->levelPaused();
}
//------------------------------------------------------------------------------
void LevelHelperLoader::setPaused(bool value){
    LHSettings::sharedInstance()->setLevelPaused(value);    
}


bool LevelHelperLoader::getIsPaused(void){
    return m_isPaused;
}
void LevelHelperLoader::setIsPaused(bool value){
    
    m_isPaused = value;
    
    CCArray* allSprites = this->allSprites();
    
    CCArray* allParallaxes = this->allParallaxes();
    
    for(int i = 0; i < allSprites->count(); ++i){
        
        LHSprite* spr = (LHSprite*)allSprites->objectAtIndex(i);
        spr->pauseAnimation();
        spr->pausePathMovement();
    }
    
    for(int i = 0; i < allParallaxes->count(); ++i){
        LHParallaxNode* node = (LHParallaxNode*)allParallaxes->objectAtIndex(i);
        node->setPaused(value);
    }
}

////------------------------------------------------------------------------------
void LevelHelperLoader::dontStretchArtOnIpad(){
    LHSettings::sharedInstance()->setStretchArt(false);
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void LevelHelperLoader::useLevelHelperCollisionHandling(void)
{
    if(0 == box2dWorld){
        CCLog("LevelHelper WARNING: Please call useLevelHelperCollisionHandling after addObjectsToWorld");
        return;
    }
    
    contactNode = LHContactNode::contactNodeWithWorld(box2dWorld);    
    if(contactNode)
        cocosLayer->addChild(contactNode);
}
//------------------------------------------------------------------------------
void LevelHelperLoader::registerBeginOrEndCollisionCallbackBetweenTagA(enum LevelHelper_TAG tagA,
                                                                       enum LevelHelper_TAG tagB,
                                                                       CCObject* obj,
                                                                       SEL_CallFuncO selector){
    if(NULL == contactNode){
        CCLog("LevelHelper WARNING: Please call registerBeginOrEndCollisionCallbackBetweenTagA after useLevelHelperCollisionHandling");
        return;
    }
    contactNode->registerBeginOrEndCollisionCallbackBetweenTagA(tagA, tagB, obj, selector);

}
//------------------------------------------------------------------------------
void LevelHelperLoader::cancelBeginOrEndCollisionCallbackBetweenTagA(enum LevelHelper_TAG tagA,
                                                                     enum LevelHelper_TAG tagB){
    if(NULL == contactNode){
        CCLog("LevelHelper WARNING: Please call cancelBeginOrEndCollisionCallbackBetweenTagA after useLevelHelperCollisionHandling");
        return;
    }

    contactNode->cancelBeginOrEndCollisionCallbackBetweenTagA((int)tagA, (int)tagB);
}
//------------------------------------------------------------------------------
void LevelHelperLoader::registerPreCollisionCallbackBetweenTagA(enum LevelHelper_TAG tagA,
                                                               enum LevelHelper_TAG tagB,
                                                               CCObject* obj,
                                                               SEL_CallFuncO selector)
{
    if(NULL == contactNode){
        CCLog("LevelHelper WARNING: Please call registerPreColisionCallbackBetweenTagA after useLevelHelperCollisionHandling");
        return;
    }
    contactNode->registerPreCollisionCallbackBetweenTagA(tagA, tagB, obj, selector);
}
//------------------------------------------------------------------------------
void LevelHelperLoader::cancelPreCollisionCallbackBetweenTagA(enum LevelHelper_TAG tagA,
                                                              enum LevelHelper_TAG tagB)
{
    if(NULL == contactNode){
        CCLog("LevelHelper WARNING: Please call registerPreColisionCallbackBetweenTagA after useLevelHelperCollisionHandling");
        return;
    }
    contactNode->cancelPreCollisionCallbackBetweenTagA((int)tagA, (int)tagB);
}
//------------------------------------------------------------------------------
void LevelHelperLoader::registerPostCollisionCallbackBetweenTagA(enum LevelHelper_TAG tagA,
                                                                enum LevelHelper_TAG tagB,
                                                                CCObject* obj,
                                                                SEL_CallFuncO selector)
{
    if(NULL == contactNode){
        CCLog("LevelHelper WARNING: Please call registerPostColisionCallbackBetweenTagA after useLevelHelperCollisionHandling");
        return;
    }
    contactNode->registerPostCollisionCallbackBetweenTagA(tagA, tagB, obj, selector);    
}
//------------------------------------------------------------------------------
void LevelHelperLoader::cancelPostCollisionCallbackBetweenTagA(enum LevelHelper_TAG tagA,
                                                               enum LevelHelper_TAG tagB)
{
    if(NULL == contactNode){
        CCLog("LevelHelper WARNING: Please call registerPreColisionCallbackBetweenTagA after useLevelHelperCollisionHandling");
        return;
    }
    contactNode->cancelPostCollisionCallbackBetweenTagA((int)tagA,(int)tagB);
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
LHDictionary* LevelHelperLoader::dictionaryInfoForSpriteNodeNamed(const std::string& name, LHDictionary* dict)
{
    LHArray* children = dict->arrayForKey("Children");
    
    if(NULL != children)
    {
        for(int i = 0; i< children->count(); ++i)
        {
            LHDictionary* childDict = children->dictAtIndex(i);

            std::string nodeType = childDict->stringForKey("NodeType");
            
            if(nodeType == "LHSprite")
            {
                if(childDict->stringForKey("UniqueName") == name)
                {
                    return childDict;
                }
            }
            else if(nodeType == "LHBatch" ||
                    nodeType == "LHLayer")
            {
                LHDictionary* retDict = dictionaryInfoForSpriteNodeNamed(name, childDict);
                if(retDict)
                    return retDict;
            }
        }
    }
    
    return NULL;
}
//------------------------------------------------------------------------------

//name is from one of a sprite already in the level
//parent will be Main Layer
//if you use custom sprite classes - this will create a sprite of that custom registered class
//method will create custom sprite if one is register for the tag of this sprite
LHSprite* LevelHelperLoader::createSpriteWithUniqueName(const std::string& name){
    return createSpriteWithUniqueName(name, mainLHLayer);
}

//use this method if you want the sprite to be child of a specific node and not the main LH node
//pass nil if you dont want a parent
//method will create custom sprite if one is register for the tag of this sprite
LHSprite* LevelHelperLoader::createSpriteWithUniqueName(const std::string& name, CCNode* parent){
    
    for(int i = 0; i< lhNodes->count(); ++i){
        
        LHDictionary* dictionary = lhNodes->dictAtIndex(i);
                
        LHDictionary* spriteInfo = dictionaryInfoForSpriteNodeNamed(name,dictionary);
        if(spriteInfo){
            
            LHDictionary* texDict = spriteInfo->dictForKey("TextureProperties");
            if(texDict)
            {
                int tag = texDict->intForKey("Tag");
                
                lh_spriteCreationMethods methods = LHCustomSpriteMgr::sharedInstance()->customSpriteClassForTag(tag);
                LHSprite* spr =  (*methods.first)(spriteInfo);

                if(spr){
                    spr->postInit();
                    if(parent)
                        parent->addChild(spr, spr->getZOrder());
                }
                return spr;
            }
        }
    }
    return NULL;
}

//name is from one of a sprite already in the level
//parent will be the batch node that is handling the image file of this sprite
//method will create custom sprite if one is register for the tag of this sprite
LHSprite* LevelHelperLoader::createBatchSpriteWithUniqueName(const std::string& name){

    for(int i = 0; i< lhNodes->count(); ++i){
        LHDictionary* dictionary = lhNodes->dictAtIndex(i);
        LHDictionary* spriteInfo = dictionaryInfoForSpriteNodeNamed(name,dictionary);
        if(spriteInfo){
            
            LHBatch* batch = batchWithUniqueName(spriteInfo->stringForKey("ParentName"));
            if(batch){
                LHDictionary* texDict = spriteInfo->dictForKey("TextureProperties");
                if(texDict)
                {
                    int tag = texDict->intForKey("Tag");

                    lh_spriteCreationMethods methods = LHCustomSpriteMgr::sharedInstance()->customSpriteClassForTag(tag);
                    LHSprite* sprite = (*methods.second)(spriteInfo, batch);
                    if(sprite){
                        batch->addChild(sprite, sprite->getZOrder());
                        sprite->postInit();
                    }
                    return sprite;
                }
            }
        }
    }
    return NULL;
}


LHSprite* LevelHelperLoader::createSpriteWithName(const std::string& name,
                                                  const std::string& shSheetName,
                                                  const std::string& shFileNoExt){
    return createSpriteWithName(name, shSheetName, shFileNoExt, mainLHLayer);
}

//use this method if you want the sprite to be child of a specific node and not the main LH node
//pass nil if you dont want a parent
LHSprite* LevelHelperLoader::createSpriteWithName(const std::string& name,
                                                  const std::string& shSheetName,
                                                  const std::string& shFileNoExt,
                                                  CCNode* parent){
    LHSprite* sprite = LHSprite::spriteWithName(name, shSheetName, shFileNoExt);
    if(sprite){
        if(parent)
            parent->addChild(sprite);
        sprite->postInit();
    }
    return sprite;
}


//use this in order to create sprites of custom types
LHSprite* LevelHelperLoader::createSpriteWithName(const std::string& name,
                                                  const std::string& shSheetName,
                                                  const std::string& shFileNoExt,
                                                  LevelHelper_TAG tag){
    return createSpriteWithName(name, shSheetName, shFileNoExt, tag, mainLHLayer);
}

//use this method if you want the sprite to be child of a specific node and not the main LH node
//pass nil if you dont want a parent
LHSprite* LevelHelperLoader::createSpriteWithName(const std::string& name,
                                                  const std::string& shSheetName,
                                                  const std::string& shFileNoExt,
                                                  LevelHelper_TAG tag,
                                                  CCNode* parent){

    LHDictionary* dictionary = SHDocumentLoader::sharedInstance()->dictionaryForSpriteNamed(name,
                                                                                            shSheetName,
                                                                                            shFileNoExt);
    if(dictionary)
    {
        lh_spriteCreationMethods methods = LHCustomSpriteMgr::sharedInstance()->customSpriteClassForTag(tag);
        
        LHSprite* sprite = (*methods.first)(dictionary);
                
        if(sprite){
            sprite->setTag(tag);
            if(parent){
                parent->addChild(sprite);
            }
            sprite->postInit();
        }
        return sprite;
    }
    return NULL;
}

LHSprite* LevelHelperLoader::createBatchSpriteWithName(const std::string& name,
                                                       const std::string& shSheetName,
                                                       const std::string& shFileNoExt){
    
    LHDictionary* dictionary = SHDocumentLoader::sharedInstance()->dictionaryForSpriteNamed(name,
                                                                                            shSheetName,
                                                                                            shFileNoExt);
    if(dictionary){
        LHBatch* batch = batchWithUniqueName(dictionary->stringForKey("SHSheetName"));
        if(!batch){
            batch = LHBatch::batchWithSheetName(shSheetName, shFileNoExt);
            mainLHLayer->addChild(batch, batch->getZOrder());
        }
        if(batch){
            LHSprite* sprite = LHSprite::batchSpriteWithDictionary(dictionary, batch);
            if(sprite){
                batch->addChild(sprite, sprite->getZOrder());
                sprite->postInit();
            }
            return sprite;
        }
    }
    return NULL;
}

//use this in order to create sprites of custom types
LHSprite* LevelHelperLoader::createBatchSpriteWithName(const std::string& name,
                                                       const std::string& shSheetName,
                                                       const std::string& shFileNoExt,
                                                       LevelHelper_TAG tag){
  
    
    LHDictionary* dictionary = SHDocumentLoader::sharedInstance()->dictionaryForSpriteNamed(name,
                                                                                            shSheetName,
                                                                                            shFileNoExt);
    if(dictionary)
    {
        LHBatch* batch = batchWithUniqueName(dictionary->stringForKey("SHSheetName"));
        if(!batch){
            batch = LHBatch::batchWithSheetName(shSheetName, shFileNoExt);
            mainLHLayer->addChild(batch, batch->getZOrder());
        }
        if(batch){
            lh_spriteCreationMethods methods = LHCustomSpriteMgr::sharedInstance()->customSpriteClassForTag(tag);
            LHSprite* sprite = (*methods.second)(dictionary, batch);
        
            if(sprite){
                sprite->setTag(tag);
                batch->addChild(sprite, sprite->getZOrder());
                sprite->postInit();
            }
            return sprite;
        }
    }
    return NULL;
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
CCSize LevelHelperLoader::gameScreenSize(void){
    return CCSizeMake(safeFrame.x, safeFrame.y);
}
//------------------------------------------------------------------------------
CCRect LevelHelperLoader::gameWorldSize(void)
{
    CCPoint  wbConv = LHSettings::sharedInstance()->convertRatio();
	
    CCRect ws = gameWorldRect;
    
    ws.origin.x *= wbConv.x;
    ws.origin.y *= wbConv.y;
    ws.size.width *= wbConv.x;
    ws.size.height *= wbConv.y;
    
    return ws;
}
////------------------------------------------------------------------------------
LevelHelperLoader::~LevelHelperLoader()
{    
////    releasePhysicBoundaries();
////    removeAllBezierNodes();
////    releaseAllJoints();
////    releaseAllSprites();
//    removeAllParallaxes();
////    releaseAllBatchNodes();
//    
////    delete lhSprites;
//    delete lhJoints;
//    delete lhParallax;
////    delete lhBeziers;
////    delete lhAnims;
////    delete lhBatchInfo;
//    
//    delete wb;
//    
//    if(NULL != contactNode){
//        contactNode->removeFromParentAndCleanup(true);
//    }

//    CCLog("LH DEALLOC");

    delete lhNodes;
    delete lhJoints;
    delete lhParallax;
    delete wb;


    physicBoundariesInLevel.removeAllObjects();
    
    LHTouchMgr::sharedInstance()->removeTouchBeginObserver(cocosLayer);
    LHCuttingEngineMgr::sharedInstance()->destroyAllPrevioslyCutSprites();
    
    parallaxesInLevel.removeAllObjects();
    jointsInLevel.removeAllObjects();
    removeMainLayer();
    

    if(NULL != contactNode){
        contactNode->removeFromParentAndCleanup(true);
    }

}
b2World* LevelHelperLoader::getPhysicsWorld(){
    return box2dWorld;
}

void LevelHelperLoader::removeMainLayer()
{
    LHSettings::sharedInstance()->removeLHMainLayer(mainLHLayer);
    mainLHLayer->removeAllChildrenWithCleanup(true);
    mainLHLayer->removeFromParentAndCleanup(true);
    mainLHLayer = NULL;
}
//
//////////////////////////////////////////////////////////////////////////////////
/////////////////////////////PRIVATE METHODS//////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
////GRAVITY
//////////////////////////////////////////////////////////////////////////////////
bool LevelHelperLoader::isGravityZero(void){
    if(gravity.x == 0 && gravity.y == 0)
        return true;
    return false;
}
//------------------------------------------------------------------------------
void LevelHelperLoader::createGravity(b2World* world)
{
	if(isGravityZero())
		CCLog("LevelHelper Warning: Gravity is not defined in the level. Are you sure you want to set a zero gravity?");
    world->SetGravity(b2Vec2(gravity.x, gravity.y));
}
//////////////////////////////////////////////////////////////////////////////////
////PHYSIC BOUNDARIES
//////////////////////////////////////////////////////////////////////////////////
b2Body* LevelHelperLoader::physicBoundarieForKey(const std::string& key){
    LHNode* spr = (LHNode*)physicBoundariesInLevel.objectForKey(key);
    if(NULL == spr)
        return 0;
    return spr->getBody();
}
//------------------------------------------------------------------------------
b2Body* LevelHelperLoader::leftPhysicBoundary(void){
    return physicBoundarieForKey("LHPhysicBoundarieLeft");
}
LHNode* LevelHelperLoader::leftPhysicBoundaryNode(void){
    return (LHNode*)physicBoundariesInLevel.objectForKey("LHPhysicBoundarieLeft");
}
//------------------------------------------------------------------------------
b2Body* LevelHelperLoader::rightPhysicBoundary(void){
	return physicBoundarieForKey("LHPhysicBoundarieRight");
}
LHNode* LevelHelperLoader::rightPhysicBoundaryNode(void){
    return (LHNode*)physicBoundariesInLevel.objectForKey("LHPhysicBoundarieRight");
}
//------------------------------------------------------------------------------
b2Body* LevelHelperLoader::topPhysicBoundary(void){
    return physicBoundarieForKey("LHPhysicBoundarieTop");
}
LHNode* LevelHelperLoader::topPhysicBoundaryNode(void){
    return (LHNode*)physicBoundariesInLevel.objectForKey("LHPhysicBoundarieTop");
}
//------------------------------------------------------------------------------
b2Body* LevelHelperLoader::bottomPhysicBoundary(void){
    return physicBoundarieForKey("LHPhysicBoundarieBottom");
}
LHNode* LevelHelperLoader::bottomPhysicBoundaryNode(void){
    return (LHNode*)physicBoundariesInLevel.objectForKey("LHPhysicBoundarieBottom");
}
//------------------------------------------------------------------------------
bool LevelHelperLoader::hasPhysicBoundaries(void){
	if(wb == NULL){
		return false;
	}
    CCRect rect = wb->rectForKey("WBRect");
    if(rect.size.width == 0 || rect.size.height == 0)
        return false;
	return true;
}
//------------------------------------------------------------------------------
CCRect LevelHelperLoader::physicBoundariesRect(void){
    CCPoint  wbConv = LHSettings::sharedInstance()->convertRatio();
    CCRect rect = wb->rectForKey("WBRect");
    rect.origin.x = rect.origin.x*wbConv.x,
    rect.origin.y = rect.origin.y*wbConv.y;
    rect.size.width = rect.size.width*wbConv.x;
    rect.size.height= rect.size.height*wbConv.y;
    return rect;
}
//------------------------------------------------------------------------------
void LevelHelperLoader::createPhysicBoundariesNoStretching(b2World * _world){

    if(_world == NULL)
        return;

    CCPoint pos_offset = LHSettings::sharedInstance()->possitionOffset();
    CCPoint  wbConv = LHSettings::sharedInstance()->convertRatio();
    
    createPhysicBoundariesHelper(_world, wbConv, CCPointMake(pos_offset.x/2.0f,
                                                   pos_offset.y/2.0f));
}
//------------------------------------------------------------------------------
void LevelHelperLoader::removePhysicBoundaries()
{
    physicBoundariesInLevel.removeAllObjects();
}
//------------------------------------------------------------------------------
void LevelHelperLoader::createPhysicBoundaries(b2World* _world)
{
    if(_world == NULL)
        return;
    CCPoint  wbConv = LHSettings::sharedInstance()->realConvertRatio();
    createPhysicBoundariesHelper(_world,
                                 wbConv,
                                 CCPointMake(0.0f, 0.0f));
}
////------------------------------------------------------------------------------
void LevelHelperLoader::setFixtureDefPropertiesFromDictionary(LHDictionary* spritePhysic, b2FixtureDef* shapeDef)
{
	shapeDef->density       = spritePhysic->floatForKey("Density");
	shapeDef->friction      = spritePhysic->floatForKey("Friction");
	shapeDef->restitution   = spritePhysic->floatForKey("Restitution");
	
	shapeDef->filter.categoryBits   = spritePhysic->intForKey("Category");
	shapeDef->filter.maskBits       = spritePhysic->intForKey("Mask");
	shapeDef->filter.groupIndex     = spritePhysic->intForKey("Group");
    
    shapeDef->isSensor = spritePhysic->boolForKey("IsSensor");
}
//------------------------------------------------------------------------------

void LevelHelperLoader::createPhysicBoundariesHelper(b2World* _world,
                                                     const CCPoint& wbConv,
                                                     const CCPoint& pos_offset)
{
	if(!hasPhysicBoundaries()){
        CCLog("LevelHelper WARNING - Please create physic boundaries in LevelHelper in order to call method \"createPhysicBoundaries\"");
        return;
    }	
    
    b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody;
	bodyDef.position.Set(0.0f, 0.0f);
    b2Body* wbBodyT = _world->CreateBody(&bodyDef);
	b2Body* wbBodyL = _world->CreateBody(&bodyDef);
	b2Body* wbBodyB = _world->CreateBody(&bodyDef);
	b2Body* wbBodyR = _world->CreateBody(&bodyDef);
	
	{
        LHDictionary dict;
        dict.setObjectForKey("LHPhysicBoundarieLeft", "UniqueName");
    
        LHNode* spr = LHNode::nodeWithDictionary(&dict);
		spr->setTag(wb->intForKey("TagLeft"));
        
#if COCOS2D_VERSION >= 0x00020000
        spr->setVisible(false);
#else
        spr->setIsVisible(false);
#endif
        spr->setBody(wbBodyL);
        wbBodyL->SetUserData(spr);
        physicBoundariesInLevel.setObject(spr, "LHPhysicBoundarieLeft");
	}
	
	{
        LHDictionary dict;
        dict.setObjectForKey("LHPhysicBoundarieRight", "UniqueName");

        LHNode* spr = LHNode::nodeWithDictionary(&dict);
		spr->setTag(wb->intForKey("TagRight"));
        
#if COCOS2D_VERSION >= 0x00020000
        spr->setVisible(false);
#else
        spr->setIsVisible(false);
#endif

        spr->setBody(wbBodyR);
        wbBodyR->SetUserData(spr);
        physicBoundariesInLevel.setObject(spr, "LHPhysicBoundarieRight");
	}
	
	{
        LHDictionary dict;
        dict.setObjectForKey("LHPhysicBoundarieTop", "UniqueName");

        LHNode* spr = LHNode::nodeWithDictionary(&dict);
		spr->setTag(wb->intForKey("TagTop"));
#if COCOS2D_VERSION >= 0x00020000
        spr->setVisible(false);
#else
        spr->setIsVisible(false);
#endif

        spr->setBody(wbBodyT);
        wbBodyT->SetUserData(spr);
        physicBoundariesInLevel.setObject(spr, "LHPhysicBoundarieTop");
	}
	
	{
        LHDictionary dict;
        dict.setObjectForKey("LHPhysicBoundarieBottom", "UniqueName");

        LHNode* spr = LHNode::nodeWithDictionary(&dict);
        
		spr->setTag(wb->intForKey("TagBottom"));
#if COCOS2D_VERSION >= 0x00020000
        spr->setVisible(false);
#else
        spr->setIsVisible(false);
#endif

        spr->setBody(wbBodyB);
        wbBodyB->SetUserData(spr);
        physicBoundariesInLevel.setObject(spr, "LHPhysicBoundarieBottom");
	}
	
    bool canSleep = wb->boolForKey("CanSleep");
	wbBodyT->SetSleepingAllowed(canSleep);
	wbBodyL->SetSleepingAllowed(canSleep);
	wbBodyB->SetSleepingAllowed(canSleep);
	wbBodyR->SetSleepingAllowed(canSleep);
	
    CCRect rect = wb->rectForKey("WBRect");
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	
    float ptm = LHSettings::sharedInstance()->lhPtmRatio();
    
    rect.origin.x += pos_offset.x;
    rect.origin.y += pos_offset.y;
    
    {//TOP
#ifdef B2_EDGE_SHAPE_H
        b2EdgeShape shape;
#else
        b2PolygonShape shape;
#endif
		
        b2Vec2 pos1 = b2Vec2(rect.origin.x/ptm*wbConv.x,
							 (winSize.height - rect.origin.y*wbConv.y)/ptm);
        
        b2Vec2 pos2 = b2Vec2((rect.origin.x + rect.size.width)*wbConv.x/ptm,
							 (winSize.height - rect.origin.y*wbConv.y)/ptm);
#ifdef B2_EDGE_SHAPE_H
		shape.Set(pos1, pos2);
#else
        shape.SetAsEdge(pos1, pos2);
#endif
		
        b2FixtureDef fixture;
        setFixtureDefPropertiesFromDictionary(wb, &fixture);
        fixture.shape = &shape;
        wbBodyT->CreateFixture(&fixture);
    }
	
    {//LEFT
#ifdef B2_EDGE_SHAPE_H
        b2EdgeShape shape;
#else
        b2PolygonShape shape;
#endif
        
		
		b2Vec2 pos1 = b2Vec2(rect.origin.x*wbConv.x/ptm,
							 (winSize.height - rect.origin.y*wbConv.y)/ptm);
        
		b2Vec2 pos2 = b2Vec2((rect.origin.x*wbConv.x)/ptm,
							 (winSize.height - (rect.origin.y + rect.size.height)*wbConv.y)/ptm);
#ifdef B2_EDGE_SHAPE_H
		shape.Set(pos1, pos2);
#else
        shape.SetAsEdge(pos1, pos2);
#endif
        
		
        b2FixtureDef fixture;
        setFixtureDefPropertiesFromDictionary(wb, &fixture);
        fixture.shape = &shape;
        wbBodyL->CreateFixture(&fixture);
    }
	
    {//RIGHT
#ifdef B2_EDGE_SHAPE_H
        b2EdgeShape shape;
#else
        b2PolygonShape shape;
#endif
        
        
        b2Vec2 pos1 = b2Vec2((rect.origin.x + rect.size.width)*wbConv.x/ptm,
							 (winSize.height - rect.origin.y*wbConv.y)/ptm);
        
        b2Vec2 pos2 = b2Vec2((rect.origin.x+ rect.size.width)*wbConv.x/ptm,
							 (winSize.height - (rect.origin.y + rect.size.height)*wbConv.y)/ptm);
#ifdef B2_EDGE_SHAPE_H
		shape.Set(pos1, pos2);
#else
        shape.SetAsEdge(pos1, pos2);
#endif
        
		
        b2FixtureDef fixture;
        setFixtureDefPropertiesFromDictionary(wb, &fixture);
        fixture.shape = &shape;
        wbBodyR->CreateFixture(&fixture);
        
    }
	
    {//BOTTOM
#ifdef B2_EDGE_SHAPE_H
        b2EdgeShape shape;
#else
        b2PolygonShape shape;
#endif
        
        
        b2Vec2 pos1 = b2Vec2(rect.origin.x*wbConv.x/ptm,
							 (winSize.height - (rect.origin.y + rect.size.height)*wbConv.y)/ptm);
        
        b2Vec2 pos2 = b2Vec2((rect.origin.x+ rect.size.width)*wbConv.x/ptm,
							 (winSize.height - (rect.origin.y + rect.size.height)*wbConv.y)/ptm);
#ifdef B2_EDGE_SHAPE_H
		shape.Set(pos1, pos2);
#else
        shape.SetAsEdge(pos1, pos2);
#endif
        
		
        b2FixtureDef fixture;
        setFixtureDefPropertiesFromDictionary(wb, &fixture);
        fixture.shape = &shape;
        wbBodyB->CreateFixture(&fixture);
    }    
}
////------------------------------------------------------------------------------
//////////////////////////////////////////////////////////////////////////////////
////PHYSICS
//////////////////////////////////////////////////////////////////////////////////
//
void LevelHelperLoader::setMeterRatio(float ratio){
	LHSettings::sharedInstance()->setLhPtmRatio(ratio);
}
//------------------------------------------------------------------------------
float LevelHelperLoader::meterRatio(){
	return LHSettings::sharedInstance()->lhPtmRatio();
}
//------------------------------------------------------------------------------
float LevelHelperLoader::pixelsToMeterRatio(){
    return LHSettings::sharedInstance()->lhPtmRatio()*LHSettings::sharedInstance()->convertRatio().x;
}
//------------------------------------------------------------------------------
float LevelHelperLoader::pointsToMeterRatio(){
    return LHSettings::sharedInstance()->lhPtmRatio();
}
//------------------------------------------------------------------------------
b2Vec2 LevelHelperLoader::pixelToMeters(CCPoint point){
    return b2Vec2(point.x / LevelHelperLoader::pixelsToMeterRatio(), 
                  point.y / LevelHelperLoader:: pixelsToMeterRatio());
}
//------------------------------------------------------------------------------
b2Vec2 LevelHelperLoader::pointsToMeters(CCPoint point){
    return b2Vec2(point.x / LHSettings::sharedInstance()->lhPtmRatio(), 
                  point.y / LHSettings::sharedInstance()->lhPtmRatio());
}
//------------------------------------------------------------------------------
CCPoint LevelHelperLoader::metersToPoints(b2Vec2 vec){
    return CCPointMake(vec.x*LHSettings::sharedInstance()->lhPtmRatio(), 
                       vec.y*LHSettings::sharedInstance()->lhPtmRatio());
}
//------------------------------------------------------------------------------
CCPoint LevelHelperLoader::metersToPixels(b2Vec2 vec){
    return ccpMult(CCPointMake(vec.x, vec.y), LevelHelperLoader::pixelsToMeterRatio());
}
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
void LevelHelperLoader::setTouchDispatcherForBezierWithTag(LHBezier* object, int tag){

    //if no touch observer is registered then do NOT add the sprite to the touch dispatcher
    if(NULL == LHTouchMgr::sharedInstance()->onTouchBeginObserverForTag(tag))
        return;
    
    object->setTagTouchBeginObserver(LHTouchMgr::sharedInstance()->onTouchBeginObserverForTag(tag));
    object->setTagTouchMovedObserver(LHTouchMgr::sharedInstance()->onTouchMovedObserverForTag(tag));
    object->setTagTouchEndedObserver(LHTouchMgr::sharedInstance()->onTouchEndedObserverForTag(tag));
    
    bool swallowTag = LHTouchMgr::sharedInstance()->shouldTouchesBeSwallowedForTag(tag);
    int priority = LHTouchMgr::sharedInstance()->priorityForTag(tag);

    bool swallow = object->getSwallowTouches();
    if(swallowTag ==true && swallow == false)
        swallow = true;
    
    object->swallowTouches = swallow;

    
    object->swallowTouches = swallow;
    
    CCTouchDispatcher* touchDispatcher = NULL;
#if COCOS2D_VERSION >= 0x00020000
    touchDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
#else
    touchDispatcher = CCTouchDispatcher::sharedDispatcher();
#endif
    
    if(touchDispatcher){
        touchDispatcher->addTargetedDelegate(object, priority, swallow);       
    }
}
//------------------------------------------------------------------------------
void LevelHelperLoader::setTouchDispatcherForSpriteWithTag(LHSprite* object, int tag)
{
    //if no touch observer is registered then do NOT add the sprite to the touch dispatcher
    if(NULL == LHTouchMgr::sharedInstance()->onTouchBeginObserverForTag(tag))
        return;
 
    object->setTagTouchBeginObserver(LHTouchMgr::sharedInstance()->onTouchBeginObserverForTag(tag));
    object->setTagTouchMovedObserver(LHTouchMgr::sharedInstance()->onTouchMovedObserverForTag(tag));
    object->setTagTouchEndedObserver(LHTouchMgr::sharedInstance()->onTouchEndedObserverForTag(tag));
    
    bool swallowTag = LHTouchMgr::sharedInstance()->shouldTouchesBeSwallowedForTag(tag);
    int priority = LHTouchMgr::sharedInstance()->priorityForTag(tag);

    bool swallow = object->getSwallowTouches();
    if(swallowTag ==true && swallow == false)
        swallow = true;
    
    object->swallowTouches = swallow;
    
    CCTouchDispatcher* touchDispatcher = NULL;
#if COCOS2D_VERSION >= 0x00020000
    touchDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
#else
    touchDispatcher = CCTouchDispatcher::sharedDispatcher();
#endif
    
    if(touchDispatcher){
        touchDispatcher->addTargetedDelegate(object, priority, swallow);        
    }
}
////------------------------------------------------------------------------------
void LevelHelperLoader::removeTouchDispatcherFromSprite(LHSprite* object){
    CCTouchDispatcher* touchDispatcher = NULL;
#if COCOS2D_VERSION >= 0x00020000
    touchDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
#else
    touchDispatcher = CCTouchDispatcher::sharedDispatcher();
#endif
    
    if(touchDispatcher){
        touchDispatcher->removeDelegate(object);    
    }
}
//------------------------------------------------------------------------------
void LevelHelperLoader::removeTouchDispatcherFromBezier(LHBezier* object){
    CCTouchDispatcher* touchDispatcher = NULL;
#if COCOS2D_VERSION >= 0x00020000
    touchDispatcher = CCDirector::sharedDirector()->getTouchDispatcher();
#else
    touchDispatcher = CCTouchDispatcher::sharedDispatcher();
#endif
    
    if(touchDispatcher){
        touchDispatcher->removeDelegate(object);
    }
}
//------------------------------------------------------------------------------
LHParallaxNode* LevelHelperLoader::parallaxNodeWithUniqueName(const std::string& uniqueName)
{
    return (LHParallaxNode*)parallaxesInLevel.objectForKey(uniqueName);
}
//------------------------------------------------------------------------------
CCArray* LevelHelperLoader::allParallaxes(void){
    
#if COCOS2D_VERSION >= 0x00020000
    CCArray* keys = parallaxesInLevel.allKeys();
    CCArray* array = CCArray::create();
    for(int i = 0; i < (int)keys->count(); ++i){
        LHParallaxNode* node = (LHParallaxNode*)parallaxesInLevel.objectForKey(((CCString*)keys->objectAtIndex(i))->getCString());
        if(node)
            array->addObject(node);
    }
    return array;
#else
    std::vector<std::string> keys = parallaxesInLevel.allKeys();
    CCArray* array = CCArray::array();
    for(int i = 0; i < (int)keys.size(); ++i){
        LHParallaxNode* node = (LHParallaxNode*)parallaxesInLevel.objectForKey(keys[i]);
      
        if(node)
            array->addObject(node);
    }
    return array;
    
#endif
}
//------------------------------------------------------------------------------
void LevelHelperLoader::removeAllParallaxes(bool removeSprites)
{
    
#if COCOS2D_VERSION >= 0x00020000
    
    CCArray* keys = parallaxesInLevel.allKeys();
    if(keys){
        for(int i = 0; i < keys->count(); ++i){
            LHParallaxNode* node = (LHParallaxNode*)parallaxesInLevel.objectForKey(((CCString*)keys->objectAtIndex(i))->getCString());
            if(NULL != node){
                node->removeSpritesOnDelete = removeSprites;
                node->removeFromParentAndCleanup(true);
            }
        }
    }
    parallaxesInLevel.removeAllObjects();
    
#else
    
    std::vector<std::string> keys = parallaxesInLevel.allKeys();
    for(int i = 0; i < keys.size(); ++i){
        LHParallaxNode* node = (LHParallaxNode*)parallaxesInLevel.objectForKey(keys[i]);
        if(NULL != node){
            node->removeSpritesOnDelete = removeSprites;
            node->removeFromParentAndCleanup(true);
        }
        
    }
    parallaxesInLevel.removeAllObjects();
#endif
}
//------------------------------------------------------------------------------
void LevelHelperLoader::removeParallaxNode(LHParallaxNode* node, bool removeSprites){
    
    if(NULL == node)
        return;    
    
    node->removeSpritesOnDelete = removeSprites;
    parallaxesInLevel.removeObjectForKey(node->getUniqueName());
    
    node->removeFromParentAndCleanup(true);
}


void LevelHelperLoader::removeJoint(LHJoint* jt){
    if(jt)
        jointsInLevel.removeObjectForKey(jt->getUniqueName());
}


void LevelHelperLoader::loadLevelHelperSceneFile(const char* levelFile,
                                                 const char* subfolder, 
                                                 const char* imgFolder)
{
    unsigned char* levelFileBuffer = NULL;
    unsigned long bufferSize = 0;
    
#if COCOS2D_VERSION >= 0x00020000
    std::string fullPath = CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(levelFile);
    levelFileBuffer = CCFileUtils::sharedFileUtils()->getFileData(fullPath.c_str(), "r", &bufferSize);
#else    
    std::string fullPath = CCFileUtils::fullPathFromRelativePath(levelFile);
    levelFileBuffer = CCFileUtils::getFileData(fullPath.c_str(), "r", &bufferSize);
#endif
    
	CCAssert(bufferSize > 0, "Invalid level file. Please add the LevelHelper scene file to Resource folder.");
    
    std::string filecontents((const char*) levelFileBuffer, bufferSize);
    std::stringstream infile(filecontents, stringstream::in);
    
    LHDictionary* dictionary = new LHDictionary(infile);
    
    processLevelFileFromDictionary(dictionary);
    
    delete dictionary;  
    
    delete [] levelFileBuffer;
}

void LevelHelperLoader::processLevelFileFromDictionary(LHDictionary* dictionary)
{
	if(0 == dictionary)
		return;
    
//    dictionary->print();
//    
    
    
	bool fileInCorrectFormat =	dictionary->stringForKey("Author") == "Bogdan Vladu" && 
                                dictionary->stringForKey("CreatedWith") == "LevelHelper";
	    
	if(fileInCorrectFormat == false)
		CCLog("This file was not created with LevelHelper or file is damaged.");
        
    LHDictionary* scenePref = dictionary->dictForKey("ScenePreference");
    safeFrame       = scenePref->pointForKey("SafeFrame");
    gameWorldRect   = scenePref->rectForKey("GameWorld");
        
    LHSettings::sharedInstance()->setHDSuffix(scenePref->stringForKey("HDSuffix"));
    LHSettings::sharedInstance()->setHD2xSuffix(scenePref->stringForKey("2HDSuffix"));
    LHSettings::sharedInstance()->setDevice(scenePref->intForKey("Device"));

        
    CCRect color = scenePref->rectForKey("BackgroundColor");
    glClearColor(color.origin.x, color.origin.y, color.size.width, 1);
        
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    LHSettings::sharedInstance()->setConvertRatio(CCPointMake(winSize.width/safeFrame.x,
                                                                 winSize.height/safeFrame.y));
    
    float safeFrameDiagonal = sqrtf(safeFrame.x* safeFrame.x + safeFrame.y* safeFrame.y);
    float winDiagonal = sqrtf(winSize.width* winSize.width + winSize.height*winSize.height);
    float PTM_conversion = winDiagonal/safeFrameDiagonal;
    
    LevelHelperLoader::setMeterRatio(LHSettings::sharedInstance()->lhPtmRatio()*PTM_conversion);
    
    
    if(LHSettings::sharedInstance()->isIpad()){
        if(!LHSettings::sharedInstance()->getStretchArt()){
            LevelHelperLoader::setMeterRatio(32.0f*2.0f);         
        }
    }
    
	////////////////////////LOAD WORLD BOUNDARIES///////////////////////////////
	if(NULL != dictionary->objectForKey("WBInfo")){
		wb = new LHDictionary(dictionary->dictForKey("WBInfo"));
	}
	   
    ////////////////////////LOAD SPRITES////////////////////////////////////////////////////
    lhNodes = new LHArray(dictionary->arrayForKey("NODES_INFO"));
    
	///////////////////////LOAD JOINTS//////////////////////////////////////////////////////////
	lhJoints = new LHArray(dictionary->arrayForKey("JOINTS_INFO"));	
	
    //////////////////////LOAD PARALLAX/////////////////////////////////////////
    lhParallax = new LHArray(dictionary->arrayForKey("PARALLAX_INFO"));

    gravity = dictionary->pointForKey("Gravity");
}
////////////////////////////////////////////////////////////////////////////////