/*============================================================================
  Copyright 2017 Koichi Murakami

  Distributed under the OSI-approved BSD License (the "License");
  see accompanying file Copyright.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the License for more information.
============================================================================*/
#ifndef JSON_PARSER_H_
#define JSON_PARSER_H_

#include <string>
#include <vector>
#include "util/picojson.h"

#ifndef DISALLOW_COPY_AND_ASSIGN
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&) = delete;      \
  void operator=(const TypeName&) = delete
#endif

class JsonParser {
public:
  typedef std::vector<bool> barray_t;
  typedef std::vector<int> iarray_t;
  typedef std::vector<double> darray_t;
  typedef std::vector<std::string> sarray_t;

  static JsonParser* GetJsonParser();
  ~JsonParser();

  bool LoadFile(const std::string& fname);

  bool Contains(const char* key) const;

  bool GetBoolValue(const char* key) const;
  int GetIntValue(const char* key) const;
  double GetDoubleValue(const char* key) const;
  std::string GetStringValue(const char* key) const;

  std::size_t GetBoolArray(const char* key, barray_t& barray) const;
  std::size_t GetIntArray(const char* key, iarray_t& iarray) const;
  std::size_t GetDoubleArray(const char* key, darray_t& darray) const;
  std::size_t GetStringArray(const char* key, sarray_t& sarray) const;

  void DumpAll() const;

private:
  JsonParser();
  DISALLOW_COPY_AND_ASSIGN(JsonParser);

  picojson::value data_map_;

};

#endif
