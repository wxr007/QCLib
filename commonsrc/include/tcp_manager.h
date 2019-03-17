#pragma once

namespace QCOOL
{

class CTcpManager
{
public :
    //通过Server发送数据;
    void ServerSendMessage(stMessage *,TServerId id,TSocketKey key);
    //通过Server接收数据.
    void ServerReceiveMessage(stMessage*,TServerId &id,TSocketKey &key);
    //通过Client发送数据;
    void ClientSendMessage(stMessage *,TServerId id);
    //通过Client接收数据.
    void ClientReceiveMessage(stMessage* ,TServerId id); 

    void AddServer();
    void AddClient();
};

}
