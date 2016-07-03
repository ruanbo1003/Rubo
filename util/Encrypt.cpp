/*
 * Encrypt.cpp
 *
 *      Author: ruanbo
 */

#include <util/Encrypt.hpp>



Encrypt::Encrypt() : _pub_key(nullptr), _pri_key(nullptr), _encode_len(0), _encode_buff(NULL),_decode_len(0),_decode_buff(NULL)
{

}

Encrypt::~Encrypt()
{
    if(_pub_key)
    {
        RSA_free(_pub_key);
    }
    if(_pri_key)
    {
        RSA_free(_pri_key);
    }

    if(_encode_buff)
    {
        delete[] _encode_buff;
        _encode_buff = nullptr;
    }
    if(_decode_buff)
    {
        delete[] _decode_buff;
        _decode_buff = nullptr;
    }
}


/*
 *   openssl genrsa -out server_pri.key 1024
 *   openssl rsa -in server_pri.key -pubout -out server_pub.key
 */
bool Encrypt::init()
{
    {
        FILE* pub_file = fopen("keys/server_pub.key", "rb");
        if(!pub_file)
        {
            LogError("fopen server public rsa key failed");
            return false;
        }

        _pub_key = RSA_new();
        if(!_pub_key)
        {
            LogError("RSA_new() return nullptr");
            fclose(pub_file);
            return false;
        }

        if(PEM_read_RSA_PUBKEY(pub_file, &_pub_key, 0, 0) == nullptr)
        {
            LogError("PEM_read_RSA_PUBKEY failed");
            fclose(pub_file);
            return false;
        }

        _encode_len = RSA_size(_pub_key) + 1;
        _encode_buff = new char[_encode_len];
        std::fill_n(_encode_buff, _encode_len, 0);
    }

    {
        FILE* pri_file = fopen("keys/server_pri.key", "rb");
        if(!pri_file)
        {
            LogError("fopen server public rsa key failed");
            return false;
        }

        _pri_key = RSA_new();
        if(!_pri_key)
        {
            LogError("RSA_new() return nullptr");
            fclose(pri_file);
            return false;
        }

        if(PEM_read_RSAPrivateKey(pri_file, &_pri_key, nullptr, nullptr) == nullptr)
        {
            LogError("PEM_read_RSA_PUBKEY failed");
            fclose(pri_file);
            return false;
        }

        _decode_len = RSA_size(_pri_key) + 1;
        _decode_buff = new char[_decode_len];
        std::fill_n(_decode_buff, _decode_len, 0);
    }

    return true;
}


string Encrypt::rsa_encode(const string& src)
{
    string dest = "";

    std::fill_n(_encode_buff, _encode_len, 0);

    int ret = RSA_public_encrypt(src.length(), (const unsigned char*)src.c_str(), (unsigned char*)_encode_buff, _pub_key, RSA_PKCS1_OAEP_PADDING);
    if(ret >= 0)
    {
        dest = string(_encode_buff, ret);
    }

    return dest;
}


string Encrypt::rsa_decode(const string& src)
{
    string dest = "";

    std::fill_n(_decode_buff, _decode_len, 0);

    int ret = RSA_private_decrypt(src.length(), (const unsigned char*)src.c_str(), (unsigned char*)_decode_buff, _pri_key, RSA_PKCS1_OAEP_PADDING);
    if(ret >= 0)
    {
        dest = string(_decode_buff, ret);
    }

    return dest;
}













