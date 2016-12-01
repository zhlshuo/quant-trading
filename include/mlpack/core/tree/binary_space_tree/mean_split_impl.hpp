/**
 * @file mean_split_impl.hpp
 * @author Yash Vadalia
 * @author Ryan Curtin
 *
 * Implementation of class(MeanSplit) to split a binary space partition tree.
 *
 * This file is part of mlpack 2.0.1.
 *
 * mlpack is free software; you may redstribute it and/or modify it under the
 * terms of the 3-clause BSD license.  You should have received a copy of the
 * 3-clause BSD license along with mlpack.  If not, see
 * http://www.opensource.org/licenses/BSD-3-Clause for more information.
 */
#ifndef __MLPACK_CORE_TREE_BINARY_SPACE_TREE_MEAN_SPLIT_IMPL_HPP
#define __MLPACK_CORE_TREE_BINARY_SPACE_TREE_MEAN_SPLIT_IMPL_HPP

#include "mean_split.hpp"

namespace mlpack {
namespace tree {

template<typename BoundType, typename MatType>
bool MeanSplit<BoundType, MatType>::SplitNode(const BoundType& bound,
                                              MatType& data,
                                              const size_t begin,
                                              const size_t count,
                                              size_t& splitCol)
{
  size_t splitDimension = data.n_rows; // Indicate invalid.
  double maxWidth = -1;

  // Find the split dimension.  If the bound is tight, we only need to consult
  // the bound's width.
  if (bound::BoundTraits<BoundType>::HasTightBounds)
  {
    for (size_t d = 0; d < data.n_rows; d++)
    {
      const double width = bound[d].Width();

      if (width > maxWidth)
      {
        maxWidth = width;
        splitDimension = d;
      }
    }
  }
  else
  {
    // We must individually calculate bounding boxes.
    math::Range* ranges = new math::Range[data.n_rows];
    for (size_t i = begin; i < begin + count; ++i)
    {
      // Expand each dimension as necessary.
      for (size_t d = 0; d < data.n_rows; ++d)
      {
        const double val = data(d, i);
        if (val < ranges[d].Lo())
          ranges[d].Lo() = val;
        if (val > ranges[d].Hi())
          ranges[d].Hi() = val;
      }
    }

    // Now, which is the widest?
    for (size_t d = 0; d < data.n_rows; d++)
    {
      const double width = ranges[d].Width();
      if (width > maxWidth)
      {
        maxWidth = width;
        splitDimension = d;
      }
    }

    delete[] ranges;
  }

  if (maxWidth == 0) // All these points are the same.  We can't split.
    return false;

  // Split in the mean of that dimension.
  double splitVal = 0.0;
  for (size_t i = begin; i < begin + count; ++i)
    splitVal += data(splitDimension, i);
  splitVal /= count;

  Log::Assert(splitVal >= bound[splitDimension].Lo());
  Log::Assert(splitVal <= bound[splitDimension].Hi());

  // Perform the actual splitting.  This will order the dataset such that points
  // with value in dimension splitDimension less than or equal to splitVal are
  // on the left of splitCol, and points with value in dimension splitDimension
  // greater than splitVal are on the right side of splitCol.
  splitCol = PerformSplit(data, begin, count, splitDimension, splitVal);

  return true;
}

template<typename BoundType, typename MatType>
bool MeanSplit<BoundType, MatType>::SplitNode(const BoundType& bound,
                                              MatType& data,
                                              const size_t begin,
                                              const size_t count,
                                              size_t& splitCol,
                                              std::vector<size_t>& oldFromNew)
{
  size_t splitDimension = data.n_rows; // Indicate invalid.
  double maxWidth = -1;

  // Find the split dimension.  If the bound is tight, we only need to consult
  // the bound's width.
  if (bound::BoundTraits<BoundType>::HasTightBounds)
  {
    for (size_t d = 0; d < data.n_rows; d++)
    {
      const double width = bound[d].Width();

      if (width > maxWidth)
      {
        maxWidth = width;
        splitDimension = d;
      }
    }
  }
  else
  {
    // We must individually calculate bounding boxes.
    math::Range* ranges = new math::Range[data.n_rows];
    for (size_t i = begin; i < begin + count; ++i)
    {
      // Expand each dimension as necessary.
      for (size_t d = 0; d < data.n_rows; ++d)
      {
        const double val = data(d, i);
        if (val < ranges[d].Lo())
          ranges[d].Lo() = val;
        if (val > ranges[d].Hi())
          ranges[d].Hi() = val;
      }
    }

    // Now, which is the widest?
    for (size_t d = 0; d < data.n_rows; d++)
    {
      const double width = ranges[d].Width();

      if (width > maxWidth)
      {
        maxWidth = width;
        splitDimension = d;
      }
    }

    delete[] ranges;
  }

  if (maxWidth == 0) // All these points are the same.  We can't split.
    return false;

  // Split in the mean of that dimension.
  double splitVal = 0.0;
  for (size_t i = begin; i < begin + count; ++i)
    splitVal += data(splitDimension, i);
  splitVal /= count;

  Log::Assert(splitVal >= bound[splitDimension].Lo());
  Log::Assert(splitVal <= bound[splitDimension].Hi());

  // Perform the actual splitting.  This will order the dataset such that points
  // with value in dimension splitDimension less than or equal to splitVal are
  // on the left of splitCol, and points with value in dimension splitDimension
  // greater than splitVal are on the right side of splitCol.
  splitCol = PerformSplit(data, begin, count, splitDimension, splitVal,
      oldFromNew);

  return true;
}

template<typename BoundType, typename MatType>
size_t MeanSplit<BoundType, MatType>::
    PerformSplit(MatType& data,
                 const size_t begin,
                 const size_t count,
                 const size_t splitDimension,
                 const double splitVal)
{
  // This method modifies the input dataset.  We loop both from the left and
  // right sides of the points contained in this node.  The points less than
  // splitVal should be on the left side of the matrix, and the points greater
  // than splitVal should be on the right side of the matrix.
  size_t left = begin;
  size_t right = begin + count - 1;

  // First half-iteration of the loop is out here because the termination
  // condition is in the middle.
  while ((data(splitDimension, left) < splitVal) && (left <= right))
    left++;
  while ((data(splitDimension, right) >= splitVal) && (left <= right) && (right > 0))
    right--;

  while (left <= right)
  {
    // Swap columns.
    data.swap_cols(left, right);

    // See how many points on the left are correct.  When they are correct,
    // increase the left counter accordingly.  When we encounter one that isn't
    // correct, stop.  We will switch it later.
    while ((data(splitDimension, left) < splitVal) && (left <= right))
      left++;

    // Now see how many points on the right are correct.  When they are correct,
    // decrease the right counter accordingly.  When we encounter one that isn't
    // correct, stop.  We will switch it with the wrong point we found in the
    // previous loop.
    while ((data(splitDimension, right) >= splitVal) && (left <= right))
      right--;
  }

  Log::Assert(left == right + 1);

  return left;
}

template<typename BoundType, typename MatType>
size_t MeanSplit<BoundType, MatType>::
    PerformSplit(MatType& data,
                 const size_t begin,
                 const size_t count,
                 const size_t splitDimension,
                 const double splitVal,
                 std::vector<size_t>& oldFromNew)
{
  // This method modifies the input dataset.  We loop both from the left and
  // right sides of the points contained in this node.  The points less than
  // splitVal should be on the left side of the matrix, and the points greater
  // than splitVal should be on the right side of the matrix.
  size_t left = begin;
  size_t right = begin + count - 1;

  // First half-iteration of the loop is out here because the termination
  // condition is in the middle.
  while ((data(splitDimension, left) < splitVal) && (left <= right))
    left++;
  while ((data(splitDimension, right) >= splitVal) && (left <= right) && (right > 0))
    right--;

  while (left <= right)
  {
    // Swap columns.
    data.swap_cols(left, right);

    // Update the indices for what we changed.
    size_t t = oldFromNew[left];
    oldFromNew[left] = oldFromNew[right];
    oldFromNew[right] = t;

    // See how many points on the left are correct.  When they are correct,
    // increase the left counter accordingly.  When we encounter one that isn't
    // correct, stop.  We will switch it later.
    while ((data(splitDimension, left) < splitVal) && (left <= right))
      left++;

    // Now see how many points on the right are correct.  When they are correct,
    // decrease the right counter accordingly.  When we encounter one that isn't
    // correct, stop.  We will switch it with the wrong point we found in the
    // previous loop.
    while ((data(splitDimension, right) >= splitVal) && (left <= right))
      right--;
  }

  Log::Assert(left == right + 1);

  return left;
}

} // namespace tree
} // namespace mlpack

#endif
