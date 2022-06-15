#pragma once
#include "stdafx.h"
#include <vector>

template <typename T>
class dynamic_array
{
public:
  dynamic_array(){};
  dynamic_array(int rows, int cols)
  {
    for(int i=0; i<rows; ++i)
    {
      data_.push_back(std::vector<T>(cols));
    }
  }
  
  dynamic_array(int rows, int cols, T val)
  {
    for(int i=0; i<rows; ++i)
    {
      data_.push_back(std::vector<T>(cols, val));
    }
  }  
  // other ctors ....

  inline std::vector<T> & operator[](int i) { return data_[i]; }

  inline const std::vector<T> & operator[] (int i) const { return data_[i]; }

  // other accessors, like at() ...

  void resize(int rows, int cols)
  {
    data_.resize(rows);
    for(int i = 0; i < rows; ++i)
      data_[i].resize(cols);
  }

  void resize(int rows, int cols, T Value)
  {
    data_.resize(rows);
    for(int i = 0; i < rows; ++i)
      data_[i].resize(cols, Value);
  }

  unsigned rows()
  {
    return data_.size();
  }

  unsigned cols()
  {
	if( data_.size() > 0)
		return data_[0].size();
	else
		return 0;
  }

  void fill(const T& value)
  {
		for(unsigned int i = 0; i < data_.size(); ++i)
			for(unsigned int j = 0; j < cols(); ++j)
					data_[i][j] = value;
  }

  void clear()
  {
     //for(unsigned int i = 0; i < data_.size(); ++i)
	//	 data_[i].clear();
	 data_.clear();
  }
  // other member functions, like reserve()....

private:
  std::vector<std::vector<T> > data_;  
};
