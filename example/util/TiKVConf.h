#ifndef TIKV_CONFIG_H
#define TIKV_CONFIG_H

#include "base_conf.h"
#include <string>
#include "tikv_common.h"

class TiKVConfig{
	public:
		static TiKVConfig* getInstance(){
			static TiKVConfig conf("./tikv_server.conf");
			return &conf;
		}
		slash::BaseConf* getBaseConf(){
			return conf;
		}
	private:
		TiKVConfig(std::string filename){
			conf = new slash::BaseConf(filename);
			if (conf->LoadConf() == 0) {
				LOG_INFO << "LoadConf true";
			} else {
				LOG_ERROR << "LoodConf error";
			}

		}

		~TiKVConfig(){
		}
		slash::BaseConf* conf;
};
#endif
