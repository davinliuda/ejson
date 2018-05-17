/**
 * @author davinliu
 * @version 2018-03-31
 * @Last revision 2018-03-31
 * 
 */ 

#ifndef _EJSON_HELLOWORLD_H_
#define _EJSON_HELLOWORLD_H_

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <stdio.h>
#include <json/json.h>

#include "ejson.h"
#include "ejson_variadic_func.h"

using namespace qidian::ejson;

struct Person {
    std::string name;
    int sex;
    uint8_t sex1;
    uint16_t sex2;
    uint32_t sex3;
    uint64_t sex4;
    int8_t sex5;
    int16_t sex6;
    int32_t sex7;
    int64_t sex8;
    std::string sex9;
    struct UserInfo{
        std::string country;
        int country_code;
		EJSON2(country, country_code)
    } user_info;
    EJSON12(name, sex, user_info, sex1, sex2, sex3, sex4, sex5, sex6, sex7, sex8, sex9)
};



struct Attendee
{
    std::string phone;
    std::string userId;
    int32_t userType;
    int32_t errorCode;
    std::string msg;
    
    Attendee()
    {
        errorCode = 0;
        userType = 0;
    }
    EJSON5(phone, userId, userType, errorCode, msg)
};

struct PlayIvrRspInfo
{
    uint32_t nonce;
    int32_t errorCode;
    std::string msg;
    std::string requestId;
    std::vector<Attendee> attendees; 
    
    PlayIvrRspInfo()
    {
        nonce = 0;
        errorCode = 0;
    }
    EJSON1(attendees)
    //EJSON5(nonce, errorCode, msg, requestId, attendees)
};


int main() {

    /*
    std::string str_json = "{\"key1\":23432342342423442432}";
    Json::Value json_value;
    Json::Reader reader;
    if (!reader.parse(str_json, json_value, false)) {
        return -1;
    }
    if(json_value.isInt64()) {
        json_value.asInt64();
    }

    //Json::Value::UInt64 aaa;
    uint64_t ddd;
    Json::Value json_value1 = (Json::Value::UInt64)ddd;
    
    int64_t arg = 9223372036854774807;
    Json::Value json_value2 = (Json::Value::Int64)arg;

    std::cout << json_value2.isDouble() << " "<< json_value2.isInt64() << " " << ;

    std::cout << "ddddddd" << std::endl;
    return 0;
    */


    //Person person;
    //person.name = "aa";
    //person.sex  = 1;

    EJson ejson;
    
	std::string json_str = "{\"attendees\":[{\"errorCode\":\"18446744073612849615\",\"msg\":\"ivr play request failed\",\"phone\":\"008602154569595\",\"userId\":\"in-66217369-be3b-41ff-9824-a2b360232e5a\",\"userType\":\"0\"}],\"errorCode\":\"902\",\"msg\":\"default error,  IsSuccHandleService() failed!\",\"nonce\":\"3435415823\"}";
	//std::string json_str = "[\"abc\", \"def\"]";
	//std::string json_str = "{\"key1\":{\"name\":\"nihao\",\"sex\":\"11\"},\"key2\":{\"name\":\"wohenhao\",\"sex\":\"22\"}}";
	int ret = ejson.from_json(json_str);

	if (ret != 0) {
		return 0;
	}

    PlayIvrRspInfo info;
    ejson.get(info);

    printf("ddd\n");
    //std::cout << "dddd" << aa <<std::endl;

	std::cout << ejson.to_json();

    //std::cout<<info.msg<<std::endl;	
    std::cout<<info.attendees.size()<<std::endl;

    /*
    Person person;
    memset(&person, 0, sizeof(person));
    person.name = "liuda";

    EJson ejson;
    ejson = person;
    std::cout << ejson.to_json();
    //std::map<std::string, std::string> dd;
    try {
        Person person1;
        ejson.get(person1);

        //std::cout << "test" << dd.size() << std::endl;

        //for(std::map<std::string, std::string>::iterator it = dd.begin(); dd.end() != it; ++it){
        //  std::cout<<"key:" << it->first << " value:" << it->second << std::endl;
        //}
        std::cout << "to_json:" << ejson.to_json() << std::endl;
        //std::cout << "name:" << person1.name << std::endl;
    } catch(const std::exception& e) {
        std::cout << e.what() << std::endl;
    }

    */
    
}


#endif
 
