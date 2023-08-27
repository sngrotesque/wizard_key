#include <fstream>
#include <stdexcept>

#include <iostream>
#include <filesystem>

#include <network/wmkc_net.c>
#include <network/wmkc_ssl.c>
#include <crypto/snc.c>
#include <wmkc_memory.c>
#include <wmkc_misc.c>
#include <wmkc_time.c>

using namespace std;

#if 1
WMKC_PRIVATE_CONST(wmkcByte) SNC_TEST_KEY[96] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
WMKC_PRIVATE_CONST(wmkcByte) SNC_TEST_IV[32] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
#endif

class wmkcFile_obj {
    private:
        wmkcChar recvbuf[4096];
        streamsize _buf_size;
        fstream fp;

    public:
        string FileStream;

        wmkcFile_obj(filesystem::path path, ios::openmode mode)
        : recvbuf(), _buf_size()
        {
            fp = fstream(path, mode);
            if(!fp.is_open()) {
                throw runtime_error("File opening failed.");
            }
        }

        ~wmkcFile_obj()
        {
            fp.close();
        }

        void read_all()
        {
            for(; ; ) {
                _buf_size = fp.read(recvbuf, sizeof(recvbuf)).gcount();
                if(!_buf_size) {
                    break;
                }

                FileStream.append(recvbuf, _buf_size);
            }
        }
};

class SNC_Encryption {
    private:
        wmkcErr_obj error;
        SNC_mode mode;
        wmkcSNC_obj *snc = wmkcNull;
        const wmkcByte *key = wmkcNull;
        const wmkcByte *iv  = wmkcNull;

    public:
        SNC_Encryption(SNC_mode mode, const wmkcByte *key, const wmkcByte *iv)
        {
            this->mode = mode;
            this->snc = snc;
            this->key = key;
            this->iv = iv;

            if((error = wmkcSNC_new(&this->snc, this->mode)).code ||
                (error = wmkcSNC_init(this->snc, this->key, this->iv)).code) {
                throw runtime_error(error.func + string(": ") + error.message);
            }
        }

        ~SNC_Encryption()
        {
            wmkcSNC_free(&this->snc);
        }

        void ecb_encrypt(wmkcByte *content, wmkcSize size)
        {
            wmkcSNC_ecb_encrypt(this->snc, content, size);
        }

        void ecb_decrypt(wmkcByte *content, wmkcSize size)
        {
            wmkcSNC_ecb_decrypt(this->snc, content, size);
        }

        void cbc_encrypt(wmkcByte *content, wmkcSize size)
        {
            wmkcSNC_cbc_encrypt(this->snc, content, size);
        }

        void cbc_decrypt(wmkcByte *content, wmkcSize size)
        {
            wmkcSNC_cbc_decrypt(this->snc, content, size);
        }

        void ctr_encrypt(wmkcByte *content, wmkcSize size)
        {
            wmkcSNC_ctr_xcrypt(this->snc, content, size);
        }

        void cfb_encrypt(wmkcByte *content, wmkcSize size, wmkc_u16 segment_size = 256)
        {
            wmkcSNC_cfb_encrypt(this->snc, content, size, segment_size);
        }

        void cfb_decrypt(wmkcByte *content, wmkcSize size, wmkc_u16 segment_size = 256)
        {
            wmkcSNC_cfb_decrypt(this->snc, content, size, segment_size);
        }
};

class sockSocket {
    private:
        wmkcNet_obj *net;
        wmkcErr_obj error;
        wmkcNetBufT recvbuf[4096];

        wmkcCSTR remote_host;
        wmkc_u16 remote_port;

    public:
        string httpSendStream;
        string httpRecvStream;

        sockSocket(wmkcCSTR name, wmkc_u16 port)
        {
            this->remote_host = name;
            this->remote_port = port;

            #ifdef WMKC_PLATFORM_WINOS
            WSADATA ws;
            if (WSAStartup(MAKEWORD(2, 2), &ws)) {
                this->error = wmkcNet_errorHandler("HTTP_Client::new_object");
                throw runtime_error(this->error.func + string(": ") + this->error.message);
            }
            #endif

            if ((this->error = wmkcNet_new(&this->net)).code ||
                (this->error = wmkcNet_socket(this->net, AF_INET, SOCK_STREAM, 0)).code) {
                throw runtime_error(this->error.func + string(": ") + this->error.message);
            }
        }

        ~sockSocket()
        {
            wmkcNet_free(&this->net);
            #ifdef WMKC_PLATFORM_WINOS
            WSACleanup();
            #endif
        }

        void add_header(wmkcCSTR content)
        {
            this->httpSendStream.append(content);
        }

        void conn()
        {
            if ((this->error = wmkcNet_connect(this->net, this->remote_host, this->remote_port)).code) {
                throw runtime_error(error.func + string(": ") + error.message);
            }
        }

        void send()
        {
            if ((this->error = wmkcNet_sendall(this->net, (wmkcNetBufT *)this->httpSendStream.c_str(),
                this->httpSendStream.size(), 0)).code) {
                throw runtime_error(this->error.func + string(": ") + this->error.message);
            }
        }

        void recv()
        {
            if((this->error = wmkcNet_recv(this->net, this->recvbuf, sizeof(this->recvbuf), 0)).code) {
                throw runtime_error(this->error.func + string(": ") + this->error.message);
            }
            this->httpRecvStream.append(this->recvbuf, this->net->tSize);
        }
};

class sslSocket {
    private:
        wmkcErr_obj error;
        wmkcCSTR remote_host;
        wmkc_u16 remote_port;

        wmkcSSL_obj *ssl;
        wmkcNet_obj *net;

        wmkcNetBufT recvbuf[4096];

        double timeout;

    public:
        string httpSendStream;
        string httpRecvStream;

        sslSocket(wmkcCSTR host, wmkc_u16 port)
        {
            this->remote_host = host;
            this->remote_port = port;

            #ifdef WMKC_PLATFORM_WINOS
            WSADATA ws;
            if (WSAStartup(MAKEWORD(2, 2), &ws)) {
                this->error = wmkcNet_errorHandler("HTTP_Client::new_object");
                throw runtime_error(this->error.func + string(": ") + this->error.message);
            }
            #endif

            if ((this->error = wmkcNet_new(&this->net)).code ||
                (this->error = wmkcNet_socket(this->net, AF_INET, SOCK_STREAM, 0)).code) {
                throw runtime_error(this->error.func + string(": ") + this->error.message);
            }
            wmkcSSL_new(&this->ssl);
            wmkcSSL_context(this->ssl, TLS_method());
            wmkcSSL_wrap_socket(this->ssl, this->net, this->remote_host);
        }

        ~sslSocket()
        {
            wmkcNet_close(this->net);
            wmkcNet_free(&this->net);
            wmkcSSL_free(&this->ssl);
        }

        void add_header(wmkcCSTR content)
        {
            this->httpSendStream.append(content);
        }

        void conn()
        {
            wmkcSSL_connect(this->ssl, this->remote_host, this->remote_port);
        }

        void send()
        {
            wmkcSSL_sendall(this->ssl, (wmkcNetBufT *)httpSendStream.c_str(), httpSendStream.size());
        }

        void recv()
        {
            wmkcMem_zero(this->recvbuf, sizeof(this->recvbuf));
            wmkcSSL_recv(this->ssl, this->recvbuf, sizeof(this->recvbuf) - 1);

            this->httpRecvStream.append(this->recvbuf, this->ssl->net->tSize);
        }
};

int main()
{
    wmkcCSTR utf8 = "p:/传送手机/11.png";
    wmkcSize size = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);

    PWCHAR utf16 = new WCHAR[size];
    wmkcMem_zero(utf16, size * sizeof(WCHAR));

    MultiByteToWideChar(CP_UTF8, 0, utf8, -1, utf16, size);

    FILE *fp = _wfopen(utf16, L"rb");
    if(!fp) {
        cout << "文件打开失败。" << endl;
    }

    delete[] utf16;
    return 0;
}
