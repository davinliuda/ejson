### EJson
目前c++的json库绝大部分都不提供json到对象，STL的绑定，使用起来需要写很多代码。本库能够提供如下功能
1) 支持json的序列化和反序列化
2) 支持`STL`、`结构体`等对象赋值到`ejson`
3）支持从`ejson`里面获取`STL`、`结构体`对象.

### 依赖
目前`ejson`依赖`jsoncpp`，`boost::any`。使用前请安装.

### 例子
```c++
EJson json;
std::string<std::string, std::string> params;
json = params;
std::string json_str = params.to_string();

EJson json1;
bool ret = json1.from_json(json_str);
if(!ret) {
    return ;
}
std::string<std::string, std::string> params1;
json1.get(params1);
```

### ejson库和jsoncpp库的对比
有如下结构体，需要序列化成json对象。

```c++
struct Friend {
    int age;
    EJSON(age);//使用ejson的时候加上宏
};
struct Person {
    int sex;
    std::string name;
    std::string nick;
    std::map<std::string, std::string> params;
    std::vector<Friend> friends;
    EJSON5(sex, name, nick, params, friends); //使用ejson的时候加上宏                         
};
```

如果你使用`jsoncpp`，代码如下:

```c++
JsonCpp jsoncpp;
jsoncpp["sex"] = person.sex;
jsoncpp["name"] = person.name;
jsoncpp["nick"] = person.nick;
JsonCpp params_json;
for(auto person.params.begin(); it != person.params.end(); ++it) {
    params_json[it->first] = it->second;
}
jsoncpp["params"] = params_json;

JsonCpp friends_json;
for(auto person.friends.begin(); it != person.friends.end(); ++it) {
    JsonCpp item(*it);
    friends_json.append(item);
}
jsoncpp["friends"] = friends_json;
```

如果你使用`EJson`，只需要如下几行简单的代码即可.

```c++
Person person;
EJson ejson;
ejson = person;
ejson.to_json();
```

使用起来是不是很简单,别忘了点赞.



### EJson原理解析
http://km.oa.com/group/25354/articles/show/340395

### 贡献者
@davinliu(刘达); 