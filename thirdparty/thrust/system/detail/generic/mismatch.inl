/*
 *  Copyright 2008-2012 NVIDIA Corporation
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include <thrust/detail/config.h>
#include <thrust/system/detail/generic/mismatch.h>
#include <thrust/iterator/iterator_traits.h>
#include <thrust/detail/internal_functional.h>
#include <thrust/find.h>

namespace thrust
{
namespace system
{
namespace detail
{
namespace generic
{

template <typename InputIterator1, typename InputIterator2>
  thrust::pair<InputIterator1, InputIterator2>
    mismatch(tag,
             InputIterator1 first1,
             InputIterator1 last1,
             InputIterator2 first2)
{
  typedef typename thrust::iterator_value<InputIterator1>::type InputType1;
  
  // XXX use a placeholder expression here
  return thrust::mismatch(first1, last1, first2, thrust::detail::equal_to<InputType1>());
} // end mismatch()

template <typename InputIterator1, typename InputIterator2, typename BinaryPredicate>
  thrust::pair<InputIterator1, InputIterator2>
    mismatch(tag,
             InputIterator1 first1,
             InputIterator1 last1,
             InputIterator2 first2,
             BinaryPredicate pred)
{
  // Contributed by Erich Elsen
  typedef thrust::tuple<InputIterator1,InputIterator2> IteratorTuple;
  typedef thrust::zip_iterator<IteratorTuple>          ZipIterator;
  
  ZipIterator zipped_first = thrust::make_zip_iterator(thrust::make_tuple(first1,first2));
  ZipIterator zipped_last  = thrust::make_zip_iterator(thrust::make_tuple(last1, first2));
  
  ZipIterator result = thrust::find_if_not(zipped_first, zipped_last, thrust::detail::tuple_binary_predicate<BinaryPredicate>(pred));
  
  return thrust::make_pair(thrust::get<0>(result.get_iterator_tuple()),
                           thrust::get<1>(result.get_iterator_tuple()));
} // end mismatch()

} // end generic
} // end detail
} // end system
} // end thrust
