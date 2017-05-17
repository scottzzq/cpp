#ifndef ENGINE_DB_H
#define ENGINE_DB_H

#include "rocksdb/db.h"

class EngineDB{
	public:
		EngineDB(){
				}
		~EngineDB(){

		}
		rocksdb::DB* get_db(){
			return db;
		}
	private:
		rocksdb::DB* db;
};
#endif
