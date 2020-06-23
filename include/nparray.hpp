/*
 * NPArray
 *
 * Copyright (c) 2020, Hunter Belanger (hunter.belanger@gmail.com)
 * All rights reserved.
 *
 * Released under the terms and conditions of the BSD 3-Clause
 * license. Please refer to the LICENSE file in the root directory
 * of this distribution for more information.
 *
 * */
#ifndef NP_ARRAY_H
#define NP_ARRAY_H

#include<array>
#include<vector>
#include<string>
#include<stdexcept>

#include<iostream>

template<class T>
class NPArray {
  private:
    bool c_continuous_;
    size_t dimensions_;

  public:
    //==========================================================================
    // Constructors and Destructors
    NPArray();
    NPArray(std::vector<size_t> init_shape, bool c_continuous=true);
    NPArray(std::vector<T> data, std::vector<size_t> init_shape,
        bool c_continuous=true);
    ~NPArray() = default;

    // Static load function
    static NPArray load(std::string fname);

    //==========================================================================
    // Indexing
    
    // Indexing operators for indexing with vector
    T& operator()(const std::vector<size_t>& indices);
    const T& operator()(const std::vector<size_t>& indices) const;

    // Variadic indexing operators
    // Access data with array idicies.
    template <typename... INDS>
    T& operator()(INDS... inds) {
      std::array<size_t, sizeof...(inds)> indices{static_cast<size_t>(inds)...};

      check_indices(indices);

      size_t indx;
      if (c_continuous_) {
        // Get linear index for row-major order
        indx = c_continuous_index(indices);
      } else {
        // Get linear index for column-major order
        indx = fortran_continuous_index(indices);
      }
      return data_[indx];
    }

    template <typename... INDS>
    const T& operator()(INDS... inds) const {
      std::array<size_t, sizeof...(inds)> indices{static_cast<size_t>(inds)...};

      check_indices(indices);
     
      size_t indx;
      if (c_continuous_) {
        // Get linear index for row-major order
        indx = c_continuous_index(indices);
      } else {
        // Get linear index for column-major order
        indx = fortran_continuous_index(indices);
      }
      return data_[indx];
    }
    
    // Linear Indexing operators
    T& operator[](size_t i);
    const T& operator[](size_t i) const;
    
    //========================================================================== 
    // Constant Methods
    
    // Return vector describing shape of array 
    std::vector<size_t> shape() const;

    // Return number of elements in array
    size_t size() const;
    
    // Returns true if data is stored as c continuous (row-major order),
    // and false if fortran continuous (column-major order)
    bool c_continuous() const;
    
    // Save array to the file fname.npy
    void save(std::string fname) const;

    //==========================================================================
    // Non-Constant Methods
    
    // Will reshape the array to the given dimensions
    void reshape(std::vector<size_t> new_shape);

    // Realocates array to fit the new size.
    // DATA CAN BE LOST IF ARRAY IS SHRUNK
    void reallocate(std::vector<size_t> new_shape);

    //==========================================================================
    // Operators
    // TODO

  private:
    std::vector<T> data_;
    std::vector<size_t> shape_;

    void check_indices(const std::vector<size_t>& indices) const;

    template<size_t D>
    void check_indices(const std::array<size_t, D>& indices) const {
      // Make sure proper number of indices
      if(indices.size() != dimensions_) {
        std::string mssg = "Improper number of indicies provided to NPArray."; 
        throw std::runtime_error(mssg);
      }

      // Make sure all index values are valid
      for(size_t i = 0; i < dimensions_; i++) {
        if(indices[i] >= shape_[i]) {
          std::string mssg = "Index provided to NPArray out of range."; 
          throw std::out_of_range(mssg);
        }
      }
    }

    size_t c_continuous_index(const std::vector<size_t>& indices) const;
    size_t fortran_continuous_index(const std::vector<size_t>& indices) const;
    
    template<size_t D>
    size_t c_continuous_index(const std::array<size_t, D>& indices) const {
      size_t indx = indices[indices.size() - 1];
      size_t coeff = 1;

      for(size_t i = indices.size() - 1; i > 0; i--) {
        coeff *= shape_[i];
        indx += coeff * indices[i-1];
      }

      return indx;
    }

    template<size_t D>
    size_t fortran_continuous_index(const std::array<size_t, D>& indices) const {
      size_t indx = indices[0];
      size_t coeff = 1;

      for (size_t i = 0; i < indices.size()-1; i++) {
        coeff *= shape_[i];
        indx += coeff * indices[i+1];
      }

      return indx;
    }
};

#endif // NP_ARRAY_H
