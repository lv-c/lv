// *********************************************************************
//  MessageQueue   version:  1.0   ¡¤  date: 2015/11/17
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
#include <lv/FrameWork/Net/MessageQueueOptions.hpp>

#include <lv/Buffer.hpp>
#include <lv/Timer.hpp>

#include <functional>


namespace lv::net
{
	class ISender
	{
	public:

		virtual ~ISender() = default;


		virtual	void	send(ConstBufferRef buf) = 0;

		virtual	bool	sendable() = 0;
	};

	class SendQueue;
	class ReceiveQueue;

	// NOT thread-safe

	class MessageQueue
	{
	public:

		using Receiver = std::function<void(Buffer)>;

	private:

		std::unique_ptr<SendQueue>		send_queue_;
		std::unique_ptr<ReceiveQueue>	receive_queue_;

		ISenderPtr	sender_;
		Receiver	receiver_;

		MessageQueueOptions		options_;

		std::vector<uint32_t>	need_reply_;

		Buffer		send_buffer_;

		double		last_reply_time_;

		Timer		timer_;

		std::unique_ptr<SteadyTimer>	deadline_timer_;

	public:

		// io is used to setup the timer. you can pass in nullptr and call on_timer yourself (every 0.2s or so)
		MessageQueue(ISenderPtr sender, Receiver receiver, boost::asio::io_context * io, 
			MessageQueueOptions const & options = MessageQueueOptions());

		~MessageQueue();


		void	send(Buffer && buf);

		// @exception
		void	on_receive(ConstBufferRef buf);

		void	on_timer();

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

	};


}