#pragma once
#include <unordered_map>

#include "core/HLE/kernel/types/SceUid.h"

class SceUidManager 
{
   private:
    // UID is a unique identifier across all purposes
    static std::unordered_map<int, SceUid*> uidMap;
    static int uidNext;  // LocoRoco expects UID to be 8bit
   public:
    static const int INVALID_ID = std::numeric_limits<int>::min();
   private:
    // ID is an identifier only unique for the same purpose.
    // Different purposes can share the save ID values.
    // An ID has always a range of valid values, e.g. [0..255]
    static std::unordered_map<std::string, std::list<int>> freeIdsMap;

   public:
    void reset();
    /** classes should call getUid to get a new unique SceUID */
    int getNewUid(std::string purpose);
    /** classes should call checkUidPurpose before using a SceUID return true is the uid is ok. */
    bool checkUidPurpose(int uid, std::string purpose, bool allowUnknown);
    /** classes should call releaseUid when they are finished with a SceUID return true on success. */
    bool releaseUid(int uid, std::string purpose);
    bool isValidUid(int uid);
    /**
     * Return a new ID for the given purpose.
     * The ID will be unique for the given purpose but will not be unique
     * across different purposes.
     * The ID will be higher of equal to minimumId, and lower or equal to
     * maximumId, i.e. in the range [minimumId..maximumId].
     * The ID will be lowest possible free ID.
     *
     * @param purpose    The ID will be unique for this purpose
     * @param minimumId  The lowest possible value for the ID
     * @param maximumId  The highest possible value for the ID
     * @return           The lowest possible free ID for the given purpose
     */
    int getNewId(std::string purpose, int minimumId, int maximumId);
    void resetIds(std::string purpose);
    /**
     * Release an ID for a given purpose. The ID had to be created first
     * by getNewId().
     * After release, the ID is marked as being free and can be returned
     * again by getNewId().
     *
     * @param id       The ID to be released
     * @param purpose  The ID will be releases for this purpose.
     * @return         true if the ID was successfully released
     *                 false if the ID could not be released
     *                       (because the purpose was not exiting or
     *                        the ID was already released)
     */
    bool releaseId(int id, std::string purpose);
};
