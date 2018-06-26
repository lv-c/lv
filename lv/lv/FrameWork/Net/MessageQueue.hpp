// *********************************************************************
//  MessageQueue   version:  1.0   ��  date: 2015/11/17
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2015 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/FrameWork/AutoLink.hpp>
#include <lv/FrameWork/Net/Fwd.hpp>

#include <lv/Buffer.hpp>
#include <lv/Timer.hpp>
#include <lv/IBufferManager.hpp>

#include <functional>


namespace lv::net
{
	class ISender
	{
	public:

		virtual ~ISender() = default;


		virtual	void	send(BufferPtr buf) = 0;

		virtual	bool	sendable() = 0;
	};

	class SendQueue;
	class ReceiveQueue;

	// NOT thread-safe

	class MessageQueue
	{
	public:

		using Receiver = std::function<void(BufferPtr)>;

	private:

		std::unique_ptr<SendQueue>		send_queue_;
		std::unique_ptr<ReceiveQueue>	receive_queue_;

		ContextPtr	context_;

		ISenderPtr	sender_;
		Receiver	receiver_;

		std::vector<uint32_t>	need_reply_;

		double	last_reply_time_;

		Timer	timer_;

		std::unique_ptr<SteadyTimer>	deadline_timer_;

		std::unique_ptr<IBufferManager>	buffer_manager_;

	public:

		MessageQueue(ContextPtr context, ISenderPtr sender, Receiver receiver);

		~MessageQueue();

		void	send(BufferPtr buf);

		void	on_receive(BufferPtr buf);

		// this function will call check_send immediately, rather than waiting for the next timer
		void	on_sendable();

		// change the sender
		void	set_sender(ISenderPtr sender);

		// change the receiver
		void	set_receiver(Receiver receiver);

		
		void	on_server_reset();

	private:

		void	check_send();

		void	fill_replies(Buffer & buf);

		void	on_timer();

	};


}