/**
 * @author davinliu
 * @version 2018-03-31
 * @Last revision 2018-03-31
 * 
 */ 

#ifndef _EJSON_ASSIGN_H_
#define _EJSON_ASSIGN_H_

#include <vector>
#include <map>
#include <string>
#include <json/json.h>
//
#include <iostream>

namespace qidian {
namespace ejson {

template<class EJSON, class DataType>
struct EJsonImport;

template<class EJSON> 
struct EJsonImport<EJSON, EJSON>
{
public:
    void Import(EJSON& data, const EJSON& arg) {
        data = arg;
    }
};

template<class EJSON>
struct EJsonImport<EJSON, uint64_t>
{
public:
    void Import(EJSON& ejson_data, const uint64_t& arg) {
        typename EJSON::EJsonValue json_value = (Json::Value::UInt64)arg;
        ejson_data.set_inner_data(json_value);
        std::cout << "IMPORT_uint64_t_value " << json_value.toStyledString()<<std::endl;
    }
};

template<class EJSON>
struct EJsonImport<EJSON, int64_t>
{
public:
    void Import(EJSON& ejson_data, const int64_t& arg) {
        typename EJSON::EJsonValue json_value = (Json::Value::Int64)arg;
        ejson_data.set_inner_data(json_value);
        std::cout << "IMPORT_int64_T_value " << json_value.toStyledString()<<std::endl;
    }
};


#define DEFINE_POD_TYPE(TYPE_NAME)                                                      \
template<class EJSON>                                                                   \
struct EJsonImport<EJSON, TYPE_NAME>                                                    \
{                                                                                       \
public:                                                                                 \
    void Import(EJSON& ejson_data, const TYPE_NAME& arg) {                              \
        typename EJSON::EJsonValue json_value(arg);                                      \
        ejson_data.set_inner_data(json_value);                                          \
        std::cout << "IMPORT_value " << #TYPE_NAME << " " << json_value.toStyledString()<<std::endl;   \
    }                                                                                   \
};                                                                                      \

DEFINE_POD_TYPE(std::string)
DEFINE_POD_TYPE(uint32_t)
DEFINE_POD_TYPE(uint16_t)
DEFINE_POD_TYPE(uint8_t)
DEFINE_POD_TYPE(int8_t)
DEFINE_POD_TYPE(int16_t)
DEFINE_POD_TYPE(int32_t)
DEFINE_POD_TYPE(bool)
DEFINE_POD_TYPE(double)
#undef DEFINE_POD_TYPE


template<class EJSON, class DataType>
struct EJsonImport<EJSON, std::vector<DataType> >
{
public:
    void Import(EJSON& ejson_data, const std::vector<DataType>& arg) {
        ejson_data.reset_array();
        for(size_t i = 0; i < arg.size(); i++) {
            ejson_data.append(arg[i]);
        }
        std::cout << "array" << " size:" << ejson_data.size() << std::endl;
    }
};


template<class EJSON, class DataType>
struct EJsonImport<EJSON, std::map<std::string, DataType> >
{
public:
    void Import(EJSON& ejson_data, const std::map<std::string, DataType>& arg) {
        
        ejson_data.reset_map();
        for(typename std::map<std::string, DataType>::const_iterator it = arg.begin();
            arg.end() != it; ++it) 
        {
            ejson_data[it->first] = it->second;
        }
        std::cout << "map" << " size:" << ejson_data.size() << std::endl;
    }
};

template<typename EJSON, typename DataType>
struct EJsonImport
{
public:
    void Import(EJSON& data, const DataType& arg) {
        data.reset_map();
        arg.ImportData(data);
    }
};


template<class EJSON, class DataType>
void EJsonImportValue(EJSON& ejson_data, const DataType& data_type) {
    EJsonImport<EJSON, DataType> ejson_import;
    ejson_import.Import(ejson_data, data_type);
}

} //namespace qidian
} //namespace ejson

#endif
