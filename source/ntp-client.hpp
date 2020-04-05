#include <arpa/inet.h>
#include <errno.h>
#include <netdb.h>
#include <stdarg.h>
#include <string>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define UNIX_OFFSET 2208988800L

#define NTP_DEFAULT_PORT "123"
#define DEFAULT_TIMEOUT 3

// Flags 00|100|011 for li=0, vn=4, mode=3
#define NTP_FLAGS 0x23

typedef struct {
    uint8_t flags;
    uint8_t stratum;
    uint8_t poll;
    uint8_t precision;
    uint32_t root_delay;
    uint32_t root_dispersion;
    uint8_t referenceID[4];
    uint32_t ref_ts_secs;
    uint32_t ref_ts_frac;
    uint32_t origin_ts_secs;
    uint32_t origin_ts_frac;
    uint32_t recv_ts_secs;
    uint32_t recv_ts_fracs;
    uint32_t transmit_ts_secs;
    uint32_t transmit_ts_frac;

} ntp_packet;

class NTPClient {
private:
    int m_timeout;
    const char* m_port;
    const char* m_server;
    std::string m_errorMessage;
    int m_errCode;

    int setError(int code, const char* fmt, ...) {
        char buff[200];
        va_list va;
        va_start(va, fmt);
        vsnprintf(buff, sizeof(buff), fmt, va);
        va_end(va);

        m_errorMessage = buff;
        return m_errCode = code;
    }

    void resetError() {
        m_errorMessage.clear();
        m_errCode = 0;
    }

    time_t intGetTime() {
        int server_sock, status;
        struct addrinfo hints, *servinfo, *ap;
        socklen_t addrlen = sizeof(struct sockaddr_storage);
        ntp_packet packet = {.flags = NTP_FLAGS};

        hints = (struct addrinfo){.ai_family = AF_INET, .ai_socktype = SOCK_DGRAM};

        if ((status = getaddrinfo(m_server, m_port, &hints, &servinfo)) != 0) {
            return setError(-1, "Unable to get address info (%s)", gai_strerror(status));
        }

        for (ap = servinfo; ap != NULL; ap = ap->ai_next) {
            server_sock = socket(ap->ai_family, ap->ai_socktype, ap->ai_protocol);
            if (server_sock != -1)
                break;
        }

        if (ap == NULL) {
            return setError(-2, "Unable to create the socket");
        }

        struct timeval timeout = {.tv_sec = m_timeout, .tv_usec = 0};

        if (setsockopt(server_sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout)) < 0) {
            return setError(-3, "Unable to set RCV timeout");
        }

        if (setsockopt(server_sock, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof(timeout)) < 0) {
            return setError(-4, "Unable to set SND timeout");
        }

        if ((status = sendto(server_sock, &packet, sizeof(packet), 0, ap->ai_addr, ap->ai_addrlen)) == -1) {
            return setError(-5, "Unable to send packet");
        }

        if ((status = recvfrom(server_sock, &packet, sizeof(packet), 0, ap->ai_addr, &addrlen)) == -1) {
            if (errno == 11 || errno == 35) { // NX: 11, OTH: 35
                return setError(-6, "Connection timeout, retry. (%ds)", m_timeout);
            } else {
                return setError(-7, "Unable to receive packet (%d)", errno);
            }
        }

        freeaddrinfo(servinfo);
        close(server_sock);

        packet.recv_ts_secs = ntohl(packet.recv_ts_secs);

        return packet.recv_ts_secs - UNIX_OFFSET;
    }

public:
    NTPClient(const char* server, const char* port, int timeout) {
        m_server = server;
        m_port = port;
        m_timeout = timeout;
    }

    NTPClient(const char* server, const char* port) : NTPClient(server, port, DEFAULT_TIMEOUT) {}
    NTPClient(const char* server) : NTPClient(server, NTP_DEFAULT_PORT, DEFAULT_TIMEOUT) {}
    NTPClient() : NTPClient("pool.ntp.org", NTP_DEFAULT_PORT, DEFAULT_TIMEOUT) {}

    void setTimeout(int timeout) {
        m_timeout = timeout;
    }

    std::string getErrorMessage() {
        return m_errorMessage;
    }

    int getErrorCode() {
        return m_errCode;
    }

    bool hasFailed() {
        return m_errCode < 0;
    }

    time_t getTime() {
        resetError();
        return intGetTime();
    }

    long getTimeOffset(time_t currentTime) {
        time_t ntpTime = getTime();
        if (!hasFailed()) {
            return ntpTime - currentTime;
        } else {
            return ntpTime;
        }
    }
};
