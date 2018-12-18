// Copyright 2015-2017 Hans Dembinski
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_HISTOGRAM_AXIS_ITERATOR_HPP
#define BOOST_HISTOGRAM_AXIS_ITERATOR_HPP

#include <boost/histogram/axis/interval_view.hpp>
#include <boost/histogram/detail/meta.hpp>
#include <boost/iterator/iterator_adaptor.hpp>
#include <boost/iterator/reverse_iterator.hpp>

namespace boost {
namespace histogram {
namespace axis {

template <typename Axis>
class iterator
    : public boost::iterator_adaptor<iterator<Axis>, int,
                                     decltype(std::declval<const Axis&>()[0]),
                                     boost::random_access_traversal_tag,
                                     decltype(std::declval<const Axis&>()[0]), int> {
public:
  explicit iterator(const Axis& axis, int idx)
      : iterator::iterator_adaptor_(idx), axis_(axis) {}

protected:
  bool equal(const iterator& other) const noexcept {
    return &axis_ == &other.axis_ && this->base_reference() == other.base_reference();
  }

  decltype(auto) dereference() const { return axis_[this->base_reference()]; }

  friend class boost::iterator_core_access;

private:
  const Axis& axis_;
};

/// Uses CRTP to inject iterator logic into Derived.
template <typename Derived>
class iterator_mixin {
public:
  using const_iterator = iterator<Derived>;
  using const_reverse_iterator = boost::reverse_iterator<const_iterator>;

  const_iterator begin() const noexcept {
    return const_iterator(*static_cast<const Derived*>(this), 0);
  }
  const_iterator end() const noexcept {
    return const_iterator(*static_cast<const Derived*>(this),
                          static_cast<const Derived*>(this)->size());
  }
  const_reverse_iterator rbegin() const noexcept {
    return boost::make_reverse_iterator(end());
  }
  const_reverse_iterator rend() const noexcept {
    return boost::make_reverse_iterator(begin());
  }
};

} // namespace axis
} // namespace histogram
} // namespace boost

#endif
