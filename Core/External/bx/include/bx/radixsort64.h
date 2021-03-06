/*
* Copyright 2010-2016 Branimir Karadzic. All rights reserved.
* License: https://github.com/bkaradzic/bx#license-bsd-2-clause
*/

// Converted to C by Robin Southern 24-11-16

#ifndef BX_RADIXSORT64_FN_NAME
#define BX_RADIXSORT64_FN_NAME radixSort64
#endif

#ifndef BX_RADIXSORT64_USER_TYPE
#define BX_RADIXSORT64_USER_TYPE uint32_t
#endif

#define BX_RADIXSORT64_BITS 11
#define BX_RADIXSORT64_HISTOGRAM_SIZE (1<<BX_RADIXSORT64_BITS)
#define BX_RADIXSORT64_BIT_MASK (BX_RADIXSORT64_HISTOGRAM_SIZE-1)

void BX_RADIXSORT64_FN_NAME(uint64_t* __restrict _keys, uint64_t* __restrict _tempKeys, BX_RADIXSORT64_USER_TYPE* __restrict _values, BX_RADIXSORT64_USER_TYPE* __restrict _tempValues, uint32_t _size)
{
  uint64_t* __restrict keys = _keys;
  uint64_t* __restrict tempKeys = _tempKeys;
  BX_RADIXSORT64_USER_TYPE* __restrict values = _values;
  BX_RADIXSORT64_USER_TYPE* __restrict tempValues = _tempValues;

  uint32_t histogram[BX_RADIXSORT64_HISTOGRAM_SIZE];
  uint16_t shift = 0;
  uint32_t pass = 0;
  for (; pass < 6; ++pass)
  {
    memset(histogram, 0, sizeof(uint32_t)*BX_RADIXSORT64_HISTOGRAM_SIZE);

    bool sorted = true;
    {
      uint64_t key = keys[0];
      uint64_t prevKey = key;
      for (uint32_t ii = 0; ii < _size; ++ii, prevKey = key)
      {
        key = keys[ii];
        uint16_t index = (key>>shift)&BX_RADIXSORT64_BIT_MASK;
        ++histogram[index];
        sorted &= prevKey <= key;
      }
    }

    if (sorted)
    {
      goto done;
    }

    uint32_t offset = 0;
    for (uint32_t ii = 0; ii < BX_RADIXSORT64_HISTOGRAM_SIZE; ++ii)
    {
      uint32_t count = histogram[ii];
      histogram[ii] = offset;
      offset += count;
    }

    for (uint32_t ii = 0; ii < _size; ++ii)
    {
      uint64_t key = keys[ii];
      uint16_t index = (key>>shift)&BX_RADIXSORT64_BIT_MASK;
      uint32_t dest = histogram[index]++;
      tempKeys[dest] = key;
      tempValues[dest] = values[ii];
    }

    uint64_t* swapKeys = tempKeys;
    tempKeys = keys;
    keys = swapKeys;

    BX_RADIXSORT64_USER_TYPE* swapValues = tempValues;
    tempValues = values;
    values = swapValues;

    shift += BX_RADIXSORT64_BITS;
  }

  done:
  if (0 != (pass&1) )
  {
    // Odd number of passes needs to do copy to the destination.
    memcpy(_keys, _tempKeys, _size*sizeof(uint64_t) );
    for (uint32_t ii = 0; ii < _size; ++ii)
    {
      _values[ii] = _tempValues[ii];
    }
  }
}

#undef BX_RADIXSORT64_BITS
#undef BX_RADIXSORT64_HISTOGRAM_SIZE
#undef BX_RADIXSORT64_BIT_MASK
#undef BX_RADIXSORT64_FN_NAME
#undef BX_RADIXSORT64_USER_TYPE
