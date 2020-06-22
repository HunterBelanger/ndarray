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
#include<nparray.hpp>

#include"npy.hpp"

#include<stdexcept>
#include<typeinfo>

template<class T>
NPArray<T>::NPArray(std::vector<size_t> init_shape) {
  if(init_shape.size() > 0) {
    shape_ = init_shape;

    size_t ne = init_shape[0];
    for(size_t i = 1; i < init_shape.size(); i++) {
      ne *= init_shape[i]; 
    }

    data_.resize(ne);

    c_continuous_ = true;

  } else {
    std::string mssg = "NPArray shape vector must have at least one element."; 
    throw std::runtime_error(mssg);
  }
}

template<class T>
NPArray<T>::NPArray(std::vector<T> data, std::vector<size_t> init_shape, bool c_continuous) {
  if(init_shape.size() > 0) {
    shape_ = init_shape;

    size_t ne = init_shape[0];
    for(size_t i = 1; i < init_shape.size(); i++) {
      ne *= init_shape[i]; 
    }

    if(ne != data.size()) {
      std::string mssg = "Shape is incompatible with number of elements provided for NPArray.";
      throw std::runtime_error(mssg);
    }

    data_ = data;

    c_continuous_ = c_continuous;

  } else {
    std::string mssg = "Shape vector must have at least one element for NPArray."; 
    throw std::runtime_error(mssg);
  }
}

template<class T>
NPArray<T> NPArray<T>::load(std::string fname) {
  // Get expected DType according to T
  DType expected_dtype;
  const char* T_type_name = typeid(T).name();

  if(T_type_name == typeid(char).name()) expected_dtype = DType::CHAR;
  else if(T_type_name == typeid(unsigned char).name()) expected_dtype = DType::UCHAR;
  else if(T_type_name == typeid(uint16_t).name()) expected_dtype = DType::UINT16;
  else if(T_type_name == typeid(uint32_t).name()) expected_dtype = DType::UINT32;
  else if(T_type_name == typeid(uint64_t).name()) expected_dtype = DType::UINT64;
  else if(T_type_name == typeid(int16_t).name()) expected_dtype = DType::INT16;
  else if(T_type_name == typeid(int32_t).name()) expected_dtype = DType::INT32;
  else if(T_type_name == typeid(int64_t).name()) expected_dtype = DType::INT64;
  else if(T_type_name == typeid(float).name()) expected_dtype = DType::FLOAT32;
  else if(T_type_name == typeid(double).name()) expected_dtype = DType::DOUBLE64;
  else {
    std::string mssg = "The datatype is not supported for NPArray."; 
    throw std::runtime_error(mssg);
  }

  // Variables to send to npy function
  char* data_ptr;
  std::vector<T> data_vector;
  std::vector<size_t> data_shape;
  DType data_dtype;
  bool data_c_continuous;

  // Load data into variables
  load_npy(fname, data_ptr, data_shape, data_dtype, data_c_continuous);

  // Ensuire DType variables match
  if(expected_dtype != data_dtype) {
    std::string mssg = "NPArray template datatype does not match specified datatype in npy file."; 
    throw std::runtime_error(mssg);
  }

  if(data_shape.size() < 1) {
    std::string mssg = "Shape vector must have at least one element for NPArray."; 
    throw std::runtime_error(mssg);
  }
  
  // Number of elements
  size_t ne = data_shape[0];
  for(size_t i = 1; i < data_shape.size(); i++) {
    ne *= data_shape[i]; 
  }

  data_vector = {reinterpret_cast<T*>(data_ptr), reinterpret_cast<T*>(data_ptr)+ne};

  // Create NPArray object
  NPArray<T> return_object(data_vector, data_shape);
  return_object.c_continuous_ = data_c_continuous;

  // Free data_ptr
  delete[] data_ptr;

  // Return object
  return return_object;
}

template<class T>
T& NPArray<T>::operator()(std::vector<size_t> indicies) {
  if (indicies.size() != shape_.size()) {
    std::string mssg = "Improper number of indicies provided to NPArray.";
    throw std::runtime_error(mssg);
  } else {
    size_t indx;
    if (c_continuous_) {
      // Get linear index for row-major order
      indx = c_continuous_index(indicies);
    } else {
      // Get linear index for column-major order
      indx = fortran_continuous_index(indicies);
    }
    return data_[indx];
  }
}

template<class T>
const T& NPArray<T>::operator()(std::vector<size_t> indicies) const {
  if (indicies.size() != shape_.size()) {
    std::string mssg = "Improper number of indicies provided to NPArray.";
    throw std::runtime_error(mssg);
  } else {
    size_t indx;
    if (c_continuous_) {
      // Get linear index for row-major order
      indx = c_continuous_index(indicies);
    } else {
      // Get linear index for column-major order
      indx = fortran_continuous_index(indicies);
    }
    return data_[indx];
  }
}

template<class T>
T& NPArray<T>::operator[](size_t i) {
  if(i >= data_.size()) {
    std::string mssg = "Invalid linear index for NPArray.";
    throw std::runtime_error(mssg);
  }

  return data_[i];
}

template<class T>
const T& NPArray<T>::operator[](size_t i) const {
  if(i >= data_.size()) {
    std::string mssg = "Invalid linear index for NPArray.";
    throw std::runtime_error(mssg);
  }

  return data_[i];
}

template<class T>
std::vector<size_t> NPArray<T>::shape() const {return shape_;}

template<class T>
size_t NPArray<T>::size() const {return data_.size();}

template<class T>
bool NPArray<T>::c_continuous() const {return c_continuous_;}

template<class T>
void NPArray<T>::save(std::string fname) const {
  // Get expected DType according to T
  DType dtype;
  const char* T_type_name = typeid(T).name();

  if(T_type_name == typeid(char).name()) dtype = DType::CHAR;
  else if(T_type_name == typeid(unsigned char).name()) dtype = DType::UCHAR;
  else if(T_type_name == typeid(uint16_t).name()) dtype = DType::UINT16;
  else if(T_type_name == typeid(uint32_t).name()) dtype = DType::UINT32;
  else if(T_type_name == typeid(uint64_t).name()) dtype = DType::UINT64;
  else if(T_type_name == typeid(int16_t).name()) dtype = DType::INT16;
  else if(T_type_name == typeid(int32_t).name()) dtype = DType::INT32;
  else if(T_type_name == typeid(int64_t).name()) dtype = DType::INT64;
  else if(T_type_name == typeid(float).name()) dtype = DType::FLOAT32;
  else if(T_type_name == typeid(double).name()) dtype = DType::DOUBLE64;
  else {
    std::string mssg = "The templated datatype is not supported by NPArray."; 
    throw std::runtime_error(mssg);
  }

  // Write data to file
  write_npy(fname, reinterpret_cast<const char*>(data_.data()), shape_, dtype, c_continuous_);
}

template<class T>
void NPArray<T>::reshape(std::vector<size_t> new_shape) {
  // Ensure new shape has proper dimensions
  if(new_shape.size() < 1) {
    std::string mssg = "Shape vector must have at least one element to reshpae NPArray.";
    throw std::runtime_error(mssg);
  } else {
    size_t ne = new_shape[0];
    
    for(size_t i = 1; i < new_shape.size(); i++) {
      ne *= new_shape[i];
    }

    if(ne == data_.size()) shape_ = new_shape;
    else {
      std::string mssg = "Shape is incompatible with number of elements in NPArray.";
      throw std::runtime_error(mssg);
    }
  }
}

template<class T>
void NPArray<T>::reallocate(std::vector<size_t> new_shape) {
  // Ensure new shape has proper dimensions
  if(new_shape.size() < 1) {
    std::string mssg = "Shape vector must have at least one element to reallocate NPArray.";
    throw std::runtime_error(mssg);
  } else {
    size_t ne = new_shape[0];
    
    for(size_t i = 1; i < new_shape.size(); i++) {
      ne *= new_shape[i];
    }

    shape_ = new_shape;
    data_.resize(ne);
  }
}

template<class T>
size_t NPArray<T>::c_continuous_index(const std::vector<size_t>& indicies) const {
  size_t indx = 0;
  // Go through all dimensions
  for (size_t k = 1; k <= shape_.size(); k++) {
    size_t product = 0;
    if (k == shape_.size())
      product = 1;
    else {
      for (size_t l = k + 1; l <= shape_.size(); l++) {
        if (product == 0)
          product = shape_[l - 1];
        else
          product *= shape_[l - 1];
      }
    }

    indx += product * indicies[k - 1];
  }
  return indx;
}

template<class T>
size_t NPArray<T>::fortran_continuous_index(const std::vector<size_t>& indicies) const {
  size_t indx = 0;
  // Go through all dimensions
  for (size_t k = 1; k <= shape_.size(); k++) {
    size_t product = 0;
    if (k == 1)
      product = 1;
    else {
      for (size_t l = 1; l <= k - 1; l++) {
        if (product == 0)
          product = shape_[l - 1];
        else
          product *= shape_[l - 1];
      }
    }

    indx += product * indicies[k - 1];
  }
  return indx;
}

// Explicit Initialization of Templates
// These are the only supported templates, due to compatability with Numpy.
template class NPArray<char>;
template class NPArray<unsigned char>;
template class NPArray<uint16_t>;
template class NPArray<uint32_t>;
template class NPArray<uint64_t>;
template class NPArray<int16_t>;
template class NPArray<int32_t>;
template class NPArray<int64_t>;
template class NPArray<float>;
template class NPArray<double>;
