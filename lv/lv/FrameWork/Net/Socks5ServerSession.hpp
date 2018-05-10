// *********************************************************************
//  Socks5ServerSession   version:  1.0   ¡¤  date: 11/04/2010
//  --------------------------------------------------------------------
//  
//  --------------------------------------------------------------------
//  Copyright (C) jcfly(lv.jcfly@gmail.com) 2010 - All Rights Reserved
// *********************************************************************
// 
// *********************************************************************

#pragma once

#include <lv/FrameWork/Net/TcpSession.hpp>
#include <lv/FrameWork/Net/PacketProxy.hpp>
#include <lv/BinaryStream/Fwd.hpp>


namespace lv::net
{
	class Socks5ServerSession : public TcpSession
	{
		using base_type = TcpSession;

		Buffer	cache_;

		enum Status
		{
			MethodSelect,
			Auth,
			Request,
			Established,
			Ended
		};

		volatile Status	status_;

		using DestSession = TcpSession;
		std::unique_ptr<DestSession>	dest_session_;

		std::vector<boost::signals2::connection>	connections_;

		bool	closed_;

	public:

		explicit Socks5ServerSession(ContextPtr context);

	protected:

		void	on_receive(Buffer const & buf) override;
		void	on_write(size_t size) override;
		void	on_error(ErrorType type, boost::system::error_code const & error) override;


	private:

		PacketProxy		send();

		/// @exception std::ios_base::failure
		void	handle_method_select(BinaryIStream & bis);
		void	handle_auth(BinaryIStream & bis);
		void	handle_request(BinaryIStream & bis);


		void	dest_on_connected();
		void	dest_on_error(ErrorType type, boost::system::error_code const & error);
		void	dest_on_receive(Buffer const & buf);

		void	send_request_response(boost::system::error_code error);

		uint8	error_to_rep(boost::system::error_code const & error);

		void	exit();
	};

}