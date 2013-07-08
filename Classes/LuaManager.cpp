//
//  LuaManager.cpp
//  SportsWorldCupOriginal
//
//  Created by ksoo k on 13. 7. 3..
//
//

#include "LuaManager.h"
LuaManager* LuaManager::s_sharedManager = 0;


lua_State* LuaManager::getLua(const std::string& luaFile)
{
//	CCLog("getLua!!");
	if(luas.find(luaFile) == luas.end())
	{
//		{
//			std::string path2 = CCFileUtils::sharedFileUtils()->fullPathForFilename(luaFile.c_str());
//			std::string path3 = CCFileUtils::sharedFileUtils()->fullPathForFilename(path2.c_str());
//			FILE* tt = fopen(path3.c_str(), "r");
//			CCLog("qqq %x", tt);
//			fclose(tt);
//		}
		
		
		
		lua_State* L = lua_open();
		luaL_openlibs(L);
		

		CCString* pstrFileContent = CCString::createWithContentsOfFile(luaFile.c_str());
		if (pstrFileContent)
		{
//			CCLog("%s", pstrFileContent->getCString());
			lua_tinker::dostring(L, pstrFileContent->getCString());
			luas[luaFile] = L;
		}
		else
		{
			CCLog("nothing %s", luaFile.c_str());
		}

		
		
	}
	return luas[luaFile];
}
