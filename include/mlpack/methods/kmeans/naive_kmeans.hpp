/**
 * @file naive_kmeans.hpp
 * @author Ryan Curtin
 *
 * An implementation of a naively-implemented step of the Lloyd algorithm for
 * k-means clustering.  This may still be the best choice for small datasets or
 * datasets with very high dimensionality.
 *
 * This file is part of mlpack 2.0.1.
 *
 * mlpack is free software; you may redstribute it and/or modify it under the
 * terms of the 3-clause BSD license.  You should have received a copy of the
 * 3-clause BSD license along with mlpack.  If not, see
 * http://www.opensource.org/licenses/BSD-3-Clause for more information.
 */
#ifndef __MLPACK_METHODS_KMEANS_NAIVE_KMEANS_HPP
#define __MLPACK_METHODS_KMEANS_NAIVE_KMEANS_HPP

namespace mlpack {
namespace kmeans {

/**
 * This is an implementation of a single iteration of Lloyd's algorithm for
 * k-means.  If your intention is to run the full k-means algorithm, you are
 * looking for the mlpack::kmeans::KMeans class instead of this one.  This class
 * is used by KMeans as the actual implementation of the Lloyd iteration.
 *
 * @param MetricType Type of metric used with this implementation.
 * @param MatType Matrix type (arma::mat or arma::sp_mat).
 */
template<typename MetricType, typename MatType>
class NaiveKMeans
{
 public:
  /**
   * Construct the NaiveKMeans object with the given dataset and metric.
   *
   * @param dataset Dataset.
   * @param metric Instantiated metric.
   */
  NaiveKMeans(const MatType& dataset, MetricType& metric);

  /**
   * Run a single iteration of the Lloyd algorithm, updating the given centroids
   * into the newCentroids matrix.
   *
   * @param centroids Current cluster centroids.
   * @param newCentroids New cluster centroids.
   */
  double Iterate(const arma::mat& centroids,
                 arma::mat& newCentroids,
                 arma::Col<size_t>& counts);

  size_t DistanceCalculations() const { return distanceCalculations; }

 private:
  //! The dataset.
  const MatType& dataset;
  //! The instantiated metric.
  MetricType& metric;

  //! Number of distance calculations.
  size_t distanceCalculations;
};

} // namespace kmeans
} // namespace mlpack

// Include implementation.
#include "naive_kmeans_impl.hpp"

#endif
