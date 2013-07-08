//
//  AdvanceSprite.cpp
//  FlyingFreak
//
//  Created by varadharaj on 16/05/11.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "AdvanceSprite.h"

AdvanceSprite::AdvanceSprite()
:m_pfnSelectorDelegate(0)
,m_pTarget(0)
,m_isPlistLoaded(false)
,m_ElaspeTime(0.0)
,m_IncrementValue(0)
,m_isAnimationRunning(false)
{
   
}

AdvanceSprite::~AdvanceSprite()
{
    if(m_isPlistLoaded)
        CC_SAFE_DELETE_ARRAY(m_AnimationFrames);
}



void AdvanceSprite::addFrames(CCArray *frames)
{
    m_AnimationFrames = frames;
	CCAnimationFrame* de = (CCAnimationFrame*)m_AnimationFrames->objectAtIndex(0);
	CCSpriteFrame* sf = de->getSpriteFrame();
	
    setDisplayFrame(de->getSpriteFrame());
}

void AdvanceSprite::addFrames(CCArray *frames, int displayTextureIndex)
{
    m_AnimationFrames = frames;
	CCAnimationFrame* de = (CCAnimationFrame*)m_AnimationFrames->objectAtIndex(displayTextureIndex);
	CCSpriteFrame* sf = de->getSpriteFrame();
	

    setDisplayFrame(sf);
}

void AdvanceSprite::increaseCurrentIndex()
{
    if(m_AnimationMode && (m_EndingIndex - m_CurrentIndex) != 0)
        m_IncrementValue += (m_EndingIndex - m_CurrentIndex) / abs(m_EndingIndex - m_CurrentIndex);
    else if((m_StartingIndex - m_CurrentIndex) != 0)
        m_IncrementValue += (m_StartingIndex - m_CurrentIndex) / abs(m_StartingIndex - m_CurrentIndex);
}

void AdvanceSprite::update(float dt)
{
    m_ElaspeTime += dt;
    while (m_ElaspeTime >= m_FrameRate) 
    {
        m_ElaspeTime -= m_FrameRate;
        
        m_CurrentIndex += m_IncrementValue;
		CCAnimationFrame* de = (CCAnimationFrame*)m_AnimationFrames->objectAtIndex(m_CurrentIndex);
		CCSpriteFrame* sf = de->getSpriteFrame();
        setDisplayFrame(sf);
        
         //Forward Animation....
        if (m_CurrentIndex == m_EndingIndex) 
        {
            if(m_NeedToRunReverseAnimation)
            {
                m_AnimationMode = false;
                increaseCurrentIndex();
                return;
            }
            
            m_RunningLoop++;
            if(m_RunningLoop == m_NumberOfLoop)
            {
                stopAnimaiton();
                //Raising Callback.
                if(m_pTarget != 0 && m_pfnSelectorDelegate != 0)
                    (m_pTarget->*m_pfnSelectorDelegate)();
                
                //Remove Object by Itself.
                if(m_NeedToDeleteItself)
                    removeObjectItself();
                return;
            }
            else
            {
                m_CurrentIndex = m_StartingIndex;
            }
        }
        
        //Reverse Animation...
        else if(m_CurrentIndex == m_StartingIndex)
        {
            m_RunningLoop++;
            if(m_RunningLoop == m_NumberOfLoop)
            {
                stopAnimaiton();
                //Raising Callback.
                if(m_pTarget != 0 && m_pfnSelectorDelegate != 0)
                    (m_pTarget->*m_pfnSelectorDelegate)();
                
                //Remove Object by Itself.
                if(m_NeedToDeleteItself)
                    removeObjectItself();
                return;
            }
            else
            {
                m_AnimationMode = true;
                increaseCurrentIndex();
            }
        }
    }
}

void AdvanceSprite::startAnimation(int startInd, int endInd, int number_Loop, SEL_CallFunc pfnSelectorDelegate, CCObject *pTarget, int NumberOfFramesPerSecond, bool NeedToRunReverseAnimation, bool NeedToDeleteItself)
{
    if(m_isAnimationRunning)
    {
        stopAnimaiton();
        
        //Reset pointer.
        m_pfnSelectorDelegate = 0;
        m_pTarget = 0;
        m_ElaspeTime = 0.0f;
    }
    
    //Assign values.
    m_isAnimationRunning = true;
    
    m_StartingIndex = startInd - 1;
    m_EndingIndex = endInd - 1;
    
    m_NumberOfLoop = number_Loop;
    
    m_pfnSelectorDelegate = pfnSelectorDelegate;
    m_pTarget = pTarget;
    if(NumberOfFramesPerSecond != -1)
        m_FrameRate = calculateFrameRate(NumberOfFramesPerSecond);
    
    //Initilize values....
    m_NeedToRunReverseAnimation = NeedToRunReverseAnimation;
    m_AnimationMode = true;
    m_CurrentIndex = m_StartingIndex;
    m_RunningLoop = 0;
    m_IncrementValue = 0;
    m_NeedToDeleteItself = NeedToDeleteItself;
    increaseCurrentIndex();
    this->scheduleUpdateWithPriority(-1);
    resumeAnimation();
}

void AdvanceSprite::stopAnimaiton()
{
    m_isAnimationRunning = false;
    this->unscheduleUpdate();
}

void AdvanceSprite::removeObjectItself()
{
    this->removeFromParentAndCleanup(true);
    delete (this);
}