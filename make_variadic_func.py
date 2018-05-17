# _*_ coding:utf-8 _*_ 

import sys

def make_func(count):
    if count == 0:
        return ""
    params_str = ""
    for i in range(count):
        params_str = params_str + "m" + str(i) + ","
    params_str = params_str[:-1]
    print "#define EJSON%d(%s) \\" % (count, params_str)
    
    print "void ImportData(qidian::ejson::EJson& ejson_data) const{            \\"
    
    for i in range(count):
        print "ejson_data[#m%d] = this->m%d;                 \\" % (i, i)

    print "}\\"


    print "void ExportData(qidian::ejson::EJson& ejson_data) {                                                     \\"

    for i in range(count):
        print "m%d = qidian::ejson::ExportData(ejson_data, #m%d, traits_type_init(m%d));             \\" % (i, i, i)

    print "}\\"
    




if __name__ == '__main__':
    
    count = sys.argv[1]


    print "#ifndef _EJSON_VARIADIC_FUNC_H_"
    print "#define _EJSON_VARIADIC_FUNC_H_"
    
    print "\n"
    print "#include \"ejson.h\""
    print "template<class T>"
    print "T traits_type_init(T) {"
    print "return T();"
    print "}"
    
    print "\n"
    for i in range(int(count)):
        make_func(i+1)
        print "\n\n"

    print "#endif"



