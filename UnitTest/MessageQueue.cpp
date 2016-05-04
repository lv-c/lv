// *********************************************************************
//  MessageQueue   version:  1.0   ¡¤  date: 2015/11/19
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2015 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#include "UnitTest.hpp"

#include <lv/FrameWork/Net/MessageQueue.hpp>
#include <lv/FrameWork/Net/MessageQueueContext.hpp>
#include <lv/SimpleBufferManager.hpp>
#include <lv/Random.hpp>
#include <lv/Exception.hpp>
#include <lv/SharedPtr.hpp>

using namespace lv;
using namespace lv::net;
using namespace std;

DEFINE_EXCEPTION_MSG(TaskFinished, runtime_error);

class Sender : public ISender
{
	typedef std::function<void(BufferPtr)>	Callback;

	Callback	callback_;

	Random<>	die_;

	bool		sendable_;

public:

	Sender()
		: sendable_(true)
	{
	}

	void	set_callback(Callback const & callback)
	{
		this->callback_ = callback;
	}

	virtual	void	send(BufferPtr buf)
	{
		BOOST_CHECK(sendable_);

		// unstable network
		if (die_(0, 100) < 50)
		{
			// make a copy because buf will be modified.
			callback_(BufferPtr(new Buffer(*buf)));
		}
	}

	virtual	bool	sendable()
	{
		sendable_ = (die_(0, 100) < 60);
		return sendable_;
	}

};

class BufferReceiver
{
	vector<BufferPtr> &	buffers_;

	size_t	num_to_receive_;

public:

	BufferReceiver(vector<BufferPtr> & buffers, size_t num_to_receive)
		: buffers_(buffers)
		, num_to_receive_(num_to_receive)
	{
	}

	void operator () (BufferPtr buf)
	{
		buffers_.push_back(buf);

		if (buffers_.size() >= num_to_receive_)
		{
			throw TaskFinished();
		}
	}
};

BOOST_AUTO_TEST_CASE(test_message_queue)
{
	Random<> die;
	vector<BufferPtr> to_send(100);
	vector<BufferPtr> received;

	// random data
	for (BufferPtr & buf : to_send)
	{
		buf.reset(new Buffer(die(0, 100)));

		for (char & c : *buf)
		{
			c = uint8(die(0, 0xFF));
		}
	}

	//

	boost::asio::io_service service;

	Sender client_sender, server_sender;

	std::shared_ptr<MessageQueueContext> ctx(new MessageQueueContext(BufferManagerPtr(new SimpleBufferManager(1024)), service, true));
	ctx->set_resend_time(0.02);

	MessageQueue client(ctx, lv::shared_from_object(client_sender), MessageQueue::Receiver());
	MessageQueue server(ctx, lv::shared_from_object(server_sender), BufferReceiver(received, to_send.size()));

	client_sender.set_callback(std::bind(&MessageQueue::on_receive, &server, std::placeholders::_1));
	server_sender.set_callback(std::bind(&MessageQueue::on_receive, &client, std::placeholders::_1));

	for (BufferPtr buf : to_send)
	{
		// will be modified. make a copy
		client.send(BufferPtr(new Buffer(*buf)));
	}

	try
	{
		service.run();
	}
	catch(TaskFinished const &)
	{
	}

	BOOST_CHECK_EQUAL(to_send.size(), received.size());

	for (size_t i = 0; i < to_send.size(); ++i)
	{
		BOOST_CHECK_EQUAL(to_send[i]->size(), received[i]->size());
		BOOST_CHECK(std::equal(to_send[i]->begin(), to_send[i]->end(), received[i]->begin()));
	}
}