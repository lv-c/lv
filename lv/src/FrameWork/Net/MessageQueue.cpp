#include <lv/FrameWork/Net/MessageQueue.hpp>
#include <lv/FrameWork/Net/MessageQueueOptions.hpp>
#include <lv/FrameWork/Net/SteadyTimer.hpp>
#include <lv/BinaryStream.hpp>
#include <lv/BinaryStream/Vector.hpp>
#include <lv/Ensure.hpp>
#include <lv/MovableOnly.hpp>

#include <deque>
#include <optional>


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
		struct Message : private MovableOnly
		{
			Message(Buffer && buf)
				: buf(std::move(buf))
				, send_time(0)
			{
			}

			std::optional<Buffer>	buf;

			double		send_time;
		};

		std::deque<Message>	messages_;

		MessageQueueOptions const &	options_;

		Timer &		timer_;

		uint32_t	next_id_;

		uint32_t	id_base_;

	public:

		SendQueue(MessageQueueOptions const & options, Timer & timer)
			: options_(options)
			, timer_(timer)
			, next_id_(0)
			, id_base_(0)
		{
		}

		void	add(Buffer && buf)
		{
			Header header = { PacketType, next_id_++ };
			buffer::insert(buf, 0, &header, sizeof(header));

			messages_.push_back(Message(std::move(buf)));
		}

		Buffer const *	msg_to_send()
		{
			for (Message & msg : messages_)
			{
				if (msg.buf)
				{
					if (msg.send_time == 0 || msg.send_time + options_.resend_time < timer_.elapsed())
					{
						msg.send_time = timer_.elapsed();
						return &(*msg.buf);
					}
				}
			}

			return nullptr;
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
		struct Message : private MovableOnly
		{
			std::optional<Buffer>	buf;

			bool		received = false;
		};

		std::deque<Message>	messages_;

		MessageQueueOptions const &	options_;

		uint32_t	id_base_;

	public:

		ReceiveQueue(MessageQueueOptions const & options)
			: options_(options)
			, id_base_(0)
		{
		}

		// return the id
		uint32_t	add(ConstBufferRef buf)
		{
			LV_ENSURE(buf.size() >= sizeof(Header), "invalid packet size");

			Header header = *reinterpret_cast<Header const *>(buf.data());
			BOOST_ASSERT(header.type == PacketType);

			int32_t index = header.id - id_base_;

			if (index >= 0)
			{
				if (messages_.size() <= uint32_t(index))
				{
					size_t num = index + 1 - messages_.size();
					LV_ENSURE(num < 100000, "invalid index:" + std::to_string(index));

					for (size_t i = 0; i < num; ++i)
					{
						messages_.push_back(Message());
					}
				}

				Message & msg = messages_[index];

				if (!msg.received)
				{
					msg.received = true;
					msg.buf = Buffer(buf.begin() + sizeof(Header), buf.end());
				}
			}

			return header.id;
		}

		std::optional<Buffer>	msg_to_receive()
		{
			std::optional<Buffer> ret;

			for (Message & msg : messages_)
			{
				if (msg.buf)
				{
					std::swap(ret, msg.buf);
					break;
				}
				else if (options_.preserve_order)
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

	MessageQueue::MessageQueue(ISenderPtr sender, Receiver receiver, boost::asio::io_context * io, 
		MessageQueueOptions const & options /* = MessageQueueOptions() */)
		: sender_(sender)
		, receiver_(receiver)
		, options_(options)
		, last_reply_time_(0)
	{
		send_queue_ = std::make_unique<SendQueue>(options_, timer_);
		receive_queue_ = std::make_unique<ReceiveQueue>(options_);

		if (io != nullptr)
		{
			deadline_timer_ = std::make_unique<SteadyTimer>(*io, std::chrono::milliseconds(200),
				[this] { on_timer(); });
		}
	}

	MessageQueue::~MessageQueue() = default;

	void MessageQueue::send(Buffer && buf)
	{
		send_queue_->add(std::move(buf));
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

	void MessageQueue::on_receive(ConstBufferRef buf)
	{
		do 
		{
			BinaryIStream bis(buf);

			uint8_t tp{};		// may be used uninitialized ?
			bis >> tp;

			if (tp == PacketType)
			{
				uint32_t id = receive_queue_->add(buf);
				need_reply_.push_back(id);

				while (std::optional<Buffer> p = receive_queue_->msg_to_receive())
				{
					receiver_(std::move(*p));
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

				buf = buf.sub(static_cast<size_t>(bis.tellg()));
			}
			else
			{
				LV_ENSURE(false, "unknown header type");
			}

		} while (!buf.empty());
	}

	void MessageQueue::on_timer()
	{
		check_send();

		if (!need_reply_.empty() && sender_->sendable() && last_reply_time_ + 0.5 < timer_.elapsed())
		{
			send_buffer_.clear();
			fill_replies(send_buffer_);

			sender_->send(send_buffer_);
		}
	}

	void MessageQueue::check_send()
	{
		if (!sender_->sendable())
		{
			return;
		}

		while (Buffer const * buf = send_queue_->msg_to_send())
		{
			send_buffer_.clear();

			if (!need_reply_.empty())
			{
				fill_replies(send_buffer_);
			}

			send_buffer_.insert(send_buffer_.end(), buf->begin(), buf->end());
			sender_->send(send_buffer_);
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
