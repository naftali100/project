#ifndef __MESSAGEBUS_H__
#define __MESSAGEBUS_H__
#pragma once

enum class MessageType {
    MessageId,
    BombJailed,
    BombTimedout,
    BombRemoveFromVector
};

class Message {

};
// MAYBE: this should be push/pop message and not post...
// ANOTHER MAYBE: when message sent its propagate like event to all objects (add handleMessage to Entity)

class MessageBus {
public:
    static void postMessage(MessageType m){
        MessageBus::getInstance().m_messageTypeCounter[m]++;
    };
    template<typename T>
    static void postMessage(MessageType m, T* attachment){
        MessageBus::getMessageTypeWithAttachmentMap<T>().emplace(m ,attachment);
    };
    // static void postMessage(std::string){};
    // static void postMessage(Message){};
    

    static int& getMessage(MessageType m){
        // TODO: should this reset the messages count? or should we return read/write reference?
        return MessageBus::getInstance().m_messageTypeCounter[m];
    }
    template<typename AttachmentType>
    static AttachmentType* getMessage(MessageType m){
        return MessageBus::getMessageTypeWithAttachmentMap<AttachmentType>()[m];
    }
    // static int getMessage(std::string);

private:
    template<typename T>
    static std::map<MessageType, T*>& getMessageTypeWithAttachmentMap(){
        static std::map<MessageType, T*> m_map;
        return m_map;
    }
    std::map<MessageType, int> m_messageTypeCounter;
    static MessageBus& getInstance(){
        static MessageBus instance;
        return instance;
    }

    MessageBus() = default;
    MessageBus(const MessageBus&) = delete;
};
#endif // __MESSAGEBUS_H__
