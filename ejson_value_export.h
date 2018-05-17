/**
 * @author davinliu
 * @version 2018-03-31
 * @Last revision 2018-03-31
 * 
 */ 

#ifndef _EJSON_EXPORT_H_
#define _EJSON_EXPORT_H_

#include <vector>
#include <map>
#include <string>
#include <sstream>
#include <boost/any.hpp>

namespace qidian {
namespace ejson {

template<typename EJSON, typename DataType>
struct EJsonExport;

template<typename EJSON>
struct EJsonExport<EJSON, EJSON>
{
public:
    void Export(EJSON& ejson_data, EJSON& arg) {
        arg = ejson_data;
    }
};


#define DEFINE_POD_TYPE(TYPE_NAME)                                                                                  \
template<typename EJSON>                                                                                            \
struct EJsonExport<EJSON, TYPE_NAME>                                                                                \
{                                                                                                                   \
public:                                                                                                             \
    void Export(EJSON& ejson_data, TYPE_NAME& arg) {                                                                \
        typename EJSON::EJsonValue val = boost::any_cast<typename EJSON::EJsonValue>(ejson_data.__inner_data__());  \
        if (val.isBool()) {                                                                                         \
            arg = (TYPE_NAME)val.asBool();                                                                          \
            return;                                                                                                 \
        } else if(val.isInt()) {                                                                                    \
            arg = (TYPE_NAME)val.asInt();                                                                           \
            return;                                                                                                 \
        } else if(val.isUInt()) {                                                                                   \
            arg = (TYPE_NAME)val.asUInt();                                                                          \
            return;                                                                                                 \
        } else if(val.isInt64()) {                                                                                  \
            arg = (TYPE_NAME)val.asInt64();                                                                         \
            return;                                                                                                 \
        } else if(val.isUInt64()) {                                                                                 \
            arg = (TYPE_NAME)val.asUInt64();                                                                        \
            return;                                                                                                 \
        } else if(val.isDouble()) {                                                                                 \
            arg = (TYPE_NAME)val.asDouble();                                                                        \
            return;                                                                                                 \
        } else if(val.isString()) {                                                                                 \
            std::stringstream ss(val.asString());                                                                   \
            ss >> arg;                                                                                              \
            return;                                                                                                 \
        } else if(val.isNull()) {                                                                                   \
            arg = TYPE_NAME();                                                                                      \
            return;                                                                                                 \
        }                                                                                                           \
        throw std::logic_error("export type error");                                                                \
    }                                                                                                               \
};                                                                                                                  \

DEFINE_POD_TYPE(int8_t)
DEFINE_POD_TYPE(int16_t)
DEFINE_POD_TYPE(int32_t)
DEFINE_POD_TYPE(int64_t)
DEFINE_POD_TYPE(uint8_t)
DEFINE_POD_TYPE(uint16_t)
DEFINE_POD_TYPE(uint32_t)
DEFINE_POD_TYPE(uint64_t)
DEFINE_POD_TYPE(double)
#undef DEFINE_POD_TYPE

template<typename EJSON>
struct EJsonExport<EJSON, bool>
{
public:
    void Export(EJSON& ejson_data, bool& arg) {
        typename EJSON::EJsonValue val = boost::any_cast<typename EJSON::EJsonValue>(ejson_data.__inner_data__());
        arg = val.asBool();
    }
};

template<typename EJSON>
struct EJsonExport<EJSON, std::string>
{
public:
    void Export(EJSON& ejson_data, std::string& arg) {
        if (!ejson_data.is_type_str()) {
            throw std::logic_error("if get string, ejson_data must be string");
        }
        typename EJSON::EJsonValue val = boost::any_cast<typename EJSON::EJsonValue>(ejson_data.__inner_data__());
        arg = val.asString();
    }
};



template<typename EJSON, typename DataType>
struct EJsonExport<EJSON, std::vector<DataType> >
{
public:
    void Export(EJSON& ejson_data, std::vector<DataType>& arg) {
        if (!ejson_data.is_type_array()) {
            throw std::logic_error("if get vector, ejson_data must be vector");
        }
        typename EJSON::EJsonArray val = boost::any_cast<typename EJSON::EJsonArray>(ejson_data.__inner_data__());
        arg.resize(val.size());
	    for(size_t i = 0; i < val.size(); i++) {
	        EJsonExport<EJSON, DataType> tmp;
            tmp.Export(val[i], arg[i]);
        }
    }
};

template<typename EJSON, typename DataType>
struct EJsonExport<EJSON, std::map<std::string, DataType> >
{
public:
    void Export(EJSON& ejson_data, std::map<std::string, DataType>& arg) {
        if (!ejson_data.is_type_map()) {
            throw std::logic_error("if get map, ejson_data must be map");
        }
        typename EJSON::EJsonMap val = boost::any_cast<typename EJSON::EJsonMap>(ejson_data.__inner_data__());
        typename EJSON::EJsonMap::iterator it = val.begin();
        for(; it != val.end(); ++it) {
            DataType item_value;
            EJsonExport<EJSON, DataType> tmp;
            tmp.Export(it->second, item_value);
            arg.insert(std::make_pair(it->first, item_value));
        }
    }
};

template<typename EJSON, typename DataType>
struct EJsonExport
{
public:
    void Export(EJSON& ejson_data, DataType& arg) {
        if(!ejson_data.is_type_map()) {
            throw std::logic_error("if get struct, ejson_data must be map");
        }
        arg.ExportData(ejson_data);
    }
};


template<typename EJSON, typename DataType>
void EJsonExportValue(EJSON& ejson_data, DataType& data_type) {
    EJsonExport<EJSON, DataType> ejson_export;
    ejson_export.Export(ejson_data, data_type);
}

} //namespace qidian
} //namespace ejson

#endif
