/**
 * @file dataset_info.hpp
 * @author Ryan Curtin
 *
 * Defines the DatasetInfo class, which holds information about a dataset.  This
 * is useful when the dataset contains categorical non-numeric features that
 * needs to be mapped to categorical numeric features.
 *
 * This file is part of mlpack 2.0.1.
 *
 * mlpack is free software; you may redstribute it and/or modify it under the
 * terms of the 3-clause BSD license.  You should have received a copy of the
 * 3-clause BSD license along with mlpack.  If not, see
 * http://www.opensource.org/licenses/BSD-3-Clause for more information.
 */
#ifndef __MLPACK_CORE_DATA_DATASET_INFO_HPP
#define __MLPACK_CORE_DATA_DATASET_INFO_HPP

#include <mlpack/core.hpp>
#include <unordered_map>
#include <boost/bimap.hpp>

namespace mlpack {
namespace data {

/**
 * The Datatype enum specifies the types of data mlpack algorithms can use.  The
 * vast majority of mlpack algorithms can only use numeric data (i.e.
 * float/double/etc.), but some algorithms can use categorical data, specified
 * via this Datatype enum and the DatasetInfo class.
 */
enum Datatype : bool /* bool is all the precision we need for two types */
{
  numeric = 0,
  categorical = 1
};

/**
 * Auxiliary information for a dataset, including mappings to/from strings and
 * the datatype of each dimension.  DatasetInfo objects are optionally produced
 * by data::Load(), and store the type of each dimension (Datatype::NUMERIC or
 * Datatype::CATEGORICAL) as well as mappings from strings to unsigned integers
 * and vice versa.
 */
class DatasetInfo
{
 public:
  /**
   * Create the DatasetInfo object with the given dimensionality.  Note that the
   * dimensionality cannot be changed later; you will have to create a new
   * DatasetInfo object.
   */
  DatasetInfo(const size_t dimensionality = 0);

  /**
   * Given the string and the dimension to which it belongs, return its numeric
   * mapping.  If no mapping yet exists, the string is added to the list of
   * mappings for the given dimension.  The dimension parameter refers to the
   * index of the dimension of the string (i.e. the row in the dataset).
   *
   * @param string String to find/create mapping for.
   * @param dimension Index of the dimension of the string.
   */
  size_t MapString(const std::string& string, const size_t dimension);

  const std::string& UnmapString(const size_t value, const size_t dimension);

  Datatype Type(const size_t dimension) const;
  Datatype& Type(const size_t dimension);

  size_t NumMappings(const size_t dimension) const;

  size_t Dimensionality() const;

  template<typename Archive>
  void Serialize(Archive& ar, const unsigned int /* version */)
  {
    ar & data::CreateNVP(types, "types");
    ar & data::CreateNVP(maps, "maps");
  }

 private:
  std::vector<Datatype> types;

  // Map entries will only exist for dimensions that are categorical.
  std::unordered_map<size_t, std::pair<boost::bimap<std::string, size_t>,
      size_t>> maps;

};

} // namespace data
} // namespace mlpack

#include "dataset_info_impl.hpp"

#endif
