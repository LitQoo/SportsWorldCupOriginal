import os
import re
root = "Classes"

migMap = [
		[r"(=\s*)(CC[a-zA-Z0-9]+::)[a-zA-Z0-9]+", r"\1\2create"],
		[r"::actionWith[a-zA-Z0-9]+", r"::create"],
		[r"addFrameWithTexture", r"addSpriteFrameWithTexture"],
		[r"CCTouchDispatcher::sharedDispatcher()", r"CCDirector::sharedDirector()->getTouchDispatcher()"],
		[r"->setIs([A-Za-z0-9]+)", r"->set\1"],
		[r"->getIs([A-Za-z0-9]+)", r"->is\1"],
		[r"setRelativeAnchorPoint", r"ignoreAnchorPointForPosition"],
		[r"CCSequence::actionOneTwo", r"CCSequence::createWithTwoActions"],
		[r"CCSequence::actions", r"CCSequence::create"],
		[r"locationInView", r"getLocationInView"],
		[r"CCParticleSystemPoint", r"CCParticleSystem"],
		[r"getObjectAtIndex", r"objectAtIndex"],
		[r"::node\(\)", r"::create()"],
		[r"kCCProgressTimerTypeHorizontalBarRL", r"kCCProgressTimerTypeBar"],
		[r"kCCProgressTimerTypeHorizontalBarLR", r"kCCProgressTimerTypeBar"],
		[r"kCCMenuTouchPriority", r"kCCMenuHandlerPriority"],
		[r"CCTextAlignmentLeft", r"kCCTextAlignmentLeft"],
		  [r"CCTextAlignmentRight", r"kCCTextAlignmentRight"],
		  [r"CCTextAlignmentCenter", r"kCCTextAlignmentCenter"],
		[r"CCNotificationCenter::sharedNotifCenter\(\)", r"CCNotificationCenter::sharedNotificationCenter()"],
		[r"CCUserDefault::create", r"CCUserDefault::sharedUserDefault"],
		[r"(CCMutableArray<.+?>\s*)", r"CCArray"],
		[r"CCDirector::create\(\)", r"CCDirector::sharedDirector"],
		  
		]
for base, dirs, names in os.walk(root):
	for name in names:
		try:
			filename = os.path.join(base, name)
			extname = os.path.splitext(filename)[1].lower()
			print extname
			if extname != ".h" and extname != ".cpp":
				continue
				
			f = open(os.path.join(base, name))
			lns = ''
			for ln in f:
				lns += ln
			f.close()
			migredStr = lns
			#print lns
			for mig in migMap:
				a = mig[0]
				b = mig[1]
				migredStr = re.sub(a, b,migredStr) 
			f = open(os.path.join(base, name), 'w')
			f.write(migredStr)
			f.close()
		except:
			pass
