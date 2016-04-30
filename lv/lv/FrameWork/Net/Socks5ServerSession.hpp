// *********************************************************************
//  Socks5ServerSession   version:  1.0   ¡¤  date: 11/04/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#ifndef LV_NET_SOCKS5SERVERSESSION_HPP
#define LV_NET_SOCKS5SERVERSESSION_HPP

#include <lv/FrameWork/Net/TcpSession.hpp>
#include <lv/FrameWork/Net/PacketProxy.hpp>
#include <lv/BinaryStream/Fwd.hpp>

namespace lv { namespace net {

	class Socks5ServerSession : public TcpSession
	{
		typedef TcpSession	base_type;

		BufferPtr	cache_;

		enum Status
		{
			MethodSelect,
			Auth,
			Request,
			Established,
			Ended
		};

		volatile Status	status_;

		typedef TcpSession	DestSession;
		std::shared_ptr<DestSession>	dest_session_;

		std::vector<boost::signals2::connection>	connections_;

		bool	closed_;

	public:

		explicit Socks5ServerSession(ContextPtr context);

	protected:

		virtual	void	on_receive(BufferPtr buf);
		virtual	void	on_write(BufferPtr buf);
		virtual	void	on_error(ErrorType type, boost::system::error_code const & error);


	private:

		PacketProxy		send();

		/// @exception std::ios_base::failure
		void	handle_method_select(BinaryIStream & bis);
		void	handle_auth(BinaryIStream & bis);
		void	handle_request(BinaryIStream & bis);


		void	dest_on_connected();
		void	dest_on_error(ErrorType type, boost::system::error_code const & error);
		void	dest_on_receive(BufferPtr buf);

		void	send_request_response(boost::system::error_code error);

		uint8	error_to_rep(boost::system::error_code const & error);

		void	exit();
	};

} }

#endif