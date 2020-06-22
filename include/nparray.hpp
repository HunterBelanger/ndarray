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

#include<vector>
#include<string>

template<class T>
class NPArray {
  public:
    //==========================================================================
    // Constructors and Destructors
    NPArray(std::vector<size_t> init_shape);
    NPArray(std::vector<T> data, std::vector<size_t> init_shape,
        bool c_continuous=true);
    ~NPArray() = default;

    // Static load function
    static NPArray load(std::string fname);

    //==========================================================================
    // Indexing
    
    // Indexing operators for indexing with vector
    T& operator()(std::vector<size_t> indicies);
    const T& operator()(std::vector<size_t> indicies) const;

    // Variadic indexing operators
    // TODO
    
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
    bool c_continuous_;

    size_t c_continuous_index(const std::vector<size_t>& indicies) const;
    size_t fortran_continuous_index(const std::vector<size_t>& indicies) const;
};

#endif // NP_ARRAY_H
