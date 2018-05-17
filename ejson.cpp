/**
 * @author davinliu
 * @version 2018-03-31
 * @Last revision 2018-03-31
 * 
 */ 

#include "ejson.h"
#include <typeinfo>

using namespace qidian::ejson;

//
#include <stdio.h>

namespace {
    EJson::EJsonValue __convert_ejson_to_jsonvalue__(EJson& node_data) {
        EJson::EJsonValue ejson_value;
        if (node_data.is_type_map()) {
            EJson::EJsonMap& val = boost::any_cast<EJson::EJsonMap&>(node_data.__inner_data__());
            EJson::EJsonMap::iterator it = val.begin();
            for(; it != val.end(); ++it) {
                ejson_value[it->first] = __convert_ejson_to_jsonvalue__(it->second);
            }
        } else if (node_data.is_type_array()) {
            EJson::EJsonArray& val = boost::any_cast<EJson::EJsonArray&>(node_data.__inner_data__());
            for(size_t i = 0; i < val.size(); i++) {
                EJson::EJsonValue item = __convert_ejson_to_jsonvalue__(val[i]);
                ejson_value.append(item);
            }
        } else if(node_data.__inner_data__().type() == typeid(EJson::EJsonValue)) {
            ejson_value = boost::any_cast<EJson::EJsonValue>(node_data.__inner_data__());
        } else {
            ejson_value = EJson::EJsonValue();
        }
        return ejson_value;
    }

    EJson __convert_json_objectvalue_to_ejson__(EJson::EJsonValue& json_value) {
        EJson ejson_value;

        if (json_value.isBool()) {
            ejson_value = json_value.asBool();
        } else if(json_value.isInt()) {
            ejson_value = (int)json_value.asInt();
        } else if(json_value.isInt64()) {
            ejson_value = (int64_t)json_value.asInt64();
        } else if(json_value.isUInt()) {
            ejson_value = (uint32_t)json_value.asUInt();
        } else if(json_value.isUInt64()) {
            ejson_value = (uint64_t)json_value.asUInt64();
        } else if(json_value.isDouble()) {
            ejson_value = json_value.asDouble();
        } else if(json_value.isString()) {
            ejson_value = json_value.asString();
        } else {
            ejson_value = EJson();
        }
        return ejson_value;
    }

    EJson __convert_jsonobject_to_ejson__(EJson::EJsonValue& json_value) {
        EJson ejson_value;
        if (json_value.type() == Json::objectValue) {
            if (!ejson_value.is_type_map()) {
                ejson_value.reset_map();
            }
            EJson::EJsonValue::Members member = json_value.getMemberNames();  
            for (EJson::EJsonValue::Members::const_iterator iter = member.begin(); iter != member.end(); iter++) {
                ejson_value[*iter] = __convert_jsonobject_to_ejson__(json_value[*iter]);
            }
        } else if(json_value.type() == Json::arrayValue) {
            if (!ejson_value.is_type_array()) {
                ejson_value.reset_array();
            }
            for ( int cnt = 0; cnt < (int)json_value.size(); ++cnt ) {
                EJson json_item = __convert_jsonobject_to_ejson__(json_value[cnt]);
                ejson_value.append<EJson>(json_item);
            }
        } else {
            ejson_value = __convert_json_objectvalue_to_ejson__(json_value);
        }
        return ejson_value;
    }
}//

EJson& EJson::operator[](uint32_t index) {
    if (!is_type_array()) {
        throw std::logic_error("operator[] must is array");
    }

    EJsonArray& val = boost::any_cast<EJsonArray&>(_node_data);
    if(val.size() < index) {
        throw std::logic_error("ejson operator[] index error");
    }
    return val[index];
}

EJson& EJson::operator[](const std::string& key) {
    printf("is_undefined111111111:%d, key:%s\n", is_undefined(), key.c_str());
    if (is_undefined()) {
        reset_map();
    } else if (!is_type_map()) {
        throw std::logic_error("ejson operator[](const std::string& key) must be set reset_map");
    }
    EJsonMap& val = boost::any_cast<EJsonMap&>(_node_data);
    EJsonMap::iterator it = val.find(key);
    if(it == val.end()) {
        EJson ejson_val;
        std::pair<EJsonMap::iterator, bool> key_val = val.insert(std::make_pair(key, ejson_val));
        return key_val.first->second;
    }
    return it->second;
}

bool EJson::has_key(const std::string& key) {
    if (!is_type_map()) {
        return false;
    }
    EJsonMap& val = boost::any_cast<EJsonMap&>(_node_data);
    if (val.find(key) == val.end()) {
        return true;
    }
    return false;
}

size_t EJson::size() {
    if (is_type_map()) {
        return boost::any_cast<EJsonMap&>(_node_data).size();
    } else if (is_type_array()) {
        return boost::any_cast<EJsonArray&>(_node_data).size();
    }
    return 0;
}

std::string EJson::to_json() {
    EJsonValue root_value = __convert_ejson_to_jsonvalue__(*this);
    Json::FastWriter writer; 
    std::string str_encode = writer.write(root_value);
    return str_encode;
} 

int EJson::from_json(const std::string& str_json) {
    _undefined = false;
    Json::Value json_value;
    Json::Reader reader;
    if (!reader.parse(str_json, json_value, false)) {
        return -1;
    }
    *this = __convert_jsonobject_to_ejson__(json_value);
    return 0;
}

bool EJson::is_type_map() {
    return _node_data.type() == typeid(EJsonMap);
}

bool EJson::is_type_array() {
    return _node_data.type() == typeid(EJsonArray);
}


#define IS_TYPE(FUNC)                                               \
    bool ret = (_node_data.type() == typeid(EJsonValue));           \
    if (!ret) {                                                     \
        return ret;                                                 \
    }                                                               \
    EJsonValue& val = boost::any_cast<EJsonValue&>(_node_data);     \
    return val.FUNC;                                                \

bool EJson::is_type_double() {
    IS_TYPE(isDouble())
}

bool EJson::is_type_integer() {
    IS_TYPE(isNumeric())
}


bool EJson::is_type_str() {
    IS_TYPE(isString())
}

bool EJson::is_type_bool() {
    IS_TYPE(isBool())
}

bool EJson::is_undefined() {
    return _undefined;
}

void EJson::reset_map() {
    _node_data = EJsonMap();
    _undefined = false;
}
        
void EJson::reset_array() {
    _node_data = EJsonArray();
    _undefined = false;
}
