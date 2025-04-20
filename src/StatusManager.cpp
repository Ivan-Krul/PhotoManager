#include "StatusManager.h"

#include "Branch.h"

StatusManager::Init(const char* path) {
  assert(path);

  mStatus.branch.path = path;
  mStatus.branch = initDirList(status.branch.path);
}
