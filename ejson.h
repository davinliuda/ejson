/**
 * @author davinliu
 * @version 2018-03-31
 * @Last revision 2018-03-31
 * 
 */ 

#ifndef _EJSON_H_
#define _EJSON_H_

#include <string>
#include <map>
#include <vector>
#include <boost/any.hpp>
#include "ejson_value_import.h"
#include "ejson_value_export.h"
#include "json/json.h"

namespace qidian {
    namespace ejson {
    
    class EJson {
    public:
        typedef Json::Value         EJsonValue;
        typedef std::map<std::string, EJson> EJsonMap;
        typedef std::vector<EJson>           EJsonArray;
    private:
        boost::any _node_data;
        bool       _undefined;
    public:
        template<class DataType>
        EJson(const DataType& data_val) : _undefined(true) {
            inner_assign(*this, data_val);
        }
        
        EJson() : _undefined(true) {
            
        }

        template<class DataType>
        static void inner_assign(EJson& ejson_data, const DataType& data_val) {
            EJsonImportValue<EJson, DataType>(ejson_data, data_val);
        }
        
        template<class DataType>
        static void inner_get(EJson& ejson_data, DataType& data_val) {
            EJsonExportValue<EJson, DataType>(ejson_data, data_val);
        }

        EJson& operator[](uint32_t index);
        EJson& operator[](const std::string& key);

        template<class DataType>
        void set(const std::string& key, const DataType& val) {
            if (is_undefined()) {
                reset_map();
            } else if (!is_type_map()) {
                throw std::logic_error("ejson set must be set reset_map");
            }
            EJsonMap& map_val = boost::any_cast<EJsonMap&>(_node_data);
            EJson json_obj;
            inner_assign(json_obj, val);
            map_val.insert(std::make_pair(key, json_obj));
        }

        template<class DataType>
        DataType& get(DataType& val) {
            inner_get(*this, val);
            return val;
        }

        template<class DataType>
        DataType get() {
            DataType tmp;
            inner_get(*this, tmp);
            return tmp;
        }

        template<class DataType>
        EJson& operator= (const DataType& val) {
            inner_assign(*this, val);
            return *this;
        }

        template<class DataType>
        void append(const DataType& val) {
            if (is_undefined()) {
                reset_array();
            }else if (!is_type_array()) {
                throw std::logic_error("must be set reset_array");
            }

            EJsonArray& node_data = boost::any_cast<EJsonArray&>(_node_data);
            EJson json_obj;
            inner_assign(json_obj, val);
            node_data.push_back(json_obj);
        }

        std::string to_json();
        int from_json(const std::string& json);

        size_t size();

        bool has_key(const std::string& key);

        void reset_map();
        void reset_array();

        bool is_type_map();
        bool is_type_double();
        bool is_type_integer();
        bool is_type_array();
        bool is_type_str();
        bool is_type_bool();
        bool is_type_null();
        bool is_undefined();

        boost::any& __inner_data__(){
            return _node_data;
        }

        template<class DataType>
        void set_inner_data(const DataType& node_data) {
            _undefined = false;
            _node_data = node_data;
        }
    };
    
    template<typename DataType>
    inline static DataType ExportData(EJson& ejson_data, const char* member, const DataType& default_val) {
        EJson::EJsonMap& val = boost::any_cast<EJson::EJsonMap&>(ejson_data.__inner_data__());
        EJson::EJsonMap::iterator it = val.find(std::string(member));
        if(it == val.end()) {
            return default_val;
        }
        return it->second.get<DataType>();
    }
    
} //namespace qidian
} //namespace ejson



#endif
