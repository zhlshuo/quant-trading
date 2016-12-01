/**
 * @file hrectbound.hpp
 *
 * Bounds that are useful for binary space partitioning trees.
 *
 * This file describes the interface for the HRectBound class, which implements
 * a hyperrectangle bound.
 *
 * This file is part of mlpack 2.0.1.
 *
 * mlpack is free software; you may redstribute it and/or modify it under the
 * terms of the 3-clause BSD license.  You should have received a copy of the
 * 3-clause BSD license along with mlpack.  If not, see
 * http://www.opensource.org/licenses/BSD-3-Clause for more information.
 */
#ifndef __MLPACK_CORE_TREE_HRECTBOUND_HPP
#define __MLPACK_CORE_TREE_HRECTBOUND_HPP

#include <mlpack/core.hpp>
#include <mlpack/core/math/range.hpp>
#include <mlpack/core/metrics/lmetric.hpp>
#include "bound_traits.hpp"

namespace mlpack {
namespace bound {

namespace meta /** Metaprogramming utilities. */ {

//! Utility struct where Value is true if and only if the argument is of type
//! LMetric.
template<typename MetricType>
struct IsLMetric
{
  static const bool Value = false;
};

//! Specialization for IsLMetric when the argument is of type LMetric.
template<int Power, bool TakeRoot>
struct IsLMetric<metric::LMetric<Power, TakeRoot>>
{
  static const bool Value = true;
};

} // namespace util

/**
 * Hyper-rectangle bound for an L-metric.  This should be used in conjunction
 * with the LMetric class.  Be sure to use the same template parameters for
 * LMetric as you do for HRectBound -- otherwise odd results may occur.
 *
 * @tparam Power The metric to use; use 2 for Euclidean (L2).
 * @tparam TakeRoot Whether or not the root should be taken (see LMetric
 *     documentation).
 */
template<typename MetricType = metric::LMetric<2, true>>
class HRectBound
{
  // It is required that HRectBound have an LMetric as the given MetricType.
  static_assert(meta::IsLMetric<MetricType>::Value == true,
      "HRectBound can only be used with the LMetric<> metric type.");

 public:
  /**
   * Empty constructor; creates a bound of dimensionality 0.
   */
  HRectBound();

  /**
   * Initializes to specified dimensionality with each dimension the empty
   * set.
   */
  HRectBound(const size_t dimension);

  //! Copy constructor; necessary to prevent memory leaks.
  HRectBound(const HRectBound& other);
  //! Same as copy constructor; necessary to prevent memory leaks.
  HRectBound& operator=(const HRectBound& other);

  //! Move constructor: take possession of another bound's information.
  HRectBound(HRectBound&& other);

  //! Destructor: clean up memory.
  ~HRectBound();

  /**
   * Resets all dimensions to the empty set (so that this bound contains
   * nothing).
   */
  void Clear();

  //! Gets the dimensionality.
  size_t Dim() const { return dim; }

  //! Get the range for a particular dimension.  No bounds checking.  Be
  //! careful: this may make MinWidth() invalid.
  math::Range& operator[](const size_t i) { return bounds[i]; }
  //! Modify the range for a particular dimension.  No bounds checking.
  const math::Range& operator[](const size_t i) const { return bounds[i]; }

  //! Get the minimum width of the bound.
  double MinWidth() const { return minWidth; }
  //! Modify the minimum width of the bound.
  double& MinWidth() { return minWidth; }

  /**
   * Calculates the center of the range, placing it into the given vector.
   *
   * @param center Vector which the center will be written to.
   */
  void Center(arma::vec& center) const;

  /**
   * Calculate the volume of the hyperrectangle.
   *
   * @return Volume of the hyperrectangle.
   */
  double Volume() const;

  /**
   * Calculates minimum bound-to-point distance.
   *
   * @param point Point to which the minimum distance is requested.
   */
  template<typename VecType>
  double MinDistance(const VecType& point,
                     typename boost::enable_if<IsVector<VecType> >* = 0) const;

  /**
   * Calculates minimum bound-to-bound distance.
   *
   * @param other Bound to which the minimum distance is requested.
   */
  double MinDistance(const HRectBound& other) const;

  /**
   * Calculates maximum bound-to-point squared distance.
   *
   * @param point Point to which the maximum distance is requested.
   */
  template<typename VecType>
  double MaxDistance(const VecType& point,
                     typename boost::enable_if<IsVector<VecType> >* = 0) const;

  /**
   * Computes maximum distance.
   *
   * @param other Bound to which the maximum distance is requested.
   */
  double MaxDistance(const HRectBound& other) const;

  /**
   * Calculates minimum and maximum bound-to-bound distance.
   *
   * @param other Bound to which the minimum and maximum distances are
   *     requested.
   */
  math::Range RangeDistance(const HRectBound& other) const;

  /**
   * Calculates minimum and maximum bound-to-point distance.
   *
   * @param point Point to which the minimum and maximum distances are
   *     requested.
   */
  template<typename VecType>
  math::Range RangeDistance(const VecType& point,
                            typename boost::enable_if<IsVector<VecType> >* = 0)
      const;

  /**
   * Expands this region to include new points.
   *
   * @tparam MatType Type of matrix; could be Mat, SpMat, a subview, or just a
   *   vector.
   * @param data Data points to expand this region to include.
   */
  template<typename MatType>
  HRectBound& operator|=(const MatType& data);

  /**
   * Expands this region to encompass another bound.
   */
  HRectBound& operator|=(const HRectBound& other);

  /**
   * Determines if a point is within this bound.
   */
  template<typename VecType>
  bool Contains(const VecType& point) const;

  /**
   * Returns the diameter of the hyperrectangle (that is, the longest diagonal).
   */
  double Diameter() const;

  /**
   * Serialize the bound object.
   */
  template<typename Archive>
  void Serialize(Archive& ar, const unsigned int version);

 private:
  //! The dimensionality of the bound.
  size_t dim;
  //! The bounds for each dimension.
  math::Range* bounds;
  //! Cached minimum width of bound.
  double minWidth;
};

// A specialization of BoundTraits for this class.
template<typename MetricType>
struct BoundTraits<HRectBound<MetricType>>
{
  //! These bounds are always tight for each dimension.
  const static bool HasTightBounds = true;
};

} // namespace bound
} // namespace mlpack

#include "hrectbound_impl.hpp"

#endif // __MLPACK_CORE_TREE_HRECTBOUND_HPP
