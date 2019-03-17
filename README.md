QCLib 的主干版本目录 创建时间2015-03-19

零：整体说明
    QCLib,游戏服务端一套完整的架构;
	暂时，实现功能为，一个二进制流数据转发的网关功能；网关功能的相关组件。
	
一、整个系统架构分为4层：项目业务层；服务器架构逻辑管理层；中间接口层；第三方库；
    暂时，完成的功能：第三方库封装；服务器架构逻辑管理层（中间接口层）。

二、第三方库封装
1、说明：第三方库，选择的是muduo，主要是基于Linux的网络、线程库。下载地址为“http://code.google.com/p/muduo/”。
2、基于muduo库，主要封装了2个类：CTcpClient/CTcpServer;
    2-1、CTcpClient，相关文件有tcp_client.cpp/h；
	    主要功能，就是创建一个客户端，去连接要求的服务器；然后，就是进行数据通信.
---附注：CTcpClient会自己内部创建线程，即一个CTcpClient一个线程.
	2-2、CTcpServer，相关文件有tcp_server.cpp/h；
	    主要功能，就是创建一个服务端，接受客户端的链接，然后进行数据通信.
---附注：CTcpServer会自己内部创建线程，提供一个参数，指定创建线程个数（N+1）.

三、服务器架构逻辑管理层
1、说明：主要是对CTcpClient/CTcpServer对象进行统一管理，包括对象管理、数据接受、发送管理。
2、基于CTcpClient/CTcpServer，封装了2个类：CClientManager/CGateManager
    2-1、CClientManager主要是对CTcpClient进行管理。（相关文件：client_manager.h）
	    一个服务端，由一个CTcpClient去连接；在一个程序中，会要求连接多个服务端，就需要多个CTcpClient对象。
		这里，就需要一个Manager来进行统一管理。
	2-2、CGateManager是一个网关功能的Manager（管理器）。（相关文件：gate_manager.h/cpp）
	    一个网关，会监听一个端口（即作为服务端，这里称为GServer）；
		同时，接收客户端（这里称为Client）数据，转发到多个服务端（这里称为Server），转发是通过网关内部客户端（这里称为GClient）进行的。
	    CGateManager就是对GServer和GClient s 进行管理的一个类。
---附注：CGateManager内部会创建2个线程，一个是处理client的业务；一个是处理server的业务。

四、其他部件
1、日志：日志，暂时用的是muduo库提供的日志。但是，做了一个简单的封装。相关文件：log4muduo.cpp/logAdaptor.h;
2、业务相关的一些类型声明（主要是业务消息），相关文件有：base_define.h；
3、function4muduo.h是业务消息转化，用到muduo相关性，所以特别提出来。

五、需要改进的地方
1、CGateManager线程处理，用到了muduo的功能；这个后期，需要改进.
2、CTcpClient发送后期需要实现缓冲功能；
3、CTcpServer/CTcpClient接收、发送缓冲，需要设置总体上限和强制发送上限,缓冲的内存形式还值得商榷。
4、所有文件，都在一个目录下了，后期需要调整。

//1-改名（h/cpp)；2-Makefile改名（共5处） 3、头文件包含
文件名修改
function4muduo --> function_for_muduo   (h)
kfile_system   --> file_system          (h)
kfile_writer   --> file_writer			(h/cpp)
klogger        --> logger				(h/cpp)
kthread_info   --> thread_info			(h/cpp)
logAdaptor     --> log_adaptor			(h)
QAtomic        --> atomic				(h)
QSingleton     --> singleton			(h)
QThread        --> thread				(h/cpp)
log4muduo      --> log_for_muduo      	(cpp)
lib_init_4muduo--> lib_init_for_muduo	(cpp)

ChatModule	   --> chat_module			(h/cpp)
DataCache	   --> data_cache			(h/cpp)
DefineGameMap  --> define_game_map		(h)
DefineLogicData--> define_logic_data	(h)

DefineModuleData-> define_module_data	(h)
GameMap		   --> game_map				(h)
LogicModuleManager->logic_module_manager(h/cpp)
LoginModule	   --> login_module			(h/cpp)
MapModule	   --> map_module			(h/cpp)
ModuleHeader   --> module_header		(h)
logic		   --> logic_main			(cpp)

类名和结构体名的修改：(2015.4.10 14:50)
CAtomicIntT    --> AtomicInt
CSingleton		--> Singleton
CThread			--> Thread
KFileSystem		--> FileSystem
KFileWriter		--> FileWriter
KLogger			--> Logger
CTcpClient		--> TcpClient
CTcpModule		--> TcpModule
stSession		--> UserSession
stMsgFromClient --> MsgFromClient
CTcpServer		--> TcpServer
stPoint			--> Point
TArea			--> Area
CTower			--> Tower
CTowerAoi		--> TowerAoi
stMsgHeader		--> MsgHeader
stCmdType		--> CmdType
stMessage		--> Message
stMsgInServer	--> MsgInServer
stSocketInfo	--> SocketInfo
stEvent			--> TcpEvent
stZeroArray		--> ZeroArray

CGateManager	--> GateManager

stMapHeader		--> MapHeader
stMapGrid		--> MapGrid
stMapGate		--> MapGate
stMapGridInfo	--> MapGridInfo
CGameMap		--> GameMap
stPlayerData	--> PlayerDataMsg
stPlayerBase	--> PlayerBase
stPlayerFigure	--> PlayerFigure
stPlayerMapData	--> PlayerMapData
CServerConfig	--> ServerConfig
CPlayerData		--> PlayerData
CLogicModuleBase--> LogicModuleBase
CChatModule		--> ChatModule
CLoginModule	--> LoginModule
CMapModule		--> MapModule
CLogicManager	--> LogicManager
CLogicModuleManager--> LogicModuleManager

枚举类型的修改：(2015.4.10 15:47)
emTcpEvent		--> TcpEventType
	emEvent_Connect		 --> TCP_EVENT_CONNECTED
	emEvent_DisConnect	 --> TCP_EVENT_DISCONNECTED
	emEvent_Error		 --> TCP_EVENT_ERROR
	emEvent_FirstRead	 --> TCP_EVENT_FIRST_READ
	
typedef名字的修改：(2015.4.10 16:47)
AtInt32			--> AtomicInt32
AtInt64			--> AtomicInt64
TLogBuffer		--> LogBuffer
TModelId		--> ModuleName
TMessageQueue	--> MessageList
TObjId			--> ObjectId
TCellId			--> CellId
TObjSet			--> ObjectSet
TMapID			--> MapId
TSocketKey		--> SocketKey
TUserId			--> UserId
TServerId		--> ServerId
TErrorCode		--> ErrorCode
UserObjIdMap	--> UserObjIdHash
PlayerDataMap	--> PlayerDataHash
TModuleID		--> ModuleId
TMapOrigID		--> MapOrigID
TCoord			--> Coordinate
UserIDList		--> UserIdList
ObjIdSet		--> ObjIdHashset










