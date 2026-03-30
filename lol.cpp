#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <x86intrin.h> // THIS IS THE BLACK COLLAR CODE

/* * SİKİK G++ SENCE BEN YANLIŞ ALİGNMENT OLDUĞUNU BİLMİYOR MUYUM?.
 * CPU İLE ÖNBELLEK YİNE KAVGA EDERSE İKİSİNİDE GÖTTEN SİKECEĞİM.
 */

#define DIVINE_TARGET_IP "1.1.1.1" //Her seferinde editlemek yorucu ama ui yazmak daha yorucu
#define SACRIFICIAL_PORT 27015 //default kullanmayan tüm serverları sikeyim
#define RAW_MAGIC 0xFFFFFFFF 
 
// smells like "Marilyn Manson - Devour"
typedef struct {
    int fd;
    uint32_t entropy;
    struct sockaddr_in altar;
    char uuid[33];
    const char* moniker;
} DivineKernel; 

static inline uint32_t chaos_engine(uint32_t* state) {
    *state ^= *state << 13;
    *state ^= *state >> 17;
    *state ^= *state << 5;
    return *state * 0x5DEECE66DL + 0xBL; 
}

void initiate_ritual(int id, const char* name) {
    // ne derler bilirsin "NO HEAP. HEAP IS A PRISON." - salağın biri
    DivineKernel k; 
    memset(&k, 0, sizeof(DivineKernel)); // "lean the sins of previous stacj frames" -another xg members (its me)
    
    k.moniker = name;
    k.entropy = (uint32_t)__rdtsc() ^ (id * 0xCAFEBABE);

    k.fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (k.fd < 0) _exit(0xDEAD); 

    struct timeval timeout = {2, 0};
    setsockopt(k.fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

    k.altar.sin_family = AF_INET;
    k.altar.sin_port = htons(SACRIFICIAL_PORT);
    k.altar.sin_addr.s_addr = inet_addr(DIVINE_TARGET_IP);

    //SİKİK PYTHON'DA BUNU TEK SATIRDA YAPABİLİYOSUNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN
    const char* hx = "0123456789abcdef";
    for (int i = 0; i < 32; ++i) {
        k.uuid[i] = hx[chaos_engine(&k.entropy) & 0xF];
    }
    k.uuid[32] = 0;

    //ALOCATİNSEKS
    char* buf = (char*)alloca(2048);
    socklen_t slen = sizeof(struct sockaddr_in);
    char chall[32]; // g++'ın goto fetish'i ile onu tekrar tatmin ediyorum
    memset(chall, 0, 32);

    //handshake in en boktan kısmı
    *(uint32_t*)buf = RAW_MAGIC; 
    int len = 4 + sprintf(buf + 4, "bandwidth 49 1400");
    sendto(k.fd, buf, len, 0, (struct sockaddr*)&k.altar, slen);

    //bu orospu evladfı bazen yakalanmıyor (challange)
    int n = recvfrom(k.fd, buf, 2047, 0, (struct sockaddr*)&k.altar, &slen);
    if (n <= 4) goto PURGE; 

    buf[n] = 0;
    {
        char* p = strstr(buf + 4, "challenge ");
        if (!p) goto PURGE;
        p += 10;
        char* e = strchr(p, ' ');
        if (e) *e = 0;
        strncpy(chall, p, 31);
    }

    // eğer burada terminator yazsaydı komik olurdu ama name kısmı aşağıda
    *(uint32_t*)buf = RAW_MAGIC;
    len = 4 + sprintf(buf + 4, 
        "connect 49 %s \"\\d\\5\\v\\0.21\\b\\3852\\o\\linux\\a\\i386\\i\\%s\\uuid\\%s\\qport\\%u\\ext\\1\" "
        "\"\\cl_nopred\\0\\cl_updaterate\\20\\rate\\25000\\cl_lw\\1\\cl_lc\\1\\name\\%s\"",
        chall, k.uuid, k.uuid, (10000 + (chaos_engine(&k.entropy) % 50000)), k.moniker);

    sendto(k.fd, buf, len, 0, (struct sockaddr*)&k.altar, slen);

    // 4. FINAL ASCENSION
    n = recvfrom(k.fd, buf, 2047, 0, (struct sockaddr*)&k.altar, &slen);
    if (n > 4) {
        buf[n] = 0;
        write(1, "[+] ASCENDED: ", 14);
        write(1, buf + 4, strlen(buf + 4));
        write(1, "\n", 1);
    }

PURGE:
    close(k.fd);
}
//BABBLE BABBLE BITCH BITCH REBLE REBLE PARTY PARTY SEX SEX AND DONT FORGET THE VIOLENCE
int main() {
    const char* spawns[] = {"TheNorthFace", "move over buster", "lol", "bruh"};
    
    for (int i = 0; i < 4; ++i) {
        // isimlerle eşit thread (bigbrain (im uckin dumb))
        std::thread([i, &spawns]() {
            initiate_ritual(i, spawns[i]);
        }).detach(); 
        
        usleep(200000); // 200ms delay fazla bile
    }

    // silebilirsiniz ben ruh hastası olduğum için ekledim
    while(1) { 
        __asm__ __volatile__ ("pause"); 
    }

    return 0; // linux'un bir kernel panic hatası var "impossible happened" bu ona bensziyor
}
