#include "StatusManager.h"

#include "Init.h"
#include "Branch.h"

void StatusManager::Init(const char* path) {
  assert(path);

  mStatus.branch.path = path;
  mStatus.branch = initDirList(mStatus.branch.path);
}
