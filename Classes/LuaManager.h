//
//  LuaManager.h
//  SportsWorldCupOriginal
//
//  Created by ksoo k on 13. 7. 3..
//
//

#ifndef __SportsWorldCupOriginal__LuaManager__
#define __SportsWorldCupOriginal__LuaManager__

#include "lua_tinker.h"
#include <map>
#include <string>
#include <assert.h>
#include "support/zip_support/unzip.h"
#include "cocos2d.h"

USING_NS_CC;
#define SQ(X) ((#X)[0])
#define MAKE_CHAR(a) char _##a = SQ(a);
class LuaManager
{
public:
	static LuaManager* shared()
	{
		if (s_sharedManager == 0)
		{
			s_sharedManager = new LuaManager();
		}
		return s_sharedManager;
	}

	void closeLua(const std::string& luaFile)
	{
		if( luas.find(luaFile) != luas.end() )
		{
			lua_close(luas[luaFile]);
			luas.erase(luaFile);
		}
	}
	void loadZip(const std::string& luaZip)
	{
		
		std::string path2 = CCFileUtils::sharedFileUtils()->fullPathForFilename(luaZip.c_str());
		do {
			unzFile uf = unzOpen(path2.c_str());
			if(NULL == uf) break;
			
			int ret = unzGoToFirstFile(uf);
			if( UNZ_OK != ret){
				unzClose(uf);
				break;
			}
			
			/// 현재  방문중인  파일  목록  정보  추출
			const  int MAX_PATH = 256;
			const  int MAX_COMMENT = 256;
			
			char filename[MAX_PATH];
			char comment[MAX_COMMENT];
			
			unz_file_info info;
			unzGetCurrentFileInfo(uf,&info,filename, MAX_PATH, NULL, 0, comment, MAX_COMMENT);
			
			//			std::cout<<"filename:"<< filename <<" Comment:"<<comment<<std::endl;
			//			std::cout<<" compressed_size:"<< info.compressed_size<<" uncompressed_size:"<< info.uncompressed_size <<std::endl;
			
			
			//현재  파일을  열기.
			//			ret = unzOpenCurrentFile(uf);
			MAKE_CHAR(o);
			MAKE_CHAR(h);
			MAKE_CHAR(s);
			
			std::string password = std::string(1, _h) + _s + _o + _o + _h + _s + _o + _o;
			
			std::string src;
			ret = unzOpenCurrentFilePassword(uf, password.c_str());
			password = "";
			const  int BUF =1024;
			Bytef in[BUF];
			int readsize(0);
			
			//			std::ofstream op;
			//			op.open(filename, std::ios_base::binary);
			
			do {
				readsize = unzReadCurrentFile(uf,(void*) in, BUF);
				src.append(&in[0], &in[readsize]);
				//				op.write( (const  char*) in, readsize);
				
			} while( 0 != readsize);
			
			//			op.close();
			
			unzCloseCurrentFile(uf);
			
			/////
			unzClose(uf);
			
			lua_State* L = lua_open();
			
			luaL_openlibs(L);
			lua_tinker::dostring(L, src.c_str());
			luas[filename] = L;
			
		} while(false);
	}
	lua_State* getLua(const std::string& luaFile);
	void addLuaString(const std::string luaFile, const std::string& luaStr)
	{
		lua_State* L = lua_open();
		
		luaL_openlibs(L);
		lua_tinker::dostring(L, luaStr.c_str());
		luas[luaFile] = L;
	}
	virtual ~LuaManager(){}
private:
	std::map<std::string, lua_State*> luas;
	LuaManager(){}
	
	
protected:
	static LuaManager* s_sharedManager;
};

#endif /* defined(__SportsWorldCupOriginal__LuaManager__) */
