#include "SceUidManager.h"


void SceUidManager::reset() {
    uidMap.clear();
/*TODO*/  // freeIdsMap.clear();
    uidNext = 1;
}
int SceUidManager::getNewUid(std::string purpose) {
    SceUid *uid = new SceUid(purpose, uidNext++);//TODO memory clear?
    uidMap.emplace(uid->getUid(), uid);

    return uid->getUid();
}
bool SceUidManager::checkUidPurpose(int uid, std::string purpose, bool allowUnknown) {
    SceUid *found = uidMap[uid];

    if (found == nullptr) {
        if (!allowUnknown) {
            /*TODO*/  //            	if (!exitCalled()) {
            /*TODO*/  //            		log.warn("Attempt to use unknown SceUID (purpose='" + purpose.toString()
                      //            +
                      //            "')");
            /*TODO*/  //            	}
            return false;
        }

    } 
    else if (purpose.compare(found->getPurpose())!=0){
        /*TODO*/  //        	if (!exitCalled()) {
/*TODO*/  //        		log.error("Attempt to use SceUID for different purpose (purpose='" + purpose.toString() +
          //        "',original='" + found.getPurpose().toString() + "')");
/*TODO*/  //        	}
            return false;
        }

        return true;
}
bool SceUidManager::releaseUid(int uid, std::string purpose) {
    SceUid *found = uidMap[uid];

    if (found == nullptr) {
    /*TODO*/  //        	if (!exitCalled()) {
    /*TODO*/  //        		log.warn("Attempt to release unknown SceUID (purpose='" + purpose.toString() +
              //        "')");
    /*TODO*/  //        	}
    /*TODO*/  //
    return false;
    }

    if (purpose.compare(found->getPurpose())==0) { 
        uidMap.erase(uid);
    } else {
    /*TODO*/  //        	if (!exitCalled()) {
    /*TODO*/  //        		log.error("Attempt to release SceUID for different purpose (purpose='" +
              //        purpose.toString()
              //        + "',original='" + found.getPurpose().toString() + "')");
    /*TODO*/  //        	}
    return false;
    }

    return true;
}
bool SceUidManager::isValidUid(int uid) { return uidMap.find(uid) != uidMap.end(); }

int SceUidManager::getNewId(std::string purpose, int minimumId, int maximumId) {
    std::list<int> freeIds = freeIdsMap[purpose];
    if (freeIds.empty()) {
        freeIds = std::list<int>();
        for (int id = minimumId; id <= maximumId; id++) {
            freeIds.push_back(id);
        }
        freeIdsMap.emplace(purpose, freeIds);
    }

    // No more free IDs?
    if (freeIds.size() <= 0) {
        // Return an invalid ID
        return INVALID_ID;
    }

    // Return the lowest free ID
    int front = freeIds.front();//return freeIds.remove(); //TODO is there a better way???
    freeIds.pop_front();
    return front;
}

void SceUidManager::resetIds(std::string purpose) { freeIdsMap.erase(purpose); }

bool SceUidManager::releaseId(int id, std::string purpose) {
    std::list<int> freeIds = freeIdsMap[purpose];

    if (freeIds.empty()) {
/*TODO*/  //        	if (!exitCalled()) {
/*TODO*/  //        		log.warn(String.format("Attempt to release ID=%d with unknown purpose='%s'", id,
          //        purpose));
/*TODO*/  //        	}
    		return false;
    	}
    // Add the id back to the freeIds list,
    // and keep the id's ordered (lowest first).
    for (std::list<int>::const_iterator lit = freeIds.begin(); lit != freeIds.end(); ++lit) {
            int currentId = *lit;
            if (currentId == id) {
/*TODO*/  //            	if (!exitCalled()) {
/*TODO*/  //            		log.warn(String.format("Attempt to release free ID=%d with purpose='%s'", id,
          //            purpose));
          //  	}
        		return false;
    		}
            if (currentId > id) {
                // Insert the id before the currentId
                /*TODO*/  //                (*lit)->set(id);
                /*TODO*/  //                (*lit)->add(currentId);
                return true;
            }
    }

    freeIds.push_back(id);

    return true;
}
