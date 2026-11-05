#include "people.h"

void Curator::assignGroup(Group groupToCur) { groupCur_ = groupToCur; }

bool Curator::transferToSubGroup(const std::string &studentId,
                                 int subGroupNumEx) {

  return groupCur_.changeStudentSubGroup(studentId, subGroupNumEx);
}
