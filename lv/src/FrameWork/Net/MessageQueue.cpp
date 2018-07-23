#include <lv/FrameWork/Net/MessageQueue.hpp>
#include <lv/FrameWork/Net/MessageQueueContext.hpp>
#include <lv/FrameWork/Net/SteadyTimer.hpp>
#include <lv/BinaryStream.hpp>
#include <lv/BinaryStream/Vector.hpp>
#include <lv/Ensure.hpp>
#include <lv/SimpleBufferManager.hpp>

#include <deque>


namespace lv::net
{
	enum HeaderType : uint8_t
	{
		PacketType	= 0xB0,
		ReplayType	= 0xB1
	};

#pragma pack(push)
#pragma pack(1)

	struct Header
	{
		HeaderType	type;

		uint32_t	id;
	};

#pragma pack(pop)

	class SendQueue
	{
		struct Message
		{
			Message(BufferPtr buf)
				: buf(buf)
				, send_time(0)
			{
			}

			BufferPtr	buf;

			double		send_time;
		};

		std::deque<Message>	messages_;

		ContextPtr	context_;

		Timer &		timer_;

		uint32_t	next_id_;

		uint32_t	id_base_;

	public:

		SendQueue(ContextPtr context, Timer & timer)
			: context_(context)
			, timer_(timer)
			, next_id_(0)
			, id_base_(0)
		{
		}

		void	add(BufferPtr buf)
		{
			Header header = { PacketType, next_id_++ };
			buffer::insert(*buf, 0, &header, sizeof(header));

			messages_.push_back(Message(buf));
		}

		BufferPtr	msg_to_send()
		{
			double resend_interval = dynamic_cast<MessageQueueContext &>(*context_).resend_time();

			for (Message & msg : messages_)
			{
				if (msg.buf)
				{
					if (msg.send_time == 0 || msg.send_time + resend_interval < timer_.elapsed())
					{
						msg.send_time = timer_.elapsed();
						return msg.buf;
					}
				}
			}

			return BufferPtr();
		}

		void	on_reply(uint32_t id)
		{
			int32_t index = id - id_base_;

			if (index >= 0)
			{
				LV_ENSURE(uint32_t(index) < messages_.size(), "invalid reply id");

				messages_[index].buf.reset();

				while (!messages_.empty() && !messages_.front().buf)
				{
					messages_.pop_front();
					id_base_ ++;
				}
			}
		}

		void	on_server_reset()
		{
			next_id_ -= id_base_;
			id_base_ = 0;

			for (Message & msg : messages_)
			{
				msg.send_time = 0;
			}
		}
	};

	//
	class ReceiveQueue
	{
		struct Message
		{
			Message()
				: received(false)
			{
			}

			BufferPtr	buf;

			bool		received;
		};

		std::deque<Message>	messages_;

		ContextPtr	context_;

		uint32_t	id_base_;

	public:

		ReceiveQueue(ContextPtr context)
			: context_(context)
			, id_base_(0)
		{
		}

		// return the id
		uint32_t	add(BufferPtr buf)
		{
			LV_ENSURE(buf->size() >= sizeof(Header), "invalid packet size");

			Header header = *reinterpret_cast<Header const *>(buf->data());
			BOOST_ASSERT(header.type == PacketType);

			int32_t index = header.id - id_base_;

			if (index >= 0)
			{
				if (messages_.size() <= uint32_t(index))
				{
					messages_.insert(messages_.end(), index - messages_.size() + 1, Message());
				}

				Message & msg = messages_[index];

				if (!msg.received)
				{
					buf->erase(buf->begin(), buf->begin() + sizeof(Header));

					msg.received = true;
					msg.buf = buf;
				}
			}

			return header.id;
		}

		BufferPtr	msg_to_receive()
		{
			BufferPtr ret;
			bool preserve_order = dynamic_cast<MessageQueueContext &>(*context_).preserve_order();

			for (Message & msg : messages_)
			{
				if (msg.buf)
				{
					std::swap(ret, msg.buf);
					break;
				}
				else if (preserve_order)
				{
					// check only the first buffer
					break;
				}
			}

			while (!messages_.empty() && messages_.front().received && !messages_.front().buf)
			{
				messages_.pop_front();
				id_base_++;
			}

			return ret;
		}


		void	on_server_reset()
		{
			id_base_ = 0;
			messages_.clear();
		}
	};


	//

	MessageQueue::MessageQueue(ContextPtr context, ISenderPtr sender, Receiver receiver)
		: context_(context)
		, sender_(sender)
		, receiver_(receiver)
		, last_reply_time_(0)
		, buffer_manager_(std::make_unique<SimpleBufferManager>(1024))
	{
		send_queue_ = std::make_unique<SendQueue>(context, timer_);
		receive_queue_ = std::make_unique<ReceiveQueue>(context);

		deadline_timer_ = std::make_unique<SteadyTimer>(context->io_wrapper(), std::chrono::milliseconds(200),
			[this] { on_timer(); });
	}

	MessageQueue::~MessageQueue() = default;

	void MessageQueue::send(BufferPtr buf)
	{
		send_queue_->add(buf);
		check_send();
	}

	void MessageQueue::on_sendable()
	{
		check_send();
	}

	void MessageQueue::on_server_reset()
	{
		need_reply_.clear();

		send_queue_->on_server_reset();
		receive_queue_->on_server_reset();
	}

	void MessageQueue::set_sender(ISenderPtr sender)
	{
		this->sender_ = sender;
	}

	void MessageQueue::set_receiver(Receiver receiver)
	{
		this->receiver_ = receiver;
	}

	void MessageQueue::on_receive(BufferPtr buf)
	{
		do 
		{
			BinaryIStream bis(buf);

			uint8_t tp;
			bis >> tp;

			if (tp == PacketType)
			{
				uint32_t id = receive_queue_->add(buf);
				need_reply_.push_back(id);

				while (BufferPtr p = receive_queue_->msg_to_receive())
				{
					receiver_(p);
				}

				break;
			}
			else if (tp == ReplayType)
			{
				uint32_t num;
				bis >> num;

				for (uint32_t i = 0; i < num; ++i)
				{
					uint32_t id;
					bis >> id;

					send_queue_->on_reply(id);
				}

				buf->erase(buf->begin(), buf->begin() + static_cast<size_t>(bis.tellg()));
			}
			else
			{
				LV_ENSURE(false, "unknown header type");
			}

		} while (!buf->empty());
	}

	void MessageQueue::on_timer()
	{
		check_send();

		if (!need_reply_.empty() && sender_->sendable() && last_reply_time_ + 0.5 < timer_.elapsed())
		{
			BufferPtr buf = buffer_manager_->get();
			fill_replies(*buf);

			sender_->send(buf);
		}
	}

	void MessageQueue::check_send()
	{
		if (!sender_->sendable())
		{
			return;
		}

		while (BufferPtr buf = send_queue_->msg_to_send())
		{
			// make a copy of the data, because the sender may modify it.
			BufferPtr new_buf = buffer_manager_->get();

			if (!need_reply_.empty())
			{
				fill_replies(*new_buf);
			}

			new_buf->insert(new_buf->end(), buf->begin(), buf->end());
			sender_->send(new_buf);
		}
	}

	void MessageQueue::fill_replies(Buffer & buf)
	{
		if (!need_reply_.empty())
		{
			BinaryOStream(buf) << ReplayType << bstream::variable_len_range<uint32_t>(need_reply_);

			need_reply_.clear();
			last_reply_time_ = timer_.elapsed();
		}
	}

}
