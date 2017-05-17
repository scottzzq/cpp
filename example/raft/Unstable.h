#ifndef UNSTABLE_H
#define UNSTABLE_H

#include "eraftpb.pb.h"
#include <vector>
#include <string>
#include "tikv_common.h"

class Unstable {
	public:
		Unstable(uint64_t off, std::string tag) : offset(off), tag(tag){
			this->entries.clear();
		}
		~Unstable(){
		}
		// the incoming unstable snapshot, if any.
		eraftpb::Snapshot snapshot;
		// all entries that have not yet been written to storage.
		std::vector<eraftpb::Entry> entries;
		uint64_t offset;
		std::string tag;

		uint64_t maybe_first_index();
		uint64_t maybe_last_index();
		uint64_t maybe_term(uint64_t idx) ;
		void stable_to(uint64_t idx, uint64_t term);
		void truncate_and_append(std::vector<eraftpb::Entry>& ents);
		std::vector<eraftpb::Entry> slice(uint64_t lo, uint64_t hi);
		void must_check_outofbounds(uint64_t lo, uint64_t hi) ;
};

#endif
