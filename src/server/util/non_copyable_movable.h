/**
 * @file non_copyable_movable.h
 * @brief Base classes for disable Copy" semantics and "Move" semantics (can be
 * used to build "Singleton" patterns).
 * @date 2021-11-25
 *
 * @copyright Copyright (c) 2021 InfraSail
 *
 */

#ifndef MRPROXYER_SRC_NETLIB_NON_COPYABLE_MOVABLE_H_
#define MRPROXYER_SRC_NETLIB_NON_COPYABLE_MOVABLE_H_

namespace util {

/**
 * @brief "Copy" semantics and "Move" semantics of the class that inherits
 * it will be prohibited.
 *
 */
class NonCopyableMovable {
 protected:
  NonCopyableMovable() = default;
  ~NonCopyableMovable() = default;

 private:
  NonCopyableMovable(const NonCopyableMovable&) = delete;
  NonCopyableMovable& operator=(const NonCopyableMovable&) = delete;
  NonCopyableMovable(NonCopyableMovable&&) = delete;
};

}  // namespace util

#endif  // !MRPROXYER_SRC_NETLIB_NON_COPYABLE_MOVABLE_H_
