/*
 * Encrypt.hpp
 *
 *      Author: ruanbo
 */

#ifndef UTIL_ENCRYPT_HPP_
#define UTIL_ENCRYPT_HPP_

#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include "comm/Noncopyable.hpp"
#include "comm/global.hpp"

class Encrypt : public NonCopyable
{
private:
    RSA* _pub_key;
    RSA* _pri_key;

    int _encode_len;
    char* _encode_buff;

    int _decode_len;
    char* _decode_buff;

public:
    Encrypt();
    virtual ~Encrypt();

public:
    bool init();

    string rsa_encode(const string& src);
    string rsa_decode(const string& src);

};

using EncryptPtr = std::tr1::shared_ptr<Encrypt>;






#endif /* UTIL_ENCRYPT_HPP_ */


