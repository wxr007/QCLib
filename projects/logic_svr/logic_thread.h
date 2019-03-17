#include "containers_define.h"
#include "base_define.h"
#include "thread.h"

class LogicThread
{
public:
	explicit LogicThread(const std::string& name = "");
	~LogicThread();
	void Start();
	void Join();
	void ThreadRun( QCOOL::Thread* this_thread );
	void SetMessage( Message* msg );
	bool HaveMessage();
	void Notify();
private:
	QCOOL::Thread m_logic_thread;
//  typedef QCOOL::BlockQueue<Message*>	MsgQueue;

	QCOOL::Condition	m_cond;
	typedef QCOOL::LoopArray<Message*>	MsgQueue;
	MsgQueue m_msg_queue;
	MsgQueue m_msg_queue_inner;
};