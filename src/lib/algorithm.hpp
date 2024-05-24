#pragma once

namespace los {

template <typename Iter, typename Val>
void fill(Iter begin, Iter end, const Val &val) {
  while (begin != end) {
    *begin = val;
    begin++;
  }
}

} // namespace los