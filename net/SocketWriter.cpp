#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <iostream>

#include "SocketWriter.h"
#include "Multiplexer.h"


void SocketWriter::writeEvent() {
    ssize_t ret = ::send(this->getFd(), writePuffer.c_str(), (writePuffer.size() < 4096) ? writePuffer.size() : 4096, MSG_DONTWAIT | MSG_NOSIGNAL);
    
    if (ret >= 0) {
        writePuffer.erase(0, ret);
        if (writePuffer.empty()) {
            Multiplexer::instance().getWriteManager().unmanageSocket(this);
        }
    } else if (errno != EAGAIN && errno != EWOULDBLOCK && errno != EINTR) {
        std::cout << "Write: " << strerror(errno) << std::endl;
        Multiplexer::instance().getWriteManager().unmanageSocket(this);
    }
}