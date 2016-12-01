/**
 * @file x_tre_split.hpp
 * @author Andrew Wells
 *
 * Defintion of the XTreeSplit class, a class that splits the nodes of an X
 * tree, starting at a leaf node and moving upwards if necessary.
 *
 * This is known to have a bug: see #368.
 *
 * This file is part of mlpack 2.0.1.
 *
 * mlpack is free software; you may redstribute it and/or modify it under the
 * terms of the 3-clause BSD license.  You should have received a copy of the
 * 3-clause BSD license along with mlpack.  If not, see
 * http://www.opensource.org/licenses/BSD-3-Clause for more information.
 */
#ifndef __MLPACK_CORE_TREE_RECTANGLE_TREE_X_TREE_SPLIT_HPP
#define __MLPACK_CORE_TREE_RECTANGLE_TREE_X_TREE_SPLIT_HPP

#include <mlpack/core.hpp>

namespace mlpack {
namespace tree /** Trees and tree-building procedures. */ {

/**
 * The X-tree paper says that a maximum allowable overlap of 20% works well.
 *
 * This code should eventually be refactored so as to avoid polluting
 * mlpack::tree with this random double.
 */
const double MAX_OVERLAP = 0.2;

/**
 * A Rectangle Tree has new points inserted at the bottom.  When these
 * nodes overflow, we split them, moving up the tree and splitting nodes
 * as necessary.
 */
class XTreeSplit
{
 public:
  /**
   * Split a leaf node using the algorithm described in "The R*-tree: An
   * Efficient and Robust Access method for Points and Rectangles."  If
   * necessary, this split will propagate upwards through the tree.
   */
  template<typename TreeType>
  static void SplitLeafNode(TreeType* tree, std::vector<bool>& relevels);

  /**
   * Split a non-leaf node using the "default" algorithm.  If this is a root
   * node, the tree increases in depth.
   */
  template<typename TreeType>
  static bool SplitNonLeafNode(TreeType* tree, std::vector<bool>& relevels);

 private:
  /**
   * Class to allow for faster sorting.
   */
  class sortStruct
  {
   public:
    double d;
    int n;
  };

  /**
   * Comparator for sorting with sortStruct.
   */
  static bool structComp(const sortStruct& s1, const sortStruct& s2)
  {
    return s1.d < s2.d;
  }

  /**
   * Insert a node into another node.
   */
  template<typename TreeType>
  static void InsertNodeIntoTree(TreeType* destTree, TreeType* srcNode);
};

} // namespace tree
} // namespace mlpack

// Include implementation
#include "x_tree_split_impl.hpp"

#endif
