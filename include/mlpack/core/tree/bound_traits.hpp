/**
 * @file bound_traits.hpp
 * @author Ryan Curtin
 *
 * A class for template metaprogramming traits for bounds.
 *
 * This file is part of mlpack 2.0.1.
 *
 * mlpack is free software; you may redstribute it and/or modify it under the
 * terms of the 3-clause BSD license.  You should have received a copy of the
 * 3-clause BSD license along with mlpack.  If not, see
 * http://www.opensource.org/licenses/BSD-3-Clause for more information.
 */
#ifndef __MLPACK_CORE_TREE_BOUND_TRAITS_HPP
#define __MLPACK_CORE_TREE_BOUND_TRAITS_HPP

namespace mlpack {
namespace bound {

/**
 * A class to obtain compile-time traits about BoundType classes.  If you are
 * writing your own BoundType class, you should make a template specialization
 * in order to set the values correctly.
 *
 * @see TreeTraits, KernelTraits
 */
template<typename BoundType>
struct BoundTraits
{
  //! If true, then the bounds for each dimension are tight.  If false, then the
  //! bounds for each dimension may be looser than the range of all points held
  //! in the bound.  This defaults to false.
  static const bool HasTightBounds = false;
};

} // namespace bound
} // namespace mlpack

#endif
