#include <lv/ZeroMQ/Device.hpp>
#include <lv/ZeroMQ/BasicSocket.hpp>

#include <boost/bind.hpp>

namespace lv { namespace zeromq {


	Device::Device()
	{
		sockets_.assign(NULL);
	}

	void Device::start(BasicSocket & sock1, BasicSocket & sock2)
	{
		BOOST_ASSERT(sockets_[0] == NULL && sockets_[1] == NULL);

		sockets_[0] = &sock1;
		sockets_[1] = &sock2;

		for(size_t i = 0; i < 2; ++i)
		{
			sockets_[i]->set_readable_callback(boost::bind(&Device::on_socket_readable, this, i));
		}
	}

	void Device::on_socket_readable(size_t index)
	{
		while(true)
		{
			zmq::message_t msg;
			sockets_[index]->socket().recv(&msg);
			
			int more = sockets_[index]->getsockopt<int>(ZMQ_RCVMORE);
			sockets_[1 - index]->socket().send(msg, more ? ZMQ_SNDMORE : 0);

			if(! more)
			{
				break;
			}
		}
	}

} }