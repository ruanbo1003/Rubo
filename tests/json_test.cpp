/*
 * json_test.cpp
 *
 *  Created on: Jan 23, 2017
 *      Author: ruanbo
 */


#include "comm/global.hpp"

#include "Includes/rapidjson/document.h"

using namespace rapidjson;


void json_tests()
{
    const char json[] = " { \"hello\" : \"world\", \"t\" : true , \"f\" : false, \"n\": null, \"i\":123, \"pi\": 3.1416, \"a\":[1, 2, 3, 4] } ";

//    Log("json:%s", json);

    rapidjson::Document document;

    if(document.Parse(json).HasParseError())
    {
        Log("document has parseerror");
        return;
    }

    char buffer[sizeof(json)];
    memcpy(buffer, json, sizeof(json));

    if(document.ParseInsitu(buffer).HasParseError())
    {
        Log("parse insitr has parse error");
        return;
    }

    if(document.IsObject() == false)
    {
        Log("document is not object");
        return;
    }

    // string
    if(document.HasMember("hello") && document["hello"].IsString())
    {
        Log("hello=%s", document["hello"].GetString());

        // change string value
        document["hello"] = "helloworld";
        // or
        //document["hello"].SetString("helloworld", strlen("helloworld"));
    }

    //find a value
    Value::MemberIterator hit = document.FindMember("hello");
    if(hit != document.MemberEnd() && hit->value.IsString())
    {
        Log("hello=%s", hit->value.GetString());
    }

    //bool: true
    if(document.HasMember("t") && document["t"].IsBool())
    {
        Log("t = %d", document["t"].GetBool());
    }

    //bool: false
    if(document.HasMember("f") && document["f"].IsBool())
    {
        Log("f = %d", document["f"].GetBool());
    }

    // int
    if(document.HasMember("i") && document["i"].IsInt())
    {
        Log("i = %d", document["i"].GetInt());
        // change int value
        document["i"] = 100;
    }

    // double
    if(document.HasMember("pi") && document["pi"].IsNumber() && document["pi"].IsDouble())
    {
        Log("pi = %f", document["pi"].GetDouble());
    }

    // array
    if(document.HasMember("a") && document["a"].IsArray())
    {
        const Value& a = document["a"];
        for(SizeType i=0; i<a.Size(); ++i)
        {
            Log("a[%d]=%d", i, a[i].GetInt());
        }

        LogLine("a = ");
        for(Value::ConstValueIterator it=a.Begin(); it!=a.End(); ++it)
        {
            LogLine("%d ", it->GetInt());
        }
        Log(" ");

        // append values to array
        Value& ca = document["a"];
        Document::AllocatorType& allocator = document.GetAllocator();
        for(int i=0; i<5; ++i)
        {
            ca.PushBack(i*10, allocator);
        }

        LogLine("a = ");
        for(Value::ConstValueIterator it=a.Begin(); it!=a.End(); ++it)
        {
            LogLine("%d ", it->GetInt());
        }
        Log(" ");
    }

    // types
    {
        const char* kTypeNames[] = { "Null", "False", "True", "Object", "Array", "String", "Number" };
        for(Value::ConstMemberIterator it=document.MemberBegin(); it!=document.MemberEnd(); ++it)
        {
            Log("memeber[%s] type is %d:%s", it->name.GetString(), it->value.GetType(), kTypeNames[it->value.GetType()]);
        }
    }

    // add one member to document
    Value name;
    {
        // one way to set string value
//        name.SetString("robert", document.GetAllocator());
        // or
//        name.SetString("robert", static_cast<SizeType>(sizeof("robert")), document.GetAllocator());
        // or
        char nameBuff[16];
        int len = sprintf(nameBuff, "%s %s", "Robert", "Go");
        name.SetString(nameBuff, static_cast<SizeType>(len), document.GetAllocator());

        // add memeber to document
        document.AddMember("name", name, document.GetAllocator());

        if(document.HasMember("name") && document["name"].IsString())
        {
            Log("name:%s", document["name"].GetString());
        }
    }


    // add one array to document
    {
        Value readers;

        readers.SetArray();

        for(int i=1; i<=5; ++i)
        {
            Value reader;
            string readerName = "reader" + std::to_string(i);

            reader.SetString(readerName.data(), document.GetAllocator());

            readers.PushBack(reader, document.GetAllocator());
        }

        LogLine("Readers:")
        for(Value::ConstValueIterator it=readers.Begin(); it!=readers.End(); ++it)
        {
            LogLine("%s ", it->GetString());
        }
        Log(" ");

        document.AddMember("readers", readers, document.GetAllocator());

        // check if add success
        if(document.HasMember("readers") && document["readers"].IsArray())
        {
            LogLine("readers:");
            const Value& a = document["readers"];
            for(Value::ConstValueIterator it=a.Begin(); it!=a.End(); ++it)
            {
                LogLine("%s ", it->GetString());
            }
            Log(" ");
        }
    }
}





















