#include "pch.h"
#include "UdpServer.h"

IAsyncAction UdpServer::StartServer(const hstring& port)
{
	m_port = port;
	m_serverDatagramSocket.MessageReceived({ this, &UdpServer::ServerDatagramSocket_MessageReceived });
	co_await m_serverDatagramSocket.BindServiceNameAsync(port);
}

IAsyncAction UdpServer::Send(const winrt::array_view<const uint8_t>& buffer)
{
	if (m_dataWriter != nullptr)
	{
		m_dataWriter.WriteBytes(buffer);
		co_await m_dataWriter.StoreAsync();
	}
}

void UdpServer::StopServer()
{
	StopStream();
	m_serverDatagramSocket = nullptr;
}

void UdpServer::StopStream()
{
	if (m_dataWriter != nullptr)
	{
		m_dataWriter.DetachStream();
		m_dataWriter = nullptr;
	}
}

IAsyncAction UdpServer::ServerDatagramSocket_MessageReceived(winrt::Windows::Networking::Sockets::DatagramSocket sender, winrt::Windows::Networking::Sockets::DatagramSocketMessageReceivedEventArgs args)
{
	StopStream();
	DataReader dataReader{ args.GetDataReader() };
	winrt::hstring request{ dataReader.ReadString(dataReader.UnconsumedBufferLength()) };
	IOutputStream outputStream = co_await sender.GetOutputStreamAsync(args.RemoteAddress(), m_port);
	m_dataWriter = DataWriter(outputStream);
}
